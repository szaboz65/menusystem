/*
  Arduino implementation to control the generic rotary encoder model.

*/

#pragma once

#include "RotaryModel.h"
#include "Arduino.h"

class RotaryCtrl : public RotaryModel {
  public:
    RotaryCtrl(RotaryModel &model, byte pina, byte pinb, byte buttonMode = INPUT_PULLUP);

    void enable();
    void disable();
    void process();

  private:
    RotaryModel &_model;
    byte _pina;
    byte _pinb;
};
