DFRobot_Beetle_COR_PR2040
===========================

* [English Version](./README.md)

这是字库芯片的驱动程序，支持中文，英文，拉丁文，日文，韩文，希腊文的显示输出。只需传入字符串就能方便显示不同的语言文字。

![产品效果图片](./resources/images/SEN0501.png)

## 产品链接（https://www.dfrobot.com）

    SKU：SEN0500/SEN0501
  
## 目录

  * [概述](#概述)
  * [库安装](#库安装)
  * [方法](#方法)
  * [兼容性](#兼容性)
  * [历史](#历史)
  * [创作者](#创作者)

## 概述

这是字库芯片的驱动程序，支持中文，英文，拉丁文，日文，韩文，希腊文的显示输出。

## 库安装
使用该库前需要先下载DFRobot_GDL库，库链接：https://github.com/DFRobot/DFRobot_GDL
然后下载库文件，将其粘贴到\Arduino\libraries目录中，然后打开examples文件夹并在该文件夹中运行演示。

## 方法

```C++
  /**
   * @fn begin
   * @brief 初始化SEN0500/SEN0501传感器
   * @return 返回值初始化状态
   * @retval 0  成功
   * @retval -1 失败
   */
  int8_t begin(void);

  /**
   * @fn getTemperature
   * @brief 获取SEN0500/SEN0501温度数据
   * @param units 温度数据单位选择
   * @n     TEMP_C 摄氏度
   * @n     TEMP_F 华氏度 
   * @return 返回获取的温度数据
   */
  float getTemperature(uint8_t unist);

  /**
   * @fn getHumidity
   * @brief 获取SEN0500/SEN0501湿度数据 
   * @return 返回获取的湿度数据
   */
  float getHumidity(void);

  /**
   * @fn getUltravioletIntensity
   * @brief 获取SEN0500/SEN0501紫外线强度指数数据 
   * @return 返回获取的紫外线强度指数数据
   */
  float getUltravioletIntensity(void);

  /**
   * @fn getLuminousIntensity
   * @brief 获取SEN0500/SEN0501光线强度数据 
   * @return 返回获取的光线强度数据
   */
  float getLuminousIntensity(void);

  /**
   * @fn getAtmospherePressure
   * @brief 获取SEN0500/SEN0501大气压强数据 
   * @param units 大气压强数据单位选择
   * @n            HPA:百帕
   * @n            KPA:千帕
   * @return 返回获取的大气压强数据
   */
  uint16_t getAtmospherePressure(uint8_t units);

  /**
   * @fn getElevation
   * @brief 获取SEN0500/SEN0501海拔数据 
   * @return 返回获取的海拔数据
   */
  float getElevation(void);
```


## 兼容性

MCU                | SoftwareSerial | HardwareSerial |      IIC      |
------------------ | :----------: | :----------: | :----------: | 
PR2040             |      √       |      X       |      √       |

## 历史
- 2022-08-24 - 1.0.0 版本

## 创作者

Written by TangJie(jie.tang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))