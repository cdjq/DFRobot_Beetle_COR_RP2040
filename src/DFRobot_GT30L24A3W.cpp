#include "GT30L24A3W.h"
#include "stdint.h"
#include "Arduino.h"

#include "SPI.h"
unsigned char gt_read_data(unsigned char* sendbuf , unsigned char sendlen , unsigned char* receivebuf, unsigned int receivelen)
{
  unsigned int i;
  digitalWrite(FONTCS, 0);
  for (i = 0; i < sendlen; i++)
  {
    SPI1.transfer(sendbuf[i]);
  }
  for (i = 0; i < receivelen; i++)
  {
    receivebuf[i] = SPI1.transfer(0x00);
  }
  digitalWrite(FONTCS, 1);
  return 1;
}

unsigned long r_dat_bat(unsigned long address, unsigned long DataLen, unsigned char *pBuff) {
  uint16_t i ;
  digitalWrite(FONTCS, 0);                     //使能器件
  SPI1.transfer(0x03);         //发送读取命令
  SPI1.transfer((uint8_t)((address) >> 16)); //发送24bit地址
  SPI1.transfer((uint8_t)((address) >> 8));
  SPI1.transfer((uint8_t)address);
  for (i = 0; i < DataLen; i++)
  {
    pBuff[i] = SPI1.transfer(0XFF); //循环读数

   // Serial.print(pBuff[i]);
   // Serial.print(" ");

  }
  //Serial.println();
  digitalWrite(FONTCS, 1);

  return  pBuff[0];


}
int GT_UID_MD5_FLAG = 1;
uint32_t GBCODE_5139;
int GT_Font_Init(void){
  pinMode(FONTCS, OUTPUT);
return 1;
}
static int Uncompress(unsigned char* result, unsigned char *a2)
{
  for ( uint8_t i = 0; i <= 5u; ++i )
  {
    *(uint8_t *)(result + 4 * i) = a2[3 * i];
    *(uint8_t *)(result + 4 * i + 1) = a2[3 * i + 1] & 0xF0;
    *(uint8_t *)(result + 4 * i + 2) = a2[3 * i + 2];
    *(uint8_t *)(result + 4 * i + 3) = 16 * a2[3 * i + 1];
  }
  return 1;
}

static int check_dot_null(unsigned char *DZ_Data, unsigned int num){
  unsigned int i; 

  for ( i = 0; i < num; ++i )
  {
    if ( DZ_Data[i] )
      return 1;
  }
  return 0;
}

unsigned char  ASCII_GetData(unsigned char asc,unsigned long ascii_kind,unsigned char *DZ_Data){
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( asc <= 0x1Fu || asc > 0x7Eu )
    return 0;
  switch ( ascii_kind )
  {
    case 1u:
      r_dat_bat(8 * (asc + 259932), 8u, DZ_Data);
      break;
    case 2u:
      r_dat_bat(8 * (asc + 260028), 8u, DZ_Data);
      break;
    case 3u:
      r_dat_bat(12 * asc + 2080864, 0xCu, DZ_Data);
      break;
    case 4u:
      r_dat_bat(26 * asc + 2090144, 0x1Au, DZ_Data);
      break;
    case 5u:
      r_dat_bat(16 * (asc + 130174), 0x10u, DZ_Data);
      break;
    case 6u:
      r_dat_bat(48 * asc + 1543800, 0x30u, DZ_Data);
      break;
    case 7u:
      r_dat_bat(48 * asc + 1549944, 0x30u, DZ_Data);
      break;
    case 8u:
      r_dat_bat(((asc + 67108832) << 6) + 2084832, 0x40u, DZ_Data);
      break;
    case 9u:
      r_dat_bat(34 * asc + 2092384, 0x22u, DZ_Data);
      break;
    case 0xAu:
      r_dat_bat(74 * asc + 1559864, 0x4Au, DZ_Data);
      break;
    case 0xBu:
      r_dat_bat(130 * asc + 2092576, 0x82u, DZ_Data);
      break;
    default:
      return 1;
  }
  return 1;
}

void gt_12_GetData (unsigned char MSB,unsigned char LSB,unsigned char *DZ_Data){
  unsigned char pBuff[20];
  int offset;
  unsigned int address;

  offset = 2109216;
  if ( GT_UID_MD5_FLAG )
  {
    if ( MSB != 169 || LSB <= 0xA3u )
    {
      if ( MSB <= 0xA0u || MSB > 0xA3u || LSB <= 0xA0u )
      {
        if ( MSB > 0xAFu && MSB <= 0xF7u && LSB > 0xA0u )
          address = 18 * (94 * MSB + LSB) + offset - 294246;
      }
      else
      {
        address = 18 * (94 * MSB + LSB) + offset - 275310;
      }
    }
    else
    {
      address = 18 * LSB + offset + 2124;
    }
    r_dat_bat(address, 0x12u, pBuff);
    Uncompress(DZ_Data, pBuff);
  }
}

void  gt_16_GetData (unsigned char MSB,unsigned char LSB,unsigned char *DZ_Data){
  int address;
  if ( GT_UID_MD5_FLAG )
  {
    if ( MSB != 169 || LSB <= 0xA3u )
    {
      if ( MSB <= 0xA0u || MSB > 0xA3u || LSB <= 0xA0u )
      {
        if ( MSB > 0xAFu && MSB <= 0xF7u && LSB > 0xA0u ){
          //Serial.println("not zero");
		  address = 32 * (94 * MSB + LSB + 134201381) + 2237484;
		}
		//Serial.println("zero");
        r_dat_bat(address, 0x20u, DZ_Data);
      }
      else
      {
        r_dat_bat(32 * (94 * MSB + LSB + 134202433) + 2237484, 0x20u, DZ_Data);
      }
    }
    else
    {
      r_dat_bat(32 * (LSB + 118) + 2237484, 0x20u, DZ_Data);
    }
  }
}

unsigned long GBK_24_GetData (unsigned char c1, unsigned char c2,unsigned char *DZ_Data){
  unsigned char temp; 
  int address;

  temp = c2;
  address = 0;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( c2 == 127 )
    address = 0;
  if ( c1 <= 0xA0u || c1 > 0xA3u || c2 <= 0xA0u )
  {
    if ( c1 != 166 || c2 <= 0xA0u )
    {
      if ( c1 == 169 && c2 > 0xA0u )
        address = 94 * c1 + c2 - 15671;
    }
    else
    {
      address = 94 * c1 + c2 - 15483;
    }
  }
  else
  {
    address = 94 * c1 + c2 - 15295;
  }
  if ( c1 <= 0xAFu || c1 > 0xF7u || c2 <= 0xA0u )
  {
    if ( c1 > 0xA0u || c1 <= 0x80u || c2 <= 0x3Fu )
    {
      if ( c1 > 0xA9u && c2 <= 0xA0u )
      {
        if ( (c2 & 0x80u) != 0 )
          temp = c2 - 1;
        address = 96 * c1 + temp - 3081;
      }
    }
    else
    {
      if ( (c2 & 0x80u) != 0 )
        temp = c2 - 1;
      address = 190 * c1 + temp - 17351;
    }
  }
  else
  {
    address = 94 * c1 + c2 - 16250;
  }
  r_dat_bat(72 * address, 0x48u, DZ_Data);
  return 72 * address;
}

