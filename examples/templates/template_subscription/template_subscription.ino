/*
 *  Adafruit - Plantilla para recibir datos
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

/*** Subscribers ***/

Adafruit_MQTT_Subscribe subscriber = Adafruit_MQTT_Subscribe(&mqtt, FEED_1, MQTT_QOS_1);

/*** Subscriber Callback ***/

void digitalCallback(char *data, uint16_t len) {
  Serial.print("Recibimos la siguiente data -> ");
  Serial.println(data);

     String message = String(data);
     message.trim();

     // tu código para procesar la data va después de esta línea
}

/*** Variables Globales ***/

QiuboESP8266 qiubo;

void setup() {
  Serial.begin(115200);

  // Conectamos a la red WiFi
  qiubo.connectToWiFi(WIFI_SSID, WIFI_PASSWORD);
  
  subscriber.setCallback(digitalCallback);
  mqtt.subscribe(&subscriber);
  
  Serial.println("Arduino Listo");
}

void loop() {
  
  if(qiubo.isConnected()) {
  
      qiubo.connectToMQTT(&mqtt); 

      // Configurarmos el timeout de recepción de paquetes
      mqtt.processPackets(500);

      // La información recibida la visualizaremos en el monitor serial
  }
}
