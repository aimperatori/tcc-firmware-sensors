// Placa: ESP32 Dev Module - v1.0.3
// Biblioteca - DHTStable - v1.0.1
// Biblioteca - WiFi - v1.2.1
// Biblioteca - MQTTPubSubClient - v0.1.3


#include <WiFi.h>
#include <MQTTPubSubClient.h>
#include "config.h"
#include "DHTStable.h"


#define MQ2_PINOUT 34
#define PIR_PINOUT 27
#define DHT_PINOUT 4

#define DELAY 2000
#define DELAY_MOV 5000

WiFiClient espClient;
//PubSubClient mqttClient(espClient);
MQTTPubSubClient mqtt;
DHTStable DHT;

unsigned long lastMsg = 0,
              lastMov = 0;
boolean movDetec,
        movDetecOff;


void IRAM_ATTR motionDetected()
{
  movDetec = true;
}

void motionDetectedSendMQTT()
{
  if (movDetec)
  {
    Serial.println("MOTION DETECTED ON!");
    mqtt.publish("entrada/movimento", "on", true, MQTT_QOS);

    movDetec = false;
    movDetecOff = true;
    lastMov = millis();
  }
}

void sendMotionDetectionOff()
{
  Serial.println("MOTION DETECTED OFF!");
  mqtt.publish("entrada/movimento", "off", true, MQTT_QOS);
  movDetecOff = false;
}

void readMQ2Sensor() {
  char gas_smoke[5];

  snprintf(gas_smoke, 5, "%d", analogRead(MQ2_PINOUT));

  Serial.print("MQ2: ");
  Serial.println(gas_smoke);
  mqtt.publish("cozinha/gas_fumaca", gas_smoke, true, MQTT_QOS);
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
  mqtt.publish("cozinha/temperatura", temp, true, MQTT_QOS);

  Serial.print("\tHumidity: ");
  Serial.println(humid);
  mqtt.publish("cozinha/umidade", humid, true, MQTT_QOS);
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

void setup_mqtt()
{
  Serial.print("connecting to broker...");
  while (!espClient.connect(MQTT_SERVER, MQTT_PORT)) {
      Serial.print(".");
      delay(1000);
  }
  Serial.println(" connected!");

  // initialize mqtt client
  mqtt.begin(espClient);
}

void reconnect()
{
  // Loop until we're reconnected
  while (!mqtt.isConnected())
  {
    setup_mqtt();
    
    Serial.print("Attempting MQTT connection...");

    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (mqtt.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD))
    {
      Serial.println("connected");
    }
    else
    {
      //Serial.print("failed, rc=");
      //Serial.print(mqtt.state());
      Serial.println("failed, try again in 5 seconds");
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

  setup_mqtt();
  
  // PIR
  pinMode(PIR_PINOUT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIR_PINOUT), motionDetected, RISING);

  // MOVEMENT
  movDetec = false;

  // ON SETUP SET MOVEMENT TO OFF
  sendMotionDetectionOff();
}

void loop()
{
  if (!mqtt.isConnected())
  {
    reconnect();
  }
  mqtt.update();


  unsigned long now = millis();

  if (now - lastMsg > DELAY)
  {
    lastMsg = now;

    readMQ2Sensor();

    readDHTSensor();
  }


  if (movDetecOff && now - lastMov > DELAY_MOV)
  {
    sendMotionDetectionOff();
  }

  motionDetectedSendMQTT();
}
