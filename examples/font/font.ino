
/*!
 * @file font.ino
 * @brief 烧录运行例程，将在屏幕上显示不同国家的语言文字
 * @n This demo supports DFRobot_Beetle_COR_RP2040.
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [tangjie] (jie.tang@dfrobot.com)
 * @version V1.0
 * @date 2022-08-24
 * @url https://github.com/DFRobot/DFRobot_Beetle_COR_RP2040
 */
#include "DFRobot_Font.h"
String chinese = "你好,你好";
String EnglishAndLatin = "Hello,Salve";
String Japan ="こんにちは";
String Korean ="안녕하세요";
String Greece="Γειασας";
DFRobot_Font font;

void setup()
{
    font.begin();//初始化屏幕和字库
    font.fillScreen(COLOR_RGB565_WHITE);//设置屏幕背景颜色
    font.setRotation(3);//设置屏幕方向
    font.playString(40,10,chinese,0,0xffff,font.eChineseFont24x24);
    font.playString(40,40,EnglishAndLatin,0,0xffff,font.eEnglishAndLatinFont12X24_A);
    font.playString(40,70,Japan,0,0xffff,font.eJapanFont24x24);
    font.playString(40,100,Korean,0,0xffff,font.eKoreanFont24x24);
    font.playString(180,10,Greece,0,0xffff,font.eGreeceFont12x24);
    
}

void loop()
{
  delay(100);
}