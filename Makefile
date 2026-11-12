# Makefile: STM32F103C8T6 (Blue Pill) + HAL + FreeRTOS, biên dịch bằng arm-none-eabi-gcc

TARGET  := firmware
BUILD   := build

# ---- Cross-platform commands (Linux / Windows) ----
ifeq ($(OS),Windows_NT)
  MKDIR = if not exist "$(subst /,\,$(dir $@))" mkdir "$(subst /,\,$(dir $@))"
  RM    = rmdir /s /q
else
  MKDIR = mkdir -p $(dir $@)
  RM    = rm -rf
endif

# ---- Công cụ ----
PREFIX  := arm-none-eabi-
CC      := $(PREFIX)gcc
CP      := $(PREFIX)objcopy
SZ      := $(PREFIX)size

# ---- Chip ----
MCU     := -mcpu=cortex-m3 -mthumb
DEFS    := -DSTM32F103xB -DUSE_HAL_DRIVER

# ---- Đường dẫn header ----
INCS := \
	-Ihal/inc \
	-Idrivers/inc \
	-Iapp/inc \
	-Iconfig \
	-Ibsp/cmsis \
	-Ibsp/hal_driver/inc \
	-Imiddlewares/FreeRTOS/inc \
	-Imiddlewares/FreeRTOS/portable

# ---- File nguồn C ----
SRCS := main.c
SRCS += bsp/system/system_stm32f1xx.c
SRCS += $(wildcard hal/src/*.c)
SRCS += $(wildcard drivers/src/*.c)
SRCS += $(wildcard app/src/*.c)
SRCS += $(filter-out %_template.c, $(wildcard bsp/hal_driver/src/*.c))
# FreeRTOS: chỉ lấy file cần dùng (kernel gốc tên tasks.c, bản của bạn là task.c)
SRCS += $(wildcard middlewares/FreeRTOS/src/task.c middlewares/FreeRTOS/src/tasks.c)
SRCS += middlewares/FreeRTOS/src/queue.c
SRCS += middlewares/FreeRTOS/src/list.c
SRCS += middlewares/FreeRTOS/portable/port.c
SRCS += middlewares/FreeRTOS/portable/heap_4.c

# ---- File khởi động và linker script ----
ASMS    := startup/startup_stm32f103xb.s
LDSCRIPT:= linker/STM32F103C8Tx_FLASH.ld

# ---- Cờ biên dịch ----
OPT     := -Os
CFLAGS  := $(MCU) $(DEFS) $(INCS) $(OPT) -Wall -g -gdwarf-2 \
           -ffunction-sections -fdata-sections -MMD -MP
ASFLAGS := $(MCU) $(DEFS) -x assembler-with-cpp -g -gdwarf-2

# ---- Cờ liên kết: newlib-nano + in được số thực (%f) ----
LDFLAGS := $(MCU) -T$(LDSCRIPT) --specs=nano.specs --specs=nosys.specs \
           -Wl,-u,_printf_float -Wl,--gc-sections \
           -Wl,-Map=$(BUILD)/$(TARGET).map,--cref

OBJS := $(addprefix $(BUILD)/, $(SRCS:.c=.o)) $(addprefix $(BUILD)/, $(ASMS:.s=.o))

.PHONY: all clean flash flash-ocd size

all: $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).bin $(BUILD)/$(TARGET).hex size

$(BUILD)/%.o: %.c
	@$(MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.s
	@$(MKDIR)
	$(CC) $(ASFLAGS) -c $< -o $@

$(BUILD)/$(TARGET).elf: $(OBJS) $(LDSCRIPT)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(BUILD)/%.bin: $(BUILD)/%.elf
	$(CP) -O binary $< $@

$(BUILD)/%.hex: $(BUILD)/%.elf
	$(CP) -O ihex $< $@

size: $(BUILD)/$(TARGET).elf
	$(SZ) $<

# Nạp bằng st-flash (ST-Link)
flash: $(BUILD)/$(TARGET).bin
	st-flash --reset write $< 0x08000000

# Nạp bằng OpenOCD (ST-Link)
flash-ocd: $(BUILD)/$(TARGET).elf
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
		-c "program $< verify reset exit"

clean:
ifeq ($(OS),Windows_NT)
	@if exist $(BUILD) $(RM) $(BUILD)
else
	$(RM) $(BUILD)
endif

-include $(OBJS:.o=.d)