unsigned long Shift_Jis_8X16_GetData(unsigned int code,unsigned char *DZ_Data){
  unsigned int address; 

  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( code > 0xA0 && code <= 0xDF )
    address = 16 * (code + 268435295) + 2912586;
  r_dat_bat(address, 0x10u, DZ_Data);
  return address;
}

unsigned long Unicode_Shift_Jis_GetData(unsigned int code,unsigned char *DZ_Data){
  unsigned int address;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( code > 0xFF60 && code <= 0xFF9F )
    address = 16 * (code + 268370079) + 2912586;
  r_dat_bat(address, 0x10u, DZ_Data);
  return address;
}

unsigned long JIS0208_16X16_GetData(unsigned char MSB,unsigned char LSB,unsigned char *DZ_Data){
  int address;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( MSB <= 0xFu && MSB && LSB <= 0x5Eu && LSB )
    address = 32 * (94 * (MSB - 1) + LSB + 0x7FFFFFF) + 2913594;
  if ( MSB <= 0x54u && MSB > 0xFu && LSB <= 0x5Eu && LSB )
    address = 32 * (94 * MSB + LSB + 134217585) + 2913594;
  if ( MSB == 85 && LSB <= 0x3Cu && LSB > 1u )
    address = 32 * (94 * MSB + LSB + 134210426) + 2913594;
  if ( MSB == 85 && LSB == 65 )
    address = 32 * (94 * MSB + LSB + 134210421) + 2913594;
  r_dat_bat(address, 0x20u, DZ_Data);
  return 32;
}

unsigned long JIS0208_24X24_GetData(unsigned char MSB,unsigned char LSB,unsigned char *DZ_Data){
  int address;

  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( MSB <= 0xFu && MSB && LSB <= 0x5Eu && LSB )
    address = 72 * (94 * (MSB - 1) + LSB) + 3275748;
  if ( MSB <= 0x2Eu && MSB > 0xFu && LSB <= 0x5Eu && LSB )
    address = 72 * (94 * MSB + LSB) + 3227508;
  if ( MSB == 47 && LSB <= 0x33u && LSB > 1u )
    address = 72 * (94 * MSB + LSB) + 3227508;
  if ( MSB <= 0x54u && MSB > 0x2Fu && LSB <= 0x5Eu && LSB )
    address = 72 * (94 * MSB + LSB) + 3225204;
  r_dat_bat(address, 0x48u, DZ_Data);
  return address;
}

unsigned long KSC5601_F_16_GetData(unsigned char MSB,unsigned char LSB,unsigned char *DZ_Data){
  int address;

  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( MSB <= 0xA0u || MSB > 0xAFu || LSB <= 0xA0u )
  {
    if ( MSB > 0xAFu && MSB <= 0xC8u && LSB > 0xA0u )
      address = 32 * (94 * MSB + LSB + 134202138) + 3161914;
    r_dat_bat(address, 0x20u, DZ_Data);
  }
  else
  {
    address = 32 * (94 * MSB + LSB + 134202433) + 3161914;
    r_dat_bat(address, 0x20u, DZ_Data);
  }
  return address;
}

unsigned long KSC5601_F_24_GetData(unsigned char MSB,unsigned char LSB,unsigned char *DZ_Data){
  int address;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( MSB <= 0xA0u || MSB > 0xAFu || LSB <= 0xA0u )
  {
    if ( MSB > 0xAFu && MSB <= 0xC8u && LSB > 0xA0u )
      address = 72 * (94 * MSB + LSB) + 2712130;
    r_dat_bat(address, 0x48u, DZ_Data);
  }
  else
  {
    address = 72 * (94 * MSB + LSB) + 2733370;
    r_dat_bat(address, 0x48u, DZ_Data);
  }
  return address;
}

unsigned long LATIN_8X16_GetData(unsigned int Fontcode,unsigned char *DZ_Data){
  unsigned int address;

  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Fontcode <= 0x1F || Fontcode > 0x7F )
  {
    if ( Fontcode <= 0x9F || Fontcode >= 0x180 )
    {
      if ( Fontcode < 0x1A0 || Fontcode >= 0x1D0 )
      {
        if ( Fontcode < 0x1F0 || Fontcode >= 0x200 )
        {
          if ( Fontcode < 0x210 || Fontcode >= 0x220 )
          {
            if ( Fontcode > 0x1E9F && Fontcode < 0x1F00 )
              address = 16 * (Fontcode + 268428016) + 2604968;
            r_dat_bat(address, 0x10u, DZ_Data);
          }
          else
          {
            address = 16 * (Fontcode + 268435312) + 2604968;
            r_dat_bat(address, 0x10u, DZ_Data);
          }
        }
        else
        {
          address = 16 * (Fontcode + 268435328) + 2604968;
          r_dat_bat(address, 0x10u, DZ_Data);
        }
      }
      else
      {
        address = 16 * (Fontcode + 268435360) + 2604968;
        r_dat_bat(address, 0x10u, DZ_Data);
      }
    }
    else
    {
      address = 16 * (Fontcode + 268435392) + 2604968;
      r_dat_bat(address, 0x10u, DZ_Data);
    }
  }
  else
  {
    address = 16 * (Fontcode + 268435424) + 2604968;
    r_dat_bat(address, 0x10u, DZ_Data);
  }
  return address;
}

unsigned long GREECE_8X16_GetData(unsigned int Fontcode,unsigned char *DZ_Data){
  unsigned int address;

  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Fontcode >= 0x370 && Fontcode < 0x3D0 )
    address = 16 * (Fontcode + 268434576) + 2612904;
  r_dat_bat(address, 0x10u, DZ_Data);
  return address;
}

unsigned long CYRILLIC_8X16_GetData(unsigned int Fontcode,unsigned char *DZ_Data){
  unsigned int address;

  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Fontcode < 0x400 || Fontcode >= 0x460 )
  {
    if ( Fontcode >= 0x490 && Fontcode < 0x500 )
      address = 16 * (Fontcode + 268434384) + 2614440;
    r_dat_bat(address, 0x10u, DZ_Data);
  }
  else
  {
    address = 16 * (Fontcode + 268434432) + 2614440;
    r_dat_bat(address, 0x10u, DZ_Data);
  }
  return address;
}

unsigned long HEBREW_8X16_GetData(unsigned int Fontcode,unsigned char *DZ_Data){
  unsigned int address;

  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Fontcode >= 0x590 && Fontcode < 0x600 )
    address = 16 * (Fontcode + 268434032) + 2617768;
  if ( Fontcode > 0xFB1C && Fontcode <= 0xFB4F )
    address = 16 * (Fontcode + 268371171) + 2619560;
  r_dat_bat(address, 0x10u, DZ_Data);
  return address;
}

