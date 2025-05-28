# Open E-Ink Firmware 开发指南

## 项目概述

这是一个完全开源的DA14585电子墨水屏固件项目，**移除了所有激活限制和锁定机制**。基于我们对原始固件的深度逆向分析，重新实现了所有核心功能。

## 🚀 核心特性

### ✅ 完全开放
- **无激活码限制**: 移除了原厂的MAC地址锁定机制
- **无使用次数限制**: 可以无限次使用所有功能
- **开源代码**: MIT许可证，可自由修改和分发
- **无隐藏功能**: 所有代码透明可见

### 🔧 技术特性
- **DA14585支持**: 基于Dialog半导体的低功耗蓝牙芯片
- **电子墨水屏驱动**: 支持常见的SPI接口电子墨水屏
- **低功耗设计**: 优化的电源管理和睡眠机制
- **蓝牙BLE**: 完整的蓝牙低功耗通信栈
- **模块化设计**: 易于扩展和定制

## 📁 项目结构

```
open_eink_firmware/
├── README.md                   # 项目说明
├── LICENSE                     # MIT许可证
├── Makefile                    # 构建配置
├── build.sh                    # 构建脚本
├── DEVELOPMENT_GUIDE.md        # 开发指南
├── config/                     # 配置文件
│   └── config.h               # 主配置文件
├── src/                        # 源代码
│   ├── main.c                 # 主程序入口
│   ├── app/                   # 应用层
│   │   ├── app.h              # 应用程序头文件
│   │   ├── display_manager.c  # 显示管理器
│   │   ├── display_manager.h
│   │   ├── button_manager.h   # 按键管理器
│   │   └── power_manager.h    # 电源管理器
│   ├── ble/                   # 蓝牙模块
│   │   ├── ble_manager.c      # BLE管理器
│   │   └── ble_manager.h
│   ├── drivers/               # 硬件驱动
│   │   ├── eink_driver.c      # 电子墨水屏驱动
│   │   └── eink_driver.h
│   ├── hal/                   # 硬件抽象层
│   └── system/                # 系统层
├── build/                      # 构建输出
├── docs/                       # 文档
└── tools/                      # 工具脚本
```

## 🛠️ 开发环境设置

### 1. 安装工具链

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install gcc-arm-none-eabi build-essential git
```

**macOS:**
```bash
brew install arm-none-eabi-gcc
```

**Windows:**
下载并安装 [GNU Arm Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)

### 2. 获取DA14585 SDK

```bash
# 下载官方SDK
wget https://www.dialog-semiconductor.com/da14585-sdk
# 或从官网下载并解压到本地目录

# 设置环境变量
export DA14585_SDK_PATH=/path/to/da14585_sdk
```

### 3. 克隆项目

```bash
git clone https://github.com/your-repo/open-eink-firmware.git
cd open-eink-firmware
```

## 🔨 构建固件

### 使用构建脚本（推荐）

```bash
# 基本构建
./build.sh

# 调试版本
./build.sh -d -c

# 发布版本并烧录
./build.sh -r -f -s

# 指定SDK路径
./build.sh --sdk-path /custom/path/to/sdk
```

### 使用Makefile

```bash
# 设置SDK路径
export DA14585_SDK_PATH=/path/to/sdk

# 构建发布版本
make clean
make all

# 构建调试版本
make clean
make DEBUG=1

# 显示大小信息
make size

# 烧录固件
make flash
```

## 📋 硬件配置

### 最小系统要求

```
MCU: DA14585 (QFN48)
Flash: 512KB (内置)
RAM: 128KB (内置)
Crystal: 16MHz ±20ppm
Power: 3.3V (1.8V-3.6V)
```

### 推荐硬件配置

```
Display: 2.13" E-Paper (250x122, SPI)
Buttons: 3x Tactile Switches
Battery: CR2032 Coin Cell
Connector: USB-C (可选，用于调试)
```

### 引脚配置

在 `config/config.h` 中修改引脚定义：

```c
// 电子墨水屏引脚
#define EINK_CS_PIN             GPIO_PIN_0
#define EINK_DC_PIN             GPIO_PIN_1
#define EINK_RST_PIN            GPIO_PIN_2
#define EINK_BUSY_PIN           GPIO_PIN_3

