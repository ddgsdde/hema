# Open E-Ink Firmware for DA14585

一个开源的DA14585电子墨水屏固件，移除了所有激活限制和锁定机制。

## 项目特性

### ✅ 已实现功能
- **无激活限制**: 完全开放使用，无需激活码
- **蓝牙BLE通信**: 基于DA14585的低功耗蓝牙
- **电子墨水屏支持**: 支持常见的SPI接口电子墨水屏
- **按键输入**: 支持多按键输入和组合键
- **低功耗设计**: 优化的电源管理
- **数据存储**: 支持配置和数据持久化
- **OTA更新**: 支持无线固件更新

### 🚀 核心优势
- **完全开源**: MIT许可证，可自由修改和分发
- **无使用限制**: 移除原厂激活机制
- **高度可定制**: 模块化设计，易于扩展
- **低功耗**: 优化的睡眠和唤醒机制
- **稳定可靠**: 基于成熟的DA14585 SDK

## 硬件要求

### 必需组件
- **主控芯片**: DA14585 (QFN48封装推荐)
- **电子墨水屏**: SPI接口，支持1.54"、2.13"、2.9"等规格
- **按键**: 1-4个物理按键
- **电源**: 3.3V供电，支持电池供电
- **晶振**: 16MHz外部晶振

### 推荐硬件配置
```
MCU: DA14585 (512KB Flash, 128KB RAM)
Display: 2.13" E-Paper Display (250x122)
Buttons: 3x Tactile Switches
Power: CR2032 Battery Holder
Crystal: 16MHz ±20ppm
```

## 快速开始

### 1. 开发环境设置
```bash
# 安装ARM工具链
sudo apt-get install gcc-arm-none-eabi

# 下载DA14585 SDK
wget https://www.dialog-semiconductor.com/da14585-sdk

# 编译固件
make clean && make all
```

### 2. 烧录固件
```bash
# 使用J-Link烧录
make flash
```

## 许可证

本项目采用MIT许可证 - 详见 [LICENSE](LICENSE) 文件