unsigned long THAILAND_8X16_GetData(unsigned int Fontcode,unsigned char *DZ_Data){
  unsigned int address;

  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Fontcode >= 0xE00 && Fontcode < 0xE60 )
    address = 16 * (Fontcode + 268431872) + 2620376;
  r_dat_bat(address, 0x10u, DZ_Data);
  return address;
}

unsigned long LATIN_16_GetData(unsigned int Fontcode,unsigned char *DZ_Data){
  unsigned int address; 

  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Fontcode <= 0x1F || Fontcode > 0x7F )
  {
    if ( Fontcode <= 0x9F || Fontcode >= 0x180 )
    {
      if ( Fontcode < 0x1A0 || Fontcode >= 0x1D0 )
      {
        if ( Fontcode < 0x1F0 || Fontcode >= 0x200 )
        {
          if ( Fontcode < 0x210 || Fontcode >= 0x220 )
          {
            if ( Fontcode > 0x1E9F && Fontcode < 0x1F00 )
              address = 34 * Fontcode + 2369464;
            r_dat_bat(address, 0x22u, DZ_Data);
          }
          else
          {
            address = 34 * Fontcode + 2617528;
            r_dat_bat(address, 0x22u, DZ_Data);
          }
        }
        else
        {
          address = 34 * Fontcode + 2618072;
          r_dat_bat(address, 0x22u, DZ_Data);
        }
      }
      else
      {
        address = 34 * Fontcode + 2619160;
        r_dat_bat(address, 0x22u, DZ_Data);
      }
    }
    else
    {
      address = 34 * Fontcode + 2620248;
      r_dat_bat(address, 0x22u, DZ_Data);
    }
  }
  else
  {
    address = 34 * Fontcode + 2621336;
    r_dat_bat(address, 0x22u, DZ_Data);
  }
  return address;
}

unsigned long GREECE_16_GetData(unsigned int Fontcode,unsigned char *DZ_Data){
  unsigned int address;

  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Fontcode >= 0x370 && Fontcode < 0x3D0 )
    address = 34 * Fontcode + 2609368;
  r_dat_bat(address, 0x22u, DZ_Data);
  return address;
}

unsigned long CYRILLIC_16_GetData(unsigned int Fontcode,unsigned char *DZ_Data){
  unsigned int address;

  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Fontcode < 0x400 || Fontcode >= 0x460 )
  {
    if ( Fontcode >= 0x490 && Fontcode < 0x500 )
      address = 34 * Fontcode + 2606104;
    r_dat_bat(address, 0x22u, DZ_Data);
  }
  else
  {
    address = 34 * Fontcode + 2607736;
    r_dat_bat(address, 0x22u, DZ_Data);
  }
  return address;
}

unsigned long ALB_16_GetData(unsigned int unicode_alb,unsigned char *DZ_Data){
  unsigned int address; 

  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( unicode_alb < 0x600 || unicode_alb >= 0x700 )
  {
    if ( unicode_alb <= 0xFB4F || unicode_alb >= 0xFC00 )
    {
      if ( unicode_alb > 0xFE6F && unicode_alb < 0xFF00 )
        address = 34 * unicode_alb + 449688;
      r_dat_bat(address, 0x22u, DZ_Data);
    }
    else
    {
      address = 34 * unicode_alb + 470904;
      r_dat_bat(address, 0x22u, DZ_Data);
    }
  }
  else
  {
    address = 34 * unicode_alb + 2597400;
    r_dat_bat(address, 0x22u, DZ_Data);
  }
  return address;
}

unsigned long Indic_16_GetData(unsigned int Fontcode,unsigned char *DZ_Data){
  unsigned int address; 

  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Fontcode >= 0x900 && Fontcode < 0xA00 )
    address = 34 * Fontcode + 2590940;
  r_dat_bat(address, 0x22u, DZ_Data);
  return address;
}

unsigned long LATIN_12X24_GetData(unsigned int Fontcode,unsigned char *DZ_Data){
  unsigned int address; 

  address = 0;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Fontcode <= 0x1F || Fontcode > 0x7F )
  {
    if ( Fontcode <= 0x9F || Fontcode >= 0x180 )
    {
      if ( Fontcode < 0x1A0 || Fontcode >= 0x1D0 )
      {
        if ( Fontcode < 0x1F0 || Fontcode >= 0x200 )
        {
          if ( Fontcode < 0x210 || Fontcode >= 0x220 )
          {
            if ( Fontcode > 0x1E9F && Fontcode < 0x1F00 )
              address = 48 * Fontcode + 2316508;
            r_dat_bat(address, 0x30u, DZ_Data);
          }
          else
          {
            address = 48 * Fontcode + 2666716;
            r_dat_bat(address, 0x30u, DZ_Data);
          }
        }
        else
        {
          address = 48 * Fontcode + 2667484;
          r_dat_bat(address, 0x30u, DZ_Data);
        }
      }
      else
      {
        address = 48 * Fontcode + 2669020;
        r_dat_bat(address, 0x30u, DZ_Data);
      }
    }
    else
    {
      address = 48 * Fontcode + 2670556;
      r_dat_bat(address, 0x30u, DZ_Data);
    }
  }
  else
  {
    address = 48 * Fontcode + 2672092;
    r_dat_bat(address, 0x30u, DZ_Data);
  }
  return address;
}

unsigned long GREECE_12X24_GetData(unsigned int Fontcode,unsigned char *DZ_Data){
  unsigned int address;

  address = 0;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Fontcode >= 0x370 && Fontcode < 0x3D0 )
    address = 48 * Fontcode + 2655196;
  r_dat_bat(address, 0x30u, DZ_Data);
  return address;
}

unsigned long CYRILLIC_12X24_GetData(unsigned int Fontcode,unsigned char *DZ_Data){
  unsigned int address;

  address = 0;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Fontcode < 0x400 || Fontcode >= 0x460 )
  {
    if ( Fontcode >= 0x490 && Fontcode < 0x500 )
      address = 48 * Fontcode + 2650588;
    r_dat_bat(address, 0x30u, DZ_Data);
  }
  else
  {
    address = 48 * Fontcode + 2652892;
    r_dat_bat(address, 0x30u, DZ_Data);
  }
  return address;
}

unsigned long HEBREW_12X24_GetData(unsigned int Fontcode,unsigned char *DZ_Data){
  unsigned int address;

  address = 0;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Fontcode >= 0x590 && Fontcode < 0x600 )
    address = 48 * Fontcode + 2643724;
  if ( Fontcode > 0xFB1C && Fontcode <= 0xFB4F )
    address = 48 * Fontcode - 373652;
  r_dat_bat(address, 0x30u, DZ_Data);
  return address;
}

