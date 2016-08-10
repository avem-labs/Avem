/**
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Description of the binary directory.
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


Description
===================
This directory contains a set of sources files that build the application to be
loaded into Flash memory using device firmware upgrade (DFU).

To build such application, some special configuration has to be preformed: 
1. Set the program load address at 0x08003000, using your toolchain linker file
2. Relocate the vector table at address 0x08003000, using the "NVIC_SetVectorTable"
   function.
 
The SysTick example provided within the STM32F10xxx,STM32L152xxx, STM32F30xxx and STM32F37xxx Standard 
Peripheral examples is used as illustration. 
   
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to 72 MHz for STM32F10xxx, STM32F30xxx  and STM32F37xxx devices and to 32MHz for 
STM32L15xxx devices, the SysTick is clocked by the AHB clock (HCLK)

A "Delay" function is implemented based on the SysTick end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


Directory contents
==================

  + binary_template\EWARM: This folder contains a preconfigured project file 
                           that produces a binary image of SysTick example to be  
                           loaded with DFU.

  + binary_template\RIDE: This folder contains a preconfigured project file 
                          that produces a binary image of SysTick example to be 
                          loaded with DFU.

  + binary_template\MDK-ARM: This folder contains a preconfigured project file 
                           that produces a binary image of SysTick example to be 
                           loaded with DFU.

  + binary_template\TASKING: This folder contains a preconfigured project file 
                           that produces a binary image of SysTick example to be 
                           loaded with DFU.

 + \TrueSTUDIO: This folder contains a preconfigured project file 
                that produces a binary image of SysTick example to be 
                loaded with DFU.                                      
                  
  + binary_template\inc: contains the binary_template firmware header files 
    - stm32f10x_conf.h    Library Configuration file for STM32F10xxx devices
    - stm32_it.h      	  Header for stm32_it.c
    - stm32l15xx_conf.h    Library Configuration file for STM32L15xxx devices
    - stm32f37x_conf.h    Library Configuration file for STM32F37xxx devices
    - stm32f30x_conf.h    Library Configuration file for STM32F30xxx devices
    - main.h              Header for main.c

  + binary_template\src: contains the binary_template firmware source files 
    - main.c              Main program
    - stm32_it.c      	  Interrupt handlers for STM32F10xxx,STM32L15xxx, STM32F30xxx and STM32F37xxx devices


Hardware environment
====================
This example runs on STMicroelectronics STM3210B-EVAL, STM3210E-EVAL, STM32L152-EVAL,STM32F303C_EVAL 
,STM32F373C_EVAL and STM32L152D-EVAL evaluation boards and can be easily tailored to any other hardware.
To select the STMicroelectronics evaluation board used to run the example, uncomment
the corresponding line in platform_config.h file.

 + STM3210B-EVAL 
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PC.06, PC.07, PC.08
      and PC.09 pins
 
 + STM3210E-EVAL
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PF.06, PF.07, PF.08
      and PF.09 pins

 + STM32L152-EVAL
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PD.00, PD.01, PD.02
      and PC.12 pins
 
 + STM32L152D-EVAL
	- Use LD1, LD2, LD3 and LD4 leds connected respectively to PD.03, PD.07, PG.14
      and PG.15 pins

 + STM32F373C_EVAL
	- Use LD1, LD2, LD3 and LD4 leds connected respectively to PC.0, PC.01, PC.02
      and PC.03 pins 
 
 + STM32F303C_EVAL
	- Use LD1, LD2, LD3 and LD4 leds connected respectively to PE.08, PE.09, PE.10
      and PE.11 pins   
   
How to use it
=============
In order to load the SysTick example with the DFU, you must do the following:
 
  + EWARM
    - Open the SysTick.eww workspace
    - In the workspace toolbar select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM3210E-EVAL_XL: to configure the project for STM32 Medium-Density Low-Power devices
        - STM32L152-EVAL: to configure the project for STM32 Low Power Medium-density devices
        - STM32L152D-EVAL: to configure the project for STM32 High-Density Low-Power devices
        - STM32373C-EVAL:  to configure the project for STM32F37xxx devices
        - STM32303C-EVAL:  to configure the project for STM32F30xxx devices  
   - Rebuild all files: Project->Rebuild all
   - Go to "Utilities\Binary" and run "hextobin.bat"
   - A binary file "SysTick.bin" will be generated under "STM3210E-EVAL\Exe" or
     "STM3210B-EVAL\Exe" or "STM32L152-EVAL\Exe" or "STM32L152D-EVAL\Exe" or "STM32373C-EVAL\Exe" or "STM32373C-EVAL\Exe"  folder depending 
     on configuration you selected. 

  + MDK-ARM
    - Open the SysTick.Uv2 project
    - In the build toolbar select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM32L152-EVAL: to configure the project for STM32 Low Power Medium-density devices
        - STM32L152D-EVAL: to configure the project for STM32 High-Density Low-Power devices
        - STM32373C-EVAL:  to configure the project for STM32F37xxx devices
        - STM32303C-EVAL:  to configure the project for STM32F30xxx devices    
    - Rebuild all files: Project->Rebuild all target files
    - Go to "Utilities\Binary" and run "axftobin.bat"
    - A binary file "STM3210B-EVAL_SysTick.bin" or "STM3210E-EVAL_SysTick.bin"  
	or "STM32L152-EVAL_SysTick.bin" or "STM32L152D-EVAL_SysTick.bin" or "STM32373C-EVAL_SysTick.bin" 
	or "STM32303C-EVAL_SysTick.bin" will be generated under "\Obj" folder.
       
 + RIDE
    - Open the SysTick.rprj project
    - In the configuration toolbar(Project->properties) select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM32L152-EVAL: to configure the project for STM32 Medium-Density Low-Power devices
        - STM32L152D-EVAL: to configure the project for STM32 High-Density Low-Power devices
        - STM32373C-EVAL:  to configure the project for STM32F37xxx devices
        - STM32303C-EVAL:  to configure the project for STM32F30xxx devices     
    - Rebuild all files: Project->build project
    - Go to "Utilities\Binary" and run "hextobin.bat"
    - A binary file "SysTick.bin" will be generated under "\STM3210E_EVAL" 
	or "\STM3210B_EVAL" or "\STM32L152D_EVAL"  or "\STM32L152_EVAL" or "\STM32373C-EVAL" 
	or "\STM32303C-EVAL" folder depending on configuration you selected.
      
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
      window then click 
      on Project->build project menu.
    - Go to "Utilities\Binary" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM32F10X_XL_SysTick.bin", 
      "STM3210B-EVAL_SysTick.bin", "STM3210E-EVAL_SysTick.bin", 
      "STM32L152-EVAL_SysTick.bin" will be generated under "\Debug" folder
      "STM32L152D-EVAL_SysTick.bin" will be generated under "\Debug" folder
      "STM32373C-EVAL_SysTick.bin" will be generated under "\Debug" folder
      "STM32303C-EVAL_SysTick.bin" will be generated under "\Debug" folder

 + The generated .bin file should be converted to the DFU format using the “DFU File
   Manager Tool” included in the “DfuSe” PC software install. For more details on
   how to convert a .bin file to DFU format please refer to the UM0412 user manual
   “Getting started with DfuSe USB device firmware upgrade STMicroelectronics extension”
   available from the STMicroelectronics microcontroller website www.st.com.

************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE******
