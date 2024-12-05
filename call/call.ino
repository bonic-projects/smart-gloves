#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <TinyGPS++.h>

HardwareSerial neo(1); // Use UART 1 on ESP32
String lati = "";
String longi = "";
float speed_kmph = 0.0; 
TinyGPSPlus gps;
Adafruit_MPU6050 mpu;



void setup() {
  Serial.println("Start of setup");

  Serial.begin(115200);
  while (!Serial) delay(10);

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }
  Serial.println("MPU6050 Found!");

  // Configure sensor settings
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

  Serial.println("");
  neo.begin(9600, SERIAL_8N1, 1, 3); // Use pins 16 (RX) and 17 (TX) for UART communication with GPS module
  delay(100);
  Serial.println("End of setup");

}

void loop() {

 

  // Read sensor data
  sensors_event_t accel, gyro, temp;
mpu.getEvent(&accel, &gyro, &temp);


  // Print accelerometer data
// Print accelerometer data
Serial.print("Acceleration (X,Y,Z): ");
Serial.print(accel.acceleration.x);
Serial.print(", ");
Serial.print(accel.acceleration.y);
Serial.print(", ");
Serial.print(accel.acceleration.z);
Serial.println(" m/s^2");

// Print gyroscope data
Serial.print("Rotation (X,Y,Z): ");
Serial.print(gyro.gyro.x);
Serial.print(", ");
Serial.print(gyro.gyro.y);
Serial.print(", ");
Serial.print(gyro.gyro.z);
Serial.println(" rad/s");

// Print temperature data
Serial.print("Temperature: ");
Serial.print(temp.temperature);
Serial.println(" degC");


  Serial.println("");

   smartDelay(1000);

  Serial.println("GPS data received");
  Serial.println("Latitude: " + lati);
  Serial.println("Longitude: " + longi);
  Serial.print("Speed: ");
  Serial.print(speed_kmph);
  Serial.println(" km/h");

  delay(1000);
}

static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (neo.available()) {
      char c = neo.read();
      gps.encode(c);
    }
  } while (millis() - start < ms);

  // Update latitude, longitude, and speed
  lati = String(gps.location.lat(), 8);
  longi = String(gps.location.lng(), 8);
  speed_kmph = gps.speed.kmph();
}
