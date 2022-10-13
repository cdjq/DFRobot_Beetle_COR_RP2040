#include "DFRobot_Font.h"

DFRobot_Font::DFRobot_Font(uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl,SPIClass *pspi):DFRobot_ST7789_172x320_HW_SPI(dc,cs,rst,bl,pspi){};

uint8_t DFRobot_Font::begin(void)
{
  DFRobot_ST7789_172x320_HW_SPI::begin();
  DFRobot_ST7789_172x320_HW_SPI::fillScreen(COLOR_RGB565_WHITE);
  GT_Font_Init();
  GBK_24_GetData(0x4f,0x60,_pBits);
  return 0;
}

void DFRobot_Font::playString(uint16_t x, uint16_t y, String str, uint16_t color, uint16_t bg, eFontSize_t font)
{
  int16_t length = str.length();
  uint16_t _x = x;
  uint16_t _y = y;
  uint16_t unicode;
  uint16_t u2g;
  uint8_t data[136] = {0};
  uint8_t *dataPtr = data;
  
  for(uint8_t i = 0; i < length; i++){
    data[i] = str[i];
  }
  DBG(length);
  while(length > 0){
    uint8_t len1 = utf8toUnicode(dataPtr,unicode);
    length = length - len1;
    dataPtr+= len1;
    switch(font){
      case DFRobot_Font::eChineseFont24x24:
        if(unicode<128){
          ASCII_GetData(unicode,ASCII_24_B,_pBits);
        }else{
          u2g = U2G(unicode);
          GBK_24_GetData((u2g >> 8)&0xff, u2g & 0xff,_pBits);
        }
        drawBitmap(_x,_y,_pBits,24,24,color,bg);
        _x += 24;
      break;
      case DFRobot_Font::eChineseFont12x12:
        if(unicode<128){
          ASCII_GetData(unicode,ASCII_12_A,_pBits);
        }else{
          u2g = U2G(unicode);
          gt_12_GetData((u2g >> 8)&0xff, u2g & 0xff,_pBits);
        }
        drawBitmap(_x,_y,_pBits,12,12,color,bg);
        _x += 12;
      break;
      case DFRobot_Font::eEnglishAndLatinFont12X12_A:
        ASCII_GetData(unicode,ASCII_12_A,_pBits);
        drawBitmap(_x,_y,_pBits,12,13,color,bg);
        _x += 12;
      break;
      case DFRobot_Font::eEnglishAndLatinFont16X16_A:
        ASCII_GetData(unicode,ASCII_16_A,_pBits);
        drawBitmap(_x,_y,_pBits,16,16,color,bg);
        _x += 16;
      break;
      case DFRobot_Font::eEnglishAndLatinFont12X24_A:
        ASCII_GetData(unicode,ASCII_12X24_A,_pBits);
        drawBitmap(_x,_y,_pBits,12,24,color,bg);
        _x += 12;
      break;
      case DFRobot_Font::eEnglishAndLatinFont12X24_P:
        ASCII_GetData(unicode,ASCII_12X24_P,_pBits);
        drawBitmap(_x,_y,_pBits,12,24,color,bg);
        _x += 12;
      break;
      case DFRobot_Font::eJapanFont24x24:
        if(unicode<128){
          ASCII_GetData(unicode,ASCII_24_B,_pBits);
        }else{
          U2K_GetData_24X24(unicode,_pBits);
        }
        drawBitmap(_x,_y,_pBits,24,24,color,bg);
        _x += 24;
      break;
      case DFRobot_Font::eKoreanFont24x24:
        if(unicode<128){
          ASCII_GetData(unicode,ASCII_24_B,_pBits);
        }else{
          U2K_GetData_24X24(unicode,_pBits);
        }
        drawBitmap(_x,_y,_pBits,24,24,color,bg);
        _x += 24;
      break;
      case DFRobot_Font::eGreeceFont12x24:
        if(unicode<128){
          ASCII_GetData(unicode,ASCII_24_B,_pBits);
        }else{
          GREECE_12X24_GetData(unicode,_pBits);
        }
        drawBitmap(_x,_y,_pBits,12,24,color,bg);
        _x += 12;
      break;
      default:
        break;
    }
  }



}

uint8_t DFRobot_Font::utf8toUnicode(uint8_t *utf8,uint16_t &uni){
  uint8_t lenght = 0;
  uint8_t index = 0;

  if(utf8[index] >= 0xfc){
    uni = utf8[index]&1;
    index++;
    for(uint8_t i=1;i<=5;i++){
      uni <<= 6;
      uni |= (utf8[index]&0x3f);
      index++;
    }
    lenght = 6;
  }else if(utf8[index] >= 0xf8){
    uni = utf8[index]&3;
    index++;
    for(uint8_t i=1;i<=4;i++){
      uni <<= 6;
      uni |= (utf8[index]& 0x03f);
      index++;
    }
    lenght = 5;
  }else if(utf8[index] >= 0xf0){
    uni = utf8[index]&7;
    index++;
    for(uint8_t i=1;i<=3;i++){
      uni <<= 6;
      uni |= (utf8[index]& 0x03f);
      index++;
    }
    lenght = 4;
  }else if(utf8[index] >= 0xe0){
    uni = utf8[index]&15;
    index++;
    for(uint8_t i=1;i<=2;i++){
      uni <<= 6;
      uni |= (utf8[index]&0x03f);
      index++;
    }
    lenght = 3;
  }else if(utf8[index] >= 0xc0){
    uni = utf8[index]&0x1f;
    index++;
    for(uint8_t i=1;i<=1;i++){
      uni <<= 6;
      uni |= (utf8[index]& 0x03f);
      index++;
    }
    lenght = 2;
  }else if(utf8[index] <=0x80){
    uni = (utf8[index]&0x7f); 
    lenght = 1;
    }else{
    Serial.println("no this");
  }
  return lenght;
}