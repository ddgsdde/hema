/**
 * @file ble_manager.h
 * @brief BLE Management Module Header
 * @author OpenHands AI Assistant
 * @date 2025-05-28
 * @license MIT License
 */

#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// BLE状态
// ============================================================================

typedef enum {
    BLE_STATE_IDLE,
    BLE_STATE_ADVERTISING,
    BLE_STATE_CONNECTED,
    BLE_STATE_DISCONNECTED
} ble_state_t;

// ============================================================================
// BLE数据回调函数类型
// ============================================================================

typedef void (*ble_data_callback_t)(const uint8_t* data, uint16_t length);

// ============================================================================
// 公共函数声明
// ============================================================================

/**
 * @brief 初始化BLE管理器
 * @return 0: 成功, 其他: 错误代码
 */
int ble_manager_init(void);

/**
 * @brief 开始广播
 * @return 0: 成功, 其他: 错误代码
 */
int ble_start_advertising(void);

/**
 * @brief 停止广播
 * @return 0: 成功, 其他: 错误代码
 */
int ble_stop_advertising(void);

/**
 * @brief 发送数据
 * @param data 数据缓冲区
 * @param length 数据长度
 * @return 0: 成功, 其他: 错误代码
 */
int ble_send_data(const uint8_t* data, uint16_t length);

/**
 * @brief 注册数据接收回调
 * @param callback 回调函数
 * @return 0: 成功, 其他: 错误代码
 */
int ble_register_data_callback(ble_data_callback_t callback);

/**
 * @brief 获取连接状态
 * @return true: 已连接, false: 未连接
 */
bool ble_is_connected(void);

/**
 * @brief 获取BLE状态
 * @return 当前BLE状态
 */
ble_state_t ble_get_state(void);

#endif // BLE_MANAGER_H