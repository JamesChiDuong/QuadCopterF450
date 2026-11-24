#!/bin/sh
# build_keil.sh - build the firmware with the Keil MDK Arm Compiler 6 (armclang)
#
# Why: on this PC Windows Smart App Control blocks the MSYS2 arm-none-eabi-gcc
# executables. The Keil toolchain (signed by Arm) is allowed, so this script
# builds the same sources with armclang + armlink + fromelf + microlib.
# The GCC Makefile is untouched and still works on other machines.
#
# Usage (from "Code/1. Control BLDC", in an MSYS2 / Git bash shell):
#   sh tools/keil/build_keil.sh            -> VARIANT=1 (RC FULL)
#   sh tools/keil/build_keil.sh 3          -> VARIANT=3 (GAMEPAD)
#   sh tools/keil/build_keil.sh 3 0        -> GAMEPAD, LCD UI disabled
#   sh tools/keil/build_keil.sh 3 1 "-DGPS_ENABLE=1"   -> extra -D options
# Output: build_keil/quadcopter.bin (+ .axf, .map). Flash: copy the .bin to the
# DIS_F429ZI drive.
#
# Differences from the GCC build (see tools/keil/startup_armclang.s):
#   - startup: tools/keil/startup_armclang.s, memory map: quadcopter.sct
#   - C library: Arm microlib; printf goes through fputc() in main.c
#   - Core/Src/syscalls.c and sysmem.c (newlib only) are not compiled
#   - optimisation -O1 (GCC build uses -O0)

VARIANT=${1:-1}
UI=${2:-1}
EXTRA=${3:-$EXTRA}       # extra compiler options, e.g. "-DGPS_ENABLE=1"
KEIL=${KEIL:-/c/Keil_v5/ARM/ARMCLANG/bin}
OUT=build_keil
JOBS=${JOBS:-8}

CC="$KEIL/armclang.exe"
LD="$KEIL/armlink.exe"
ELF="$KEIL/fromelf.exe"

FLAGS="--target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb"
CFLAGS="$FLAGS -std=gnu11 -O1 -g -ffunction-sections -fdata-sections -D__MICROLIB \
 -DUSE_HAL_DRIVER -DSTM32F429xx -DDEBUG -DFC_VARIANT=$VARIANT -DUI_LCD_ENABLE=$UI \
 -ICore/Inc -IDrivers/STM32F4xx_HAL_Driver/Inc -IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy \
 -IMiddlewares/Third_Party/FreeRTOS/Source/include \
 -IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS \
 -IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F \
 -IDrivers/CMSIS/Device/ST/STM32F4xx/Include -IDrivers/CMSIS/Include \
 -IMiddlewares/ST/STM32_USB_Host_Library/Core/Inc \
 -Wno-unused-parameter -Wno-missing-variable-declarations $EXTRA"

SRCS=$(ls Core/Src/*.c | grep -v -e 'Core/Src/syscalls.c' -e 'Core/Src/sysmem.c')
SRCS="$SRCS $(ls Drivers/STM32F4xx_HAL_Driver/Src/*.c)"
SRCS="$SRCS $(ls Middlewares/Third_Party/FreeRTOS/Source/*.c)"
SRCS="$SRCS Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c"
SRCS="$SRCS Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c"
SRCS="$SRCS Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c"
SRCS="$SRCS $(ls Middlewares/ST/STM32_USB_Host_Library/Core/Src/*.c)"

rm -rf "$OUT"
mkdir -p "$OUT/obj"
echo "armclang build: VARIANT=$VARIANT UI=$UI $EXTRA"

# Compile in parallel. Object name = path with / replaced by _ (unique).
export CC CFLAGS OUT
echo "$SRCS" | tr ' ' '\n' | grep -v '^$' | xargs -P "$JOBS" -I{} sh -c '
  o="$OUT/obj/$(echo "{}" | tr "/" "_" | sed "s/\.c$/.o/")"
  "$CC" $CFLAGS -c "{}" -o "$o" 2>"$o.log" || { echo "ERROR in {}"; cat "$o.log"; exit 255; }
  [ -s "$o.log" ] && { echo "warnings in {}:"; cat "$o.log"; }
  exit 0
' || { echo "BUILD FAILED (compile)"; exit 1; }

"$CC" $FLAGS -c tools/keil/startup_armclang.s -o "$OUT/obj/startup_armclang.o" \
  || { echo "BUILD FAILED (startup)"; exit 1; }

"$LD" --cpu=Cortex-M4.fp.sp --library_type=microlib --strict \
  --scatter=tools/keil/quadcopter.sct --entry=Reset_Handler \
  --summary_stderr --info=summarysizes --map --list="$OUT/quadcopter.map" \
  "$OUT"/obj/*.o -o "$OUT/quadcopter.axf" \
  || { echo "BUILD FAILED (link)"; exit 1; }

"$ELF" --bin --output="$OUT/quadcopter.bin" "$OUT/quadcopter.axf" \
  || { echo "BUILD FAILED (fromelf)"; exit 1; }

echo "OK: $OUT/quadcopter.bin ($(wc -c < "$OUT/quadcopter.bin") bytes)"
