#pragma once

#include "../taskman/TaskMan.h"
#include "MenuNav.h"


class MenuNavTask : public Task
{
public:
  MenuNavTask(MenuNav *menunav = nullptr) {setmenunav(menunav);}
  
  public:
	void begin();
	void end();
	void run();
	
	void setmenunav(MenuNav *menunav);

};