unsigned long THAILAND_16X24_GetData(unsigned int Fontcode,unsigned char *DZ_Data){
  unsigned int address; 

  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Fontcode >= 0xE00 && Fontcode < 0xE60 )
    address = 50 * Fontcode + 2602368;
  r_dat_bat(address, 0x32u, DZ_Data);
  return address;
}

unsigned long ALB_24_GetData(unsigned int UNIcode_alb,unsigned char *DZ_Data){
  unsigned int address;

  address = 0;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( UNIcode_alb < 0x600 || UNIcode_alb >= 0x700 )
  {
    if ( UNIcode_alb <= 0xFB4F || UNIcode_alb >= 0xFC00 )
    {
      if ( UNIcode_alb > 0xFE6F && UNIcode_alb < 0xFF00 )
        address = 74 * UNIcode_alb - 2001978;
      r_dat_bat(address, 0x4Au, DZ_Data);
    }
    else
    {
      address = 74 * UNIcode_alb - 1955802;
      r_dat_bat(address, 0x4Au, DZ_Data);
    }
  }
  else
  {
    address = 74 * UNIcode_alb + 2672454;
    r_dat_bat(address, 0x4Au, DZ_Data);
  }
  return address;
}

unsigned long Khmer_24_GetData(unsigned int Fontcode,unsigned char *DZ_Data){
  unsigned int address;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Fontcode >= 0x1780 && Fontcode <= 0x17DD )
    address = 74 * Fontcode + 2383558;
  if ( Fontcode > 0x17DF && Fontcode <= 0x17E9 )
    address = 74 * Fontcode + 2383410;
  if ( Fontcode > 0x17EF && Fontcode <= 0x17F9 )
    address = 74 * Fontcode + 2382966;
  if ( Fontcode > 0x19DF && Fontcode <= 0x19EF )
    address = 74 * Fontcode + 2347446;
  if ( Fontcode > 0x19EF && Fontcode < 0x1A00 )
    address = 74 * Fontcode + 2347742;
  r_dat_bat(address, 0x4Au, DZ_Data);
  return address;
}

unsigned long ICCS_Addr(unsigned char Icode,unsigned long BaseAddr){
return 1;
}

unsigned long Katakana_GetData(unsigned char code,unsigned char *DZ_Data){
  int address; 
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( (code & 0x80u) == 0 || code > 0x9Fu )
  {
    if ( code <= 0xA0u || code > 0xDFu )
    {
      if ( code > 0xDFu )
      {
        address = 72 * code + 2865462;
        r_dat_bat(address, 0x48u, DZ_Data);
      }
    }
    else
    {
      address = 48 * code + 2870790;
      r_dat_bat(address, 0x30u, DZ_Data);
    }
  }
  else
  {
    address = 72 * code + 2866998;
    r_dat_bat(address, 0x48u, DZ_Data);
  }
  return address;
}

unsigned long CP_Patch_8X16_GetData(unsigned int code,unsigned char *DZ_Data){
  char dest[0x4C] = {"="}; 
  unsigned int address;
  int v8; 
  unsigned char v9; 

  v8 = 2851686;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  for ( uint8_t i = 0; i <= 0x12u; ++i )
  {
    if ( code == *(uint32_t *)dest[4*i] )
    {
      v9 = i;
      break;
    }
  }
  address = v8 + 16 * v9;
  r_dat_bat(v8 + 16 * v9, 0x10u, DZ_Data);
  return address;
}

unsigned long CP_Patch_16X16_GetData(unsigned int code,unsigned char *DZ_Data){
  signed int address; 

  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( code == 8359 )
    address = 2851990;
  if ( code == 8362 )
    address = 2852022;
  r_dat_bat(address, 0x20u, DZ_Data);
  return address;
}

unsigned long CP_P_KATAKANA_16X16_GetData(unsigned int code,unsigned char *DZ_Data){
  unsigned int address; 

  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( code <= 0xDF || code > 0xE3 )
  {
    if ( code == 254 )
      address = 2852182;
    r_dat_bat(address, 0x20u, DZ_Data);
  }
  else
  {
    address = 32 * (code + 134217504) + 2852054;
    r_dat_bat(address, 0x20u, DZ_Data);
  }
  return address;
}
unsigned long cp_patch_12x24(unsigned int code,unsigned char *DZ_Data){
  unsigned int address; 

  if ( GT_UID_MD5_FLAG )
  {
    if ( code == 402 )
    {
      address = 2852214;
      r_dat_bat(0x2B8576u, 0x30u, DZ_Data);
    }
    else if ( code <= 0x392 || code > 0x3C6 )
    {
      if ( code <= 0x2012 || code > 0x20AC )
      {
        if ( code <= 0x2218 || code > 0x2265 )
        {
          if ( code <= 0x230F || code > 0x2321 )
          {
            if ( code < 0x2500 || code > 0x25A0 )
            {
              if ( code <= 0x2C5 || code > 0x2DD )
              {
                if ( code <= 0x2115 || code > 0x2122 )
                  return 0;
                address = 48 * code + 2469030;
                r_dat_bat(address, 0x30u, DZ_Data);
              }
              else
              {
                address = 48 * code + 2840358;
                r_dat_bat(address, 0x30u, DZ_Data);
              }
            }
            else
            {
              address = 48 * code + 2412054;
              r_dat_bat(address, 0x30u, DZ_Data);
            }
          }
          else
          {
            address = 48 * code + 2434998;
            r_dat_bat(address, 0x30u, DZ_Data);
          }
        }
        else
        {
          address = 48 * code + 2443158;
          r_dat_bat(address, 0x30u, DZ_Data);
        }
      }
      else
      {
        address = 48 * code + 2460630;
        r_dat_bat(address, 0x30u, DZ_Data);
      }
    }
    else
    {
      address = 48 * code + 2808342;
      r_dat_bat(address, 0x30u, DZ_Data);
    }
    return address;
  }
  return 0;
}

