@echo off

if exist .\..\..\Project\Device_Firmware_Upgrade\binary_template\TrueSTUDIO\STM3210C-EVAL\Debug\STM3210C-EVAL.elf  ("arm-elf-objcopy.exe" -O binary ".\..\..\Project\Device_Firmware_Upgrade\binary_template\TrueSTUDIO\STM3210C-EVAL\Debug\STM3210C-EVAL.elf" ".\..\..\Project\Device_Firmware_Upgrade\binary_template\TrueSTUDIO\STM3210C-EVAL\Debug\STM3210C-EVAL_SysTick.bin")
if exist .\..\..\Project\Device_Firmware_Upgrade\binary_template\TrueSTUDIO\STM3210B-EVAL\Debug\STM3210B-EVAL.elf  ("arm-elf-objcopy.exe" -O binary ".\..\..\Project\Device_Firmware_Upgrade\binary_template\TrueSTUDIO\STM3210B-EVAL\Debug\STM3210B-EVAL.elf" ".\..\..\Project\Device_Firmware_Upgrade\binary_template\TrueSTUDIO\STM3210B-EVAL\Debug\STM3210B-EVAL_SysTick.bin")
if exist .\..\..\Project\Device_Firmware_Upgrade\binary_template\TrueSTUDIO\STM3210E-EVAL\Debug\STM3210E-EVAL.elf  ("arm-elf-objcopy.exe" -O binary ".\..\..\Project\Device_Firmware_Upgrade\binary_template\TrueSTUDIO\STM3210E-EVAL\Debug\STM3210E-EVAL.elf" ".\..\..\Project\Device_Firmware_Upgrade\binary_template\TrueSTUDIO\STM3210E-EVAL\Debug\STM3210E-EVAL_SysTick.bin")
if exist .\..\..\Project\Device_Firmware_Upgrade\binary_template\TrueSTUDIO\STM3210E-EVAL_XL\Debug\STM32F10X_XL.elf    ("arm-elf-objcopy.exe" -O binary ".\..\..\Project\Device_Firmware_Upgrade\binary_template\TrueSTUDIO\STM3210E-EVAL_XL\Debug\STM32F10X_XL.elf" ".\..\..\Project\Device_Firmware_Upgrade\binary_template\TrueSTUDIO\STM3210E-EVAL_XL\Debug\STM32F10X_XL_SysTick.bin")
if exist .\..\..\Project\Device_Firmware_Upgrade\binary_template\TrueSTUDIO\STM32L152-EVAL\Debug\STM32L152-EVAL.elf  ("arm-elf-objcopy.exe" -O binary ".\..\..\Project\Device_Firmware_Upgrade\binary_template\TrueSTUDIO\STM32L152-EVAL\Debug\STM32L152-EVAL.elf" ".\..\..\Project\Device_Firmware_Upgrade\binary_template\TrueSTUDIO\STM32L152-EVAL\Debug\STM32L152-EVAL_SysTick.bin")

pause

