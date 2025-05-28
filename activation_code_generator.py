#!/usr/bin/env python3
"""
DA14585电子墨水屏固件激活码生成器

基于对HM213固件的逆向工程分析，这个工具可以根据设备MAC地址的后6位
生成对应的激活码。

重要发现：
1. 设备使用三个按键通过蓝牙发送激活码：
   - 数字 1 -> 按键代码 0xE102
   - 数字 2 -> 按键代码 0xE101  
   - 数字 3 -> 按键代码 0xE100
2. 激活码为7位数字，每位只能是1、2、3（三进制系统）
3. 算法尚未完全破解，需要更多示例数据

已知示例：
- MAC后6位: 682BFE -> 激活码: 2322231
- MAC后6位: 67A78C -> 激活码: 1331222

使用方法：
1. 查看设备屏幕上显示的MAC地址后6位
2. 运行此脚本并输入这6位十六进制数字
3. 使用生成的激活码通过三个按键在设备上激活

注意：当前算法基于有限的示例数据，可能不完全准确。

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
        int(mac_last_6, 16)
        
        # 检查是否是已知的示例
        known_examples = {
            "682BFE": "2322231",
            "67A78C": "1331222"
        }
        
        if mac_last_6 in known_examples:
            return known_examples[mac_last_6]
        
        # 对于未知的MAC地址，尝试推断算法
        # 注意：这个算法可能不准确，需要更多数据验证
        return "未知：需要更多示例数据来确定算法"
    
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
    print("重要发现：")
    print("- 激活码为7位数字，每位只能是1、2、3")
    print("- 通过三个按键输入：1(0xE102), 2(0xE101), 3(0xE100)")
    print("- 算法尚未完全破解，需要更多示例")
    print()
    print("使用说明：")
    print("1. 查看设备屏幕上显示的芯片MAC地址")
    print("2. 输入MAC地址的后6位十六进制数字")
    print("3. 使用生成的激活码通过三个按键激活设备")
    print()
    
    # 显示已知示例
    print("已知示例：")
    known_examples = [
        ("682BFE", "2322231"),
        ("67A78C", "1331222"),
    ]
    
    for mac, code in known_examples:
        print(f"  MAC后6位: {mac} -> 激活码: {code}")
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
        
        if "未知" in result:
            print("提示：如果你有这个MAC地址对应的正确激活码，")
            print("请提供给开发者以改进算法。")
        print()

if __name__ == "__main__":
    main()