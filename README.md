# Avem :bird:
[![Build Status](https://travis-ci.org/but0n/Avem.png)](https://travis-ci.org/but0n/Avem)

[UAV] - [STM32F103] - 6 Axes sensor MPU6050 - Quaternion &amp; Euler Angles Compute - PID Controller

# Todolist
* Software **I2C**
* Init **MPU6050**
* Get 3 axes `Gryo` and 3 axes `Accel` data from MPU6050
* Cumpute Quaternion and **Euler** Angles
* **PWM** output
* Driving motors with **L9110S**
* **PID** Control
* Schematic
* PCB Layout (*Now I'm watting for PCB*)
* 使用PWM外接商品电调驱动无刷电机-[Video on YouTube](https://youtu.be/iHYVgTmxoSw)
	* **Wi-Fi**(`ESP8266`)
	* PCB Test (Failed:sad:)
	* PID debug
	* Design **frame**
	* **GPS**
	* 3D print or CNC cut
	* ... ...

----

# Software

:octocat:[but0n/MPU6050_stm32](https://github.com/but0n/MPU6050_stm32)

#Hardware

## 3D Moudle

![](rm_img/PCB/3dpcb.png)

## PCB Layout

![](rm_img/PCB/pcb.png)

## Schematic

### Microcontroller -- STM32F103
![](rm_img/stm.png)

### 3.3v Power supply (DC2DC)
![](rm_img/1117.png)

### RESET
![](rm_img/reset.png)

### OSC
![](rm_img/osc.png)

### USART In-System Programming
![](rm_img/usart.png)

### Motor driver
![](rm_img/l9110.png)

### MPU6050
![](rm_img/mpu6050.png)

### MOTORS 4-CHANNEL LEFT & 4-CHANNEL RIGHT
![](rm_img/motors.png)
