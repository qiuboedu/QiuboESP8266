/*
 *  Mi Primera Aplicación con IoT
 */

// Bibliotecas

#include "DHTesp.h"
#include "QiuboESP8266.h"

/*** Datos de Conexión WiFi ***/

#define WIFI_SSID     "<SSID>"        // Reemplaza por el nombre de tu red
#define WIFI_PASSWORD "<PASSWORD>"    // Reemplaza por la contraseña de tu red

/*** Adafruit.io Configuración ***/

#define AIO_SERVER      "io.adafruit.com"       // io.adafruit.com
#define AIO_SERVERPORT  1883                    // usar 8883 para SSL
#define AIO_USERNAME    "<username>"            // Usuario de Adafruit
#define AIO_KEY         "<API KEY>"             // API Key de Adafruit

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);

/*** Publishers ***/
Adafruit_MQTT_Publish publisher = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperatura");

/*** Subscribers ***/
Adafruit_MQTT_Subscribe subscriber = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/boton", MQTT_QOS_1);

/*** Variables Globales ***/

DHTesp dht;
unsigned long prevMillis = 0;
QiuboESP8266 qiubo;

/*** Subscriber Callback ***/

void digitalCallback(char *data, uint16_t len) {
  Serial.print("Hemos recibido un dato -> ");
  Serial.println(data);

     String message = String(data);
      message.trim();
      if (message == "ON") {digitalWrite(12, HIGH);}
      if (message == "OFF") {digitalWrite(12, LOW);}
}

void setup() {

  dht.setup(D5, DHTesp::DHT11);
  pinMode(12, OUTPUT);          // D6 salida digital
  digitalWrite(12, LOW);

  Serial.begin(115200);

  subscriber.setCallback(digitalCallback);
  mqtt.subscribe(&subscriber);

  /** Indica el nombre de red y la contraseña **/

  qiubo.connectToWiFi(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Arduino Listo");
}

void loop() {

  if(qiubo.isConnected()) {

    qiubo.connectToMQTT(&mqtt);

    mqtt.processPackets(500);
    unsigned long currentMillis = millis();

    if(currentMillis - prevMillis >= 5000) {
      prevMillis = currentMillis;
      float t = dht.getTemperature();
      publisher.publish(t);
      Serial.print("Temp: ");
      Serial.println(t);
    }
  }
}
