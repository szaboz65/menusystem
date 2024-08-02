/*
	OS independent generic rotary encoder model.
  Based on the Observer pattern the rotary model notify the listener object.
  This implementation has only one listener.
  https://en.wikipedia.org/wiki/Observer_pattern

*/
#include "RotaryModel.h"

RotaryListener::RotaryListener(RotaryModel &model)
  : _model(model)
{
  _model.addlistener(this);
}

void RotaryModel::notify(RotaryListener::Events theevent) {
  if (_listener) _listener->EventHandler(theevent);
}


RotaryModel::RotaryModel(int amoves_per_click ) {
  moves_per_click(amoves_per_click);
  reset();
}

void RotaryModel::moves_per_click(int amoves_per_click)
{
  if (amoves_per_click < 1) {
    //#pragma message("At least one move per click required, reverting to default")
    amoves_per_click = MOVES_PER_CLICK;
  }
  _moves_per_click = amoves_per_click;
}

void RotaryModel::reset() {
  _last_position = 0;
  _position = 0;
  _direction = Directions::Unknow;
}


void RotaryModel::enable(bool statea, bool stateb) {
  make_state(statea, stateb);
  _enabled = true;
}

void RotaryModel::disable() {
  _enabled = false;
}

void RotaryModel::process(bool statea, bool stateb) {
  if( !_enabled) return;
  
  int s = make_state(statea, stateb);
  switch (s) {
    case 1: case 7: case 8: case 14:
      _position++; break;
    case 2: case 4: case 11: case 13:
      _position--; break;
    case 3: case 12:
      _position += 2; break;
    case 6: case 9:
      _position -= 2; break;
    default:
      //case 0: case 5: case 10: case 15:
      break;
  }

  if (_position - _last_position >= _moves_per_click) {
    _direction = Directions::Right;
    notify(RotaryListener::Events::Changed);
    do {
      notify(RotaryListener::Events::Right);
      _last_position += _moves_per_click;
    } while ( _position - _last_position >= _moves_per_click);
  }
  else if (_last_position - _position >= _moves_per_click) {
    _direction = Directions::Left;
    notify(RotaryListener::Events::Changed);
    do {
      notify(RotaryListener::Events::Left);
      _last_position -= _moves_per_click;
    } while ( _last_position  - _position >= _moves_per_click);
  }
}

int RotaryModel::make_state(bool statea, bool stateb) {
  int s = _prev_state & 3;
  if (statea) s |= 4;
  if (stateb) s |= 8;
  _prev_state = (s >> 2);
  return s;
}
