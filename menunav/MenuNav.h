#pragma once

#include "../menusystem/MenuSystem.h"

class MenuNav {

  public:
    enum class Actions {
      NONE,
      NEXT,
      PREV,
      SELECT,
      BACK
    };

    MenuNav(MenuSystem *ms=nullptr);
    bool setMenuSystem(MenuSystem *ms);
    bool doAction(Actions action, int param=0);
    bool begin();
    bool end();
    bool isActive() const { return _is_active; }
    bool isLoop() const { return _loop; }
	void loop(bool l) { _loop = l; }
	
  protected:
    MenuSystem *_ms;
	bool _is_active = false;
	bool _loop = false;
};
