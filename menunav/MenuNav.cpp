#include "MenuNav.h"


MenuNav::MenuNav(MenuSystem *ms)
  : _ms(ms)
{
	if(ms) ms->reset();
}

bool MenuNav::setMenuSystem(MenuSystem *ms) {
  if(!ms) return false;
  _ms=ms;
  _ms->reset();
  if( _is_active )
    _ms->display();
  return true;
}

bool MenuNav::doAction(Actions action, int param)
{
  if(!_ms) return false;
  switch (action) {
    case Actions::NONE:
      if( _is_active) _ms->display();
      break;
    case Actions::PREV:
      if(_ms->prev(_loop)) {
        if( _is_active) _ms->display();
      }
      break;
    case Actions::NEXT:
      if(_ms->next(_loop)) {
        if( _is_active) _ms->display();
      }
      break;
    case Actions::BACK:
      _ms->back();
      if( _is_active) _ms->display();
      break;
    case Actions::SELECT:
      _ms->select(param);
      if( _is_active) _ms->display();
      break;
    default:
      return false;
  }
  return true;
}

bool MenuNav::begin() {
	if(!_ms) return false;
	_is_active = true;
	_ms->display();
	return true;
}

bool MenuNav::end() {
	_is_active = false;
	return true;
}
