# Open E-Ink Firmware Makefile
# Author: OpenHands AI Assistant
# Date: 2025-05-28
# License: MIT License

# ============================================================================
# 项目配置
# ============================================================================

PROJECT_NAME = open_eink_firmware
VERSION = 1.0.0

# ============================================================================
# 工具链配置
# ============================================================================

# ARM工具链前缀
TOOLCHAIN_PREFIX = arm-none-eabi-

# 编译器
CC = $(TOOLCHAIN_PREFIX)gcc
AS = $(TOOLCHAIN_PREFIX)as
LD = $(TOOLCHAIN_PREFIX)ld
OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy
OBJDUMP = $(TOOLCHAIN_PREFIX)objdump
SIZE = $(TOOLCHAIN_PREFIX)size

# ============================================================================
# 目录配置
# ============================================================================

# 源码目录
SRC_DIR = src
APP_DIR = $(SRC_DIR)/app
BLE_DIR = $(SRC_DIR)/ble
DRIVERS_DIR = $(SRC_DIR)/drivers
HAL_DIR = $(SRC_DIR)/hal
SYSTEM_DIR = $(SRC_DIR)/system

# 配置目录
CONFIG_DIR = config

# 构建目录
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

# DA14585 SDK路径（需要设置环境变量）
SDK_PATH ?= $(DA14585_SDK_PATH)
ifeq ($(SDK_PATH),)
$(error Please set DA14585_SDK_PATH environment variable)
endif

# SDK包含目录
SDK_INC_DIR = $(SDK_PATH)/sdk/platform/include
SDK_BLE_INC_DIR = $(SDK_PATH)/sdk/ble_stack/include
SDK_ARCH_INC_DIR = $(SDK_PATH)/sdk/platform/arch/boot/GCC

# ============================================================================
# 源文件
# ============================================================================

# 应用程序源文件
APP_SOURCES = \
    $(SRC_DIR)/main.c \
    $(APP_DIR)/display_manager.c \
    $(APP_DIR)/button_manager.c \
    $(APP_DIR)/power_manager.c \
    $(APP_DIR)/app.c

# 驱动程序源文件
DRIVER_SOURCES = \
    $(DRIVERS_DIR)/eink_driver.c \
    $(DRIVERS_DIR)/button_driver.c \
    $(DRIVERS_DIR)/power_driver.c

# BLE源文件
BLE_SOURCES = \
    $(BLE_DIR)/ble_manager.c \
    $(BLE_DIR)/custom_service.c

# HAL源文件
HAL_SOURCES = \
    $(HAL_DIR)/gpio_hal.c \
    $(HAL_DIR)/spi_hal.c \
    $(HAL_DIR)/timer_hal.c

# 系统源文件
SYSTEM_SOURCES = \
    $(SYSTEM_DIR)/system_init.c \
    $(SYSTEM_DIR)/interrupt_handlers.c

# 所有源文件
SOURCES = $(APP_SOURCES) $(DRIVER_SOURCES) $(BLE_SOURCES) $(HAL_SOURCES) $(SYSTEM_SOURCES)

# 目标文件
OBJECTS = $(SOURCES:%.c=$(OBJ_DIR)/%.o)

# ============================================================================
# 编译选项
# ============================================================================

# CPU配置
CPU = -mcpu=cortex-m0
FPU = 
FLOAT-ABI = 

# MCU定义
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# 预处理器定义
DEFINES = \
    -DCFG_APP \
    -DCFG_ALLROLES \
    -DCFG_CON=8 \
    -DCFG_SECURITY_ON \
    -DCFG_ATTC \
    -DCFG_ATTS \
    -DCFG_PRF \
    -DCFG_NB_PRF=10 \
    -DCFG_CHNL_ASSESS \
    -DCFG_EXT_DB \
    -DCFG_RF_ATLAS \
    -DCFG_NVDS \
    -DCFG_WDOG \
    -DDA14585 \
    -D__DA14585__ \
    -D__EXCLUDE_ROM_FUNC_TABLES__

