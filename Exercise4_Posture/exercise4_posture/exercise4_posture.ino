#include <MadgwickAHRS.h>
#include <Arduino_LSM6DSOX.h>
#include <WiFiNINA.h>

Madgwick filter;
unsigned long microsPerReading, microsPrevious;

// Thresholds
#define POSTURE_THRESHOLD_LOW 80.0
#define POSTURE_THRESHOLD_HIGH 100.0

#define TEMP_HIGH_THRESHOLD 28.0 

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // RGB LED pins
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  setRGB(false, false, false); // all off

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  filter.begin(104); //NANO HZ
  microsPerReading = 1000000 / 25; // 25 Hz
  microsPrevious = micros();
}

void loop() {
  float ax, ay, az;
  float gx, gy, gz;
  float temp;
  unsigned long microsNow = micros();

  if (microsNow - microsPrevious >= microsPerReading) {

    // Read IMU
    if (IMU.accelerationAvailable())
      IMU.readAcceleration(ax, ay, az);
    if (IMU.gyroscopeAvailable())
      IMU.readGyroscope(gx, gy, gz);

    // Read temperature
    if (IMU.temperatureAvailable()) {
      int rawTemp;
      IMU.readTemperature(rawTemp);
      temp = (float)rawTemp;
    }

    // Update Madgwick filter
    filter.updateIMU(gx, gy, gz, ax, ay, az);
    //To degrees
    float pitch = atan2(ay, az) * RAD_TO_DEG;

    // Evaluate posture and temperature
    bool badPosture = (pitch < POSTURE_THRESHOLD_LOW || pitch > POSTURE_THRESHOLD_HIGH);
    bool highTemp   = (temp > TEMP_HIGH_THRESHOLD);

    // Serial output
    Serial.print("Pitch: ");
    Serial.print(pitch);
    Serial.print("°  Temp: ");
    Serial.print(temp);
    Serial.print("°C  Posture: ");
    Serial.print(badPosture ? "BAD" : "OK");
    Serial.print("  Temp: ");
    Serial.println(highTemp ? "TOO HIGH" : "OK");

    // RGB LED alarm:
    // Red   = bad posture
    // Blue  = temperature too high
    // Yellow (R+G) = both
    // Green = all good
    if (badPosture && highTemp) {
      setRGB(true, true, false);  // yellow
    } else if (badPosture) {
      setRGB(true, false, false); // red
    } else if (highTemp) {
      setRGB(false, false, true); // blue
    } else {
      setRGB(false, true, false); // green
    }
    microsPrevious += microsPerReading;
  }
}

void setRGB(bool r, bool g, bool b) {
  digitalWrite(LEDR, r ? HIGH : LOW);
  digitalWrite(LEDG, g ? HIGH : LOW);
  digitalWrite(LEDB, b ? HIGH : LOW);
}
