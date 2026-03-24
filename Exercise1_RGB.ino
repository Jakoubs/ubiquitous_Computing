#include <WiFiNINA.h> 

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // declare pin 9 to be an output:
  digitalWrite(LEDR, HIGH);
  delay(500);
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, HIGH);
  delay(500);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, HIGH);
  delay(500);
  digitalWrite(LEDB, LOW);
}
