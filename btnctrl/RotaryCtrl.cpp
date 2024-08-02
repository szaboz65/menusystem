/*
  Arduino implementation to control the generic rotary encoder model.

*/

#include "RotaryCtrl.h"

RotaryCtrl::RotaryCtrl(RotaryModel &model, byte pina, byte pinb, byte buttonMode)
  : _model(model)
  ,  _pina(pina)
  ,  _pinb(pinb)
{
  pinMode(pina, buttonMode);
  pinMode(pinb, buttonMode);
}

void RotaryCtrl::enable() {
  _model.enable(digitalRead(_pina) == HIGH, digitalRead(_pinb) == HIGH);
}

void RotaryCtrl::disable() {
  _model.disable();
}

void RotaryCtrl::process() {
  _model.process(digitalRead(_pina) == HIGH, digitalRead(_pinb) == HIGH);
}
