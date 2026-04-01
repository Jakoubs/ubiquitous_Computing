#include <SPI.h>
#include <Arduino_LSM6DSOX.h>
#include <WiFiNINA.h> 

char ssid[] = "test";
char pw[] = "ardo123ardo333";
int status = WL_IDLE_STATUS;

int mediumBorder = 20; 
int highBorder = 32; 
int temperature = 0;

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  connectServer();
  server.begin(); 
  Serial.print("Web UI ready at: http://"); Serial.println(WiFi.localIP());
  
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  
  digitalWrite(LEDR, LOW); 
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);
}

void loop() {
  checkClient();
  if (IMU.temperatureAvailable()) {
    IMU.readTemperature(temperature);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
  }
  setLed();
}

void setLed(){
  if (temperature > highBorder) {          
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);
    digitalWrite(LEDR, HIGH);
  } 
  else if (temperature >= mediumBorder) {    
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDB, LOW);
    digitalWrite(LEDG, HIGH);
  } 
  else {                    
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, HIGH);
  }
}

void connectServer(){
  while (status != WL_CONNECTED){
    Serial.println("Connect to server..");
    status = WiFi.begin(ssid,pw);
    delay(5000);
  }
  Serial.println("connected!");
}

void checkClient() {
  WiFiClient client = server.available();
  if (client) {
    String request = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;

        // Check if the HTTP header is complete
        if (c == '\n' && request.endsWith("\r\n\r\n")) {
          
          // --- MANDATORY CORS HEADERS ---
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json");
          client.println("Access-Control-Allow-Origin: *");
          client.println("Access-Control-Allow-Methods: GET, POST, OPTIONS");
          client.println("Access-Control-Allow-Headers: *");
          client.println("Connection: close");
          client.println(); // End of headers

          // 1. Handle Browser "Preflight" OPTIONS request
          if (request.indexOf("OPTIONS") != -1) {
            break; 
          }

          // 2. Extract parameters (borderMed, borderHigh)
          String action = setValue(request); 

          // 3. Send Logic Response
          if (action == "getTemp" || request.indexOf("GET /getTemp") != -1) {
            client.print("{\"temp\": ");
            client.print(temperature);
            client.print("}");
          } else if (action != "") {
            client.print("{\"status\": \"updated\", \"param\": \"" + action + "\"}");
          } else {
            client.print("{\"status\": \"connected\"}");
          }
          break; 
        }
      }
    }
    delay(1);
    client.stop(); 
  }
}

String setValue(String request) {
  int queryStart = request.indexOf('?');
  if (queryStart == -1) return ""; 

  int equalPos = request.indexOf('=', queryStart);
  int spacePos = request.indexOf(' ', equalPos);

  if (equalPos != -1 && spacePos != -1) {
    String paramName = request.substring(queryStart + 1, equalPos);
    String paramValue = request.substring(equalPos + 1, spacePos);

    Serial.println("Param: " + paramName + " | Value: " + paramValue);

    int valAsInt = paramValue.toInt();
    if (paramName == "borderMed") {
      mediumBorder = valAsInt;
    } 
    else if (paramName == "borderHigh") {
      highBorder = valAsInt;
    }
    return paramName;
  }
  return ""; 
}