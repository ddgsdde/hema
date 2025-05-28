/**
 * @file power_manager.h
 * @brief Power Management Module Header
 * @author OpenHands AI Assistant
 * @date 2025-05-28
 * @license MIT License
 */

#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// 电源状态
// ============================================================================

typedef enum {
    POWER_STATE_ACTIVE,
    POWER_STATE_IDLE,
    POWER_STATE_SLEEP,
    POWER_STATE_DEEP_SLEEP
} power_state_t;

// ============================================================================
// 公共函数声明
// ============================================================================

/**
 * @brief 初始化电源管理器
 * @return 0: 成功, 其他: 错误代码
 */
int power_manager_init(void);

/**
 * @brief 处理电源管理
 */
void power_manager_process(void);

/**
 * @brief 检查是否可以进入睡眠模式
 * @return true: 可以睡眠, false: 不能睡眠
 */
bool power_manager_can_sleep(void);

/**
 * @brief 获取电池电压
 * @return 电池电压 (mV)
 */
uint16_t power_get_battery_voltage(void);

/**
 * @brief 获取电池电量百分比
 * @return 电池电量 (0-100%)
 */
uint8_t power_get_battery_percentage(void);

/**
 * @brief 检查是否为低电量
 * @return true: 低电量, false: 电量正常
 */
bool power_is_low_battery(void);

#endif // POWER_MANAGER_H