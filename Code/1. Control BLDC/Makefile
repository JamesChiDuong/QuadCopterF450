# Command-line build of the QuadCopter firmware (independent of STM32CubeIDE).
#
#   make                  -> build/quadcopter.elf/.bin/.hex  (FC_VARIANT_RC_FULL)
#   make VARIANT=2        -> UART TEST variant
#   make VARIANT=3        -> GAMEPAD variant (USB gamepad on CN6)
#   make UI=0             -> without LCD task
#   make clean
#
# Toolchain: arm-none-eabi-gcc in PATH (e.g. MSYS2 package
# mingw-w64-ucrt-x86_64-arm-none-eabi-gcc, or the one shipped with CubeIDE).
# Flags mirror the CubeIDE Debug configuration (-O0 -g3), plus -O2 option:
#   make OPT=-O2

TARGET   := quadcopter
BUILD    := build
VARIANT  ?= 1
UI       ?= 1
OPT      ?= -O0 -g3

# MSYS make hands gcc a temp dir it cannot use on some Windows setups
# ("Cannot create temporary file in C:\Windows\"): use one inside build/.
$(shell mkdir -p $(BUILD)/tmp)
export TMPDIR := $(abspath $(BUILD)/tmp)
export TMP    := $(TMPDIR)
export TEMP   := $(TMPDIR)

PREFIX   := arm-none-eabi-
CC       := $(PREFIX)gcc
OBJCOPY  := $(PREFIX)objcopy
SIZE     := $(PREFIX)size

MCU      := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

DEFS     := -DUSE_HAL_DRIVER -DSTM32F429xx -DDEBUG \
            -DFC_VARIANT=$(VARIANT) -DUI_LCD_ENABLE=$(UI)

INCS     := -ICore/Inc \
            -IDrivers/STM32F4xx_HAL_Driver/Inc \
            -IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy \
            -IMiddlewares/Third_Party/FreeRTOS/Source/include \
            -IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS \
            -IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F \
            -IDrivers/CMSIS/Device/ST/STM32F4xx/Include \
            -IDrivers/CMSIS/Include \
            -IMiddlewares/ST/STM32_USB_Host_Library/Core/Inc

C_SRCS   := $(wildcard Core/Src/*.c) \
            $(wildcard Drivers/STM32F4xx_HAL_Driver/Src/*.c) \
            $(wildcard Middlewares/Third_Party/FreeRTOS/Source/*.c) \
            Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c \
            Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c \
            Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c \
            $(wildcard Middlewares/ST/STM32_USB_Host_Library/Core/Src/*.c)
ASM_SRCS := Core/Startup/startup_stm32f429zitx.s

OBJS     := $(addprefix $(BUILD)/,$(C_SRCS:.c=.o)) $(addprefix $(BUILD)/,$(ASM_SRCS:.s=.o))

CFLAGS   := $(MCU) -std=gnu11 $(OPT) $(DEFS) $(INCS) -ffunction-sections -fdata-sections \
            -Wall -fstack-usage -MMD -MP --specs=nano.specs
LDSCRIPT := STM32F429ZITX_FLASH.ld
LDFLAGS  := $(MCU) -T$(LDSCRIPT) --specs=nosys.specs --specs=nano.specs -u _printf_float \
            -Wl,-Map=$(BUILD)/$(TARGET).map -Wl,--gc-sections -static \
            -Wl,--start-group -lc -lm -Wl,--end-group

all: $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).bin $(BUILD)/$(TARGET).hex
	@$(SIZE) $(BUILD)/$(TARGET).elf 2>/dev/null || echo "(size tool unavailable) see $(BUILD)/$(TARGET).map"

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo CC $<
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.s
	@mkdir -p $(dir $@)
	@echo AS $<
	@$(CC) $(MCU) -x assembler-with-cpp -c $< -o $@

# DIRECT_LD=1: call arm-none-eabi-ld directly (see tools/gcc-link.sh), for
# Windows machines where collect2.exe is blocked by Smart App Control.
DIRECT_LD ?= 0
ifeq ($(DIRECT_LD),1)
LINK := sh tools/gcc-link.sh $(CC)
else
LINK := $(CC)
endif

$(BUILD)/$(TARGET).elf: $(OBJS) $(LDSCRIPT)
	@echo LD $@
	@$(LINK) $(OBJS) $(LDFLAGS) -o $@

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	@$(OBJCOPY) -O binary $< $@

$(BUILD)/$(TARGET).hex: $(BUILD)/$(TARGET).elf
	@$(OBJCOPY) -O ihex $< $@

clean:
	rm -rf $(BUILD)

-include $(OBJS:.o=.d)

.PHONY: all clean
