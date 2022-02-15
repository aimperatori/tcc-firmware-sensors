// Placa: ESP32 Dev Module - v1.0.3
// Biblioteca - DHTStable - v1.0.1

#include "DHTStable.h"

#define MQ2_PINOUT 34
#define PIR_PINOUT 27
#define DHT_PINOUT 4

#define DELAY 2000


unsigned long lastMsg = 0;
DHTStable DHT;


void IRAM_ATTR motionDetected() {  
  Serial.println("MOTION DETECTED!!!");  
}

void readMQ2Sensor() {
  int analogSensor = analogRead(MQ2_PINOUT);
  
  Serial.print("MQ2: ");
  Serial.println(analogSensor);  
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

  Serial.print("\tHumidity: ");
  Serial.println(humid);  
}

void setup() {
  // MQ2
  pinMode(MQ2_PINOUT, INPUT);

  Serial.begin(115200);

  // PIR
  pinMode(PIR_PINOUT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIR_PINOUT), motionDetected, RISING);  
}

void loop() {

  unsigned long now = millis();
  
  if (now - lastMsg > DELAY) {
    lastMsg = now;
    
    readMQ2Sensor();

    readDHTSensor();    
  }
}