# 包含目录
INCLUDES = \
    -I$(CONFIG_DIR) \
    -I$(SRC_DIR) \
    -I$(APP_DIR) \
    -I$(BLE_DIR) \
    -I$(DRIVERS_DIR) \
    -I$(HAL_DIR) \
    -I$(SYSTEM_DIR) \
    -I$(SDK_INC_DIR) \
    -I$(SDK_BLE_INC_DIR) \
    -I$(SDK_ARCH_INC_DIR)

# 编译标志
CFLAGS = $(MCU) $(DEFINES) $(INCLUDES) -Wall -Wextra -Wno-unused-parameter -fdata-sections -ffunction-sections

# 调试/发布配置
ifdef DEBUG
    CFLAGS += -g -O0 -DDEBUG
    BUILD_TYPE = debug
else
    CFLAGS += -Os -DNDEBUG
    BUILD_TYPE = release
endif

# 链接标志
LDFLAGS = $(MCU) -specs=nano.specs -T$(SDK_PATH)/sdk/platform/arch/boot/GCC/DA14585.lds -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(PROJECT_NAME).map,--cref

# 库文件
LIBS = -lc -lm -lnosys

# ============================================================================
# 构建规则
# ============================================================================

.PHONY: all clean flash size info help

# 默认目标
all: $(BUILD_DIR)/$(PROJECT_NAME).bin

# 创建目录
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/$(APP_DIR)
	@mkdir -p $(OBJ_DIR)/$(BLE_DIR)
	@mkdir -p $(OBJ_DIR)/$(DRIVERS_DIR)
	@mkdir -p $(OBJ_DIR)/$(HAL_DIR)
	@mkdir -p $(OBJ_DIR)/$(SYSTEM_DIR)

# 编译C文件
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# 链接
$(BUILD_DIR)/$(PROJECT_NAME).elf: $(OBJECTS)
	@echo "LD $@"
	@$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@

# 生成二进制文件
$(BUILD_DIR)/$(PROJECT_NAME).bin: $(BUILD_DIR)/$(PROJECT_NAME).elf
	@echo "OBJCOPY $@"
	@$(OBJCOPY) -O binary $< $@

# 生成十六进制文件
$(BUILD_DIR)/$(PROJECT_NAME).hex: $(BUILD_DIR)/$(PROJECT_NAME).elf
	@echo "OBJCOPY $@"
	@$(OBJCOPY) -O ihex $< $@

# 清理
clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR)

# 显示大小信息
size: $(BUILD_DIR)/$(PROJECT_NAME).elf
	@echo "Size information:"
	@$(SIZE) $<

# 烧录固件（需要J-Link）
flash: $(BUILD_DIR)/$(PROJECT_NAME).bin
	@echo "Flashing firmware..."
	@JLinkExe -device DA14585 -if SWD -speed 1000 -autoconnect 1 -CommanderScript flash.jlink

# 生成反汇编文件
disasm: $(BUILD_DIR)/$(PROJECT_NAME).elf
	@echo "Generating disassembly..."
	@$(OBJDUMP) -d $< > $(BUILD_DIR)/$(PROJECT_NAME).dis

# 项目信息
info:
	@echo "Project: $(PROJECT_NAME) v$(VERSION)"
	@echo "Build type: $(BUILD_TYPE)"
	@echo "Toolchain: $(TOOLCHAIN_PREFIX)"
	@echo "SDK path: $(SDK_PATH)"
	@echo "Sources: $(words $(SOURCES)) files"
	@echo "Objects: $(words $(OBJECTS)) files"

# 帮助信息
help:
	@echo "Available targets:"
	@echo "  all      - Build the firmware (default)"
	@echo "  clean    - Clean build files"
	@echo "  flash    - Flash firmware to device"
	@echo "  size     - Show size information"
	@echo "  disasm   - Generate disassembly file"
	@echo "  info     - Show project information"
	@echo "  help     - Show this help"
	@echo ""
	@echo "Build options:"
	@echo "  DEBUG=1  - Build debug version"
	@echo ""
	@echo "Environment variables:"
	@echo "  DA14585_SDK_PATH - Path to DA14585 SDK"

# ============================================================================
# 依赖关系
# ============================================================================

# 自动生成依赖关系
-include $(OBJECTS:.o=.d)

$(OBJ_DIR)/%.d: %.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -MM -MT $(@:.d=.o) $< > $@