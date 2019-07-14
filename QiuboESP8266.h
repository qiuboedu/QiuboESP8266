#ifndef QiuboESP8266_h
#define QiuboESP8266_h

#include "Arduino.h"
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

class QiuboESP8266
{
  public:
    QiuboESP8266();
    void println(String message);
    void connectToWiFi(String ssid, String password);
    bool isConnected();
    void connectToMQTT(Adafruit_MQTT_Client* mqtt);
};

#endif
