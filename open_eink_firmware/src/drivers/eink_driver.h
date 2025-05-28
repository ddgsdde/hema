/**
 * @file eink_driver.h
 * @brief E-Ink Display Driver Header
 * @author OpenHands AI Assistant
 * @date 2025-05-28
 * @license MIT License
 */

#ifndef EINK_DRIVER_H
#define EINK_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// 公共函数声明
// ============================================================================

/**
 * @brief 初始化电子墨水屏
 * @return 0: 成功, 其他: 错误代码
 */
int eink_init(void);

/**
 * @brief 清屏
 * @return 0: 成功, 其他: 错误代码
 */
int eink_clear(void);

/**
 * @brief 设置像素点
 * @param x X坐标
 * @param y Y坐标
 * @param color 颜色 (0=黑色, 1=白色)
 * @return 0: 成功, 其他: 错误代码
 */
int eink_set_pixel(uint16_t x, uint16_t y, uint8_t color);

/**
 * @brief 获取像素点颜色
 * @param x X坐标
 * @param y Y坐标
 * @return 像素颜色 (0=黑色, 1=白色, 0xFF=错误)
 */
uint8_t eink_get_pixel(uint16_t x, uint16_t y);

/**
 * @brief 画线
 * @param x0 起始X坐标
 * @param y0 起始Y坐标
 * @param x1 结束X坐标
 * @param y1 结束Y坐标
 * @param color 颜色
 * @return 0: 成功, 其他: 错误代码
 */
int eink_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color);

/**
 * @brief 画矩形
 * @param x X坐标
 * @param y Y坐标
 * @param width 宽度
 * @param height 高度
 * @param color 颜色
 * @param filled 是否填充
 * @return 0: 成功, 其他: 错误代码
 */
int eink_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color, bool filled);

/**
 * @brief 显示文本
 * @param x X坐标
 * @param y Y坐标
 * @param text 文本内容
 * @param font_size 字体大小
 * @param color 颜色
 * @return 0: 成功, 其他: 错误代码
 */
int eink_display_text(uint16_t x, uint16_t y, const char* text, uint8_t font_size, uint8_t color);

/**
 * @brief 刷新显示
 * @param mode 刷新模式 (0=全刷新, 1=部分刷新)
 * @return 0: 成功, 其他: 错误代码
 */
int eink_refresh(uint8_t mode);

/**
 * @brief 进入睡眠模式
 * @return 0: 成功, 其他: 错误代码
 */
int eink_sleep(void);

/**
 * @brief 从睡眠模式唤醒
 * @return 0: 成功, 其他: 错误代码
 */
int eink_wakeup(void);

#endif // EINK_DRIVER_H