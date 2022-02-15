// Fonte: https://randomnerdtutorials.com/esp32-pir-motion-sensor-interrupts-timers/

const int motionSensor = 27;


void IRAM_ATTR detectsMovement() {
  
  Serial.println("MOTION DETECTED!!!");
  
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);
}

void loop() {

}
