#!/bin/bash

# Open E-Ink Firmware Build Script
# Author: OpenHands AI Assistant
# Date: 2025-05-28
# License: MIT License

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 打印带颜色的消息
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# 显示帮助信息
show_help() {
    echo "Open E-Ink Firmware Build Script"
    echo ""
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -h, --help     Show this help message"
    echo "  -c, --clean    Clean build files before building"
    echo "  -d, --debug    Build debug version"
    echo "  -r, --release  Build release version (default)"
    echo "  -f, --flash    Flash firmware after building"
    echo "  -s, --size     Show size information after building"
    echo "  --sdk-path     Specify DA14585 SDK path"
    echo ""
    echo "Examples:"
    echo "  $0                    # Build release version"
    echo "  $0 -d -c             # Clean and build debug version"
    echo "  $0 -r -f -s          # Build release, flash, and show size"
    echo "  $0 --sdk-path /path/to/sdk  # Use specific SDK path"
}

# 默认参数
BUILD_TYPE="release"
CLEAN_BUILD=false
FLASH_FIRMWARE=false
SHOW_SIZE=false
SDK_PATH_OVERRIDE=""

# 解析命令行参数
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            exit 0
            ;;
        -c|--clean)
            CLEAN_BUILD=true
            shift
            ;;
        -d|--debug)
            BUILD_TYPE="debug"
            shift
            ;;
        -r|--release)
            BUILD_TYPE="release"
            shift
            ;;
        -f|--flash)
            FLASH_FIRMWARE=true
            shift
            ;;
        -s|--size)
            SHOW_SIZE=true
            shift
            ;;
        --sdk-path)
            SDK_PATH_OVERRIDE="$2"
            shift 2
            ;;
        *)
            print_error "Unknown option: $1"
            show_help
            exit 1
            ;;
    esac
done

# 检查工具链
check_toolchain() {
    print_info "Checking ARM toolchain..."
    
    if ! command -v arm-none-eabi-gcc &> /dev/null; then
        print_error "ARM toolchain not found. Please install gcc-arm-none-eabi"
        print_info "Ubuntu/Debian: sudo apt-get install gcc-arm-none-eabi"
        print_info "macOS: brew install arm-none-eabi-gcc"
        exit 1
    fi
    
    local gcc_version=$(arm-none-eabi-gcc --version | head -n1)
    print_success "ARM toolchain found: $gcc_version"
}

# 检查SDK路径
check_sdk() {
    print_info "Checking DA14585 SDK..."
    
    if [[ -n "$SDK_PATH_OVERRIDE" ]]; then
        export DA14585_SDK_PATH="$SDK_PATH_OVERRIDE"
    fi
    
    if [[ -z "$DA14585_SDK_PATH" ]]; then
        print_error "DA14585_SDK_PATH not set"
        print_info "Please set the environment variable or use --sdk-path option"
        print_info "Example: export DA14585_SDK_PATH=/path/to/da14585_sdk"
        exit 1
    fi
    
    if [[ ! -d "$DA14585_SDK_PATH" ]]; then
        print_error "SDK path does not exist: $DA14585_SDK_PATH"
        exit 1
    fi
    
    print_success "SDK found: $DA14585_SDK_PATH"
}

# 清理构建文件
clean_build() {
    if [[ "$CLEAN_BUILD" == true ]]; then
        print_info "Cleaning build files..."
        make clean
        print_success "Build files cleaned"
    fi
}

# 构建固件
build_firmware() {
    print_info "Building firmware ($BUILD_TYPE)..."
    
    local make_args=""
    if [[ "$BUILD_TYPE" == "debug" ]]; then
        make_args="DEBUG=1"
    fi
    
    if make $make_args; then
        print_success "Firmware built successfully"
    else
        print_error "Build failed"
        exit 1
    fi
}

# 显示大小信息
show_size_info() {
    if [[ "$SHOW_SIZE" == true ]]; then
        print_info "Size information:"
        make size
    fi
}

# 烧录固件
flash_firmware() {
    if [[ "$FLASH_FIRMWARE" == true ]]; then
        print_info "Flashing firmware..."
        
        if ! command -v JLinkExe &> /dev/null; then
            print_warning "J-Link not found, skipping flash"
            print_info "Please install J-Link software or flash manually"
            return
        fi
        
        if make flash; then
            print_success "Firmware flashed successfully"
        else
            print_error "Flash failed"
            exit 1
        fi
    fi
}

# 主函数
main() {
    echo "========================================"
    echo "Open E-Ink Firmware Build Script"
    echo "========================================"
    echo ""
    
    check_toolchain
    check_sdk
    clean_build
    build_firmware
    show_size_info
    flash_firmware
    
    echo ""
    print_success "Build process completed!"
    
    if [[ -f "build/open_eink_firmware.bin" ]]; then
        local file_size=$(stat -c%s "build/open_eink_firmware.bin" 2>/dev/null || stat -f%z "build/open_eink_firmware.bin" 2>/dev/null)
        print_info "Firmware size: $file_size bytes"
        print_info "Output file: build/open_eink_firmware.bin"
    fi
}

# 运行主函数
main