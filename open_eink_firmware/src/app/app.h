/**
 * @file app.h
 * @brief Application Header
 * @author OpenHands AI Assistant
 * @date 2025-05-28
 * @license MIT License
 */

#ifndef APP_H
#define APP_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

// ============================================================================
// 应用程序状态
// ============================================================================

typedef enum {
    APP_STATE_INIT,
    APP_STATE_READY,
    APP_STATE_CONNECTED,
    APP_STATE_SLEEP,
    APP_STATE_ERROR
} app_state_t;

// ============================================================================
// 公共函数声明
// ============================================================================

/**
 * @brief 应用程序初始化
 * @return 0: 成功, 其他: 错误代码
 */
int app_init(void);

/**
 * @brief 获取应用程序状态
 * @return 当前应用程序状态
 */
app_state_t app_get_state(void);

/**
 * @brief 设置应用程序状态
 * @param state 新状态
 */
void app_set_state(app_state_t state);

/**
 * @brief 应用程序主循环处理
 */
void app_process(void);

#endif // APP_H