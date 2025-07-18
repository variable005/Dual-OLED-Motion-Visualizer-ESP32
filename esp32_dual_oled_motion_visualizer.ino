#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 oled1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);  // 0x3C
Adafruit_SSD1306 oled2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);  // 0x3D
Adafruit_MPU6050 mpu;

#define BUTTON_PIN 15

float centerPitch = 0;
float centerRoll = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if (!oled1.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED1 not found");
    while (1);
  }

  if (!oled2.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    Serial.println("OLED2 not found");
    while (1);
  }

  if (!mpu.begin()) {
    oled1.clearDisplay();
    oled1.setCursor(0, 0);
    oled1.println("MPU6050 FAIL");
    oled1.display();
    while (1);
  }

  oled1.clearDisplay();
  oled1.setTextSize(1);
  oled1.setTextColor(SSD1306_WHITE);
  oled1.setCursor(0, 0);
  oled1.println("OLED1 READY");
  oled1.display();

  oled2.clearDisplay();
  oled2.setTextSize(1);
  oled2.setTextColor(SSD1306_WHITE);
  oled2.setCursor(0, 0);
  oled2.println("Project: Dual OLED IMU");
  oled2.println("By: Hariom");
  oled2.display();

  delay(1500);  // Show intro for 1.5 seconds
}

void loop() {
  sensors_event_t acc, gyro, temp;
  mpu.getEvent(&acc, &gyro, &temp);

  float pitch = atan2(acc.acceleration.y, acc.acceleration.z) * 180 / PI;
  float roll = atan2(-acc.acceleration.x, sqrt(acc.acceleration.y * acc.acceleration.y + acc.acceleration.z * acc.acceleration.z)) * 180 / PI;

  if (digitalRead(BUTTON_PIN) == LOW) {
    centerPitch = pitch;
    centerRoll = roll;
    delay(500);
  }

  float adjPitch = pitch - centerPitch;
  float adjRoll = roll - centerRoll;

  int joyX = map((int)adjRoll, -90, 90, 0, SCREEN_WIDTH - 1);
  int joyY = map((int)adjPitch, -90, 90, SCREEN_HEIGHT - 1, 0);

  // --- OLED1: X/Y graph ---
  oled1.clearDisplay();
  oled1.drawLine(0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2, SSD1306_WHITE);
  oled1.drawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, SSD1306_WHITE);
  oled1.fillCircle(joyX, joyY, 2, SSD1306_WHITE);
  oled1.display();

  // --- OLED2: Detailed info ---
  oled2.clearDisplay();
  oled2.setTextSize(1);
  oled2.setCursor(0, 0);
  oled2.println("Project: Dual OLED IMU");
  oled2.println("By: Hariom");
  oled2.println();

  oled2.print("Pitch: ");
  oled2.println(adjPitch, 1);
  oled2.print("Roll : ");
  oled2.println(adjRoll, 1);
  oled2.println();

  oled2.print("AccX: ");
  oled2.println(acc.acceleration.x, 2);
  oled2.print("AccY: ");
  oled2.println(acc.acceleration.y, 2);
  oled2.print("AccZ: ");
  oled2.println(acc.acceleration.z, 2);
  oled2.println();

  oled2.print("GyroX:");
  oled2.println(gyro.gyro.x, 2);
  oled2.print("GyroY:");
  oled2.println(gyro.gyro.y, 2);
  oled2.print("GyroZ:");
  oled2.println(gyro.gyro.z, 2);
  oled2.println();

  oled2.print("Temp : ");
  oled2.print(temp.temperature, 1);
  oled2.println(" C");

  oled2.display();

  delay(100);
}
