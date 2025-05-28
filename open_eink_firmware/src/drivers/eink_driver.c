/**
 * @file eink_driver.c
 * @brief E-Ink Display Driver for DA14585
 * @author OpenHands AI Assistant
 * @date 2025-05-28
 * @license MIT License
 * 
 * 电子墨水屏驱动程序
 * 支持常见的SPI接口电子墨水屏
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "eink_driver.h"
#include "config.h"
#include "gpio.h"
#include "spi.h"
#include "timer.h"

// ============================================================================
// 私有定义
// ============================================================================

// E-Ink命令定义
#define EINK_CMD_DRIVER_OUTPUT_CONTROL          0x01
#define EINK_CMD_BOOSTER_SOFT_START_CONTROL     0x0C
#define EINK_CMD_GATE_SCAN_START_POSITION       0x0F
#define EINK_CMD_DEEP_SLEEP_MODE                0x10
#define EINK_CMD_DATA_ENTRY_MODE_SETTING        0x11
#define EINK_CMD_SW_RESET                       0x12
#define EINK_CMD_TEMPERATURE_SENSOR_CONTROL     0x1A
#define EINK_CMD_MASTER_ACTIVATION              0x20
#define EINK_CMD_DISPLAY_UPDATE_CONTROL_1       0x21
#define EINK_CMD_DISPLAY_UPDATE_CONTROL_2       0x22
#define EINK_CMD_WRITE_RAM                      0x24
#define EINK_CMD_WRITE_VCOM_REGISTER            0x2C
#define EINK_CMD_WRITE_LUT_REGISTER             0x32
#define EINK_CMD_SET_DUMMY_LINE_PERIOD          0x3A
#define EINK_CMD_SET_GATE_TIME                  0x3B
#define EINK_CMD_BORDER_WAVEFORM_CONTROL        0x3C
#define EINK_CMD_SET_RAM_X_ADDRESS_START_END    0x44
#define EINK_CMD_SET_RAM_Y_ADDRESS_START_END    0x45
#define EINK_CMD_SET_RAM_X_ADDRESS_COUNTER      0x4E
#define EINK_CMD_SET_RAM_Y_ADDRESS_COUNTER      0x4F
#define EINK_CMD_TERMINATE_FRAME_READ_WRITE     0xFF

// 显示模式
#define EINK_DISPLAY_MODE_FULL                  0
#define EINK_DISPLAY_MODE_PARTIAL               1

// 颜色定义
#define EINK_COLOR_WHITE                        0xFF
#define EINK_COLOR_BLACK                        0x00

// ============================================================================
// 私有变量
// ============================================================================

static bool eink_initialized = false;
static uint8_t display_buffer[EINK_WIDTH * EINK_HEIGHT / 8];
static uint8_t current_display_mode = EINK_DISPLAY_MODE_FULL;
static uint32_t refresh_counter = 0;

// LUT表（查找表）用于控制刷新时序
static const uint8_t lut_full_update[] = {
    0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22,
    0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88,
    0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51,
    0x35, 0x51, 0x51, 0x19, 0x01, 0x00
};

static const uint8_t lut_partial_update[] = {
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// ============================================================================
// 私有函数声明
// ============================================================================

static void eink_spi_init(void);
static void eink_gpio_init(void);
static void eink_reset(void);
static void eink_send_command(uint8_t command);
static void eink_send_data(uint8_t data);
static void eink_send_data_buffer(const uint8_t* data, uint16_t length);
static void eink_wait_busy(void);
static void eink_set_lut(const uint8_t* lut);
static void eink_set_memory_area(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end);
static void eink_set_memory_pointer(uint16_t x, uint16_t y);

// ============================================================================
// 公共函数实现
// ============================================================================

/**
 * @brief 初始化电子墨水屏
 * @return 0: 成功, 其他: 错误代码
 */
