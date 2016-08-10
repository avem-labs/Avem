/**
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Description of the USB VirtualComport_Loopback  Demo.
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
The aim of this example is to read data from and write data to USB devices using the CDC protocol. 
It makes the USB device look like a serial port (NO serial cable connectors:You can see the data
transferred to and from via USB instead of USB-to-USART bridge connection). 
This example loops back the contents of a text file over usb port. To run the example, Type a message using the Pc's keyboard.
Any data that shows in HyperTerminal is received from the device.
This VirtualComport_Loopback Demo provides the firmware examples for the STM32F10xxx, STM32L15xxx,
STM32F30xxx and STM32F37xxx families.

- OUT transfers (receive the data from the PC to STM32):
	When a packet is received from the PC on the OUT pipe (EP3), EP3_IN_Callback will process the receive data.
	To receive something to the STM32 data are stored in the Receive_Buffer[] by calling CDC_Receive_DATA() 
 
- IN transfers (to send the data received from the STM32 to the PC):
	When a packet is sent from the STM32 on the IN pipe (EP1), EP1_IN_Callback will process the send data.
	To send something to the HOST we put the data in to the Send_Buffer[] buffer and call CDC_Send_DATA() 

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

  - STM3210E-EVAL Set-up 
     - Jumper JP14 (USB disconnect) should be connected in position 2-3.

  - STM32L152-EVAL Set-up 
     - Jumper JP19 should be not connected. 
  
  - STM32L152D-EVAL Set-up 
	- None	
    
  - STM32F373C_EVAL  Set-up 
	- None

  - STM32F303C_EVAL  Set-up 
	- None

How to use it
=============
 + EWARM
    - Open the VirtualComport_Loopback.eww workspace.
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
    - Open the VirtualComport_Loopback.Uv2 project
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
    - Open the VirtualComport_Loopback.rprj project.
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
