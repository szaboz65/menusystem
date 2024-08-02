#include "../my_config.h"
#include "../btnctrl/ButtonModel.h"
#include "../btnctrl/RotaryModel.h"
#include "MenuNavTask.h"

class MenuNavProxy {
  public:
    MenuNavProxy(MenuNav *menunav = nullptr) : _menunav(menunav) {}
    void setmenunav(MenuNav *menunav) {
      _menunav = menunav;
    }
    bool doAction(MenuNav::Actions action, int param = 0) {
      if (_menunav) return _menunav->doAction(action, param);
      return false;
    }
	void begin() {
		if (_menunav) _menunav->begin();
	}
	void end() {
		if (_menunav) _menunav->end();
	}
  private:
    MenuNav *_menunav;
};

//---------------------------------------------------------------
class RotaryList : public RotaryListener
{
  public:
    RotaryList(RotaryModel &model, MenuNavProxy &menunav) : RotaryListener(model), _menunav(menunav) {}

    void EventHandler(Events theevent) {
      if (theevent == Events::Left) {
        _menunav.doAction(MenuNav::Actions::PREV);
      } else if (theevent == Events::Right) {
        _menunav.doAction(MenuNav::Actions::NEXT);
      }
    }
  private:
    MenuNavProxy &_menunav;
};

//---------------------------------------------------------------
class ButtonListA : public ButtonListener {
  public:
    ButtonListA(ButtonModel &model, MenuNavProxy &menunav) : ButtonListener(model), _menunav(menunav) {}

    void EventHandler(ButtonListener::Events theevent) {
      if (theevent == Events::Click) {
        _menunav.doAction(MenuNav::Actions::SELECT, 0);
      } else if ( theevent == Events::DoubleClick) {
        _menunav.doAction(MenuNav::Actions::SELECT, 1);
      } else if (theevent == Events::LongClick) {
        _menunav.doAction(MenuNav::Actions::SELECT, 2);
      }
    }
  private:
    MenuNavProxy &_menunav;
};

//---------------------------------------------------------------
class ButtonListB : public ButtonListener {
  public:
    ButtonListB(ButtonModel &model, MenuNavProxy &menunav) : ButtonListener(model), _menunav(menunav) {}

    void EventHandler(ButtonListener::Events theevent) {
      if (theevent == Events::Click ) {
        _menunav.doAction(MenuNav::Actions::BACK);
      }
    }
  private:
    MenuNavProxy &_menunav;
};

//---------------------------------------------------------------
MenuNavProxy MenuNavProxy;

RotaryModel rotarymodel;
RotaryCtrl rotary1(rotarymodel, ROTARY_PINA, ROTARY_PINB);
RotaryList rotarylist(rotarymodel, MenuNavProxy);

ButtonModel buttonmodela;
ButtonCtrl ButtonA(buttonmodela, BUTTON_A_PIN);
ButtonListA ButtonListenerA(buttonmodela, MenuNavProxy);

ButtonModel buttonmodelb;
ButtonCtrl ButtonB(buttonmodelb, BUTTON_B_PIN);
ButtonListB ButtonListenerB(buttonmodelb, MenuNavProxy);


//---------------------------------------------------------------
void MenuNavTask::begin() {
  ButtonA.enable();
  ButtonB.enable();
  rotary1.enable();
  MenuNavProxy.begin();
  start();
}

void MenuNavTask::end() {
  stop();
  MenuNavProxy.end();
  ButtonA.disable();
  ButtonB.disable();
  rotary1.disable();
}

void MenuNavTask::run() {
  ButtonA.process();
  ButtonB.process();
  rotary1.process();
}

void MenuNavTask::setmenunav(MenuNav *menunav) {
  MenuNavProxy.setmenunav(menunav);
}
