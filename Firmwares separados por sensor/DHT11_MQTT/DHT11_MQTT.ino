// Placa: ESP32 Dev Modele 1.0.3
// DHT11 stable v1.0.1 

#include "DHTStable.h"
#include <WiFi.h>
#include <PubSubClient.h>

DHTStable DHT;

#define DHT11_PIN 4

//const char* ssid = "Imperatori";
//const char* password = "28102014";
//const char* mqtt_server = "10.0.0.180";
//const int   mqtt_port = 1883;
unsigned long lastMsg = 0;

//#define MQTT_USER "mqtt-user"
//#define MQTT_PASSWORD "123qwe"

//WiFiClient espClient;
//PubSubClient client(espClient);

/*
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outAnderson", "hello world");
      // ... and resubscribe
      //client.subscribe("inAnderson");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
*/

void setup()
{
  Serial.begin(115200);

//  setup_wifi();

//  client.setServer(mqtt_server, mqtt_port);
}


void loop()
{
  char temp[5], humid[5];
  
//  if (!client.connected()) {
//    reconnect();
//  }
//  client.loop();
  
  // READ DATA
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;

    Serial.print("DHT11");
    int chk = DHT.read11(DHT11_PIN);
    switch (chk)
    {
      case DHTLIB_OK:  
        Serial.println("OK\t"); 
        break;
      case DHTLIB_ERROR_CHECKSUM: 
        Serial.println("Checksum error,\t"); 
        break;
      case DHTLIB_ERROR_TIMEOUT: 
        Serial.println("Time out error,\t"); 
        break;
      default: 
        Serial.println("Unknown error,\t"); 
        break;
    }

    snprintf(temp, 5, "%f", DHT.getTemperature());
    snprintf(humid, 5, "%f", DHT.getHumidity());
    
    Serial.print("Publish message Temperature: ");
    Serial.println(temp);
//    client.publish("temperature", temp);

    Serial.print("Publish message Humidity: ");
    Serial.println(humid);
//    client.publish("humidity", humid);
  }
}
