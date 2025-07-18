# ESP32 Dual OLED Motion Visualizer

> _“Simplicity is not the absence of complexity, but the clarity of its arrangement.”_

---

## Project Overview

A fully simulated embedded system built around the **ESP32-C6**, the **MPU6050** IMU, and **two SSD1306 OLED displays** operating on a **shared I2C bus**. Developed entirely within the **Wokwi simulator**, this project demonstrates real-time motion tracking, sensor data visualization, and display coordination without needing any physical hardware.

---

## Key Features

- **Dual Display Interface**:
  - **OLED 1 (Left):** Visualizes orientation as a **2D joystick-like dot**, representing pitch and roll.
  - **OLED 2 (Right):** Outputs filtered sensor telemetry — accelerometer (g), gyroscope (°/s), temperature (°C), and orientation (degrees).
- **Shared I2C Line Handling**: Three I2C devices (2 OLEDs + 1 MPU6050) work seamlessly using distinct I2C addresses on GPIO 21 (SDA) and GPIO 22 (SCL).
- **Live Sensor Processing**:
  - Raw sensor values are scaled, filtered, and formatted for readability.
  - Derived pitch and roll calculated using basic trigonometry on accelerometer values.
- **Display Timing Arbitration**:
  - OLED updates are intelligently paced to maintain readability.
  - Sensor data polling occurs at a faster rate, ensuring responsiveness.
- **Modular, Extensible Code**:
  - Clear separation between sensor logic, display rendering, and loop timing.
  - Fully customizable rendering styles and telemetry formats.

---

## Technical Breakdown

### ➤ Sensor Input (MPU6050)

- **Interface:** I2C  
- **Functions Used:**
  - `getAcceleration()`
  - `getRotation()`
  - `getTemperature()`
- **Smoothing Technique:**
  - Simple moving average over multiple reads to reduce jitter.
- **Orientation Computation:**
  - Pitch = `atan2(Ax, sqrt(Ay² + Az²))`
  - Roll  = `atan2(Ay, sqrt(Ax² + Az²))`

### ➤ Display Logic

- **OLED 1:**
  - Maps pitch and roll angles to X,Y dot position on screen center (64x32 origin).
  - Dot reflects tilt direction and magnitude.
- **OLED 2:**
  - Displays:
    - Acceleration (X, Y, Z)
    - Gyro rates (X, Y, Z)
    - Pitch & Roll angles
    - Temperature
  - Uses `Adafruit_SSD1306` text mode with mono 6×8 font.

### ➤ I2C Coordination

- **Bus Lines:**
  - **SDA:** GPIO 21  
  - **SCL:** GPIO 22  
- **Addresses:**
  - **MPU6050:** `0x68`
  - **OLED 1:** `0x3C`
  - **OLED 2:** `0x3D` (configured in software)

### ➤ Timing Strategy

- **Sensor Polling:** Every ~50ms  
- **Display Updates:**
  - OLED 1 (Dot visual): Every ~100ms
  - OLED 2 (Textual): Every ~250ms
- **Non-blocking Loop Design:** No use of `delay()`, instead uses `millis()` time checks.

---

## Hardware Simulation Setup

| Component             | Description                                           |
|-----------------------|-------------------------------------------------------|
| **ESP32-C6 DevKit**   | Central microcontroller (Wokwi supported)             |
| **MPU6050 IMU**       | 6-axis motion sensor with accelerometer + gyro        |
| **SSD1306 OLED (x2)** | 128×64 monochrome I2C displays                        |
| **Tactile Button**    | Connected but unused (future interaction potential)   |
| **I2C Lines**         | Shared SDA (GPIO 21) and SCL (GPIO 22)                |

---

## Potential Extensions

- **Gesture Recognition Engine**
- **Serial/Bluetooth Telemetry Streaming**
- **ESP-NOW or Wi-Fi Based Remote Monitoring**
- **Physical Deployment on Wearables or Robotics**
- **Tilt-Based Game Controls / UI Input**
- **Data Logging on SD Card or Cloud**

---

> Developed entirely in Wokwi — no physical hardware used.  
> _Built with clarity, not complexity._  

**Created by Hariom Sharnam
