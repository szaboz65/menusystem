/*
	OS independent generic button model.
  Based on the Observer pattern the button model notify the listener object.
  This implementation has only one listener.
  https://en.wikipedia.org/wiki/Observer_pattern
*/

#include "ButtonModel.h"

ButtonListener::ButtonListener(ButtonModel &model)
  : _model(model)
{
  _model.addlistener(this);
}

void ButtonModel::notify(ButtonListener::Events theevent) {
  if (_listener) _listener->EventHandler(theevent);
}


void ButtonModel::enable(bool astate) {
  _click_count = 0;
  _prev_state = astate;
  _pressed_triggered = false;
  _longclick_detected = false;
  _enabled = true;
}

void ButtonModel::disable() {
  _enabled = false;
}

void ButtonModel::process(bool astate, unsigned long now) {
  if ( !_enabled) return;

  // is button pressed?
  if (_prev_state == true && astate == false) {
    _down_ms = now;
    _pressed_triggered = false;
    _click_ms = _down_ms;
    _click_count++;

    // is the button released?
  } else if (_prev_state == false && astate == true) {
    unsigned int down_time_ms = now - _down_ms;
    // is it beyond debounce time?
    if (down_time_ms >= _debounce_time_ms) {
      // was it a longclick? (preceeds single / double / triple clicks)
      if (down_time_ms >= _longclick_time_ms) {
        _longclick_detected = true;
      }
      // trigger release
      notify(ButtonListener::Events::Changed);
      notify(ButtonListener::Events::Released);
    }

    // trigger pressed event (after debounce has passed)
  } else if (astate == false && !_pressed_triggered && (now - _down_ms >= _debounce_time_ms)) {
    _pressed_triggered = true;
    // trigger pressed
    notify(ButtonListener::Events::Changed);
    notify(ButtonListener::Events::Pressed);

    // is the button pressed and the time has passed for multiple clicks?
  } else if (astate == true && _click_ms && now - _click_ms > _doubleclick_time_ms) {
    // was there a longclick?
    if (_longclick_detected) {
      // was it part of a combination?
      if (_click_count == 1) {
        notify(ButtonListener::Events::LongClick);
      }
      _longclick_detected = false;
      // determine the number of single clicks
    } else if (_click_count > 0) {
      switch (_click_count) {
        case 1:
          notify(ButtonListener::Events::Click);
          break;
        case 2:
          notify(ButtonListener::Events::DoubleClick);
          break;
        case 3:
          notify(ButtonListener::Events::TripleClick);
          break;
      }
    }
    _click_count = 0;
    _click_ms = 0;
  }
  _prev_state = astate;
}
