//  ----------------------------------------------------------------------------
//  Program: Library communication DHT sensor with pic
//  Writter: Hoang Vu
//  Link refer: 
//    https://simple-circuit.com/mikroc-dht22-sensor-pic-microcontroller/
//    https://labprojectsbd.com/2020/05/17/how-to-interface-dht22-am2302-rht03-sensor-with-pic16f877a/
//  Decriptions: Thu vien giao tiep voi DHT sensor thuoc cac loai sau day
//               DHT11, DHT21, DHT22
//               Ba sensor co cac khai bao chon phu hop khi giao tiep
//               "DHT11" --> Chon DHT11
//               "DHT2x" --> Chon DHT21 hoac DHT22
//               ( CA 2 SENSOR DEU CO CAC CHI SO KHA TUONG DUONG NHAU )
//  Verison: 1.0.1
//  Update: 3/3/2022
//  ----------------------------------------------------------------------------

#pragma once

//  Declare the communication pin with the sensor
sbit DHT_PIN at RC0_bit;
sbit DHT_PIN_Direction at TRISC0_bit;

//  Declare function used library
void sensor_Response();
unsigned short dht_Read_Byte();
int dht_Read_Data(int *Humidity, int *Temperature);
char* dht_Humidity(int value);
char* dht_Temperature(int value);
void dht_begin(char str[]);