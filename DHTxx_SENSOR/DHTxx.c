#include "DHTxx.h"

//   Gloabal variable
short sel_dht = 0;

//  ----------------------------------------------------------------------------
//  Prototype:     sensor_Response()
//  Input:         NOTHING
//  Description:   HAM THUC HIEN GUI TIN HIEU BAT DAU TOI DHT22
//                 SAU DO DHT22 SE GUI  TIN HIEU PHAN HOI VE uC
//  Return:        NOTHING
//  Requires:      BO QUA HAM NAY KHI DUNG THU VIEN
//  ----------------------------------------------------------------------------
void sensor_Response()
{
    // SEND START SIGNAL TO SENSOR DHT22
    DHT_PIN_Direction = 0;        // CONFIG PIN IS OUTPUT
    DHT_PIN = 0;                  // OUTPUT LOW
    delay_ms(18);
    DHT_PIN = 1;                  // OUTPUT HIGH
    delay_us(30);
    DHT_PIN_Direction = 1;        // CONFIG PIN IS INPUT

    // WAIT RESPONE FROM SENSOR DHT TO PIC
    while(DHT_PIN);               // 20us bit 1
    while(!DHT_PIN);              // 80us bit 0
    while(DHT_PIN);               // 80us bit 1
}

//  ----------------------------------------------------------------------------
//  Prototype:     unsigned short DHT_Read_Byte()
//  Input:         NOTHING
//  Description:   HAM THUC HIEN DOC MOT BYTE DU LIEU CUA DHT GUI VE uC
//                 VOI BIT HIGH = 50us LEVEL LOGIC 0 + 70us LEVEL LOGIC 1
//                     BIT LOW = 50us LEVEL LOGIC 0 + 26-28us LEVEL LOGIC 1
//  Return:        1 BYTE DU LIEU CUA DHT 22
//  Requires:      Bo qua ham nay khi dung thu vien
//  ----------------------------------------------------------------------------
unsigned short dht_Read_Byte()
{
    unsigned short i, DHT_BYTE = 0;
    for(i = 0; i < 8; i++)
    {
         while(!DHT_PIN);              // DHT PIN BECOME HIGH
         delay_us(40);
         if(DHT_PIN)
         {
              DHT_BYTE |= (1 << (7 - i));
              while(DHT_PIN);
         }
    }
    return DHT_BYTE;
}

//  ----------------------------------------------------------------------------
//  Prototype:     int DHT_Read_Data(int *Humidity, int *Temperature)
//  Input:         Humidity: Do am can truyen vao range (0 - 100%)
//
//                 Temperature: Nhiet do can truyen vao range -40~80celsius
//                 Nhiet do theo datasheet cua DHT co the am (NEGATIVE)
//
//  Description:   Ham thuc hien doc 5 byte du lieu cua DHT gui ve uC
//                 Voi byte cuoi cung la checksum nham kiem tra loi
//                 Khi doc du lieu tu DHT
//
//  Return:        Return 1 --> Data humi and temp correct
//                 Return 0 --> Data error
//  Requires:      De chac chan du lieu nhan duoc la chinh xac hay kiem tra loi
//  ----------------------------------------------------------------------------
int dht_Read_Data(int *Humidity, int *Temperature)
{
    unsigned short DHT_DATA[4];
    unsigned short checkSum;
    sensor_Response();
    DHT_DATA[0] = DHT_Read_Byte();
    DHT_DATA[1] = DHT_Read_Byte();
    DHT_DATA[2] = DHT_Read_Byte();
    DHT_DATA[3] = DHT_Read_Byte();
    checkSum = DHT_Read_Byte();
    if(sel_dht == 1)
    {
         *Humidity = DHT_DATA[0];
         *Temperature = DHT_DATA[2];
    }
    else if(sel_dht == 2)
    {
         *Humidity = (DHT_DATA[0] << 8) | DHT_DATA[1];
         *Temperature = (DHT_DATA[2] << 8) | DHT_DATA[3];
    }
    if((*Temperature & 0x8000) == 0x8000)       // IF TEMPERATURE IS NEGATIVE
    {
        *Temperature &= 0x7FFF;
        *Temperature *= -1;
    }
    if(checkSum == ((DHT_DATA[0] + DHT_DATA[1] + DHT_DATA[2] + DHT_DATA[3]) & 0xFF))
    {
        return 1;                              // DATA CORRECT
    }
    else
    {
        return 0;                              // DATA ERROR
    }
}

