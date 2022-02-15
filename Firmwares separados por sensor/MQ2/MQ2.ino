// FONTE: https://randomnerdtutorials.com/guide-for-mq-2-gas-smoke-sensor-with-arduino/ e https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/

#define MQ2PIN 34

void setup() {  
  pinMode(MQ2PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  int analogSensor = analogRead(MQ2PIN);

  Serial.print("MQ2: ");
  Serial.println(analogSensor);

  delay(1000);
}
