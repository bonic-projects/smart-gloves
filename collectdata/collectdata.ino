#include <Adafruit_MPU6050.h>
#include <Wire.h>

// Flex sensor definitions
int flexPins[] = {25, 33, 32, 35, 34}; // Flex sensor pins
int flexADC[5] = {0};                  // Current readings
int sensorMin[5] = {4095, 4095, 4095, 4095, 4095}; // Minimum readings
int sensorMax[5] = {0, 0, 0, 0, 0};               // Maximum readings
float flexAngles[5];

// MPU6050 sensor object
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200); // Start serial communication

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
      if (flexADC[i] < sensorMin[i]) {
        sensorMin[i] = flexADC[i]; // Update minimum value
      }
      if (flexADC[i] > sensorMax[i]) {
        sensorMax[i] = flexADC[i]; // Update maximum value
      }
    }
    delay(100); // Small delay for stability in sensor readings
  }

  // Print calibration results for flex sensors
  for (int i = 0; i < 5; i++) {
    Serial.print("Sensor on pin ");
    Serial.print(flexPins[i]);
    Serial.print(" - Min: ");
    Serial.print(sensorMin[i]);
    Serial.print(", Max: ");
    Serial.println(sensorMax[i]);
  }

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }
  Serial.println("MPU6050 Found!");

  // Configure MPU6050 sensor settings
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

  Serial.println("Setup complete!");
}

void loop() {
  // Read and process flex sensor data
  for (int i = 0; i < 5; i++) {
    flexADC[i] = analogRead(flexPins[i]);

    // Constrain the sensor value to the calibrated range
    flexADC[i] = constrain(flexADC[i], sensorMin[i], sensorMax[i]);

    // Map the sensor value to an angle range (0 to 90 degrees)

    float flexAngles[i] = map(flexADC[i], sensorMin[i], sensorMax[i], 0, 90);

    // Print the sensor readings and angle
    Serial.print("Flex sensor on pin ");
    Serial.print(flexPins[i]);
    Serial.print(": Value: ");
    Serial.print(flexADC[i]);
    Serial.print(", Angle: ");
    Serial.println(flexAngles[i]);
  }

  // Read MPU6050 data
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

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

  Serial.println(); // Blank line for readability
  delay(1000);      // Delay before the next set of readings
}
