#ifndef _DFROBOT_PLAY_H_
#define _DFROBOT_PLAY_H_
#include "GT30L24A3W.h"
#include "DFRobot_GDL.h"
#include "Arduino.h"
#include<SPI.h>

#define PLAY_ENABLE_DBG ///< 打开这个宏, 可以看到程序的详细运行过程
#ifdef PLAY_ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

class DFRobot_Font : public DFRobot_ST7789_172x320_HW_SPI{
public:
  /**
   * @enum eFontSize_t
   * @brief 设置字体大小
   */
  typedef enum{
    eChineseFont24x24,
    eChineseFont12x12,
    eEnglishAndLatinFont12X12_A,
    eEnglishAndLatinFont16X16_A,
    eEnglishAndLatinFont12X24_A,
    eEnglishAndLatinFont12X24_P,
    eJapanFont24x24,
    eKoreanFont24x24,
    eGreeceFont12x24,

  }eFontSize_t;
  /**
   * @fn DFRobot_play
   * @brief Construct a new dfrobot play object
   */
  DFRobot_Font(uint8_t dc = 8, uint8_t cs = 9, uint8_t rst = 15, uint8_t bl = 7);

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
  
private:
  uint8_t utf8toUnicode(uint8_t *utf8,uint16_t &uni);
  void drawString(uint16_t unicode);
  unsigned char _pBits[136];

};

#endif