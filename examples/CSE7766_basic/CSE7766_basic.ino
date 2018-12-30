/*
CSE7766_basic

Created stand alone library for the Sonoff Pow R2 power switch with energy monotor witch uses the CSE7766 as power monitor sensor.
Based upon the excelent espurna platform of Xose Pérez "Home automation firmware for ESP8266-based devices"

https://github.com/xoseperez/espurna/blob/dev/code/espurna/sensors/CSE7766Sensor.h

ATTENTION!

The serial port ground from the Sonoff device is directly connected to AC-mains..
Be carfull it could potentially destroy your devices if connected directly!

For this reason I using a telnet as remote debug connection from Joao Lopes and OTA updates.
https://github.com/JoaoLopesF/RemoteDebug


Copyright (C) 2016-2018 by Xose Pérez <xose dot perez at gmail dot com>
Copyright (C) 2018 by Ingeniuske  <ingeniuske at gmail dot com>


This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "CSE7766.h"
#include "debug.h"
CSE7766 myCSE7766;

#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>

#include "RemoteDebug.h"        //https://github.com/JoaoLopesF/RemoteDebug
RemoteDebug Debug;

// GPIOs
#define RELAY_PIN       12
#define LED             13  

// Check values every 2 seconds
#define UPDATE_TIME                     2000



// Enable debug prints to telnet
#ifndef TEL_DEBUG
#define TEL_DEBUG                    1
#endif
// Debug sensors
#ifndef SENSOR_DEBUG
#define SENSOR_DEBUG                 1
#endif


// SSID and password
const char* ssid = "your ssid goes here";
const char* password = "your password goes here";
// Host mDNS
#define HOST_NAME "remotedebug-CSE7766"


void debugSend(const char * format, ...) {

    va_list args;
    va_start(args, format);
    char test[1];
    int len = ets_vsnprintf(test, 1, format, args) + 1;
    char * buffer = new char[len];
    ets_vsnprintf(buffer, len, format, args);
    va_end(args);

    DEBUG_MSG(buffer);
    delete[] buffer;

}

void setup() {

    // Initialize
    myCSE7766.setRX(1);
    myCSE7766.begin(); // will initialize serial to 4800 bps

    // Debug
    Serial.println("**** Setup: initializing ...");
    // WiFi connection
    WiFi.begin(ssid, password);
    Serial.println("");
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Register host name in WiFi and mDNS

    String hostNameWifi = HOST_NAME;
    hostNameWifi.concat(".local");
    WiFi.hostname(hostNameWifi);
    if (MDNS.begin(HOST_NAME)) {
        Serial.print("* MDNS responder started. Hostname -> ");
        Serial.println(HOST_NAME);
    }
    MDNS.addService("telnet", "tcp", 23);

    // Initialize the telnet server of RemoteDebug
    Debug.begin(HOST_NAME); // Initiaze the telnet server
    Debug.setResetCmdEnabled(true); // Enable the reset command

    Serial.println("* Arduino RemoteDebug Library");
    Serial.println("*");
    Serial.print("* WiFI connected. IP address: ");
    Serial.println(WiFi.localIP());

    ArduinoOTA.onStart([]() {
    Serial.println("ArduinoOTA start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nArduinoOTA end");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();

  // Close the relay to switch on the load
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  // initialise digital pin LED_SONOFF as an output.
  pinMode(LED, OUTPUT);
}


void loop()
{
    static int mLedState = 0;
    static unsigned long mLastTime = 0;
    // Remote update
    ArduinoOTA.handle();

    // Remote debug over telnet
    Debug.handle();

    if ((millis() - mLastTime) >= UPDATE_TIME) {

        // Time
        mLastTime = millis();

        // toggle led
        mLedState ^=1;
        digitalWrite(LED, mLedState);

        // read CSE7766
        myCSE7766.handle();

       
        #if TEL_DEBUG
        DEBUG_MSG("Voltage %.4f V\n", myCSE7766.getVoltage());
        DEBUG_MSG("Current %.4f A\n", myCSE7766.getCurrent());
        DEBUG_MSG("ActivePower %.4f W\n", myCSE7766.getActivePower());
        DEBUG_MSG("ApparentPower %.4f VA\n", myCSE7766.getApparentPower());
        DEBUG_MSG("ReactivePower %.4f VAR\n", myCSE7766.getReactivePower());
        DEBUG_MSG("PowerFactor %.4f %\n", myCSE7766.getPowerFactor());
        DEBUG_MSG("Energy %.4f Ws\n", myCSE7766.getEnergy());
        #endif

     }
}
