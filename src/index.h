extern const char INDEX_HTML[] PROGMEM =  R"=====(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
      @viewport{width:device-width;zoom:0;}
      @-ms-viewport {width: device-width ;}
    html{
          font-family: Arial;
          display: inline-block;
          margin: auto auto;
          text-align: left;
          max-width: 700px;
          max-height:15vw;
          min-width: 15vw;
          min-height:8vw;
    }
    h2 {
        font-size: 5vw;
        margin: 1vw 5vw;
        width: 45vw;
    }
    input[type=submit] {
      border: 0;
      line-height: 3vw;
      padding: 0vw 1vw;
      font-size: 2vw;
      text-align: center;
      
      font-weight: bold;
      padding: .3vw .9vw;
      text-decoration:none;
      margin: 0vw 0vw;
      cursor: pointer;
      width: auto;
      box-shadow: inset 1vw 2vw 2.1vw rgba(255, 255, 255, .6),
                  inset -.1vw -.2vw .5vw rgba(0, 0, 0, .6);
   }
   .help {
                border: 0;
                line-height: 3.5vw;

                padding: 1vw 1vw;
                text-align: center;
                font-weight:bold;
                font-size: 2.2vw;

                cursor: pointer;
                background-color: rgb(255, 255, 255);
                box-shadow: inset 1vw 1vw 1vw rgba(255, 255, 255, 0.6),
                    inset -.3vw -.25vw .3vw rgba(0, 0, 0, 0.6);

                border:10vw;

                margin-left:.3vw;
                margin-top: 4vw;
                padding: 1vw;

                width:60vw;
                margin-inline: 1vw;
    }
    .status-grid{
                display:grid;
                grid-template-columns:9vw 11vw 10vw 5vw;
                grid-template-rows: 6vw 6vw 1vw;

                gap: .2vw 1vw;
                box-sizing: border-box;
                width: min(100%, 45vw);
                min-width: 39vw;
                height: 18vw;

                margin-top: 1vw;
                margin-left: 5vw;
                margin-right: 5vw;

                background-color: #ced3c7;
                justify-items: center;

                padding: 2vw 1.8vw;

                font-size:2.5vw;
                font-weight: bold;
                text-align:center;
            }
    .settings-grid{
                display:grid;
                grid-template-columns:22vw 18vw 14vw;
                grid-template-rows: 8vw 12vw;
                width: 55vw;

                margin-top: 0vw;
                margin-left: 0vw;
                padding: 0vw 0vw;

                justify-items: left;

                font-size:2.5vw;
                font-weight: bold;
                text-align:left;
                justify-content: space-evenly;
            }
            .refresh {
              grid-column: 4 / span 1;
              grid-row: 2 / span 2;
                
            }
            .indicators {
              grid-row: 2 / span 2;
            }
            .details{
              grid-column:1 / span 3
            }

  </style>
  <title>Cabinet Environment</title>
