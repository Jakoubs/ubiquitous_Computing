#include <Arduino_LSM6DSOX.h>
#include <WiFiNINA.h>


// Onboard RGB LED (richtige Pins für RP2040)
#define LEDR 22
#define LEDG 23
#define LEDB 24

int temperature = 0;

void setup() {
  Serial.begin(9600);
  delay(2000); // wichtig statt while(!Serial)

  Serial.println("Temperature Sensor Test");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
}

void loop() {
  if (IMU.temperatureAvailable()) {
    IMU.readTemperature(temperature);

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    setRGB(temperature);
  }

  delay(1000);
}

// RGB LED Steuerung
void setRGB(int temp) {
  if (temp > 32) {          // heiß → rot
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);
    digitalWrite(LEDR, HIGH);
    
  } 
  else if (temp >= 20) {    // angenehm → grün
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDB, LOW);
    digitalWrite(LEDG, HIGH);
    
  } 
  else {                    // kalt → blau
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, HIGH);
  }
}