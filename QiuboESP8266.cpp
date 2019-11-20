
#include "QiuboESP8266.h"

QiuboESP8266::QiuboESP8266() { /* empty constructor */ }

#define AIO_SERVER      "io.adafruit.com" // io.adafruit.com
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "qiuboedu"
#define AIO_KEY         "22d175fcd21b459887afb26fbd5d5cea"

void QiuboESP8266::println(String message)
{
  Serial.println(message);
}

void QiuboESP8266::connectToWiFi(String ssid, String password)
{
  // Conexión WIFI
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int contconexion = 0;

  while (WiFi.status() != WL_CONNECTED and contconexion < 50) { //Cuenta hasta 50 si no se puede conectar lo cancela
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) {
      //para usar con ip fija
      IPAddress ip(192,168,1,156);
      IPAddress gateway(192,168,1,1);
      IPAddress subnet(255,255,255,0);
      WiFi.config(ip, gateway, subnet);

      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.println(WiFi.localIP());
  }
  else {
      Serial.println("");
      Serial.println("Error de conexion");
  }
}

bool QiuboESP8266::isConnected()
{
  return WiFi.status() == WL_CONNECTED;
}

void QiuboESP8266::connectToMQTT(Adafruit_MQTT_Client* mqtt)
{
  int8_t ret;
  // Stop if already connected.
  if ((*mqtt).connected()) {
    return;
  }

  Serial.println("Connecting to mqtt->.. ");

  uint8_t retries = 3;
  while ((ret = (*mqtt).connect()) != 0) { // connect will return 0 for connected
       Serial.println((*mqtt).connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 10 seconds...");
       (*mqtt).disconnect();
       delay(10000);  // wait 10 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