int eink_init(void)
{
    if (eink_initialized) {
        return ERROR_NONE;
    }
    
    // 初始化SPI和GPIO
    eink_spi_init();
    eink_gpio_init();
    
    // 硬件复位
    eink_reset();
    
    // 等待设备就绪
    eink_wait_busy();
    
    // 软件复位
    eink_send_command(EINK_CMD_SW_RESET);
    eink_wait_busy();
    
    // 驱动输出控制
    eink_send_command(EINK_CMD_DRIVER_OUTPUT_CONTROL);
    eink_send_data((EINK_HEIGHT - 1) & 0xFF);
    eink_send_data(((EINK_HEIGHT - 1) >> 8) & 0xFF);
    eink_send_data(0x00);
    
    // 升压软启动控制
    eink_send_command(EINK_CMD_BOOSTER_SOFT_START_CONTROL);
    eink_send_data(0xD7);
    eink_send_data(0xD6);
    eink_send_data(0x9D);
    
    // VCOM设置
    eink_send_command(EINK_CMD_WRITE_VCOM_REGISTER);
    eink_send_data(0xA8);
    
    // 虚拟线周期设置
    eink_send_command(EINK_CMD_SET_DUMMY_LINE_PERIOD);
    eink_send_data(0x1A);
    
    // 门时间设置
    eink_send_command(EINK_CMD_SET_GATE_TIME);
    eink_send_data(0x08);
    
    // 数据输入模式
    eink_send_command(EINK_CMD_DATA_ENTRY_MODE_SETTING);
    eink_send_data(0x03);
    
    // 设置LUT
    eink_set_lut(lut_full_update);
    
    // 清空显示缓冲区
    memset(display_buffer, EINK_COLOR_WHITE, sizeof(display_buffer));
    
    eink_initialized = true;
    
    #ifdef DEBUG_UART_ENABLED
    printf("E-Ink display initialized successfully\n");
    #endif
    
    return ERROR_NONE;
}

/**
 * @brief 清屏
 * @return 0: 成功, 其他: 错误代码
 */
int eink_clear(void)
{
    if (!eink_initialized) {
        return ERROR_INIT_FAILED;
    }
    
    // 清空缓冲区
    memset(display_buffer, EINK_COLOR_WHITE, sizeof(display_buffer));
    
    return ERROR_NONE;
}

/**
 * @brief 设置像素点
 * @param x X坐标
 * @param y Y坐标
 * @param color 颜色 (0=黑色, 1=白色)
 * @return 0: 成功, 其他: 错误代码
 */
int eink_set_pixel(uint16_t x, uint16_t y, uint8_t color)
{
    if (!eink_initialized) {
        return ERROR_INIT_FAILED;
    }
    
    if (x >= EINK_WIDTH || y >= EINK_HEIGHT) {
        return ERROR_INVALID_PARAM;
    }
    
    uint32_t byte_index = (y * EINK_WIDTH + x) / 8;
    uint8_t bit_index = x % 8;
    
    if (color == 0) {
        // 黑色
        display_buffer[byte_index] &= ~(0x80 >> bit_index);
    } else {
        // 白色
        display_buffer[byte_index] |= (0x80 >> bit_index);
    }
    
    return ERROR_NONE;
}

/**
 * @brief 获取像素点颜色
 * @param x X坐标
 * @param y Y坐标
 * @return 像素颜色 (0=黑色, 1=白色, 0xFF=错误)
 */
uint8_t eink_get_pixel(uint16_t x, uint16_t y)
{
    if (!eink_initialized || x >= EINK_WIDTH || y >= EINK_HEIGHT) {
        return 0xFF;
    }
    
    uint32_t byte_index = (y * EINK_WIDTH + x) / 8;
    uint8_t bit_index = x % 8;
    
    return (display_buffer[byte_index] & (0x80 >> bit_index)) ? 1 : 0;
}

/**
 * @brief 画线
 * @param x0 起始X坐标
 * @param y0 起始Y坐标
 * @param x1 结束X坐标
 * @param y1 结束Y坐标
 * @param color 颜色
 * @return 0: 成功, 其他: 错误代码
 */
int eink_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color)
{
    if (!eink_initialized) {
        return ERROR_INIT_FAILED;
    }
    
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    
    while (1) {
        eink_set_pixel(x0, y0, color);
        
        if (x0 == x1 && y0 == y1) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
    
    return ERROR_NONE;
}

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
int eink_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color, bool filled)
{
    if (!eink_initialized) {
        return ERROR_INIT_FAILED;
    }
    
    if (filled) {
        for (uint16_t i = 0; i < height; i++) {
            for (uint16_t j = 0; j < width; j++) {
                eink_set_pixel(x + j, y + i, color);
            }
        }
    } else {
        // 画边框
        eink_draw_line(x, y, x + width - 1, y, color);                    // 上边
        eink_draw_line(x, y + height - 1, x + width - 1, y + height - 1, color); // 下边
        eink_draw_line(x, y, x, y + height - 1, color);                   // 左边
        eink_draw_line(x + width - 1, y, x + width - 1, y + height - 1, color);  // 右边
    }
    
    return ERROR_NONE;
}

