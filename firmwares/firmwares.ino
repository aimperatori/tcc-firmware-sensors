// Placa: ESP32 Dev Module - v1.0.3
// Biblioteca - DHTStable - v1.0.1
// Biblioteca - WiFi - v1.2.1
// Biblioteca - PubSubClient - v2.4.0


#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "DHTStable.h"


#define MQ2_PINOUT 34
#define PIR_PINOUT 27
#define DHT_PINOUT 4

#define DELAY 2000


WiFiClient espClient;
PubSubClient mqttClient(espClient);
DHTStable DHT;

unsigned long lastMsg = 0;
boolean movDetec;


void IRAM_ATTR motionDetected() 
{
  movDetec = true;
}

void motionDetectedSendMQTT()
{
  if(movDetec)
  {
    char movement[1] = {'1'};
    
    Serial.println("MOTION DETECTED!!!");
    mqttClient.publish("movement", movement);

    movDetec = false;
  }  
}

void readMQ2Sensor() {
  char gas_smoke[5];
 
  snprintf(gas_smoke, 5, "%d", analogRead(MQ2_PINOUT));
  
  Serial.print("MQ2: ");
  Serial.println(gas_smoke);
  mqttClient.publish("gas_smoke", gas_smoke);
}

void readDHTSensor() {
  char temp[5], humid[5];

  Serial.print("DHT11 ");
  int chk = DHT.read11(DHT_PINOUT);
  switch (chk)
  {
    case DHTLIB_OK:  
      Serial.print("OK\t"); 
      break;
    case DHTLIB_ERROR_CHECKSUM: 
      Serial.print("Checksum error,\t"); 
      break;
    case DHTLIB_ERROR_TIMEOUT: 
      Serial.print("Time out error,\t"); 
      break;
    default: 
      Serial.print("Unknown error,\t"); 
      break;
  }

  snprintf(temp, 5, "%f", DHT.getTemperature());
  snprintf(humid, 5, "%f", DHT.getHumidity());

  Serial.print("Temperature: ");
  Serial.print(temp);
  mqttClient.publish("temperature", temp);

  Serial.print("\tHumidity: ");
  Serial.println(humid);
  mqttClient.publish("humidity", humid);
}

void setup_wifi()
{
  delay(10);
  
  // We start by connecting to a WiFi network
  Serial.print("\nConnecting to ");
  Serial.println(SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(WiFi.status());
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  // Loop until we're reconnected
  while (!mqttClient.connected())
  {
    Serial.print("Attempting MQTT connection...");
    
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    // Attempt to connect
    if (mqttClient.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) 
    {
      Serial.println("connected");
    }
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() 
{
  // MQ2
  pinMode(MQ2_PINOUT, INPUT);

  Serial.begin(115200);
  
  setup_wifi();
  
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);

  // PIR
  pinMode(PIR_PINOUT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIR_PINOUT), motionDetected, RISING);

  // MOVEMENT
  movDetec = false;
}

void loop() 
{
  if (!mqttClient.connected()) 
  {
    reconnect();
  }
  mqttClient.loop();

  unsigned long now = millis();
  
  if (now - lastMsg > DELAY)
  {
    lastMsg = now;
    
    readMQ2Sensor();

    readDHTSensor();    
  }

  motionDetectedSendMQTT();
}
