const int LED_PIN = 13;    // LED Pin
const int BUTTON_PIN = 2;  // Button Pin

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // interner Pull-up Widerstand
}

void loop() {
  // --- Eingehende Serial-Daten (von Node-Red) ---
  if (Serial.available() > 0) {
    String incoming = Serial.readStringUntil('\n');
    incoming.trim();

    if (incoming == "1") {
      digitalWrite(LED_PIN, HIGH);  // LED an
    } else if (incoming == "0") {
      digitalWrite(LED_PIN, LOW);   // LED aus
    }
  }

  // --- Button-Zustand an Node-Red senden ---
  int buttonState = digitalRead(BUTTON_PIN);

  // INPUT_PULLUP: LOW = gedrückt, HIGH = nicht gedrückt → invertieren
  if (buttonState == LOW) {
    Serial.println("1");  // Button gedrückt
  } else {
    Serial.println("0");  // Button nicht gedrückt
  }

  delay(100); // kurze Pause um Serial nicht zu überfluten
}