#include <Arduino.h>
#include "WiFi.h"
#include "AsyncTCP.h"
#include <AsyncElegantOTA.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <index.h>
#include <string.h>

//#define dbg

#define DHTPIN    18     // Digital pin connected to the DHT sensor 
#define DHTTYPE   DHT11  // DHT 11
#define HUMID_PIN 16
#define HEAT_PIN  17
#define delayMS   3000


DHT dht(DHTPIN, DHTTYPE);
uint32_t lastMillis = millis();

  typedef enum{readings, settingsRefresh, settingsSet, wifiRefresh, wifiSet}wsSendType;

  const char *SSID = "AP T/H: 10.0.1.14/wifi"; //ssid must be <= 32chars
  const uint32_t wsDelay=10000;  // WebSock send delay and last send times

  IPAddress  // soft AP IP info
          ip_AP(10,0,1,14)
        , ip_AP_GW(10,0,1,14)
        , ip_subNet(255,255,255,128);

// objects
AsyncWebServer server(80);
AsyncWebSocket webSock("/");

typedef struct WifiCreds_t{
      String    SSID;
      String    PWD;
      bool      isDHCP=false;
      IPAddress IP;
      IPAddress GW;
      IPAddress MASK;

      std::string toStr(){
        char s[150];
        sprintf(s, "{\"SSID\":\"%s\",\"PWD\":\"%s\",\"isDHCP\":%s,\"IP\":\"%s\",\"GW\":\"%s\",\"MASK\":\"%s\"}", SSID.c_str(), PWD.c_str(), isDHCP?"true":"false", IP.toString().c_str(), GW.toString().c_str(), MASK.toString().c_str());
        return(s);
      }
} WifiCreds_t;
WifiCreds_t creds;

typedef struct systemValues_t{   // temp/humid & threshold values
        float t1;
        int   h1;
        bool  heat1;
        bool  humid1;
        int   t1_on;
        int   t1_off;
        int   h1_on;
        int   h1_off;
        unsigned int delay = delayMS;

        std::string toStr(){    // make JSON string
            char c[200];
            int n = sprintf(c, "[{\"t1\":%.1f,\"h1\":%i,\"heat1\":%i,\"humid1\":%i},{\"t1_on\":%i,\"t1_off\":%i,\"h1_on\":%i,\"h1_off\":%i,\"delay\":%u}]", t1, h1, heat1, humid1, t1_on, t1_off, h1_on, h1_off, delay);
            return std::string(c, n);
        }
} systemValues_t;
systemValues_t sv;

/******  PROTOTYPES  *******/
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
void notFound(AsyncWebServerRequest *request);
void initLocalStruct();
bool wifiConnect(wifi_mode_t m);

