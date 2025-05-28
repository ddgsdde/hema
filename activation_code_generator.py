#!/usr/bin/env python3
"""
DA14585电子墨水屏固件激活码生成器

基于对HM213固件的逆向工程分析，这个工具可以根据设备MAC地址的后6位
生成对应的7位激活码。

分析发现：
1. 设备使用三个按键（0xE100, 0xE200, 0xE201）输入激活码
2. 激活码为7位十进制数字
3. 算法：激活码 = int(MAC后6位, 16) % 10000000

使用方法：
1. 查看设备屏幕上显示的MAC地址后6位
2. 运行此脚本并输入这6位十六进制数字
3. 使用生成的7位激活码在设备上激活

作者：OpenHands AI Assistant
日期：2025-05-28
"""

def generate_activation_code(mac_last_6):
    """
    根据MAC地址后6位生成激活码
    
    参数:
        mac_last_6 (str): MAC地址后6位十六进制字符串
        
    返回:
        str: 7位激活码或错误信息
    """
    try:
        # 清理输入
        mac_last_6 = mac_last_6.upper().strip().replace(':', '').replace('-', '')
        
        # 验证长度
        if len(mac_last_6) != 6:
            return "错误：MAC地址后6位必须是6个十六进制字符"
        
        # 验证是否为有效的十六进制
        mac_int = int(mac_last_6, 16)
        
        # 应用破解的算法
        activation_code = mac_int % 10000000
        
        return f"{activation_code:07d}"
    
    except ValueError:
        return "错误：请输入有效的十六进制字符（0-9, A-F）"

def main():
    print("=" * 60)
    print("DA14585电子墨水屏固件激活码生成器")
    print("=" * 60)
    print()
    print("基于HM213固件逆向工程分析")
    print("支持的设备型号：HM213_A25H01, HM213_A25L01, HM213_B25H01, HM213_B25L01")
    print()
    print("使用说明：")
    print("1. 查看设备屏幕上显示的芯片MAC地址")
    print("2. 输入MAC地址的后6位十六进制数字")
    print("3. 使用生成的7位激活码通过三个按键激活设备")
    print()
    
    # 显示一些示例
    print("示例：")
    examples = [
        ("123456", "如果MAC地址是 XX:XX:XX:12:34:56"),
        ("ABCDEF", "如果MAC地址是 XX:XX:XX:AB:CD:EF"),
        ("A1B2C3", "如果MAC地址是 XX:XX:XX:A1:B2:C3"),
    ]
    
    for mac, description in examples:
        code = generate_activation_code(mac)
        print(f"  {description}")
        print(f"  输入: {mac} -> 激活码: {code}")
        print()
    
    # 交互式输入
    while True:
        print("-" * 40)
        mac_input = input("请输入MAC地址后6位（输入'quit'退出）: ").strip()
        
        if mac_input.lower() in ['quit', 'exit', 'q']:
            print("再见！")
            break
        
        if not mac_input:
            continue
            
        result = generate_activation_code(mac_input)
        print(f"结果: {result}")
        print()

if __name__ == "__main__":
    main()