unsigned long ISO8859_8X16_GetData(unsigned int Fontcode,unsigned char Num,unsigned char *DZ_Data){
  unsigned int address;

  address = 0;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Fontcode > 0x7F && Fontcode <= 0xFF )
  {
    switch ( Num )
    {
      case 1u:
        address = 16 * (Fontcode + 268435328) + 2883894;
        r_dat_bat(address, 0x10u, DZ_Data);
        break;
      case 2u:
        address = 16 * (Fontcode + 268435328) + 2885942;
        r_dat_bat(address, 0x10u, DZ_Data);
        break;
      case 3u:
        address = 16 * (Fontcode + 268435328) + 2887990;
        r_dat_bat(address, 0x10u, DZ_Data);
        break;
      case 4u:
        address = 16 * (Fontcode + 268435328) + 2890038;
        r_dat_bat(address, 0x10u, DZ_Data);
        break;
      case 5u:
        address = 16 * (Fontcode + 268435328) + 2892086;
        r_dat_bat(address, 0x10u, DZ_Data);
        break;
      case 7u:
        address = 16 * (Fontcode + 268435328) + 2894134;
        r_dat_bat(address, 0x10u, DZ_Data);
        break;
      case 8u:
        address = 16 * (Fontcode + 268435328) + 2896182;
        r_dat_bat(address, 0x10u, DZ_Data);
        break;
      case 9u:
        address = 16 * (Fontcode + 268435328) + 2898230;
        r_dat_bat(address, 0x10u, DZ_Data);
        break;
      case 0xAu:
        address = 16 * (Fontcode + 268435328) + 2900278;
        r_dat_bat(address, 0x10u, DZ_Data);
        break;
      case 0xBu:
        address = 16 * (Fontcode + 268435328) + 2902326;
        r_dat_bat(address, 0x10u, DZ_Data);
        break;
      case 0xDu:
        address = 16 * (Fontcode + 268435328) + 2904374;
        r_dat_bat(address, 0x10u, DZ_Data);
        break;
      case 0xEu:
        address = 16 * (Fontcode + 268435328) + 2906422;
        r_dat_bat(address, 0x10u, DZ_Data);
        break;
      case 0xFu:
        address = 16 * (Fontcode + 268435328) + 2908470;
        r_dat_bat(address, 0x10u, DZ_Data);
        break;
      case 0x10u:
        address = 16 * (Fontcode + 268435328) + 2910518;
      default:
        r_dat_bat(address, 0x10u, DZ_Data);
        break;
    }
  }
  return address;
}

unsigned long U2J_GetData(unsigned int Unicode,unsigned char *DZ_Data){
  unsigned char pBuff[2]; 
  int v6; 
  unsigned int v7; 
  v7 = 0;
  v6 = 4084090;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Unicode > 0x7FE || Unicode <= 0x1F )
  {
    if ( Unicode > 0x27FE || Unicode < 0x2000 )
    {
      if ( Unicode > 0x30FE || Unicode < 0x3000 )
      {
        if ( Unicode > 0x33FE || Unicode < 0x3200 )
        {
          if ( Unicode > 0x9FFE || Unicode < 0x4E00 )
          {
            if ( Unicode > 0xFFFE || Unicode < 0xFE00 )
            {
              if ( Unicode <= 0x2B07 && Unicode > 0x2B04 )
              {
                pBuff[0] = 12;
                pBuff[1] = Unicode + 49;
              }
            }
            else
            {
              v7 = Unicode - 39200;
            }
          }
          else
          {
            v7 = Unicode - 15136;
          }
        }
        else
        {
          v7 = Unicode - 8480;
        }
      }
      else
      {
        v7 = Unicode - 8224;
      }
    }
    else
    {
      v7 = Unicode - 6176;
    }
  }
  else
  {
    v7 = Unicode - 32;
  }
  if ( Unicode <= 0x2B04 || Unicode > 0x2B07 )
    r_dat_bat(2 * v7 + v6, 2u, pBuff);
  JIS0208_16X16_GetData(pBuff[0], pBuff[1], DZ_Data);
  return (pBuff[0] << 8) + pBuff[1];
}

unsigned long Shift_JIS_TO_JIS0208(unsigned int code16){
  unsigned char pBuff[2];
  int v4; 

  v4 = 4136762;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( code16 <= 0x813F || code16 > 0x9FFC )
  {
    if ( code16 <= 0xE03F || code16 > 0xEAA4 )
    {
      if ( code16 <= 0x873F || code16 > 0x879C )
      {
        if ( code16 <= 0x879C || code16 > 0x9FFC )
        {
          if ( code16 <= 0xEEEE || code16 > 0xEEF8 )
          {
            if ( code16 <= 0xFA3F || code16 > 0xFA49 )
            {
              if ( code16 <= 0xFA49 || code16 > 0xFA53 )
              {
                switch ( code16 )
                {
                  case 0xFA58u:
                    pBuff[0] = 13;
                    pBuff[1] = 14;
                    break;
                  case 0xFA59u:
                    pBuff[0] = 11;
                    pBuff[1] = 91;
                    break;
                  case 0xFA5Au:
                    pBuff[0] = 11;
                    pBuff[1] = 93;
                    break;
                  case 0xFA5Bu:
                    pBuff[0] = 2;
                    pBuff[1] = 72;
                    break;
                }
              }
              else
              {
                pBuff[0] = 10;
                pBuff[1] = code16 - 73;
              }
            }
            else
            {
              pBuff[0] = 10;
              pBuff[1] = code16 - 43;
            }
          }
          else
          {
            pBuff[0] = 10;
            pBuff[1] = code16 + 38;
          }
        }
        else
        {
          r_dat_bat(2 * (code16 + 2147448932) + v4, 2u, pBuff);
          pBuff[0] -= 32;
          pBuff[1] -= 32;
        }
      }
      else
      {
        r_dat_bat(2 * (code16 + 2147449024) + v4, 2u, pBuff);
        pBuff[0] -= 32;
        pBuff[1] -= 32;
      }
    }
    else
    {
      r_dat_bat(2 * (code16 + 2147434176) + v4, 2u, pBuff);
      pBuff[0] -= 32;
      pBuff[1] -= 32;
    }
  }
  else
  {
    r_dat_bat(2 * (code16 + 2147450624) + v4, 2u, pBuff);
    pBuff[0] -= 32;
    pBuff[1] -= 32;
  }
  return (pBuff[0] << 8) + pBuff[1];
}
unsigned long U2K_GetData_16X16(unsigned int Unicode,unsigned char *DZ_Data){
  unsigned char pBuff[2]; 
  int v6; 
  unsigned int v7; 

  v6 = 4158142;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Unicode > 0x451 || Unicode <= 0xA0 )
  {
    if ( Unicode > 0x266D || Unicode <= 0x2014 )
    {
      if ( Unicode > 0x33DD || Unicode < 0x3000 )
      {
        if ( Unicode > 0xD79D || Unicode < 0xAC00 )
        {
          if ( Unicode < 0x10000 && Unicode >= 0xF900 )
            v7 = Unicode - 49018;
        }
        else
        {
          v7 = Unicode - 40472;
        }
      }
      else
      {
        v7 = Unicode - 9718;
      }
    }
    else
    {
      v7 = Unicode - 7268;
    }
  }
  else
  {
    v7 = Unicode - 161;
  }
  r_dat_bat(2 * v7 + v6, 2u, pBuff);
  KSC5601_F_16_GetData(pBuff[0], pBuff[1], DZ_Data);
  return (pBuff[0] << 8) + pBuff[1];
}

