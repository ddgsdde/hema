/**
 * @file config.h
 * @brief Open E-Ink Firmware Configuration
 * @author OpenHands AI Assistant
 * @date 2025-05-28
 * @license MIT License
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// ============================================================================
// 固件版本信息
// ============================================================================
#define FIRMWARE_VERSION        "1.0.0"
#define FIRMWARE_BUILD_DATE     __DATE__
#define FIRMWARE_BUILD_TIME     __TIME__

// ============================================================================
// 调试配置
// ============================================================================
#define DEBUG_UART_ENABLED      1
#define DEBUG_LEVEL             2   // 0=None, 1=Error, 2=Warning, 3=Info, 4=Debug

// ============================================================================
// 硬件配置
// ============================================================================

// 电子墨水屏配置
#define EINK_SPI_PORT           SPI_PORT_0
#define EINK_CS_PIN             GPIO_PIN_0
#define EINK_DC_PIN             GPIO_PIN_1
#define EINK_RST_PIN            GPIO_PIN_2
#define EINK_BUSY_PIN           GPIO_PIN_3
#define EINK_WIDTH              250
#define EINK_HEIGHT             122
#define EINK_ROTATION           0   // 0, 90, 180, 270

// 按键配置
#define BUTTON_COUNT            3
#define BUTTON_1_PIN            GPIO_PIN_4
#define BUTTON_2_PIN            GPIO_PIN_5
#define BUTTON_3_PIN            GPIO_PIN_6
#define BUTTON_DEBOUNCE_MS      50
#define BUTTON_LONG_PRESS_MS    1000

// 电源配置
#define POWER_ENABLE_PIN        GPIO_PIN_7
#define BATTERY_ADC_CHANNEL     ADC_CHANNEL_P0_1
#define LOW_BATTERY_THRESHOLD   2800    // mV
#define SLEEP_TIMEOUT_MS        30000   // 30秒无操作进入睡眠

// LED配置（可选）
#define LED_COUNT               1
#define LED_1_PIN               GPIO_PIN_8

// ============================================================================
// 蓝牙配置
// ============================================================================

// 设备信息
#define BLE_DEVICE_NAME         "OpenEInk"
#define BLE_MANUFACTURER_NAME   "OpenSource"
#define BLE_MODEL_NUMBER        "OEI-001"
#define BLE_SERIAL_NUMBER       "OS2025001"
#define BLE_HARDWARE_REVISION   "1.0"
#define BLE_FIRMWARE_REVISION   FIRMWARE_VERSION
#define BLE_SOFTWARE_REVISION   "1.0"

// 连接参数
#define BLE_MIN_CONN_INTERVAL   6       // 7.5ms
#define BLE_MAX_CONN_INTERVAL   12      // 15ms
#define BLE_SLAVE_LATENCY       0
#define BLE_SUPERVISION_TIMEOUT 100     // 1s

// 广播参数
#define BLE_ADV_INTERVAL_MIN    160     // 100ms
#define BLE_ADV_INTERVAL_MAX    320     // 200ms
#define BLE_ADV_TIMEOUT         0       // 无超时

// 安全配置
#define BLE_SECURITY_ENABLED    0       // 0=无安全, 1=启用配对
#define BLE_BONDING_ENABLED     0       // 0=不绑定, 1=支持绑定

// 自定义服务UUID (128-bit)
#define EINK_SERVICE_UUID       {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, \
                                 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0}

// 特征UUID
#define EINK_DISPLAY_CHAR_UUID  {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF1, \
                                 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0}
#define EINK_BUTTON_CHAR_UUID   {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF2, \
                                 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0}
#define EINK_CONFIG_CHAR_UUID   {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF3, \
                                 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0}

// ============================================================================
// 显示配置
// ============================================================================

// 字体配置
#define FONT_SMALL_SIZE         8
#define FONT_MEDIUM_SIZE        12
#define FONT_LARGE_SIZE         16
#define DEFAULT_FONT_SIZE       FONT_MEDIUM_SIZE

// 显示刷新配置
#define DISPLAY_FULL_REFRESH_INTERVAL   10  // 每10次部分刷新后进行一次全刷新
#define DISPLAY_AUTO_REFRESH_ENABLED    1   // 自动刷新
#define DISPLAY_REFRESH_DELAY_MS        100 // 刷新延迟

// 界面配置
#define UI_TITLE_HEIGHT         20
#define UI_STATUS_HEIGHT        15
#define UI_MARGIN               5
#define UI_LINE_SPACING         2

// ============================================================================
// 存储配置
// ============================================================================

// NVDS配置
#define NVDS_ENABLED            1
#define NVDS_TAG_USER_CONFIG    0x90
#define NVDS_TAG_DISPLAY_CONFIG 0x91
#define NVDS_TAG_BLE_CONFIG     0x92

// Flash配置
#define FLASH_USER_DATA_ADDR    0x1F000     // 用户数据存储地址
#define FLASH_USER_DATA_SIZE    0x1000      // 4KB用户数据空间

// ============================================================================
// 电源管理配置
// ============================================================================

// 睡眠模式配置
#define SLEEP_MODE_ENABLED      1
#define DEEP_SLEEP_ENABLED      1
#define HIBERNATION_ENABLED     0   // 休眠模式（需要外部唤醒）

// 唤醒源配置
#define WAKEUP_FROM_BUTTON      1
#define WAKEUP_FROM_BLE         1
#define WAKEUP_FROM_TIMER       1

// 电池监控
#define BATTERY_MONITOR_ENABLED 1
#define BATTERY_CHECK_INTERVAL  60000   // 1分钟检查一次电池

// ============================================================================
// 看门狗配置
// ============================================================================
#define WATCHDOG_ENABLED        1
#define WATCHDOG_DEFAULT_PERIOD 10000   // 10秒

// ============================================================================
// 功能开关
// ============================================================================

// 核心功能
#define FEATURE_DISPLAY         1   // 显示功能
#define FEATURE_BUTTONS         1   // 按键功能
#define FEATURE_BLE             1   // 蓝牙功能
#define FEATURE_POWER_MGMT      1   // 电源管理

// 扩展功能
#define FEATURE_OTA_UPDATE      1   // OTA更新
#define FEATURE_DATA_LOGGING    0   // 数据记录
#define FEATURE_RTC             0   // 实时时钟
#define FEATURE_SENSORS         0   // 传感器支持

// 调试功能
#define FEATURE_DEBUG_MENU      1   // 调试菜单
#define FEATURE_FACTORY_RESET   1   // 恢复出厂设置
#define FEATURE_SYSTEM_INFO     1   // 系统信息显示

// ============================================================================
// 错误代码定义
// ============================================================================
#define ERROR_NONE              0x00
#define ERROR_INIT_FAILED       0x01
#define ERROR_DISPLAY_FAILED    0x02
#define ERROR_BUTTON_FAILED     0x03
#define ERROR_BLE_FAILED        0x04
#define ERROR_POWER_FAILED      0x05
#define ERROR_STORAGE_FAILED    0x06
#define ERROR_INVALID_PARAM     0x07
#define ERROR_TIMEOUT           0x08
#define ERROR_NO_MEMORY         0x09
#define ERROR_HARDWARE_FAULT    0x0A

// ============================================================================
// 编译时检查
// ============================================================================
#if EINK_WIDTH > 300 || EINK_HEIGHT > 300
#error "Display resolution too high for DA14585 memory"
#endif

#if BUTTON_COUNT > 8
#error "Too many buttons defined"
#endif

#if !FEATURE_DISPLAY
#error "Display feature cannot be disabled"
#endif

#endif // CONFIG_H