/**
 * @file display_manager.h
 * @brief Display Management Module Header
 * @author OpenHands AI Assistant
 * @date 2025-05-28
 * @license MIT License
 */

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <stdint.h>

// ============================================================================
// 公共函数声明
// ============================================================================

/**
 * @brief 初始化显示管理器
 * @return 0: 成功, 其他: 错误代码
 */
int display_init(void);

/**
 * @brief 显示欢迎界面
 */
void display_welcome_screen(void);

/**
 * @brief 显示主菜单
 */
void display_main_menu(void);

/**
 * @brief 显示错误界面
 * @param error_msg 错误信息
 */
void display_error_screen(const char* error_msg);

/**
 * @brief 处理按键输入
 * @param button_id 按键ID
 * @param event 按键事件
 */
void display_handle_button(uint8_t button_id, uint8_t event);

/**
 * @brief 处理显示更新
 */
void display_process(void);

/**
 * @brief 设置状态文本
 * @param text 状态文本
 */
void display_set_status(const char* text);

/**
 * @brief 强制刷新显示
 */
void display_force_refresh(void);

#endif // DISPLAY_MANAGER_H