/**
 * @file button_manager.h
 * @brief Button Management Module Header
 * @author OpenHands AI Assistant
 * @date 2025-05-28
 * @license MIT License
 */

#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// 按键事件类型
// ============================================================================

typedef enum {
    BUTTON_EVENT_PRESS,
    BUTTON_EVENT_RELEASE,
    BUTTON_EVENT_LONG_PRESS
} button_event_t;

// ============================================================================
// 按键回调函数类型
// ============================================================================

typedef void (*button_callback_t)(uint8_t button_id, button_event_t event);

// ============================================================================
// 公共函数声明
// ============================================================================

/**
 * @brief 初始化按键管理器
 * @return 0: 成功, 其他: 错误代码
 */
int button_init(void);

/**
 * @brief 注册按键回调函数
 */
void button_register_callbacks(void);

/**
 * @brief 处理按键事件
 */
void button_process(void);

/**
 * @brief 获取按键状态
 * @param button_id 按键ID
 * @return 按键状态 (0=释放, 1=按下)
 */
uint8_t button_get_state(uint8_t button_id);

#endif // BUTTON_MANAGER_H