/**
 * @brief 显示文本
 * @param x X坐标
 * @param y Y坐标
 * @param text 文本内容
 * @param font_size 字体大小
 * @param color 颜色
 * @return 0: 成功, 其他: 错误代码
 */
int eink_display_text(uint16_t x, uint16_t y, const char* text, uint8_t font_size, uint8_t color)
{
    if (!eink_initialized || !text) {
        return ERROR_INVALID_PARAM;
    }
    
    // 简单的8x8字符显示（可以扩展为更复杂的字体系统）
    uint16_t char_x = x;
    uint16_t char_y = y;
    
    while (*text) {
        if (*text == '\n') {
            char_x = x;
            char_y += font_size + 2;
        } else {
            // 这里应该调用字体渲染函数
            // 简化实现：画一个矩形代表字符
            eink_draw_rectangle(char_x, char_y, font_size - 2, font_size - 2, color, false);
            char_x += font_size;
        }
        
        if (char_x >= EINK_WIDTH) {
            char_x = x;
            char_y += font_size + 2;
        }
        
        if (char_y >= EINK_HEIGHT) {
            break;
        }
        
        text++;
    }
    
    return ERROR_NONE;
}

/**
 * @brief 刷新显示
 * @param mode 刷新模式 (0=全刷新, 1=部分刷新)
 * @return 0: 成功, 其他: 错误代码
 */
int eink_refresh(uint8_t mode)
{
    if (!eink_initialized) {
        return ERROR_INIT_FAILED;
    }
    
    // 设置内存区域
    eink_set_memory_area(0, 0, EINK_WIDTH - 1, EINK_HEIGHT - 1);
    eink_set_memory_pointer(0, 0);
    
    // 写入显示数据
    eink_send_command(EINK_CMD_WRITE_RAM);
    eink_send_data_buffer(display_buffer, sizeof(display_buffer));
    
    // 根据模式设置LUT
    if (mode == EINK_DISPLAY_MODE_FULL || refresh_counter % DISPLAY_FULL_REFRESH_INTERVAL == 0) {
        eink_set_lut(lut_full_update);
        current_display_mode = EINK_DISPLAY_MODE_FULL;
    } else {
        eink_set_lut(lut_partial_update);
        current_display_mode = EINK_DISPLAY_MODE_PARTIAL;
    }
    
    // 启动显示更新
    eink_send_command(EINK_CMD_DISPLAY_UPDATE_CONTROL_2);
    eink_send_data(0xC4);
    eink_send_command(EINK_CMD_MASTER_ACTIVATION);
    eink_send_command(EINK_CMD_TERMINATE_FRAME_READ_WRITE);
    
    // 等待刷新完成
    eink_wait_busy();
    
    refresh_counter++;
    
    #ifdef DEBUG_UART_ENABLED
    printf("Display refreshed (mode: %s, counter: %d)\n", 
           (current_display_mode == EINK_DISPLAY_MODE_FULL) ? "full" : "partial", 
           refresh_counter);
    #endif
    
    return ERROR_NONE;
}

/**
 * @brief 进入睡眠模式
 * @return 0: 成功, 其他: 错误代码
 */
int eink_sleep(void)
{
    if (!eink_initialized) {
        return ERROR_INIT_FAILED;
    }
    
    eink_send_command(EINK_CMD_DEEP_SLEEP_MODE);
    eink_send_data(0x01);
    
    #ifdef DEBUG_UART_ENABLED
    printf("E-Ink display entered sleep mode\n");
    #endif
    
    return ERROR_NONE;
}

/**
 * @brief 从睡眠模式唤醒
 * @return 0: 成功, 其他: 错误代码
 */
int eink_wakeup(void)
{
    if (!eink_initialized) {
        return ERROR_INIT_FAILED;
    }
    
    // 硬件复位唤醒
    eink_reset();
    
    // 重新初始化（简化版）
    eink_wait_busy();
    eink_send_command(EINK_CMD_SW_RESET);
    eink_wait_busy();
    
    #ifdef DEBUG_UART_ENABLED
    printf("E-Ink display wakeup from sleep\n");
    #endif
    
    return ERROR_NONE;
}

// ============================================================================
// 私有函数实现
// ============================================================================

