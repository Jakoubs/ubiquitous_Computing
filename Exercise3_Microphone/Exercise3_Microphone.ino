#include <WiFiNINA.h>
#include <PDM.h>

bool LED_SWITCH = false;

static const char channels = 1;
static const int freq = 20000; //20 Khz
short buffer[512]; //16 bit pro abtastwert
volatile int readIn;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LEDB, OUTPUT);
  while(!Serial);
  PDM.onReceive(onPDMdata);
  if(!PDM.begin(channels,freq)){
    Serial.println("failed to start PDM");
    while(1);
  }
}

void loop() {

  // put your main code here, to run repeatedly:
  if(readIn){
    for(int i = 0; i<readIn; i++){
      if(channels==2){
        Serial.print("L:");
        Serial.print(buffer[i]);
        Serial.print(" R:");
        i++;
      }
      Serial.println(buffer[i]);
      if(buffer[i] > 10000 || buffer[i] <= -10000) {
        LED_SWITCH = !LED_SWITCH;
        if(LED_SWITCH){
          Serial.println();
          digitalWrite(LEDB, HIGH);
          Serial.println("ON!");
          Serial.println();
          delay(1000);
        } else {
          Serial.println();
          digitalWrite(LEDB, LOW);
          Serial.println("OFF!");
          Serial.println();
          delay(1000);
        }
      }
    }
      readIn = 0;
  }
}

void onPDMdata() {
  int bytesAvailable = PDM.available();
  PDM.read(buffer, bytesAvailable);
  readIn = bytesAvailable / 2;
}