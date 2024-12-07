#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include "BluetoothSerial.h"
//#include "Audio.h"


// Firebase Configuration
#define WIFI_SSID "Autobonics_4G"
#define WIFI_PASSWORD "autobonics@27"
#define API_KEY "AIzaSyAtkIyHIoMSB2Wc1-_nV00XZodMCS1Eh6E"              // Add API key here
#define DATABASE_URL "https://smart-glove-f2b81-default-rtdb.asia-southeast1.firebasedatabase.app/"         // Add database URL here
#define USER_EMAIL "device@gmail.com"
#define USER_PASSWORD "12345678"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseData stream;
BluetoothSerial SerialBT;
//Audio audio;
Adafruit_MPU6050 mpu;

// Function Prototypes
void streamCallback(StreamData data);
void streamTimeoutCallback(bool timeout);
void constructWord(String cp);
void sendBT();
void setup();
void loop();
void readmpu();
void readflex();
void update();
//void audio_info(const char *info);


int flexPins[] = {33, 32, 35, 34,39}; // Flex sensor pins
int flexADC[5] = {0};                  // Current readings
int sensorMin[5] = {4095, 4095, 4095, 4095, 4095}; // Minimum readings
int sensorMax[5] = {0, 0, 0, 0, 0};               // Maximum readings
float flexAngles[5];

String constructedWord = "";  // Declare `constructedWord` as a global variable
String temp = "";  // Temporary variable to store the last character

float accelX, accelY, accelZ;     // Variables to store accelerometer data
float gyroX, gyroY, gyroZ;        // Variables to store gyroscope data

#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26




String uid;
String path;

unsigned long sendDataPrevMillis = 0;

void streamCallback(StreamData data) {
  Serial.println("NEW DATA!");

  String p = data.dataPath();

  Serial.println(p);
  printResult(data);  // see addons/RTDBHelper.h

  FirebaseJson jVal = data.jsonObject();
  FirebaseJsonData gesture;

  jVal.get(gesture, "gesture");

  if (gesture.success) {
    Serial.println("Success data direciton");
    String value = gesture.to<String>();
    constructWord(value);
}
}

void streamTimeoutCallback(bool timeout) {
  if (timeout)
    Serial.println("stream timed out, resuming...\n");

  if (!stream.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
}


void constructWord(String cp) {
    if (cp != temp) { // Check if the current character is different from the last one
        if (cp == "space") {
            sendBT(); // Send the constructed constructedWord
            constructedWord = "";    // Clear the constructedWord after sending
        } else {
            constructedWord += cp; // Concatenate the character to the constructedWord
        }
        temp = cp; // Update the last character
    }
}
 
void sendBT() { 
    Serial.println(constructedWord);
    SerialBT.print(constructedWord); 
    SerialBT.print(" ");
//    audio.connecttospeech(constructedWord.c_str(), "en");
}


void setup() {

  SerialBT.begin("Sign-language");
  Serial.begin(115200);

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

  // Connect to Wi-Fi
  // Adjust size as needed
//  WiFi.disconnect();
//  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nConnected with IP: " + WiFi.localIP().toString());

  // Firebase setup
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(2048);

  // Getting the user UID
  Serial.println("Getting User UID...");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  uid = auth.token.uid.c_str();
  Serial.println("User UID: " + uid);
  path = "devices/" + uid + "/reading";

    if (!Firebase.beginStream(stream, "devices/" + uid + "/prediction"))
    Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());  

  Firebase.setStreamCallback(stream, streamCallback, streamTimeoutCallback);
//
//  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
//  audio.setVolume(100);
}

void loop() {
//  audio.loop();
  readmpu();
  readflex();
  update();
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

  // Uncomment the following lines for debugging
  /*
  Serial.print("Accel (X, Y, Z): ");
  Serial.println(String(accelX) + ", " + String(accelY) + ", " + String(accelZ));
  Serial.print("Gyro (X, Y, Z): ");
  Serial.println(String(gyroX) + ", " + String(gyroY) + ", " + String(gyroZ));
  */
}

void readflex() {
  for (int i = 0; i < 5; i++) {
    flexADC[i] = analogRead(flexPins[i]);
    flexADC[i] = constrain(flexADC[i], sensorMin[i], sensorMax[i]);
    flexAngles[i] = ::map(flexADC[i], sensorMin[i], sensorMax[i], 0, 90);

    // Uncomment for debugging
    /*
    Serial.print("Flex sensor on pin ");
    Serial.print(flexPins[i]);
    Serial.print(": Value: ");
    Serial.print(flexADC[i]);
    Serial.print(", Angle: ");
    Serial.println(flexAngles[i]);
    */
  }
}

void update() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 2000)) {
    sendDataPrevMillis = millis();
    FirebaseJson json;
    json.set("little", flexAngles[0]);
    json.set("ring", flexAngles[1]);
    json.set("middle", flexAngles[2]);
    json.set("pointer", flexAngles[3]);
    json.set("thumb", flexAngles[4]);
    json.set("accel_x", accelX);
    json.set("accel_y", accelY);
    json.set("accel_z", accelZ);
    json.set("gyro_x", gyroX);
    json.set("gyro_y", gyroY);
    json.set("gyro_z", gyroZ);
    json.set(F("ts/.sv"), F("timestamp"));

    Serial.printf("Data upload %s\n",Firebase.setJSON(fbdo, path.c_str(), json) ? "success" : fbdo.errorReason().c_str());
  }
}

void audio_info(const char *info) {
  Serial.print("audio_info: "); Serial.println(info);
}
