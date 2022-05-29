#include "DHTxx.h"

// LCD module connections
sbit LCD_RS at RD1_bit;
sbit LCD_EN at RD3_bit;
sbit LCD_D4 at RD4_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D7 at RD7_bit;

sbit LCD_RS_Direction at TRISD1_bit;
sbit LCD_EN_Direction at TRISD3_bit;
sbit LCD_D4_Direction at TRISD4_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D7_Direction at TRISD7_bit;
// End LCD module connections

char character[] = { 4,4,14,10,17,17,17,14,          // HUMIDITY
                     4,14,10,10,31,31,31,14, 153 };  // THERMOMETER

// HAM TAO KI TU CHO LCD
void CustomChar(char *arr)
{
     char i = 0;
     lcd_cmd(64);    // address cgram 0 = 0x40
     while(arr[i] != 153)
     {
          lcd_chr_cp(arr[i]);
          i++;
     }
     lcd_cmd(_LCD_RETURN_HOME);
}


void main() 
{
    char *txt;
    int val_Humi, val_Temp;
    // ------ CAU HINH LCD ------ //
    TRISD.B2 = 0;
    LATD.B2 = 0;                       // R/W LCD IS LOW
    Lcd_Init();                        // Initialize LCD
    Lcd_Cmd(_LCD_CLEAR);               // Clear display
    Lcd_Cmd(_LCD_CURSOR_OFF);          // Cursor off
    CustomChar(character);             // CUSTOM ICON
    Lcd_Out(1, 1, "Humi  : ");
    Lcd_Chr(1, 6, 0);                  // ICON HUMIDITY
    Lcd_Out(2, 1, "Temp  : ");
    Lcd_Chr(2, 6, 1);                  // ICON THERMOMETER
    
    // DHT11 --> DHT11
    // DHT2x --> DHT21 or DHT22
    dht_begin("DHT2x");

    while(1)
    {
        if(dht_Read_Data(&val_Humi, &val_Temp))
        {
            txt = dht_Humidity(val_Humi);
            Lcd_Out(1, 9, txt);         // Value Humidity
            Lcd_Out_Cp("%   ");
            txt = dht_Temperature(val_Temp);
            Lcd_Out(2, 9, txt);         // Value Temperature
            Lcd_Chr_Cp(223);
            Lcd_Out_Cp("C     ");
        }
        else
        {
            Lcd_Out(3, 1, "ERROR!");
        }
        delay_ms(2000);
    }
}