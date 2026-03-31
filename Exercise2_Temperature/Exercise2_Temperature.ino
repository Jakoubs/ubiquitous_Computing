#include <WiFiNINA.h>

String incoming = "";

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
while(Serial1.available()){
    delay(2);
    char c = Serial1.read();
    incoming += c;
    Serial1.write("Hello world!");
  }
}
