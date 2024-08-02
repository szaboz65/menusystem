#include "my_config.h"
#include "my_settings.h"
	
WFMSettings::WFMSettings()
: eeprom_id(EEPROM_INITIALE)
, param(0)
{
  strcpy(host, HOST);
  strcpy(sn, SN);
  strcpy(ssid, WIFI_SSID);
  strcpy(pass, WIFI_PASS);
  loop = 0;
  brightness = 100;
  strcpy(mpass, LOGIN_PASS);
  //memset(spare, 0, sizeof(spare));
}

bool WFMSettings::isvalid()
{
  if( eeprom_id != EEPROM_INITIALE)
	  return false;  // invalid

  return true;  //valid
}

void WFMSettings::dump()
{
  Serial.print(F("Param: "));
  Serial.println(param);
  Serial.print(F("HOST: "));
  Serial.println(host);  
  Serial.print(F("SN: "));
  Serial.println(sn);  
}
