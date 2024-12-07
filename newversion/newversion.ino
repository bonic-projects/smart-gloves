 #include <Arduino.h>
#include <WiFi.h>
#include <BluetoothSerial.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include "Audio.h"

// Define pins and variables for the Flex sensors and MPU
int flexPins[] = {33, 32, 35, 34, 39};
int flexADC[5] = {0};
int sensorMin[5] = {4095, 4095, 4095, 4095, 4095};
int sensorMax[5] = {0, 0, 0, 0, 0};
float flexAngles[5];

float accelX, accelY, accelZ;
float gyroX, gyroY, gyroZ;

// BluetoothSerial object for Bluetooth communication
BluetoothSerial SerialBT;
Adafruit_MPU6050 mpu;
Audio audio;

#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26

// Timer variables
unsigned long sendDataPrevMillis = 0;

void setup() {
  SerialBT.begin("Sign-language");
  Serial.begin(115200);

esp_wifi_set_rx_buffer_num(8);  // Increase buffer number (default is 4)
esp_wifi_set_rx_buffer_size(2048);  // Increase buffer size (default is 1500)

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin( "Autobonics_4G", "autobonics@27");
  // Initialize flex sensors
  for (int i = 0; i < 5; i++) {
    pinMode(flexPins[i], INPUT);
    Serial.print("Calibrating flex sensor connected to pin ");
    Serial.println(flexPins[i]);
  }

  // Calibrate flex sensors
  unsigned long calibrationStartTime = millis();
  while (millis() - calibrationStartTime < 10000) {
    for (int i = 0; i < 5; i++) {
      flexADC[i] = analogRead(flexPins[i]);
      sensorMin[i] = min(sensorMin[i], flexADC[i]); // Update minimum value
      sensorMax[i] = max(sensorMax[i], flexADC[i]); // Update maximum value
    }
    delay(100); // Stability delay
  }

  // MPU initialization
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }
  Serial.println("MPU6050 Found!");

  // Configure MPU6050 sensor settings
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
}

void loop() {
  listenBluetooth();  // Listen for Bluetooth data
  readmpu();
  readflex();
  update();
  audio.loop();
}

void readmpu() {
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  accelX = accel.acceleration.x;
  accelY = accel.acceleration.y;
  accelZ = accel.acceleration.z;

  gyroX = gyro.gyro.x;
  gyroY = gyro.gyro.y;
  gyroZ = gyro.gyro.z;
}

void readflex() {
  for (int i = 0; i < 5; i++) {
    flexADC[i] = analogRead(flexPins[i]);
    flexADC[i] = constrain(flexADC[i], sensorMin[i], sensorMax[i]);
    flexAngles[i] = ::map(flexADC[i], sensorMin[i], sensorMax[i], 0, 90);
  }
}

void update() {
  if (millis() - sendDataPrevMillis > 2000) {
    sendDataPrevMillis = millis();

    // Create JSON object to store the data
    String jsonData = "{";
    jsonData += "\"little\":" + String(flexAngles[0]) + ",";
    jsonData += "\"ring\":" + String(flexAngles[1]) + ",";
    jsonData += "\"middle\":" + String(flexAngles[2]) + ",";
    jsonData += "\"pointer\":" + String(flexAngles[3]) + ",";
    jsonData += "\"thumb\":" + String(flexAngles[4]) + ",";
    jsonData += "\"accel_x\":" + String(accelX) + ",";
    jsonData += "\"accel_y\":" + String(accelY) + ",";
    jsonData += "\"accel_z\":" + String(accelZ) + ",";
    jsonData += "\"gyro_x\":" + String(gyroX) + ",";
    jsonData += "\"gyro_y\":" + String(gyroY) + ",";
    jsonData += "\"gyro_z\":" + String(gyroZ);
    jsonData += "}";

    // Send JSON data via Bluetooth
    SerialBT.println(jsonData);
    Serial.println(jsonData);
  }
}

// Function to listen for Bluetooth data
void listenBluetooth() {
  if (SerialBT.available()) {
    String incomingData = SerialBT.readString();
    Serial.println(incomingData);
    audio.connecttospeech("word recived", "en");
    audio.connecttospeech(incomingData.c_str(), "en");
    
  }
}