unsigned long U2K_GetData_24X24(unsigned int Unicode,unsigned char *DZ_Data){
  unsigned char pBuff[2]; 
  int address; 
  unsigned int offset;
  address = 4158142;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( Unicode > 0x451 || Unicode <= 0xA0 )
  {
    if ( Unicode > 0x266D || Unicode <= 0x2014 )
    {
      if ( Unicode > 0x33DD || Unicode < 0x3000 )
      {
        if ( Unicode > 0xD79D || Unicode < 0xAC00 )
        {
          if ( Unicode < 0x10000 && Unicode >= 0xF900 )
            offset = Unicode - 49018;
        }
        else
        {
          offset = Unicode - 40472;
        }
      }
      else
      {
        offset = Unicode - 9718;
      }
    }
    else
    {
      offset = Unicode - 7268;
    }
  }
  else
  {
    offset = Unicode - 161;
  }
  r_dat_bat(2 * offset + address, 2u, pBuff);
  KSC5601_F_24_GetData(pBuff[0], pBuff[1], DZ_Data);
  return (pBuff[0] << 8) + pBuff[1];
}

unsigned long U2G_GetData_12X12(unsigned int  unicode,unsigned char *DZ_Data){
  unsigned char pBuff[2];
  int v6; 
  int v7; 
  unsigned int address; 
  v7 = 2517590;
  address = 0;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( unicode > 0x451 || unicode <= 0x9F )
  {
    if ( unicode > 0x2642 || unicode <= 0x200F )
    {
      if ( unicode > 0x33D5 || unicode < 0x3000 )
      {
        if ( unicode > 0x9FA5 || unicode < 0x4E00 )
        {
          if ( unicode > 0xFE6B || unicode <= 0xFE2F )
          {
            if ( unicode > 0xFF5E || unicode <= 0xFF00 )
            {
              if ( unicode > 0xFFE5 || unicode <= 0xFFDF )
              {
                if ( unicode > 0xFA29 || unicode <= 0xF92B )
                {
                  if ( unicode > 0xE864 || unicode <= 0xE815 )
                  {
                    if ( unicode > 0x2ECA || unicode <= 0x2E80 )
                    {
                      if ( unicode > 0x49B7 || unicode <= 0x4946 )
                      {
                        if ( unicode > 0x4DAE || unicode <= 0x4C76 )
                        {
                          if ( unicode > 0x3CE0 || unicode <= 0x3446 )
                          {
                            if ( unicode <= 0x478D && unicode > 0x4054 )
                            {
                              v6 = 55380;
                              address = 2 * (unicode + 2147467178) + 55380;
                            }
                          }
                          else
                          {
                            v6 = 50976;
                            address = 2 * (unicode + 2147470265) + 50976;
                          }
                        }
                        else
                        {
                          v6 = 50352;
                          address = 2 * (unicode + 2147464073) + 50352;
                        }
                      }
                      else
                      {
                        v6 = 50126;
                        address = 2 * (unicode + 2147464889) + 50126;
                      }
                    }
                    else
                    {
                      v6 = 49978;
                      address = 2 * (unicode + 2147471743) + 49978;
                    }
                  }
                  else
                  {
                    v6 = 49820;
                    address = 2 * (unicode + 2147424234) + 49820;
                  }
                }
                else
                {
                  v6 = 49312;
                  address = 2 * (unicode + 2147419860) + 49312;
                }
              }
              else
              {
                v6 = 49142;
                address = 2 * (unicode + 2147418144) + 49142;
              }
            }
            else
            {
              v6 = 48954;
              address = 2 * (unicode + 2147418367) + 48954;
            }
          }
          else
          {
            v6 = 48834;
            address = 2 * (unicode + 2147418576) + 48834;
          }
        }
        else
        {
          v6 = 7030;
          address = 2 * (unicode + 2147463680) + 7030;
        }
      }
      else
      {
        v6 = 5066;
        address = 2 * (unicode + 2147471360) + 5066;
      }
    }
    else
    {
      v6 = 1892;
      address = 2 * (unicode + 2147475440) + 1892;
    }
  }
  else
  {
    v6 = 0;
    address = 2 * (unicode + 2147483488);
  }
  address += v7;
  r_dat_bat(address, 2u, pBuff);
  gt_12_GetData(pBuff[0], pBuff[1], DZ_Data);
  return (pBuff[0] << 8) + pBuff[1];
}
unsigned long U2G_GetData_16X16(unsigned int  unicode,unsigned char *DZ_Data){
  unsigned char pBuff[2]; // [sp+8h] [bp-14h]
  int v6; // [sp+Ch] [bp-10h]
  int v7; // [sp+10h] [bp-Ch]
  unsigned int address; // [sp+14h] [bp-8h]

  v7 = 2517590;
  address = 0;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( unicode > 0x451 || unicode <= 0x9F )
  {
    if ( unicode > 0x2642 || unicode <= 0x200F )
    {
      if ( unicode > 0x33D5 || unicode < 0x3000 )
      {
        if ( unicode > 0x9FA5 || unicode < 0x4E00 )
        {
          if ( unicode > 0xFE6B || unicode <= 0xFE2F )
          {
            if ( unicode > 0xFF5E || unicode <= 0xFF00 )
            {
              if ( unicode > 0xFFE5 || unicode <= 0xFFDF )
              {
                if ( unicode > 0xFA29 || unicode <= 0xF92B )
                {
                  if ( unicode > 0xE864 || unicode <= 0xE815 )
                  {
                    if ( unicode > 0x2ECA || unicode <= 0x2E80 )
                    {
                      if ( unicode > 0x49B7 || unicode <= 0x4946 )
                      {
                        if ( unicode > 0x4DAE || unicode <= 0x4C76 )
                        {
                          if ( unicode > 0x3CE0 || unicode <= 0x3446 )
                          {
                            if ( unicode <= 0x478D && unicode > 0x4054 )
                            {
                              v6 = 55380;
                              address = 2 * (unicode + 2147467178) + 55380;
                            }
                          }
                          else
                          {
                            v6 = 50976;
                            address = 2 * (unicode + 2147470265) + 50976;
                          }
                        }
                        else
                        {
                          v6 = 50352;
                          address = 2 * (unicode + 2147464073) + 50352;
                        }
                      }
                      else
                      {
                        v6 = 50126;
                        address = 2 * (unicode + 2147464889) + 50126;
                      }
                    }
                    else
                    {
                      v6 = 49978;
                      address = 2 * (unicode + 2147471743) + 49978;
                    }
                  }
                  else
                  {
                    v6 = 49820;
                    address = 2 * (unicode + 2147424234) + 49820;
                  }
                }
                else
                {
                  v6 = 49312;
                  address = 2 * (unicode + 2147419860) + 49312;
                }
              }
              else
              {
                v6 = 49142;
                address = 2 * (unicode + 2147418144) + 49142;
              }
            }
            else
            {
              v6 = 48954;
              address = 2 * (unicode + 2147418367) + 48954;
            }
          }
          else
          {
            v6 = 48834;
            address = 2 * (unicode + 2147418576) + 48834;
          }
        }
        else
        {
          v6 = 7030;
          address = 2 * (unicode + 2147463680) + 7030;
        }
      }
      else
      {
        v6 = 5066;
        address = 2 * (unicode + 2147471360) + 5066;
      }
    }
    else
    {
      v6 = 1892;
      address = 2 * (unicode + 2147475440) + 1892;
    }
  }
  else
  {
    v6 = 0;
    address = 2 * (unicode + 2147483488);
  }
  address += v7;
  r_dat_bat(address, 2u, pBuff);
  gt_16_GetData(pBuff[0], pBuff[1], DZ_Data);
  return (pBuff[0] << 8) + pBuff[1];
}
unsigned long U2G_GetData_24X24(unsigned int  unicode,unsigned char *DZ_Data){
  unsigned char pBuff[2]; 
  int v6; 
  int v7; 
  unsigned int address;

  v7 = 2517590;
  address = 0;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( unicode > 0x451 || unicode <= 0x9F )
  {
    if ( unicode > 0x2642 || unicode <= 0x200F )
    {
      if ( unicode > 0x33D5 || unicode < 0x3000 )
      {
        if ( unicode > 0x9FA5 || unicode < 0x4E00 )
        {
          if ( unicode > 0xFE6B || unicode <= 0xFE2F )
          {
            if ( unicode > 0xFF5E || unicode <= 0xFF00 )
            {
              if ( unicode > 0xFFE5 || unicode <= 0xFFDF )
              {
                if ( unicode > 0xFA29 || unicode <= 0xF92B )
                {
                  if ( unicode > 0xE864 || unicode <= 0xE815 )
                  {
                    if ( unicode > 0x2ECA || unicode <= 0x2E80 )
                    {
                      if ( unicode > 0x49B7 || unicode <= 0x4946 )
                      {
                        if ( unicode > 0x4DAE || unicode <= 0x4C76 )
                        {
                          if ( unicode > 0x3CE0 || unicode <= 0x3446 )
                          {
                            if ( unicode <= 0x478D && unicode > 0x4054 )
                            {
                              v6 = 55380;
                              address = 2 * (unicode + 2147467178) + 55380;
                            }
                          }
                          else
                          {
                            v6 = 50976;
                            address = 2 * (unicode + 2147470265) + 50976;
                          }
                        }
                        else
                        {
                          v6 = 50352;
                          address = 2 * (unicode + 2147464073) + 50352;
                        }
                      }
                      else
                      {
                        v6 = 50126;
                        address = 2 * (unicode + 2147464889) + 50126;
                      }
                    }
                    else
                    {
                      v6 = 49978;
                      address = 2 * (unicode + 2147471743) + 49978;
                    }
                  }
                  else
                  {
                    v6 = 49820;
                    address = 2 * (unicode + 2147424234) + 49820;
                  }
                }
                else
                {
                  v6 = 49312;
                  address = 2 * (unicode + 2147419860) + 49312;
                }
              }
              else
              {
                v6 = 49142;
                address = 2 * (unicode + 2147418144) + 49142;
              }
            }
            else
            {
              v6 = 48954;
              address = 2 * (unicode + 2147418367) + 48954;
            }
          }
          else
          {
            v6 = 48834;
            address = 2 * (unicode + 2147418576) + 48834;
          }
        }
        else
        {
          v6 = 7030;
          address = 2 * (unicode + 2147463680) + 7030;
        }
      }
      else
      {
        v6 = 5066;
        address = 2 * (unicode + 2147471360) + 5066;
      }
    }
    else
    {
      v6 = 1892;
      address = 2 * (unicode + 2147475440) + 1892;
    }
  }
  else
  {
    v6 = 0;
    address = 2 * (unicode + 2147483488);
  }
  address += v7;
  r_dat_bat(address, 2u, pBuff);
  GBK_24_GetData(pBuff[0], pBuff[1], DZ_Data);
  return (pBuff[0] << 8) + pBuff[1];
}
unsigned long BIG5_GBK( unsigned char h,unsigned char l){
  unsigned char pBuff[2];
  int GBK_Code;
  int offset;
  unsigned int address; 
  offset = 2576666;
  GBK_Code = 0;
  pBuff[1] = h;
  pBuff[0] = l;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( pBuff[1] <= 0xA0u || pBuff[0] <= 0x3Fu || pBuff[0] > 0x7Eu )
  {
    if ( pBuff[1] > 0xA0u && pBuff[0] > 0xA0u && pBuff[0] != 255 )
      address = 157 * (pBuff[1] - 161) + pBuff[0] - 161 + 63;
  }
  else
  {
    address = 157 * (pBuff[1] - 161) + pBuff[0] - 64;
  }
  address = offset + 2 * address;
  r_dat_bat(address, 1u, &pBuff[1]);
  r_dat_bat(address + 1, 1u, pBuff);
  GBK_Code = (pBuff[1] << 8) + pBuff[0];
  return GBK_Code;
}

