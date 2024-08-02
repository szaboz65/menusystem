/*
	Input fields for the menusystem
*/

#include "MenuFields.h"
#include "MenuRenderer.h"


// *********************************************************
// FieldList
// *********************************************************
FieldList::FieldList(const char * name, const char **items)
  : MenuItem(name)
  , _items(items)
{
  _nitems = count_items();
}

void FieldList::render(MenuRenderer const& renderer) const {
  renderer.render(*this);
}

bool FieldList::next(bool loop) {
  if (_nitems == 0)
    return false;
  int last_sel = _selected_item;
  if (_selected_item < _nitems - 1) {
    _selected_item++;
  } else if ( loop ) {
    _selected_item = 0;
  }
  if (last_sel != _selected_item) {
    notify(MenuListener::Events::Changed);
    return true;
  }
  return false;
}

bool FieldList::prev(bool loop) {
  if (_nitems == 0)
    return false;
  int last_sel = _selected_item;
  if (_selected_item > 0) {
    _selected_item--;
  } else if ( loop ) {
    _selected_item = _nitems - 1;
  }
  if (last_sel != _selected_item) {
    notify(MenuListener::Events::Changed);
    return true;
  }
  return false;
}

bool FieldList::select(int /*mode*/) {
  if ( !has_focus()) {
    set_focus();
    return true;
  }
  _original_item = _selected_item;
  notify(MenuListener::Events::Save);
  set_focus(false);
  return false;
}

bool FieldList::back() {
  if (has_focus()) {
    _selected_item = _original_item;
    notify(MenuListener::Events::Changed);
    set_focus(false);
    return true;
  }
  return false;
}

void FieldList::reset() {
  _selected_item = _original_item;
  notify(MenuListener::Events::Load);
}

void FieldList::selected(int selected) {
  if ( selected < _nitems)
    _original_item = _selected_item = selected;

}

const char *FieldList::value() const {
  if ( _selected_item < _nitems)
    return _items[_selected_item];
  return nullptr;
}

int FieldList::count_items() const {
  int n = 0;
  while (_items[n]) ++n;
  return n;
}



// *********************************************************
// FieldEdit
// *********************************************************
const char FieldEdit::alpha[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxy";
const char FieldEdit::alphacaps[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char FieldEdit::alphanum[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxy0123456789";
const char FieldEdit::url[] = " abcdefghijklmnopqrstuvwxy0123456789.+-_~?/#";
const char FieldEdit::intnumbers[] = "0123456789";
const char FieldEdit::floatnumbers[] = "0123456789.";
const char FieldEdit::signnumbers[] = "0123456789.+-*/";


FieldEdit::FieldEdit(const char * name, uint8_t length)
  : MenuItem(name)
  , _length(length)
{
  _text.reserve(length);
  _original_text.reserve(length);
}

void FieldEdit::render(MenuRenderer const& renderer) const {
  renderer.render(*this);
}

bool FieldEdit::next(bool loop) {
  if ( !_charset) return false;
  int length = strlen(_charset);
  if ( !length) return false;
  int last_idx = _charset_index;
  if (_charset_index < length - 1) {
    _charset_index++;
  } else if ( loop ) {
    _charset_index = 0;
  }
  if (last_idx != _charset_index) {
    _text[_current_index] = _charset[_charset_index];
    notify(MenuListener::Events::Changed);
  }
  return true;
}

bool FieldEdit::prev(bool loop) {
  if ( !_charset) return false;
  int length = strlen(_charset);
  if ( !length) return false;
  int last_idx = _charset_index;
  if (_charset_index > 0) {
    _charset_index--;
  } else if ( loop ) {
    _charset_index = length - 1;
  }
  if (last_idx != _charset_index) {
    _text[_current_index] = _charset[_charset_index];
    notify(MenuListener::Events::Changed);
  }
  return true;
}

bool FieldEdit::select(int mode) {
  if ( !has_focus()) {
    if ( _text.length() < 1) _text += _charset[0];
    current_index(0);
    set_focus();
    return true;
  }
  if (mode == 0 && _current_index < _length - 1) {
    while ( _text.length() <= _current_index + 1) _text += _charset[0];
    current_index(_current_index + 1);
    return true;
  }
  if (mode == 2 && _current_index > 0) {
    current_index(_current_index - 1);
    return true;
  }
  if ( mode == 1 || (mode == 0 && _current_index == _length - 1)) {
    if ( !validate() ) {
      _original_text = _text;
      _original_text.trim();
      notify(MenuListener::Events::Save);
      set_focus(false);
    }
  }
  return false;
}

bool FieldEdit::back() {
  if (has_focus()) {
    _text = _original_text;
    notify(MenuListener::Events::Changed);
    set_focus(false);
    return true;
  }
  return false;
}

void FieldEdit::reset() {
  _text = _original_text;
  notify(MenuListener::Events::Load);
  current_index(0);
}

void FieldEdit::text(String atext) {
  _text = _original_text = atext.substring(0, _length);
}

String FieldEdit::text() const {
  //	String t(_text);
  //	t.trim();
  //	return t;
  return _text;
}

void FieldEdit::charset(CharSet acharset) {
  switch (acharset) {
    case CharSet::Alpha: 		charset(alpha); break;
    case CharSet::AlphaCaps: 	charset(alphacaps); break;
    case CharSet::AlphaNum: 	charset(alphanum); break;
    case CharSet::Url:	 		charset(url); break;
    case CharSet::IntNumber: 	charset(intnumbers); break;
    case CharSet::FloatNumber: 	charset(floatnumbers); break;
    case CharSet::SignNumbers: 	charset(signnumbers); break;
    default: break;
  }
}

void FieldEdit::charset(const char *acharset) {
  _charset = acharset;
}

int FieldEdit::find_charset_index(char letter) {
  int n = strlen(_charset);
  while ( n && _charset[--n] != letter);
  return n;
}

void FieldEdit::current_index(int index) {
  _current_index = index;
  _charset_index = index < _text.length() ? find_charset_index(_text[_current_index]) : 0;
}


// *********************************************************
// FieldAlphaCapsEdit
// *********************************************************
FieldAlphaCapsEdit::FieldAlphaCapsEdit(const char * name, uint8_t length)
  : FieldEdit(name, length)
{
  charset(CharSet::AlphaCaps);
}

// *********************************************************
// FieldAlphaNumEdit
// *********************************************************
FieldAlphaNumEdit::FieldAlphaNumEdit(const char * name, uint8_t length)
  : FieldEdit(name, length)
{
  charset(CharSet::AlphaNum);
}

// *********************************************************
// FieldUrlEdit
// *********************************************************
FieldUrlEdit::FieldUrlEdit(const char * name, uint8_t length)
  : FieldEdit(name, length)
{
  charset(CharSet::Url);
}


// *********************************************************
// FieldIntEdit
// *********************************************************
FieldIntEdit::FieldIntEdit(const char * name, int min, int max)
  : FieldEdit(name, String(max).length())
  , _imin(min)
  , _imax(max)
{
  charset(CharSet::IntNumber);
}

void FieldIntEdit::render(MenuRenderer const& renderer) const {
  renderer.render(*this);
}

bool FieldIntEdit::validate() {
  int val = value();
  if ( val < _imin || val > _imax)
    return true;
  return false;
}

int FieldIntEdit::value() const {
  return text().toInt();
}

void FieldIntEdit::value(int val) {
  String t(val);
  text(t);
}
