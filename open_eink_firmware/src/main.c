/**
 * @file main.c
 * @brief Open E-Ink Firmware for DA14585 - Main Application
 * @author OpenHands AI Assistant
 * @date 2025-05-28
 * @license MIT License
 * 
 * 开源电子墨水屏固件主程序
 * 移除了所有激活限制，完全开放使用
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// DA14585 SDK includes
#include "arch.h"
#include "arch_api.h"
#include "arch_main.h"
#include "arch_system.h"
#include "arch_wdg.h"
#include "gpio.h"
#include "uart.h"
#include "spi.h"
#include "timer.h"
#include "rwip.h"
#include "gap.h"
#include "gattc.h"
#include "gapc.h"

// Application includes
#include "app.h"
#include "display_manager.h"
#include "button_manager.h"
#include "power_manager.h"
#include "ble_manager.h"
#include "config.h"

/**
 * @brief 系统初始化
 */
static void system_init(void)
{
    // 系统时钟配置
    SetBits16(CLK_AMBA_REG, PCLK_DIV, 0);
    SetBits16(CLK_AMBA_REG, HCLK_DIV, 0);
    
    // GPIO初始化
    GPIO_init();
    
    // 看门狗配置
    wdg_reload(WATCHDOG_DEFAULT_PERIOD);
    wdg_resume();
    
    // UART初始化（调试用）
    #ifdef DEBUG_UART_ENABLED
    uart_init(UART1, 115200);
    printf("Open E-Ink Firmware v%s\n", FIRMWARE_VERSION);
    printf("Build: %s %s\n", __DATE__, __TIME__);
    printf("No activation required - Open Source!\n");
    #endif
}

/**
 * @brief 硬件初始化
 */
static void hardware_init(void)
{
    // 电子墨水屏初始化
    if (display_init() != 0) {
        #ifdef DEBUG_UART_ENABLED
        printf("ERROR: Display initialization failed\n");
        #endif
    }
    
    // 按键初始化
    if (button_init() != 0) {
        #ifdef DEBUG_UART_ENABLED
        printf("ERROR: Button initialization failed\n");
        #endif
    }
    
    // 电源管理初始化
    power_manager_init();
    
    #ifdef DEBUG_UART_ENABLED
    printf("Hardware initialization completed\n");
    #endif
}

/**
 * @brief 应用程序初始化
 */
static void app_init(void)
{
    // BLE协议栈初始化
    ble_manager_init();
    
    // 显示欢迎界面
    display_welcome_screen();
    
    // 注册按键回调
    button_register_callbacks();
    
    #ifdef DEBUG_UART_ENABLED
    printf("Application initialization completed\n");
    #endif
}

/**
 * @brief 主循环
 */
static void main_loop(void)
{
    while (1) {
        // 处理BLE事件
        rwip_schedule();
        
        // 处理按键事件
        button_process();
        
        // 处理显示更新
        display_process();
        
        // 电源管理
        power_manager_process();
        
        // 喂狗
        wdg_reload(WATCHDOG_DEFAULT_PERIOD);
        
        // 进入低功耗模式
        if (power_manager_can_sleep()) {
            arch_ble_ext_wakeup_on();
            arch_set_sleep_mode(ARCH_SLEEP_OFF);
        }
    }
}

/**
 * @brief 主函数
 */
int main(void)
{
    // 系统初始化
    system_init();
    
    // 硬件初始化
    hardware_init();
    
    // 应用程序初始化
    app_init();
    
    #ifdef DEBUG_UART_ENABLED
    printf("Starting main loop...\n");
    #endif
    
    // 主循环
    main_loop();
    
    return 0;
}

/**
 * @brief 系统错误处理
 */
void assert_err(const char *condition, const char * file, int line)
{
    #ifdef DEBUG_UART_ENABLED
    printf("ASSERT ERROR: %s at %s:%d\n", condition, file, line);
    #endif
    
    // 显示错误信息
    display_error_screen("System Error");
    
    // 重启系统
    arch_system_reset();
}

/**
 * @brief 系统警告处理
 */
void assert_warn(const char *condition, const char * file, int line)
{
    #ifdef DEBUG_UART_ENABLED
    printf("ASSERT WARNING: %s at %s:%d\n", condition, file, line);
    #endif
}

/**
 * @brief 硬件错误处理
 */
void platform_reset(uint32_t error)
{
    #ifdef DEBUG_UART_ENABLED
    printf("Platform reset: error code 0x%08X\n", error);
    #endif
    
    // 保存错误信息到Flash
    // save_error_log(error);
    
    // 重启系统
    arch_system_reset();
}