/*
 *  Adafruit - Plantilla para enviar datos
 */

/** Bibliotecas **/

#include "QiuboESP8266.h"

/*** Datos de Conexión WiFi ***/

#define WIFI_SSID     "<SSID>"        // Reemplaza por el nombre de tu red
#define WIFI_PASSWORD "<PASSWORD>"    // Reemplaza por la contraseña de tu red

/*** Adafruit.io Configuración ***/

#define AIO_SERVER      "io.adafruit.com"       // io.adafruit.com
#define AIO_SERVERPORT  1883                    // use 8883 for SSL
#define AIO_USERNAME    "<username>"            // Usuario de Adafruit
#define AIO_KEY         "<API KEY>"             // API Key de Adafruit
#define FEED_PATH       AIO_USERNAME "/feeds/"

/** Identificador del Feed donde vamos a publicar **/

#define FEED_1 FEED_PATH "<feed_name>"  // <-- Reemplazar por el nombre de tu feed

/*** Configuración de Cliente MQTT ***/

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);

/*** Publishers ***/

Adafruit_MQTT_Publish publisher = Adafruit_MQTT_Publish(&mqtt, FEED_1);

/*** Variables Globales ***/

QiuboESP8266 qiubo;
unsigned long prevMillis = 0;

void setup() {
  Serial.begin(115200);

  // Conectamos a la red WiFi
  qiubo.connectToWiFi(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Arduino Listo");
}

void loop() {
  
  if(qiubo.isConnected()) {
  
      qiubo.connectToMQTT(&mqtt); 

       // Obtenemos la marca de tiempo actual
       unsigned long currentMillis = millis();

        // Validamos para publicar cada 2500 milisegundos (2.5 segundos)
       if(currentMillis - prevMillis >= 2500) {
          prevMillis = currentMillis;
          // Realizamos la publicación
          publisher.publish("Hola Mundo");
       }
  }

}
