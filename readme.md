This ESP32 project controls two relays inteded to turn on/off a heater(69w light bulb) and a small dehumififier.
The MCU hosts three web pages and an OTA page.
The root page shows tem & humidity & relay status
The Settings page is for setting temp & humidity thresholds for turning on/off the Relays
The WiFi page allow entry of the SSID/PWD for a network and a fixed IP if so desired.

The MCU attemps to logon to the last stored SSID.
  Upon failing to log on the MCU initiates an Access Point "AP T/H: 10.0.1.14"
  Help will "uin-hide" the options for WiFi and settings and OTA buttons.

I am using this in a cabinet where I store things that are affected by humidity.

This version is for an ESP32 / there is another version for ESP8266


I have not had success in figuring out:
1) why the file system SPIFFS or LittleFS won't initiate from the ESP32
  - won't work until uploaded a "dummy" string using Arduino IDE
    and the SPIFFS or the LittleFS adding.  Using Platformio upload doesn't work either!
    Once initialized with Arduino IDE everything works as expected.

2) Just couldn't get ESP32 debugging to work.  Tried with esp-prog and with an esp32S3 USB/JTAG
   - spent over a week (videos, reading, trying, configuring,...)estimate > 80 hours. :-( never once got it.

3) Platformio.ini compile for both ESP32 & ESP8266