/**
 * @brief 初始化SPI
 */
static void eink_spi_init(void)
{
    spi_cfg_t spi_cfg = {
        .spi_ms = SPI_MS_MODE_MASTER,
        .spi_cp = SPI_CP_MODE_0,
        .spi_speed = SPI_SPEED_MODE_4MHz,
        .spi_wsz = SPI_WSZ_MODE_8BIT,
        .spi_cs = SPI_CS_0,
        .cs_pad.port = GPIO_PORT_0,
        .cs_pad.pin = EINK_CS_PIN,
    };
    
    spi_initialize(&spi_cfg);
}

/**
 * @brief 初始化GPIO
 */
static void eink_gpio_init(void)
{
    // DC引脚
    GPIO_ConfigurePin(GPIO_PORT_0, EINK_DC_PIN, OUTPUT, PID_GPIO, false);
    
    // RST引脚
    GPIO_ConfigurePin(GPIO_PORT_0, EINK_RST_PIN, OUTPUT, PID_GPIO, false);
    
    // BUSY引脚
    GPIO_ConfigurePin(GPIO_PORT_0, EINK_BUSY_PIN, INPUT, PID_GPIO, false);
}

/**
 * @brief 硬件复位
 */
static void eink_reset(void)
{
    GPIO_SetActive(GPIO_PORT_0, EINK_RST_PIN);
    timer_usleep(200000); // 200ms
    GPIO_SetInactive(GPIO_PORT_0, EINK_RST_PIN);
    timer_usleep(200000); // 200ms
    GPIO_SetActive(GPIO_PORT_0, EINK_RST_PIN);
    timer_usleep(200000); // 200ms
}

/**
 * @brief 发送命令
 */
static void eink_send_command(uint8_t command)
{
    GPIO_SetInactive(GPIO_PORT_0, EINK_DC_PIN); // DC = 0 (命令)
    spi_send(&command, 1, SPI_OP_BLOCKING);
}

/**
 * @brief 发送数据
 */
static void eink_send_data(uint8_t data)
{
    GPIO_SetActive(GPIO_PORT_0, EINK_DC_PIN); // DC = 1 (数据)
    spi_send(&data, 1, SPI_OP_BLOCKING);
}

/**
 * @brief 发送数据缓冲区
 */
static void eink_send_data_buffer(const uint8_t* data, uint16_t length)
{
    GPIO_SetActive(GPIO_PORT_0, EINK_DC_PIN); // DC = 1 (数据)
    spi_send(data, length, SPI_OP_BLOCKING);
}

/**
 * @brief 等待忙信号
 */
static void eink_wait_busy(void)
{
    uint32_t timeout = 0;
    while (GPIO_GetPinStatus(GPIO_PORT_0, EINK_BUSY_PIN) == true) {
        timer_usleep(10000); // 10ms
        timeout++;
        if (timeout > 500) { // 5秒超时
            #ifdef DEBUG_UART_ENABLED
            printf("E-Ink busy timeout!\n");
            #endif
            break;
        }
    }
}

/**
 * @brief 设置LUT表
 */
static void eink_set_lut(const uint8_t* lut)
{
    eink_send_command(EINK_CMD_WRITE_LUT_REGISTER);
    eink_send_data_buffer(lut, 30);
}

/**
 * @brief 设置内存区域
 */
static void eink_set_memory_area(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
    eink_send_command(EINK_CMD_SET_RAM_X_ADDRESS_START_END);
    eink_send_data((x_start >> 3) & 0xFF);
    eink_send_data((x_end >> 3) & 0xFF);
    
    eink_send_command(EINK_CMD_SET_RAM_Y_ADDRESS_START_END);
    eink_send_data(y_start & 0xFF);
    eink_send_data((y_start >> 8) & 0xFF);
    eink_send_data(y_end & 0xFF);
    eink_send_data((y_end >> 8) & 0xFF);
}

/**
 * @brief 设置内存指针
 */
static void eink_set_memory_pointer(uint16_t x, uint16_t y)
{
    eink_send_command(EINK_CMD_SET_RAM_X_ADDRESS_COUNTER);
    eink_send_data((x >> 3) & 0xFF);
    
    eink_send_command(EINK_CMD_SET_RAM_Y_ADDRESS_COUNTER);
    eink_send_data(y & 0xFF);
    eink_send_data((y >> 8) & 0xFF);
}