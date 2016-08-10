/**
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Description of the USB Virtual_COM_Port Demo.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


Example description
===================
In modern PCs, USB is the standard communication port for almost all peripherals. 
However many industrial software applications still use the classic COM Port (UART).
The Virtual COM Port Demo provides a simple solution to bypass this problem; it
uses the USB-FS-Device as a COM port by affecting the legacy PC application designed for
COM Port communication.
This Virtual COM Port Demo provides the firmware examples for the STM32F10xxx,STM32L15xxx,
STM32F30xxx and STM32F37xxx families.

Note that with this demo only the following configuration are supported:
-> 7bit data with parity (Even or Odd).
-> 8bit data with and without parity.

This demo is using two different methods for the IN and OUT transfers in order 
to manage the data rate difference between USB and USART buses:

 - OUT transfers (from Host to Device):
     When a packet is received from the host on the OUT pipe (EP3), the Endpoint
     callback function is called when the transfer is complete and all the received
     data are then sent through USART peripheral in polling mode. This allows all
     incoming OUT packets to be NAKed till the current packet is completely transferred
     through the USART interface.
 
 - IN transfers (from Device to Host):
     For IN data, a large circular buffer is used. USART and USB respectively write
     and read to/from this buffer independently.
     USART RXNE interrupt is used to write data into the buffer. This interrupt 
     has the highest priority, which allows to avoid overrun and data loss conditions.
     USB IN endpoint (EP1) is written with the received data into the SOF interrupt 
     callback. 
     Into this callback, a frame counter counts the number of passed frames since 
     the last time IN endpoint was written. This allows to write IN endpoint at
     separated frames when enough data are present in the data buffer. 
     To modify the number of frame between IN write operations, modify the value 
     of the define "VCOMPORT_IN_FRAME_INTERVAL" in "usb_endp.c" file.
     To allow high data rate performance, SOF interrupt is managed with highest 
     priority (thus SOF interrupt is checked before all other interrupts).     


More details about this Demo implementation is given in the User manual 
"UM0424 STM32F10xxx USB development kit", available for download from the ST
microcontrollers website: www.st.com/stm32


Directory contents
==================
 + \inc: contains the Demo firmware header files
 + \EWARM: contains preconfigured projects for EWARM toolchain
 + \RIDE: contains preconfigured projects for RIDE toolchain
 + \MDK-ARM: contains preconfigured projects for MDK-ARM toolchain
 + \TASKNIG: contains preconfigured projects for TASKING toolchain
 + \TrueSTUDIO: contains preconfigured projects for TrueSTUDIO toolchain          
 + \src: contains the Demo firmware source files


Hardware environment
====================
This example runs these STMicroelectronics evaluation boards and can be easily tailored to any other hardware: 

- STM3210B-EVAL
- STM3210E-EVAL
- STM32L152-EVAL
- STM32F373C_EVAL
- STM32F303C_EVAL
- STM32L152D-EVAL

To select the STMicroelectronics evaluation board used to run the example, uncomment
the corresponding line in platform_config.h file.

  - STM3210B-EVAL Set-up 
     - Jumper JP1 (USB disconnect) should be connected in position 2-3.
     - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN6 and Host (PC) serial port.

  - STM3210E-EVAL Set-up 
     - Jumper JP14 (USB disconnect) should be connected in position 2-3.
     - Connect a null-modem female/female RS232 cable between the DB9 connector
      CN12 and Host (PC) serial port. 

  - STM32L152-EVAL Set-up 
     - Jumper JP19 should be not connected.
     - Jumper JP5 should be connected in 2-3 position (RS232)
     - Connect a null-modem female/female RS232 cable between the DB9 connector
       CN2 and Host (PC) serial port.
     - The signal Bootloader_RESET (shared with CTS signal) HENSE USART cable with 
      RX,TX and GND lines ONLY is mandatory
  
  - STM32L152D-EVAL Set-up 
	- Jumper JP4 should be connected in 2-3 position (RS232)
	- Connect a null-modem female/female RS232 cable between the DB9 connector	
      CN1 and Host (PC) serial port.
	- In STM32L152D-EVAL RevB CTS pin is connected directly (without jumper) to MCU Reset: HENSE USART cable with 
      RX,TX and GND lines ONLY is mandatory. When using EVAL Rev A Put JP3 to position 2-3	
    
  - STM32F373C_EVAL  Set-up 
	- Jumper JP6 should be connected in 2-3 position (RS232)
        - Jumper JP7 should be connected in 1-2 position.
        - Connect a null-modem female/female RS232 cable between the DB9 connector
          CN12 and Host (PC) serial port.

  - STM32F303C_EVAL  Set-up 
	- Jumper JP13 should be connected in 2-3 position (RS232)
        - Connect a null-modem female/female RS232 cable between the DB9 connector
          CN8 and Host (PC) serial port.

How to use it
=============
 + EWARM
    - Open the VirtualCOMPort.eww workspace.
    - In the workspace toolbar select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM32L152-EVAL: to configure the project for STM32 Medium-Density Low-Power devices
        - STM32L152D-EVAL: to configure the project for STM32 High-Density Low-Power devices
        - STM32373C-EVAL:  to configure the project for STM32F37xxx devices
        - STM32303C-EVAL:  to configure the project for STM32F30xxx devices     
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM
    - Open the VirtualCOMPort.Uv2 project
    - In the build toolbar select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM32L152-EVAL: to configure the project for STM32 Medium-Density Low-Power devices
        - STM32L152D-EVAL: to configure the project for STM32 High-Density Low-Power devices
        - STM32373C-EVAL:  to configure the project for STM32F37xxx devices
        - STM32303C-EVAL:  to configure the project for STM32F30xxx devices 
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5) 

 + RIDE
    - Open the VirtualCOMPort.rprj project.
    - In the configuration toolbar(Project->properties) select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM32L152-EVAL: to configure the project for STM32 Medium-Density Low-Power devices
        - STM32L152D-EVAL: to configure the project for STM32 High-Density Low-Power devices
        - STM32373C-EVAL:  to configure the project for STM32F37xxx devices
        - STM32303C-EVAL:  to configure the project for STM32F30xxx devices  
    - Rebuild all files: Project->build project
    - Load project image: Debug->start(ctrl+D)
    - Run program: Debug->Run(ctrl+F9)

 + TASKING
    - Open TASKING toolchain.
      - Click on File->Import, select General->'Existing Projects into Workspace' 
        and then click "Next". 
      - Browse to TASKING workspace directory and select the project: 
       - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM32L152-EVAL: to configure the project for STM32 Medium-Density Low-Power devices
        - STM32L152D-EVAL: to configure the project for STM32 High-Density Low-Power devices
        - STM32373C-EVAL:  to configure the project for STM32F37xxx devices
        - STM32303C-EVAL:  to configure the project for STM32F30xxx devices  
    - Rebuild all project files: Select the project in the "Project explorer" 
        window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
        Run->Debug (F11)

 + TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace 
      directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory and select the project:  
        - STM3210B-EVAL: to load the project for STM32 Medium-density devices
        - STM3210E-EVAL: to load the project for STM32 High-density devices
        - STM3210E_EVAL_XL: to load the project for STM32 XL-density devices
        - STM32L152_EVAL: to load the project for STM32 Medium-Density Low-Power devices
        - STM32L152D-EVAL: to configure the project for STM32 High-Density Low-Power devices
        - STM32373C-EVAL:  to configure the project for STM32F37xxx devices
        - STM32303C-EVAL:  to configure the project for STM32F30xxx devices  
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)

************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE******
