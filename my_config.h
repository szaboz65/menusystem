#pragma once

//version info
#define VERSION 100
#define VERSION_STR "100"
#define BUILDDATE_STR "2024.07.30"

//login info
#define LOGIN_PASS "123456"   //max 6 char
//device info
#define MODEL "MODEL"
#define SN    "12345678"     //max 10 char

//WIFI info
#define WIFI_SSID "WIFI_SSID"  //max 15 char
#define WIFI_PASS "WIFI_PASS"  // max 15 char

//server info
#define HOST "www.url.com"    //max 31 char
#define PORT 80
#define URI  "/index.php"

//eeprom config
#define EEPROM_INITIALE 0x1e2f3eb4

//button inputs
#define ROTARY_PINA 13
#define ROTARY_PINB 15
#define BUTTON_A_PIN  12
#define BUTTON_B_PIN  35

//battery input
#define ADC_PIN 34  // TTGO battery input