// 按键引脚
#define BUTTON_1_PIN            GPIO_PIN_4
#define BUTTON_2_PIN            GPIO_PIN_5
#define BUTTON_3_PIN            GPIO_PIN_6
```

## 🔧 功能定制

### 1. 添加新的显示内容

```c
// 在 src/app/display_manager.c 中添加
void display_custom_screen(void)
{
    eink_clear();
    eink_display_text(10, 10, "Custom Screen", FONT_LARGE_SIZE, 0);
    eink_display_text(10, 30, "Your content here", FONT_MEDIUM_SIZE, 0);
    eink_refresh(1);
}
```

### 2. 添加新的按键功能

```c
// 在按键处理函数中添加
void handle_custom_button_action(uint8_t button_id)
{
    switch(button_id) {
        case 1:
            display_custom_screen();
            break;
        case 2:
            // 其他功能
            break;
    }
}
```

### 3. 添加新的BLE服务

```c
// 在 src/ble/ble_manager.c 中添加
static int setup_custom_service(void)
{
    // 定义自定义服务UUID
    uint8_t custom_uuid[] = {0x12, 0x34, ...};
    
    // 添加服务和特征
    gatt_add_custom_service(custom_uuid);
    
    return ERROR_NONE;
}
```

## 🧪 测试和调试

### 1. 串口调试

启用调试输出：
```c
#define DEBUG_UART_ENABLED      1
#define DEBUG_LEVEL             3
```

连接串口工具（115200 baud）查看调试信息。

### 2. 单元测试

```bash
# 运行测试（如果有）
make test
```

### 3. 硬件测试

1. **显示测试**: 检查电子墨水屏是否正常显示
2. **按键测试**: 验证所有按键响应
3. **蓝牙测试**: 使用手机APP连接测试
4. **电源测试**: 测量功耗和电池寿命

## 📱 移动端应用

### Android应用开发

```java
// 连接到设备
BluetoothGatt gatt = device.connectGatt(context, false, gattCallback);

// 发送显示数据
byte[] displayData = "Hello World".getBytes();
characteristic.setValue(displayData);
gatt.writeCharacteristic(characteristic);
```

### iOS应用开发

```swift
// 连接到设备
centralManager.connect(peripheral, options: nil)

// 发送数据
peripheral.writeValue(data, for: characteristic, type: .withResponse)
```

## 🔒 安全考虑

### 1. 移除的安全限制

原始固件中的以下限制已被移除：
- MAC地址验证
- 激活码检查
- 使用次数限制
- 时间锁定

### 2. 新的安全特性（可选）

```c
// 可选：添加简单的配对验证
#define BLE_SECURITY_ENABLED    1
#define BLE_BONDING_ENABLED     1
```

## 📊 性能优化

### 1. 内存优化

```c
// 减少显示缓冲区大小
#define EINK_BUFFER_SIZE        (EINK_WIDTH * EINK_HEIGHT / 8)

// 使用静态内存分配
static uint8_t display_buffer[EINK_BUFFER_SIZE];
```

### 2. 功耗优化

```c
// 启用深度睡眠
#define DEEP_SLEEP_ENABLED      1

// 优化广播间隔
#define BLE_ADV_INTERVAL_MIN    800  // 500ms
#define BLE_ADV_INTERVAL_MAX    1600 // 1000ms
```

### 3. 显示优化

```c
// 使用部分刷新
eink_refresh(EINK_DISPLAY_MODE_PARTIAL);

// 减少全刷新频率
#define DISPLAY_FULL_REFRESH_INTERVAL   20
```

## 🐛 故障排除

### 常见问题

**Q: 编译失败，提示找不到SDK**
```bash
A: 确保设置了正确的SDK路径
export DA14585_SDK_PATH=/correct/path/to/sdk
```

**Q: 电子墨水屏无显示**
```bash
A: 检查以下项目：
1. SPI引脚连接是否正确
2. 电源供电是否稳定
3. 控制引脚配置是否匹配
4. 显示驱动是否兼容
```

**Q: 蓝牙无法连接**
```bash
A: 检查以下项目：
1. 广播是否正常启动
2. 设备名称是否正确
3. 服务UUID是否匹配
4. 手机蓝牙是否开启
```

**Q: 功耗过高**
```bash
A: 优化建议：
1. 启用睡眠模式
2. 降低广播频率
3. 优化显示刷新
4. 检查GPIO配置
```

## 🤝 贡献指南

### 1. 代码规范

- 使用4空格缩进
- 函数名使用下划线命名法
- 常量使用大写字母
- 添加适当的注释

### 2. 提交流程

```bash
# 1. Fork项目
git fork https://github.com/your-repo/open-eink-firmware.git

# 2. 创建功能分支
git checkout -b feature/new-feature

# 3. 提交更改
git commit -am "Add new feature"

# 4. 推送分支
git push origin feature/new-feature

# 5. 创建Pull Request
```

### 3. 测试要求

- 所有新功能需要添加测试
- 确保现有测试通过
- 提供硬件测试报告

## 📚 参考资料

### 官方文档
- [DA14585 Datasheet](https://www.dialog-semiconductor.com/da14585-datasheet)
- [DA14585 SDK Documentation](https://www.dialog-semiconductor.com/da14585-sdk-docs)

### 技术资料
- [ARM Cortex-M0 Reference](https://developer.arm.com/documentation/dui0497/a)
- [Bluetooth Low Energy Specification](https://www.bluetooth.com/specifications/bluetooth-core-specification/)

### 开发工具
- [GNU Arm Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)
- [J-Link Software](https://www.segger.com/downloads/jlink/)

## 📞 支持和联系

- **项目主页**: https://github.com/your-repo/open-eink-firmware
- **问题反馈**: https://github.com/your-repo/open-eink-firmware/issues
- **讨论区**: https://github.com/your-repo/open-eink-firmware/discussions

---

**免责声明**: 本固件为开源项目，仅供学习和研究使用。使用者需自行承担使用风险。

**许可证**: MIT License - 详见 [LICENSE](LICENSE) 文件