unsigned long  U2G(unsigned int  unicode){
  unsigned char pBuff[2];
  int v4;
  int offset;
  unsigned int address;

  offset = 2517590;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( unicode > 0x451 || unicode <= 0x9F )
  {
    if ( unicode > 0x2642 || unicode <= 0x200F )
    {
      if ( unicode > 0x33D5 || unicode < 0x3000 )
      {
        if ( unicode > 0x9FA5 || unicode < 0x4E00 )
        {
          if ( unicode > 0xFE6B || unicode <= 0xFE2F )
          {
            if ( unicode > 0xFF5E || unicode <= 0xFF00 )
            {
              if ( unicode > 0xFFE5 || unicode <= 0xFFDF )
              {
                if ( unicode > 0xFA29 || unicode <= 0xF92B )
                {
                  if ( unicode > 0xE864 || unicode <= 0xE815 )
                  {
                    if ( unicode > 0x2ECA || unicode <= 0x2E80 )
                    {
                      if ( unicode > 0x49B7 || unicode <= 0x4946 )
                      {
                        if ( unicode > 0x4DAE || unicode <= 0x4C76 )
                        {
                          if ( unicode > 0x3CE0 || unicode <= 0x3446 )
                          {
                            if ( unicode <= 0x478D && unicode > 0x4054 )
                            {
                              v4 = 55380;
                              address = 2 * (unicode + 2147467178) + 55380;
                            }
                          }
                          else
                          {
                            v4 = 50976;
                            address = 2 * (unicode + 2147470265) + 50976;
                          }
                        }
                        else
                        {
                          v4 = 50352;
                          address = 2 * (unicode + 2147464073) + 50352;
                        }
                      }
                      else
                      {
                        v4 = 50126;
                        address = 2 * (unicode + 2147464889) + 50126;
                      }
                    }
                    else
                    {
                      v4 = 49978;
                      address = 2 * (unicode + 2147471743) + 49978;
                    }
                  }
                  else
                  {
                    v4 = 49820;
                    address = 2 * (unicode + 2147424234) + 49820;
                  }
                }
                else
                {
                  v4 = 49312;
                  address = 2 * (unicode + 2147419860) + 49312;
                }
              }
              else
              {
                v4 = 49142;
                address = 2 * (unicode + 2147418144) + 49142;
              }
            }
            else
            {
              v4 = 48954;
              address = 2 * (unicode + 2147418367) + 48954;
            }
          }
          else
          {
            v4 = 48834;
            address = 2 * (unicode + 2147418576) + 48834;
          }
        }
        else
        {
          v4 = 7030;
          address = 2 * (unicode + 2147463680) + 7030;
        }
      }
      else
      {
        v4 = 5066;
        address = 2 * (unicode + 2147471360) + 5066;
      }
    }
    else
    {
      v4 = 1892;
      address = 2 * (unicode + 2147475440) + 1892;
    }
  }
  else
  {
    v4 = 0;
    address = 2 * (unicode + 2147483488);
  }
  address += offset;
  r_dat_bat(address, 2u, pBuff);
  return (pBuff[0] << 8) + pBuff[1];
}

