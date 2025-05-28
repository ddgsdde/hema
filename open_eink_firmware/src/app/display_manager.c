/**
 * @file display_manager.c
 * @brief Display Management Module
 * @author OpenHands AI Assistant
 * @date 2025-05-28
 * @license MIT License
 * 
 * 显示管理模块
 * 负责界面显示、菜单管理和用户交互
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "display_manager.h"
#include "eink_driver.h"
#include "config.h"

// ============================================================================
// 私有定义
// ============================================================================

// 界面状态
typedef enum {
    SCREEN_WELCOME,
    SCREEN_MAIN_MENU,
    SCREEN_SETTINGS,
    SCREEN_ABOUT,
    SCREEN_ERROR,
    SCREEN_SLEEP
} screen_state_t;

// 菜单项
typedef struct {
    const char* title;
    void (*action)(void);
} menu_item_t;

// ============================================================================
// 私有变量
// ============================================================================

static screen_state_t current_screen = SCREEN_WELCOME;
static uint8_t menu_selection = 0;
static bool display_needs_refresh = false;
static char status_text[64] = "Ready";

// ============================================================================
// 私有函数声明
// ============================================================================

static void draw_welcome_screen(void);
static void draw_main_menu(void);
static void draw_settings_screen(void);
static void draw_about_screen(void);
static void draw_error_screen(const char* error_msg);
static void draw_header(const char* title);
static void draw_status_bar(void);
static void menu_action_settings(void);
static void menu_action_about(void);
static void menu_action_sleep(void);

// 主菜单项
static const menu_item_t main_menu_items[] = {
    {"Settings", menu_action_settings},
    {"About", menu_action_about},
    {"Sleep", menu_action_sleep},
};

static const uint8_t main_menu_count = sizeof(main_menu_items) / sizeof(menu_item_t);

// ============================================================================
// 公共函数实现
// ============================================================================

/**
 * @brief 初始化显示管理器
 * @return 0: 成功, 其他: 错误代码
 */
int display_init(void)
{
    int result = eink_init();
    if (result != 0) {
        return result;
    }
    
    // 显示欢迎界面
    current_screen = SCREEN_WELCOME;
    display_needs_refresh = true;
    
    #ifdef DEBUG_UART_ENABLED
    printf("Display manager initialized\n");
    #endif
    
    return ERROR_NONE;
}

/**
 * @brief 显示欢迎界面
 */
void display_welcome_screen(void)
{
    current_screen = SCREEN_WELCOME;
    display_needs_refresh = true;
}

/**
 * @brief 显示主菜单
 */
void display_main_menu(void)
{
    current_screen = SCREEN_MAIN_MENU;
    menu_selection = 0;
    display_needs_refresh = true;
}

/**
 * @brief 显示错误界面
 * @param error_msg 错误信息
 */
void display_error_screen(const char* error_msg)
{
    current_screen = SCREEN_ERROR;
    strncpy(status_text, error_msg ? error_msg : "Unknown Error", sizeof(status_text) - 1);
    status_text[sizeof(status_text) - 1] = '\0';
    display_needs_refresh = true;
}

/**
 * @brief 处理按键输入
 * @param button_id 按键ID
 * @param event 按键事件
 */
void display_handle_button(uint8_t button_id, uint8_t event)
{
    switch (current_screen) {
        case SCREEN_WELCOME:
            if (event == 1) { // 按键按下
                display_main_menu();
            }
            break;
            
        case SCREEN_MAIN_MENU:
            if (button_id == 1 && event == 1) { // 按键1：上
                if (menu_selection > 0) {
                    menu_selection--;
                    display_needs_refresh = true;
                }
            } else if (button_id == 2 && event == 1) { // 按键2：下
                if (menu_selection < main_menu_count - 1) {
                    menu_selection++;
                    display_needs_refresh = true;
                }
            } else if (button_id == 3 && event == 1) { // 按键3：确认
                if (main_menu_items[menu_selection].action) {
                    main_menu_items[menu_selection].action();
                }
            }
            break;
            
        case SCREEN_SETTINGS:
        case SCREEN_ABOUT:
            if (event == 1) { // 任意按键返回主菜单
                display_main_menu();
            }
            break;
            
        case SCREEN_ERROR:
            if (event == 1) { // 任意按键返回主菜单
                display_main_menu();
            }
            break;
            
        default:
            break;
    }
}

/**
 * @brief 处理显示更新
 */
void display_process(void)
{
    if (!display_needs_refresh) {
        return;
    }
    
    // 清屏
    eink_clear();
    
    // 根据当前界面绘制内容
    switch (current_screen) {
        case SCREEN_WELCOME:
            draw_welcome_screen();
            break;
            
        case SCREEN_MAIN_MENU:
            draw_main_menu();
            break;
            
        case SCREEN_SETTINGS:
            draw_settings_screen();
            break;
            
        case SCREEN_ABOUT:
            draw_about_screen();
            break;
            
        case SCREEN_ERROR:
            draw_error_screen(status_text);
            break;
            
        default:
            break;
    }
    
    // 刷新显示
    eink_refresh(1); // 使用部分刷新
    
    display_needs_refresh = false;
    
    #ifdef DEBUG_UART_ENABLED
    printf("Display updated (screen: %d)\n", current_screen);
    #endif
}

/**
 * @brief 设置状态文本
 * @param text 状态文本
 */
void display_set_status(const char* text)
{
    if (text) {
        strncpy(status_text, text, sizeof(status_text) - 1);
        status_text[sizeof(status_text) - 1] = '\0';
        display_needs_refresh = true;
    }
}