void update_sMsgFromString( const std::string &scaleChar, systemValues_t& p);
std::string valFromJson(const std::string &json, const std::string &element);
bool saveStruct(); // write the local systemValues_t struct string to LittleFS
void setRelays();
bool initCreds();
void setCreds(const std::string& s);
bool saveCreds();


  void setup() {
#ifdef dbg
  Serial.begin(115200);delay(500);
  Serial.printf("%i :  SETUP\n", __LINE__);Serial.flush();
  if(!SPIFFS.begin())Serial.printf("%i : SPIFFS init Failed\n", __LINE__);
#else
  SPIFFS.begin();
#endif

  if(initCreds() == false)wifiConnect(WIFI_MODE_AP);
  if(wifiConnect(WIFI_MODE_STA))wifiConnect(WIFI_MODE_AP);

#ifdef dbg
  WiFi.printDiag(Serial); Serial.flush();
#endif

  initLocalStruct();              // update sv using LittleFs's JSON if available
  pinMode(HEAT_PIN, OUTPUT);
  pinMode(HUMID_PIN, OUTPUT);

  dht.begin();

  webSock.onEvent(onWsEvent);
  server.addHandler(&webSock);
 
  // Route for root
  server.on("/"        , HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/html", INDEX_HTML);});
  server.on("/wifi"    , HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/html", WIFI_HTML);});
  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/html", SETTINGS_HTML);});

  AsyncElegantOTA.begin(&server);
  server.begin();
}

  void loop() {
  delay(100);
#ifdef dbg
  Serial.print(".");Serial.flush();
#endif

  if(millis()-lastMillis > sv.delay){ // check DHT reading every sv.delay ms
Serial.printf("%i : T:%.2f\tH: %.2f\n", __LINE__, dht.readTemperature(true), dht.readHumidity());
    sv.t1 = dht.readTemperature(true);
    sv.h1 = dht.readHumidity();
    setRelays();
    webSock.textAll(sv.toStr().c_str());
    webSock.cleanupClients();

#ifdef dbg
  Serial.printf("\n%i : IP: %s\nLoop: sv: %s\n", __LINE__, WiFi.localIP().toString().c_str(), sv.toStr().c_str());Serial.flush();
#endif
    lastMillis = millis();
  }
}
  void setRelays(){
    // current state and between on/off temps or above/below absolute on/off boundary
    if(sv.t1<sv.t1_on)sv.heat1=true;
    if(sv.t1>sv.t1_off)sv.heat1=false;

    if(sv.h1>sv.h1_on)sv.humid1=true;
    if(sv.h1<sv.h1_off)sv.humid1=false;

    digitalWrite(HEAT_PIN, !sv.heat1);
    digitalWrite(HUMID_PIN , sv.humid1);
  }
  void notFound(AsyncWebServerRequest *request){request->send_P(200, "text/html", INDEX_HTML);}
  void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
    switch(type){
      case WS_EVT_CONNECT: return;
      case WS_EVT_DISCONNECT: return;
      case WS_EVT_ERROR: return;
      case WS_EVT_PONG:return;
      case WS_EVT_DATA:
        AwsFrameInfo * info = (AwsFrameInfo*)arg;
        if(info->final && !info->index && info->len == len && info->opcode == WS_TEXT){
//            data[len]=0;
            std::string const s=std::string((char *)data, len);
#ifdef dbg
  Serial.printf("\n%i : %s : WS: data: %s, %i\n", __LINE__, __FUNCTION__, s.c_str(), s.length());Serial.flush();
#endif
          uint8_t src = ::atoi(valFromJson(s, "src").c_str());
          switch(src){
            case readings:
              sv.t1 = dht.readTemperature(true);
              sv.h1 = dht.readHumidity();
              setRelays();
              client->text(sv.toStr().c_str());
              break;
            case settingsRefresh: client->text(sv.toStr().c_str());break;
            case settingsSet: update_sMsgFromString(s, sv); if(saveStruct())client->text(sv.toStr().c_str());break;
            case wifiRefresh: client->text(creds.toStr().c_str());break;
            case wifiSet: setCreds(s); if(saveCreds())client->text(creds.toStr().c_str()); delay(3000); ESP.restart();break;
          }
      }
    }
  }

  // load LittleFS copy of JSON into the local scale's systemValues_t
  void initLocalStruct(){
        File f = SPIFFS.open("/systemValues_t.json", "r");
        if(f){
                const std::string s = f.readString().c_str();
                f.close();
                update_sMsgFromString(s, sv);
        }
#ifdef dbg
  Serial.printf("%i : initLocalStruct : sv:%s\n", __LINE__, sv.toStr().c_str());Serial.flush();
#endif       
  }
  bool saveStruct(){
        File f = SPIFFS.open(F("/systemValues_t.json"), "w");
        if(f){
  #ifdef dbg
    Serial.printf("%i : %s : saveValues Success: sv: %s\n", __LINE__, __FUNCTION__, sv.toStr().c_str());Serial.flush();
  #endif
                f.print(sv.toStr().c_str());
                f.close();
                return true;
        }
        else{
  #ifdef dbg
    Serial.printf("%i : %s : saveValues FAIL: sv: %s\n", __LINE__, __FUNCTION__, sv.toStr().c_str());Serial.flush();
  #endif
            return false;
          }
  }
  bool saveCreds(){
    File f = SPIFFS.open(F("/creds.json"), "w");
    if(f){
          f.print(creds.toStr().c_str());
          f.close();
  #ifdef dbg
    Serial.printf("%i : %s : saveCreds Success: creds: %s\n", __LINE__, __FUNCTION__, creds.toStr().c_str());Serial.flush();
  #endif
          return true;
    }
    else{
  #ifdef dbg
    Serial.printf("%i : saveCreds FAILED: creds: %s\n", __LINE__, creds.toStr().c_str());Serial.flush();
  #endif
        return(false);
    }
  }
  bool initCreds(){
          File f = SPIFFS.open(F("/creds.json"), "r");
          if(f){
                  std::string s = f.readString().c_str();
                  f.close();
                  setCreds(s);
                  return(true);
          }
  #ifdef dbg
    Serial.printf("\n%i : Failed init creds from LittleFS: %s\n", __LINE__, creds.toStr().c_str());Serial.flush();
  #endif
    return(false);
  }
  void setCreds(const std::string& s){
    creds.SSID=valFromJson(s, "SSID").c_str();
    creds.PWD=valFromJson(s, "PWD").c_str();
    creds.isDHCP=::atoi(valFromJson(s, "isDHCP").c_str());
    creds.IP.fromString(valFromJson(s, "IP").c_str());
    creds.GW.fromString(valFromJson(s, "GW").c_str());
    creds.MASK.fromString(valFromJson(s, "MASK").c_str());
  }
  void update_sMsgFromString(const std::string &json, systemValues_t &p){
#ifdef dbg
  Serial.printf("%i : %s : JSON: %s\n", __LINE__, __FUNCTION__, json.c_str());Serial.flush();
#endif
    p.t1_on     =::atof(valFromJson(json, "t1_on").c_str());
    p.t1_off    =::atof(valFromJson(json, "t1_off").c_str());
    p.h1_on     =::atoi(valFromJson(json, "h1_on").c_str());
    p.h1_off    =::atoi(valFromJson(json, "h1_off").c_str());
    p.delay     =::atoi(valFromJson(json, "delay").c_str());
    
  }
  std::string valFromJson(const std::string &json, const std::string &element){// got stack dumps with ArduinoJson
    size_t start, end;
  //         var str= '[{"t1":0,"h1":0},{"t1_on":75,"t1_off":82,"h1_on":75,"h1_off":85,"delay":7}]';
  //         std::string str= '[{"t1":0,"h1":0},{"t1_on":75,"t1_off":82,"h1_on":75,"h1_off":85,"delay":7}]';
    start = json.find(element);
    start = json.find(":", start)+1;
    if(json.substr(start,1) =="\"")start++;
    end  = json.find_first_of(",]}\"", start);
    return(json.substr(start, end-start));
  }
  bool wifiConnect(wifi_mode_t m)
  {
    uint32_t ret = 0;
    wl_status_t WiFi_status;

    WiFi.disconnect(false, true);
    WiFi.softAPdisconnect(false);

    if(WiFi.getMode() != m)WiFi.mode(m);// WIFI_AP_STA,WIFI_AP; WIFI_STA;

#ifdef dbg
  Serial.printf("%i : %s : mode: %i\n", __LINE__, __FUNCTION__,  m);Serial.flush();
#endif
    ret = 0;
    switch(m){
            case WIFI_STA:  // mode == 1
                            if(creds.isDHCP == false)
                              ret = WiFi.config(creds.IP, creds.GW, creds.MASK);
                            WiFi_status = WiFi.begin(creds.SSID.c_str(), creds.PWD.c_str(),2);
                            break;
            case WIFI_AP:   // mode == 2
                            ret = WiFi.softAPConfig(ip_AP, ip_AP_GW, ip_subNet);ret = ret<<1;
                            ret = ret | WiFi.softAP(SSID, "");
                            WiFi_status = WiFi.begin();
                            break;
            case WIFI_AP_STA: break;  // mode == 3
            case WIFI_OFF:    break;
    }
    unsigned int startup = millis();
    while(WiFi.status() != WL_CONNECTED){
          delay(250);
#ifdef dbg          
          Serial.print(WiFi.status());
#endif
          if(millis() - startup >= 15000) break;
    }
#ifdef dbg
  Serial.printf("\n");Serial.flush();
  Serial.printf("%i : %s : IP: %s, GW: %s, Mask: %s, DNS: %s\n"
          , __LINE__, __FUNCTION__, WiFi.localIP().toString().c_str(), WiFi.gatewayIP().toString().cstr()
          , WiFi.subnetMask().toString().c_str(), WiFi.subnetMask().toString().c_str());Serial.flush();
#endif
    return(WiFi.status() ^ WL_CONNECTED);
  }
  String mac2string(const uint8_t *mac){
    char macStr[18];
    snprintf(macStr, 18, "%02X:%02X:%02X:%02X:%02X:%02X",
         mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return(macStr);
  }