int CodePage_UC(unsigned char code, unsigned char cpsel){
  unsigned char pBuff[2]; 
  unsigned int address; 

  switch ( cpsel )
  {
    case 0u:
      address = 2 * (code + 1420143);
      break;
    case 1u:
      address = 2 * (code + 1420399);
      break;
    case 2u:
      address = 2 * (code + 1420655);
      break;
    case 3u:
      address = 2 * (code + 1420911);
      break;
    case 4u:
      address = 2 * (code + 1421167);
      break;
    case 5u:
      address = 2 * (code + 1421423);
      break;
    case 6u:
      address = 2 * (code + 1421679);
      break;
    case 7u:
      address = 2 * (code + 1421807);
      break;
    case 8u:
      address = 2 * (code + 1422063);
      break;
    case 9u:
      address = 2 * (code + 1422319);
      break;
    case 0xAu:
      address = 2 * (code + 1422575);
      break;
    case 0xBu:
      address = 2 * (code + 1422831);
      break;
    case 0xCu:
      address = 2 * (code + 1423087);
      break;
    case 0xDu:
      address = 2 * (code + 1423343);
      break;
    case 0xEu:
      address = 2 * (code + 1423599);
      break;
    case 0xFu:
      address = 2 * (code + 1423855);
      break;
    case 0x10u:
      address = 2 * (code + 1424111);
      break;
    case 0x11u:
      address = 2 * (code + 1424367);
      break;
    case 0x12u:
      address = 2 * (code + 1424623);
      break;
    case 0x13u:
      address = 2 * (code + 1424879);
      break;
    case 0x14u:
      address = 2 * (code + 1425135);
      break;
    case 0x15u:
      address = 2 * (code + 1425231);
      break;
    case 0x16u:
      address = 2 * (code + 1425455);
      break;
    case 0x17u:
      address = 2 * (code + 1425455);
      break;
    default:
      break;
  }
  r_dat_bat(address, 2u, pBuff);
  return (pBuff[0] << 8) + pBuff[1];
}

void CP_12X24_GetData(unsigned int code,unsigned char cpsel,unsigned char *DZ_data){
  int Fontcode;
  if ( GT_UID_MD5_FLAG )
  {
    Fontcode = CodePage_UC((unsigned char)code, cpsel);
    if ( Fontcode )
    {
      if ( !LATIN_12X24_GetData(Fontcode, DZ_data) || !check_dot_null(DZ_data, 0x30u) )
      {
        if ( !GREECE_12X24_GetData(Fontcode, DZ_data) || !check_dot_null(DZ_data, 0x30u) )
        {
          if ( !CYRILLIC_12X24_GetData(Fontcode, DZ_data) || !check_dot_null(DZ_data, 0x30u) )
          {
            if ( !HEBREW_12X24_GetData(Fontcode, DZ_data) || !check_dot_null(DZ_data, 0x30u) )
            {
              if ( !ALB_24_GetData(Fontcode, DZ_data) || !check_dot_null(DZ_data, 0x30u))
              {
                cp_patch_12x24(Fontcode, DZ_data);
                check_dot_null(DZ_data, 0x30u);
              }
            }
          }
        }
      }
    }
  }
}
void INTERNATIONAL_CHARACTER_12X24(unsigned char Icode,unsigned long INTERNATIONAL_CHARACTER,unsigned char  *DZ_Data){
  char *v4 = "#$@[\\]^`{|}~"; 
  unsigned char pBuff[2]; 
  unsigned int address; 
  if ( GT_UID_MD5_FLAG )
  {
    for ( uint8_t i = 0; i <= 0xBu; ++i )
    {
      if ( Icode == v4[i] )
        address = INTERNATIONAL_CHARACTER + 2 * i;
    }
    r_dat_bat(address, 2u, pBuff);
    GBCODE_5139 = (pBuff[0] << 8) + pBuff[1];
    if ( (pBuff[0] << 8) + pBuff[1] )
    {
      if ( !LATIN_12X24_GetData(GBCODE_5139, DZ_Data) || !check_dot_null(DZ_Data, 48) )
      {
        if ( !GREECE_12X24_GetData(GBCODE_5139, DZ_Data) || !check_dot_null(DZ_Data, 48) )
        {
          if ( !CYRILLIC_12X24_GetData(GBCODE_5139, DZ_Data) || !check_dot_null(DZ_Data, 48) )
          {
            if ( !HEBREW_12X24_GetData(GBCODE_5139, DZ_Data) || !check_dot_null(DZ_Data, 48) )
            {
              if ( !ALB_24_GetData(GBCODE_5139, DZ_Data) || !check_dot_null(DZ_Data, 48) )
              {
                cp_patch_12x24(GBCODE_5139, DZ_Data);
                check_dot_null(DZ_Data, 48);
              }
            }
          }
        }
      }
    }
  }
}


unsigned long GB18030_24_GetData (unsigned char c1, unsigned char c2, unsigned char c3, unsigned char c4,unsigned char *DZ_Data){
  unsigned char v7;
  int v8; 

  v7 = c2;
  v8 = 0;
  if ( !GT_UID_MD5_FLAG )
    return 0;
  if ( c2 == 127 )
    v8 = 0;
  if ( c1 <= 0xA3u || c1 > 0xA5u || c2 <= 0xA0u )
  {
    if ( c1 <= 0xA6u || c1 > 0xA8u || c2 <= 0xA0u )
    {
      if ( c1 <= 0xA7u || c1 > 0xA9u || c2 > 0xA0u )
      {
        if ( c1 != 129 || c2 <= 0x38u )
        {
          if ( c1 == 130 )
            v8 = 1260 * (c2 - 48) + 719 + 10 * (c3 - 129) + c4 - 48;
        }
        else
        {
          v8 = 10 * (c3 - 238) + c4 + 501;
        }
      }
      else
      {
        if ( (c2 & 0x80u) != 0 )
          v7 = c2 - 1;
        v8 = 96 * (c1 - 168) + v7 - 64 + 376;
      }
    }
    else
    {
      v8 = 94 * (c1 - 167) + c2 - 161 + 188;
    }
  }
  else
  {
    v8 = 94 * (c1 - 164) + c2 - 161;
  }
  r_dat_bat(72 * v8 + 1569356, 0x48u, DZ_Data);
  return 72 * v8 + 1569356;
}