/**
 * @brief 强制刷新显示
 */
void display_force_refresh(void)
{
    display_needs_refresh = true;
}

// ============================================================================
// 私有函数实现
// ============================================================================

/**
 * @brief 绘制欢迎界面
 */
static void draw_welcome_screen(void)
{
    // 绘制标题
    eink_display_text(50, 30, "OpenEInk", FONT_LARGE_SIZE, 0);
    eink_display_text(40, 50, "Open Source", FONT_MEDIUM_SIZE, 0);
    eink_display_text(30, 70, "E-Ink Firmware", FONT_MEDIUM_SIZE, 0);
    
    // 绘制版本信息
    eink_display_text(60, 90, "v" FIRMWARE_VERSION, FONT_SMALL_SIZE, 0);
    
    // 绘制提示信息
    eink_display_text(20, 110, "Press any key to continue", FONT_SMALL_SIZE, 0);
    
    // 绘制边框
    eink_draw_rectangle(5, 5, EINK_WIDTH - 10, EINK_HEIGHT - 10, 0, false);
}

/**
 * @brief 绘制主菜单
 */
static void draw_main_menu(void)
{
    draw_header("Main Menu");
    
    // 绘制菜单项
    for (uint8_t i = 0; i < main_menu_count; i++) {
        uint16_t y = 30 + i * 20;
        
        // 绘制选择指示器
        if (i == menu_selection) {
            eink_display_text(10, y, ">", FONT_MEDIUM_SIZE, 0);
        }
        
        // 绘制菜单项文本
        eink_display_text(25, y, main_menu_items[i].title, FONT_MEDIUM_SIZE, 0);
    }
    
    draw_status_bar();
}

/**
 * @brief 绘制设置界面
 */
static void draw_settings_screen(void)
{
    draw_header("Settings");
    
    eink_display_text(10, 30, "Display:", FONT_MEDIUM_SIZE, 0);
    eink_display_text(20, 45, "Brightness: Auto", FONT_SMALL_SIZE, 0);
    eink_display_text(20, 60, "Refresh: Optimized", FONT_SMALL_SIZE, 0);
    
    eink_display_text(10, 80, "Power:", FONT_MEDIUM_SIZE, 0);
    eink_display_text(20, 95, "Sleep: 30s", FONT_SMALL_SIZE, 0);
    
    eink_display_text(10, 110, "Press any key to return", FONT_SMALL_SIZE, 0);
}

/**
 * @brief 绘制关于界面
 */
static void draw_about_screen(void)
{
    draw_header("About");
    
    eink_display_text(10, 30, "OpenEInk Firmware", FONT_MEDIUM_SIZE, 0);
    eink_display_text(10, 45, "Version: " FIRMWARE_VERSION, FONT_SMALL_SIZE, 0);
    eink_display_text(10, 60, "Build: " __DATE__, FONT_SMALL_SIZE, 0);
    
    eink_display_text(10, 80, "Features:", FONT_SMALL_SIZE, 0);
    eink_display_text(15, 95, "- No activation required", FONT_SMALL_SIZE, 0);
    eink_display_text(15, 105, "- Open source", FONT_SMALL_SIZE, 0);
    eink_display_text(15, 115, "- Low power design", FONT_SMALL_SIZE, 0);
}

/**
 * @brief 绘制错误界面
 */
static void draw_error_screen(const char* error_msg)
{
    draw_header("Error");
    
    // 绘制错误图标（简单的X）
    eink_draw_line(50, 40, 70, 60, 0);
    eink_draw_line(70, 40, 50, 60, 0);
    
    // 绘制错误信息
    eink_display_text(10, 80, error_msg, FONT_MEDIUM_SIZE, 0);
    
    eink_display_text(10, 110, "Press any key to continue", FONT_SMALL_SIZE, 0);
}

/**
 * @brief 绘制标题栏
 */
static void draw_header(const char* title)
{
    // 绘制标题背景
    eink_draw_rectangle(0, 0, EINK_WIDTH, UI_TITLE_HEIGHT, 0, true);
    
    // 绘制标题文本（白色）
    eink_display_text(10, 5, title, FONT_MEDIUM_SIZE, 1);
}

/**
 * @brief 绘制状态栏
 */
static void draw_status_bar(void)
{
    uint16_t status_y = EINK_HEIGHT - UI_STATUS_HEIGHT;
    
    // 绘制状态栏背景
    eink_draw_line(0, status_y, EINK_WIDTH, status_y, 0);
    
    // 绘制状态文本
    eink_display_text(5, status_y + 2, status_text, FONT_SMALL_SIZE, 0);
    
    // 绘制电池指示器（简化）
    eink_draw_rectangle(EINK_WIDTH - 25, status_y + 2, 20, 10, 0, false);
    eink_draw_rectangle(EINK_WIDTH - 5, status_y + 5, 3, 4, 0, true);
}

// ============================================================================
// 菜单动作函数
// ============================================================================

/**
 * @brief 设置菜单动作
 */
static void menu_action_settings(void)
{
    current_screen = SCREEN_SETTINGS;
    display_needs_refresh = true;
}

/**
 * @brief 关于菜单动作
 */
static void menu_action_about(void)
{
    current_screen = SCREEN_ABOUT;
    display_needs_refresh = true;
}

/**
 * @brief 睡眠菜单动作
 */
static void menu_action_sleep(void)
{
    display_set_status("Entering sleep mode...");
    display_process();
    
    // 进入睡眠模式
    eink_sleep();
    current_screen = SCREEN_SLEEP;
}