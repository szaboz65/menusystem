/*
	Arduino implementation to control the generic button model.

*/

#include "ButtonCtrl.h"

ButtonCtrl::ButtonCtrl(ButtonModel &model, byte apin, byte buttonMode)
  : _model(model)
  , _pin(apin)
{
  pinMode(apin, buttonMode);
}

void ButtonCtrl::enable() {
  _model.enable(readstate());
}

void ButtonCtrl::disable() {
  _model.disable();
}

void ButtonCtrl::process() {
  _model.process(readstate(), millis());
}

bool ButtonCtrl::readstate() const
{
  return digitalRead(_pin) == HIGH;
}
