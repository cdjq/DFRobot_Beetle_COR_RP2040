DFRobot_Beetle_COR_PR2040
===========================

* [中文版](./README_CN.md)

这是字库芯片的驱动程序，支持中文，英文，拉丁文，日文，韩文，希腊文的显示输出。只需传入字符串就能方便显示不同的语言文字。

![产品效果图片](./resources/images/SEN0501.png)
  
## Product Link (https://www.dfrobot.com)
    SKU: SEN0500/SEN0501

## Table of Contents

  * [Summary](#summary)
  * [Installation](#installation)
  * [Methods](#methods)
  * [Compatibility](#compatibility)
  * [History](#history)
  * [Credits](#credits)

## Summary

这是字库芯片的驱动程序，支持中文，英文，拉丁文，日文，韩文，希腊文的显示输出。

## Installation

使用该库前需要先下载DFRobot_GDL库，库链接：https://github.com/DFRobot/DFRobot_GDL
然后下载库文件，将其粘贴到\Arduino\libraries目录中，然后打开examples文件夹并在该文件夹中运行演示。

## Methods

```C++
    /**
   * @fn begin
   * @brief 初始化字库芯片
   * @return uint8_t 初始化状态
   */
  uint8_t begin(void);

  /**
   * @fn playString
   * @brief 显示输出
   * @param x X坐标
   * @param y Y坐标
   * @param str 显示的字符串
   * @param color 显示字体颜色
   * @param bg 显示字体背景色
   * @param font 显示字体的大小
   */
  void playString(uint16_t x, uint16_t y, String str, uint16_t color, uint16_t bg, eFontSize_t font);
```

## Compatibility

MCU                | SoftwareSerial | HardwareSerial |      IIC      |
------------------ | :----------: | :----------: | :----------: | 
PR2040             |      √       |      X       |      √       |

## History

- 2022-08-24 - Version 1.0.0 released.

## Credits

Written by TangJie(jie.tang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))
