#pragma once
#include "utils/settings.h"

// settings data
struct WFMSettings
{
  unsigned long eeprom_id;
  uint16_t param;
  char sn[12];
  char host[32];
  char ssid[16];
  char pass[16];
  char loop;
  char brightness;
  char mpass[8];
  
  WFMSettings();
  bool isvalid();
  void dump();
};

/*******************************/
//settings
extern Settings<WFMSettings> Setting;
