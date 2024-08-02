#pragma once
#include "Arduino.h"
#include <EEPROM.h>

template <class SData>
class Settings
{
  public:
    SData data;
	
	Settings(uint8_t eeaddress = 0)
	: _eeaddress(eeaddress)
	{
	}
	
	bool begin()
	{
	  SData temp;
	  bool success = load(temp);
	  if( success && !temp.isvalid())
	  {
		//Erasing EEPROM
		SData tosave;
		success = save(tosave);
		if( success)
		{
			success = load(temp);
		}
	  }
	  if( success && temp.isvalid())
	  {
		//EEPROM ok
		data = temp;
		return true;
	  }
	  //EEPROM error
	  return false;
	}
	
	bool load()
	{
		return load(data);
	}

	bool save()
	{
		return save(data);
	}
	
  private:
    uint8_t _eeaddress=0;
	bool load(SData &data) const
	{
	  EEPROM.get(_eeaddress, data);
	  return true;
	}

	bool save(SData &data) const
	{
	  EEPROM.put(_eeaddress, data);
	  EEPROM.commit();
	  return true;
	}
};