</head>
  <input id="bt_help" type="submit" value="Help" style="font-size: 14px;line-height: 1.5;" onclick="f_help()">
    <h2>ESP32 DHT Server</h2>
    <main class="status-grid">
      <div style="padding: .7vw 0vw;">
          <span><i class="fas fa-thermometer-half" style="color:#059e8a;"></i>Temp</span>
     </div>
      <div style="padding: .7vw 0vw;">
          <span id="t1">23.3</span>&deg;<small style="font-size:2vw; padding: 3vw 5vw 0vw .3vw;"><sup>f</sup></small>
      </div>

      <div style="padding: .7vw 0vw;">
          <span><i class="fas fa-tint" style="color:#00add6"></i>Humid</span>
      </div>
      <div style="padding: .7vw 0vw;">
          <span id="h1" style ="padding: 4vw 0vw;">h</span>%
      </div>
      <div class="indicators">
        <span style="width: 10vw; font-weight: bolder;">Heat</span><br>
        <span id="heat1"><i class="fas fa-toggle-on" style="color: green;"></i>on</span>
      </div>
      <div class="indicators">
        <span style="width: 13vw; font-weight: bolder;">Humid</span><br>
        <span id="humid1"><i class="fas fa-toggle-off" style="color:red;"></i>off</span>
      </div>
      <div></div><div></div>
      <div class="refresh">
        <br>
        <span><input type="submit" value="Refresh" onclick="f_refresh()" style="background:rgb(0, 255, 50);"></span>
      </div>
  </main>
  <main>
     <!-- comment: NOTES -->
      <span id="help" hidden="true">
        <br>
        <main class="settings-grid">
            <div><span class="help"
              value="Controller Limits"
              type="submit"
              onclick="f_redirect('settings')"
              style="margin-left:10w;background: rgb(155, 219, 149)"
              title="Web Page for setting Fan ON/OFF Parameters"
              >Controller Limits</span>
            </div>
            <div><span class="help"
              value="WiFi Settings"
              type="submit"
              onclick="f_redirect('wifi')"
              style="background: orange;"
              title="Web Page for setting WiFi SSID/PWD & IP"
              >WiFi Settings</span>
            </div>
            <div><span class="help"
              value="Firmware"
              type="submit"
              onclick="f_redirect('update')"
              style="background:rgb(249, 253, 0);color: red;"
              title="Web Page for updating firmware"
              >Firmware</span>
            </div>
            <div class="details">
              <details><hr>JSON Inbound
                <aside id="json1" style="background-color:white;font-weight: normal; font-size: 1.5vw; word-wrap:  break-word;">{}</aside>
                <hr>
              </details>
            </div>
        </main>
      </span>
  </main>

  <script language = "javascript" type = "text/javascript">

    let webSock       = new WebSocket('ws://'+window.location.hostname+':80');
    webSock.onopen    = function(evt){f_refresh();}
    webSock.onmessage = function(evt){f_webSockOnMessage(evt);}
    webSock.onerror   = function(evt){f_webSockOnError(evt);}
    const readings=0, settingsRefresh=1, settingsSet=2, wifiRefresh=3, wifiSet=4;

    function f_webSockOnMessage(evt){
      if(typeof evt.data === "string"){
        document.getElementById("json1").innerHTML = evt.data;
        var t='<i class="fas fa-toggle-on" style="color: green;"></i>on';
        var f='<i class="fas fa-toggle-off" style="color:red;"></i>off';
        j=JSON.parse(evt.data);
          for(var key in j[0])
            if(key=="heat1" || key=="humid1")document.getElementById(key).innerHTML=j[0][key]?t:f;else
            document.getElementById(key).innerHTML=j[0][key]==-999?"n/a":j[0][key];
        document.getElementById("json1").innerHTML=evt.data;
      }
    }
    function f_webSockOnError(evt){}
    let f_help = () => {
            if (document.getElementById("help").getAttribute("hidden")){
                document.getElementById("help").removeAttribute("hidden");
                document.getElementById("bt_help").innerHTML = "Un-Help";
            }else{
                document.getElementById("help").setAttribute("hidden", true);
                document.getElementById("bt_help").innerHTML = "Help";
            }
        };
    function f_redirect(where){window.location.href = "/"+where;}
    function f_refresh(){
      webSock.send("{src:"+readings+"}");
    }
  </script>
</html>
)=====";

extern const char SETTINGS_HTML[] PROGMEM =  R"=====(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta name="file" content="set_bounds.html">
    <meta name="author" content="Joe Belson 20221009">
    <meta name="what" content="esp32 html for WiFi connected temp/humidity bounds.">
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Set Temp / Humidity Bounds for 110VAC relays</title>
  </head>
