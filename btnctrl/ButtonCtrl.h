/*
	Arduino implementation to control the generic button model.

*/
#pragma once

#include "ButtonModel.h"
#include "Arduino.h"

class ButtonCtrl  {
  public:
    ButtonCtrl(ButtonModel &model, byte apin, byte buttonMode = INPUT_PULLUP);

    void enable();
    void disable();
    void process();

  protected:
    ButtonModel &_model;

  private:
    byte _pin;
    bool readstate() const;
};
