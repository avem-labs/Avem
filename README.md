# Avem :bird:

![Build Status](https://travis-ci.org/avem-labs/Avem.svg?branch=develop) ![License](https://img.shields.io/github/license/but0n/Avem.svg) ![Releases](https://img.shields.io/github/release/but0n/Avem.svg) ![platform](https://img.shields.io/badge/platform-MacOS%20%7C%20Linux-orange.svg)

## Introduction

Avem is an innovative UAV project that leverages the power of the STM32F103 microcontroller combined with the 6 Axes sensor MPU6050. The project aims to provide a robust solution for UAV enthusiasts, offering features like Quaternion & Euler Angles computation and a PID Controller. Born out of a passion for flying and technology, Avem seeks to push the boundaries of what's possible in the UAV domain.

![img](https://github.com/avem-labs/Avem/raw/develop/docs/images/header.jpg)

For more details and videos, please visit [this link](http://bbs.5imx.com/forum.php?mod=viewthread&tid=1227960&extra=page%3D1).

## Installation and Setup

### Prerequisites

Before you begin, ensure you have the following installed:

- **STM32CubeIDE**: This is the official development environment for STM32. You can download it from the [official website](https://www.st.com/en/development-tools/stm32cubeide.html).
- **Git**: To clone and manage the project repository. If not installed, you can download it from [here](https://git-scm.com/).

### Steps

1. **Clone the Repository**:

   ```
   bashCopy code
   git clone https://github.com/avem-labs/Avem.git
   ```

2. **Navigate to the Project Directory**:

   ```
   bashCopy code
   cd Avem
   ```

3. **Set Up the Hardware**:

   - Connect the STM32F103 board to your computer.
   - Ensure the MPU6050 sensor is correctly connected to the board as per the I/O list.

4. **Open STM32CubeIDE**:

   - Launch STM32CubeIDE and open the Avem project.
   - Build the project to ensure there are no errors.

5. **Upload the Firmware**:

   - Connect the STM32F103 board if not already connected.
   - In STM32CubeIDE, click on the "Run" button to upload the firmware to the board.

6. **Set Up Additional Components**: If you're using additional components like Wi-Fi or GPS, ensure they are correctly connected and configured as per the project documentation.

## Usage

### Calibrating the MPU6050 Sensor

Before flying, it's crucial to calibrate the MPU6050 sensor to ensure accurate readings:

1. Place the UAV on a flat surface.
2. Power on the UAV and wait for a few seconds.
3. Follow the on-screen instructions (if any) to complete the calibration.

### Flying the UAV

1. **Power On**: Ensure the UAV's battery is fully charged and turn on the UAV.
2. **Connect to the Controller**: If using a remote controller, ensure it's paired with the UAV.
3. **Takeoff**:
   - Place the UAV on a flat surface.
   - Use the controller or the provided software interface to initiate takeoff.
4. **Flying**:
   - Use the controller's joysticks to maneuver the UAV.
   - Monitor the UAV's altitude, speed, and battery levels.
5. **Landing**:
   - Find a clear and flat area for landing.
   - Use the controller or software interface to initiate the landing sequence.
6. **Data Monitoring**:
   - The provided software interface displays real-time data from the UAV, including altitude, speed, and sensor readings.
   - Ensure you monitor these metrics to ensure safe and efficient flying.
7. **Safety Precautions**:
   - Always fly in open areas away from obstacles.
   - Do not fly in adverse weather conditions.
   - Ensure the UAV's battery levels are adequate before flying.

## I/O List

| Component | Pin              | Description   |
| --------- | ---------------- | ------------- |
| MPU6050   | `SCL` PB15       | *Description* |
| MPU6050   | `SDA` PB14       | *Description* |
| BLDC      | `Channel 1` PA6  | *Description* |
| BLDC      | `Channel 2` PA7  | *Description* |
| BLDC      | `Channel 3` PB0  | *Description* |
| BLDC      | `Channel 4` PB1  | *Description* |
| Wi-Fi     | `USART3_TX` PB10 | *Description* |
| Wi-Fi     | `USART3_RX` PB11 | *Description* |
| GPS       | TBD              | *Description* |

## To-do list

-  Software I2C
-  Init MPU6050
-  Get 3 axes `Gryo` and 3 axes `Accel` data from MPU6050
-  Compute Quaternion and Euler Angles
-  PWM output
-  ~~Driving motors with L9110S~~
-  PID Control
-  Schematic
-  PCB Layout
-  Brushless DC motor-[Video via YouTube](https://youtu.be/iHYVgTmxoSw)
-  Wi-Fi (`ESP8266`)
-  [Cascade PID control](https://github.com/but0n/Avem/blob/master/docs/README.md)
-  FreeRTOS
-  Host: Flask + pyserial
-  PID debug
-  GPS

## Hardware

![img](https://github.com/avem-labs/Avem/raw/develop/docs/images/PF.png)



### Schematic

![img](https://github.com/avem-labs/Avem/raw/develop/docs/images/pcb.png)

### PCB Layout

![img](https://github.com/avem-labs/Avem/raw/develop/docs/images/PCB/demoV3.0.JPG)

For more detailed documentation, please refer to the [Documents section](https://chat.openai.com/c/docs/).



------

Made with ♥ by but0n

------