<style type="text/css">
      body {font-size: 12px; background-color: #f6f6ff;font-family: Calibri, Myriad;text-align: center;}
      body p{text-align: left;}
      table th{text-align: center;width: 23pt;border-bottom:2px solid rgb(8, 8, 8);}
      table.calibrate {font-size:smaller ; margin-left:10px; margin-right: auto;  width: 300px;  border: 1px solid black; border-style:solid;}
      table.calibrate caption {	background-color: #f79646;	color: #fff;	font-size: large;	font-weight: bold;	letter-spacing: .3em;}
      table.calibrate thead th {
                  padding: 3px;
                  background-color: #fde9d9;
                  font-size:16px;
                  border-width: 1px;
                  border-style: solid;
                  border-color: #f79646 #ccc;
      }
      table.calibrate td {
                  font-size: 12px;
                  font-weight: bold;
                  text-align: center;
                  padding: 2px;
                  background-color:shite;
                  color:black;
                  font-weight: bold;
      }
</style>
  <body style="text-align: left;">
    <input id="help" type="button" value="Help" onclick="f_help()"/><br><br>
    <table class="calibrate">
      <caption>Set Temp & Humidity Bounds</caption>
        <thead>
            <tr>
                <th><label for="t1_on">Heat On Temp<sup>&degf</sup></label></th>
                <th><label for="t1_off">Heat Off Temp<sup>&degf</sup></label></th>
                <th><label for="h1_on">Humid On<sup>%</sup></label></th>
                <th><label for="h1_off">Humid Off<sup>%</sup></label></th>
                <th><label for="delay">Sensor<br>Delay(sec)</label></th>
            </tr>
            <tr>
                  <td><input type="number" id="t1_on"  name="limit" min="55" max="87"     onchange="f_updates()"></td>
                  <td><input type="number" id="t1_off" name="limit" min="60" max="87"     onchange="f_updates()"></td>
                  <td><input type="number" id="h1_on"  name="limit" min="50" max="87"     onchange="f_updates()"></td>
                  <td><input type="number" id="h1_off" name="limit" min="50" max="90"     onchange="f_updates()"></td>
                  <td><input type="number" id="delay"  name="limit" min="1"  max="300000" onchange="f_updates()"></td>
            </tr>
        </thead>
    </table>
    <input id="Submit" type="submit" value="Submit" onclick="f_submit()" style="margin: 10px 10px; color: black; background-color: rgb(248, 138, 171); font-weight: bold;"/>
    <input id="refresh" type="submit" value="Refresh" onclick="f_refresh()" style="margin: 1px; color: black; background-color: yellow; font-weight: bold;"/>
    
    <p name="settings"  hidden><b style="color:black">Current MCU Settings (inbound, submitted)</b></p>
    <p name="settings" id="json1" hidden>JSON1</p>
    <p name="settings" id="json2" hidden>JSON2</p>

  </body>
  <script language = "javascript" type = "text/javascript">

      let webSock       = new WebSocket('ws://'+window.location.hostname+':80');
      webSock.onopen    = function(evt){f_refresh();}
      webSock.onmessage = function(evt){f_webSockOnMessage(evt);}
      webSock.onerror   = function(evt){f_webSockOnError(evt);}
      webSock.onclose   = function(evt){f_webSockOnClose(evt);}

      let updates = false; // websock updates when false; true after user param change
      const readings=0, settingsRefresh=1, settingsSet=2, wifiRefresh=3, wifiSet=4;

      function f_webSockOnMessage(evt){
        if(updates)return;  // we will only apply the json if the corresponding form field is ""
        if(typeof evt.data === "string"){
          j=JSON.parse(evt.data);
          document.getElementById("json1").innerHTML=evt.data;
            for(var key in j[1]){
              console.log(key);
                if(j[1].hasOwnProperty(key)){
                  console.log(key);
                    if(document.getElementById(key).value=="")
                    document.getElementById(key).value=key!="delay"?j[1][key]:j[1][key]/1000;
                }
            }
        }
      }
      function f_webSockOnError(evt){}
      function f_submit(){
        var str= '[{"t1":0,"h1":0},{"t1_on":75,"t1_off":82,"h1_on":75,"h1_off":85, "delay":3000},{"src":'+settingsSet+'}]';
        console.log(str);

        j=JSON.parse(str);

        for(var key in j[1]){
          console.log(key);
            j[1][key]=document.getElementById(key).value*1;
            if(key=="delay")j[1][key]=j[1][key]*1000;
        }
        document.getElementById("json2").innerHTML = JSON.stringify(j);
        console.log("sending:", JSON.stringify(j));
        webSock.send(JSON.stringify(j));
        updates = false;  // reset after refresh/submit
      }
      function f_refresh(){
        var x=document.getElementsByName("limit");
        for(var i=0;i<x.length;i++)x[i].value="";
        updates = false;  // reset after refresh/submit
        console.log("{src:"+settingsRefresh+"}");
        webSock.send("{src:"+settingsRefresh+"}");
      }
      function f_updates(){
        updates = true;
      }
      function f_help(){
        let v = document.getElementById("help");
        let settings = document.getElementsByName("settings");

        settings.forEach((setting) => {
            setting.hidden = v.value=="Help"?false:true;
          });
          v.value=v.value=="Help"?"Un-Help":"Help";
      }
  </script>
</html>
)=====";

  const char WIFI_HTML[] PROGMEM =  R"=====(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta name="file" content="set_bounds.html">
    <meta name="author" content="Joe Belson 20210822">
    <meta name="what" content="esp8266 html for WiFi connected temp/humidity bounds.">
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Set on/off relay bounds</title>
  </head>

  <body style="text-align: left;">
    <hr>
      <table class="calibrate">
        <caption style="font-weight: bolder;">Set Temp & Humidity Relays SSID : PWD<br><br></caption>
          <thead>
            <tr style="font-size: 2.0vw;font-weight: bold;">
              <th>SSID</th>
              <th>PWD</th>
            </tr>
          </thead>
          <tbody>
              <tr>
                  <td><input type="text" id="SSID" name="cred" onclick="f_update()" placeholder="WiFi SSID"/></td>
                  <td><input type="text" id="PWD" name="cred" onclick="f_update()" placeholder="WiFi PWD"/></td>
                  <td><input type="submit" id="submit" onclick="f_submit()" hidden = isDHCP.checked></td>
              </tr>
          </tbody>
      </table>
      <br>
      <label for="dhcp">DHCP:</label><input type="checkbox" id="isDHCP" onclick="f_dhcp(checked)">
      <table><tr><td></td></tr></table>
      <table name="dhcp" class="calibrate">
        <tr style="font-size: 2.0vw;font-weight: bold;">
          <td><label for="IP">IP</label></td>
          <td><label for="GW">Gateway</label></td>
          <td><label for="MASK">Mask</label></td>
        </tr>
        <tr>
          <td>
               <input type="text"  id="IP"
                style="width: 7rem;" onclick="f_update()" placeholder="Fixed IP"
                minlength="7" maxlength="15" size="15"
                required pattern="^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$">
            </td>
            <td>
              <input type="text" id="GW"
                style="width: 7rem;" onclick="f_update()" placeholder="Network Gateway"
                minlength="7" maxlength="15" size="15"
                required pattern="^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$">
            </td>
            <td>
              <input type="text" id="MASK"
              style="width: 7rem;" onclick="f_update()" placeholder="Network Mask"
              minlength="7" maxlength="15" size="15"
              required pattern="^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$">
            </td>
            <td><input type="submit" id="submit2" style="width:4.5rem;" onclick="f_submit()"/></td>
          </tr>
        </table>
        <br>
        <input type="submit" value="Refresh" style="width:4.5rem;" onclick="f_refresh()"/>
    </body>
    <script language = "javascript" type = "text/javascript">
  
        let webSock       = new WebSocket('ws://'+window.location.hostname+':80');
        webSock.onopen    = function(evt){f_refresh();}
        webSock.onmessage = function(evt){f_webSockOnMessage(evt);}
        webSock.onerror   = function(evt){f_webSockOnError(evt);}
        const readings=0, settingsRefresh=1, settingsSet=2, wifiRefresh=3, wifiSet=4;
        var str= '{"SSID":"0","PWD":"0", "IP":"0","GW":"0","MASK":"0","isDHCP":false}';
        let updates = false;

        function f_webSockOnMessage(evt){// need to get the inbound json to fill in the form
          if(updates)return;
          if(typeof evt.data === "string"){
              const j=JSON.parse(evt.data);
              if(!j.hasOwnProperty("MASK"))return;
              for(let key in j){
                let element = document.getElementById(key);
                if(element == "null")continue;
                switch(key){
                    case "isDHCP":
                      element.checked = j[key];
                      f_dhcp(j[key]);
                      continue;
                    case "PWD": element.value = "**********";continue;
                    default: element.value=j[key]==-999?"n/a":j[key];
              }
           }
          }
        }
        function f_webSockOnError(evt){}
        function f_refresh(){
          updates=false;
          webSock.send("{src:"+wifiRefresh+"}");
        }
        function f_update(){
          updates = true;
        }
        function f_submit(){
          j=JSON.parse(str);
          for(var key in j){
            if(key=="isDHCP")j[key]=document.getElementById(key).checked;
            else j[key]=document.getElementById(key).value;
          }
          j.src=wifiSet;
          webSock.send(JSON.stringify(j));
          updates = false;
        }
        function f_dhcp(checked){
          const n=document.getElementsByName("dhcp");
          if(checked){
            for(var i=0;i<n.length;i++)n[i].setAttribute("hidden", "hidden");
            document.getElementById("submit").removeAttribute("hidden");
          }else{
            for(var i=0;i<n.length;i++)n[i].removeAttribute("hidden");
            document.getElementById("submit").setAttribute("hidden", "hidden");
          }
          updates=true;
        }
     </script>
  </html>
      )=====";
