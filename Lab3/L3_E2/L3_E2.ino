#include "Arduino_LSM6DSOX.h"  // eingebauter IMU mit Temperatursensor

const int LED_PIN = 13;
const int BUTTON_PIN = 2;
int lastButtonState = HIGH;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  IMU.begin();
}

void loop() {
  // --- Eingehende Serial-Daten (von Node-Red) ---
  if (Serial.available() > 0) {
    String incoming = Serial.readStringUntil('\n');
    incoming.trim();
    if (incoming == "1") digitalWrite(LED_PIN, HIGH);
    else if (incoming == "0") digitalWrite(LED_PIN, LOW);
  }

  // --- Button ---
  int buttonState = digitalRead(BUTTON_PIN);
  if (buttonState != lastButtonState) {
    lastButtonState = buttonState;
    if (buttonState == LOW) Serial.println("1");
    else Serial.println("0");
  }

  // --- Temperatur alle 1 Sekunde senden ---
  static unsigned long lastTemp = 0;
  if (millis() - lastTemp >= 1000) {
    lastTemp = millis();
    float temperature;
    IMU.readTemperatureFloat(temperature);
    Serial.print("T:");
    Serial.println(temperature);
  }

  delay(50);
}