//  ----------------------------------------------------------------------------
//  Prototype:     dht_Humidity(int value, char *str)
//  Input:         value: Do am can truyen vao range (0 - 1000)
//
//                 *str: Chuoi ki tu duoc dung de convert value to string
//
//  Description:   Ham thuc hien chuyen doi gia tri do am (humidity)
//                 thanh chuoi theo code ASCII de hien thi len lcd
//
//  Return:        Nothing
//  Requires:      Can phai su dung ham DHT_Read_Data(...) de doc du lieu tu
//                 DHT moi co the su dung duoc
//  ----------------------------------------------------------------------------
char* dht_Humidity(int value)
{
//     value = value * 0.95;            // Hieu chinh sai so +-5%
     static char str[5];
     switch(sel_dht)
     {
          case 1:
          {
               str[0] = value / 10 + 0x30;
               str[1] = value % 10 + 0x30;
               str[2] = '\0';
               str[3] = '\0';
               str[4] = '\0';
               break;
          }
          case 2:
          {
               if(value >= 1000)                // Humi = 100%
               {
                    str[0] = value / 1000 + 0x30;
                    str[1] = (value / 100) % 10 + 0x30;
                    str[2] = (value / 10) % 10 + 0x30;
                    str[3] = '\0';
                    str[4] = '\0';
               }
               else if(value == 0)             // Humi = 0%
               {
                    str[0] = '0';
                    str[1] = '\0';
                    str[2] = '\0';
                    str[3] = '\0';
                    str[4] = '\0';
               }
               else if(value < 100)             // Humi < 10% & Humi >= 0%
               {
                    str[0] = value / 10 + 0x30;
                    str[1] = '.';
                    str[2] = value % 10 + 0x30;
                    str[3] = '\0';
                    str[4] = '\0';
               }
               else                           // Humi >= 10% & Humi < 100%
               {
                    str[0] = value / 100 + 0x30;
                    str[1] = (value / 10) % 10 + 0x30;
                    str[2] = '.';
                    str[3] = value % 10 + 0x30;
                    str[4] = '\0';
               }
               break;
          }
     }
     return str;
}

//  ----------------------------------------------------------------------------
//  Prototype:     dht_Temperature(int value, char *str)
//  Input:         value: Nhiet do can truyen vao range (-400 --> 800)
//
//                 *str: Chuoi ki tu duoc dung de convert value to string
//
//  Description:   Ham thuc hien chuyen doi gia tri nhiet do (temperature)
//                 thanh chuoi theo code ASCII de hien thi len lcd
//
//  Return:        Nothing
//  Requires:      Can phai su dung ham DHT_Read_Data(...) de doc du lieu tu
//                 DHT moi co the su dung duoc
//  ----------------------------------------------------------------------------
char* dht_Temperature(int value)
{
//     value = value - 5;         // Hieu chinh sai so +- 0.5 celsius
     static char str[5];
     switch(sel_dht)
     {
          case 1:                 // SU DUNG CHO DHT11
          {
               if(value < 10)               // Temp < 10 celsius
               {
                    str[0] = value + 0x30;
                    str[1] = '\0';
                    str[2] = '\0';
                    str[3] = '\0';
                    str[4] = '\0';
               }
               else if(value >= 10)         // Temp >= 10 celsius
               {
                    str[0] = value / 10 + 0x30;
                    str[1] = value % 10 + 0x30;
                    str[2] = '\0';
                    str[3] = '\0';
                    str[4] = '\0';
               }
               break;
          }
          case 2:                 // SU DUNG CHO DHT21, DHT22
          {
               if(value >= 0 && value < 100)             // Temp < 10 celsius & >= 0
               {
                    str[0] = value / 10 + 0x30;
                    str[1] = '.';
                    str[2] = value % 10 + 0x30;
                    str[3] = '\0';
                    str[4] = '\0';
               }
               else if(value >= 100)                    // Temp >= 10 celsius
               {
                    str[0] = value / 100 + 0x30;
                    str[1] = (value / 10) % 10 + 0x30;
                    str[2] = '.';
                    str[3] = value % 10 + 0x30;
                    str[4] = '\0';
               }
               else if(value <= -100)                   // Temp = 10 celsius
               {
                    str[0] = '-';
                    str[1] = (value * -1) / 100 + 0x30;
                    str[2] = ((value * -1) / 10) % 10 + 0x30;
                    str[3] = '.';
                    str[4] = (value * -1) % 10 + 0x30;
               }
               else if(value > -100 && value < 0)       // Temp < 0 celsius & >= -10
               {
                    str[0] = '-';
                    str[1] = (value * -1) / 10 + 0x30;
                    str[2] = '.';
                    str[3] = (value * -1) % 10 + 0x30;
                    str[4] = '\0';
               }
          }
     }
     return str;
}

//  ----------------------------------------------------------------------------
//  Prototype:     void Dht_begin(char str[])
//  Input:         str[]: Chuoi ki tu "DHT11" hoac "DHT2x" DE CHON SENSOR PHU HOP
//
//  Description:   Ham nhan chuoi xac nhan sensor phu hop
//
//  Return:        Nothing
//  Requires:      Su dung dau tien de chon sensor
//  ----------------------------------------------------------------------------
void dht_begin(char str[])
{
     if(strcmp(str, "DHT11") == 0)
     {
          sel_dht = 1;
     }
     else if(strcmp(str, "DHT2x") == 0)
     {
          sel_dht = 2;
     }
}