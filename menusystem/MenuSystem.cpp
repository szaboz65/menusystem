/*
	OS independent menusystem
*/

#include "MenuSystem.h"
#include "MenuRenderer.h"
#include <stdlib.h>


MenuListener::MenuListener(MenuComponent &component)
  : _component(component)
{
  _component.addlistener(this);
}


// *********************************************************
// MenuComponent
// *********************************************************

void MenuComponent::notify(MenuListener::Events theevent) {
  if (_listener) _listener->EventHandler(theevent);
}

MenuComponent::MenuComponent(const char* name)
  : _name(name)
  ,  _has_focus(false)
  ,  _is_current(false)
{
}

void MenuComponent::set_focus(bool has_focus) {
  if ( _has_focus != has_focus) {
    _has_focus = has_focus;
    notify( _has_focus ? MenuListener::Events::Enter : MenuListener::Events::Exit);
  }
}

void MenuComponent::set_current(bool is_current) {
  if ( _is_current != is_current) {
    if (!is_current && _has_focus)
      _has_focus = false;
    _is_current = is_current;
    notify( _is_current ? MenuListener::Events::Activate : MenuListener::Events::Deactivate);
  }
}

// *********************************************************
// MenuItem
// *********************************************************
MenuItem::MenuItem(const char* name)
  : MenuComponent(name) {
}

void MenuItem::reset() {
  // Do nothing.
}

bool MenuItem::next(bool loop) {
  return false;
}

bool MenuItem::prev(bool loop) {
  return false;
}

bool MenuItem::select(int /*mode*/) {
  return false;
}

bool MenuItem::back() {
  return false;
}

void MenuItem::render(MenuRenderer const& renderer) const {
  renderer.render(*this);
}

// *********************************************************
// Focusable MenuItem
// *********************************************************
FocusableMenuItem::FocusableMenuItem(const char *name) : MenuItem(name) 
{
}

bool FocusableMenuItem::select(int /*param*/) { 
  if(!has_focus()) {
    set_focus(); 
    return true;
  } 
  return false;
}

bool FocusableMenuItem::back() { 
  if( has_focus()) { 
    set_focus(false); 
    return true;
  } 
  return false;
}

// *********************************************************
// Menu
// *********************************************************
Menu::Menu(const char* name)
  : MenuComponent(name)
  , _menu_components(nullptr)
  , _p_parent(nullptr)
  , _num_components(0)
  , _current_component_num(0)
{
}

void Menu::render(MenuRenderer const& renderer) const {
  renderer.render(*this);
}

bool Menu::next(bool loop) {
  if (!_num_components || !get_current_component()) {
    return false;
  }
  if (_menu_components[_current_component_num]->has_focus()) {
    return _menu_components[_current_component_num]->next(loop);
  }
  if (_current_component_num != _num_components - 1) {
    _menu_components[_current_component_num]->set_current(false);
    _current_component_num++;
    _menu_components[_current_component_num]->set_current();
    return true;
  } else if (loop) {
    _menu_components[_current_component_num]->set_current(false);
    _current_component_num = 0;
    _menu_components[_current_component_num]->set_current();
    return true;
  }
  return false;
}

bool Menu::prev(bool loop) {
  if (!_num_components || !get_current_component()) {
    return false;
  }
  if (_menu_components[_current_component_num]->has_focus()) {
    return _menu_components[_current_component_num]->prev(loop);
  }
  if (_current_component_num != 0) {
    _menu_components[_current_component_num]->set_current(false);
    _current_component_num--;
    _menu_components[_current_component_num]->set_current();
    return true;
  } else if (loop) {
    _menu_components[_current_component_num]->set_current(false);
    _current_component_num = _num_components - 1;
    _menu_components[_current_component_num]->set_current();
    return true;
  }
  return false;
}

bool Menu::select(int /*mode*/) {
  if ( get_current_component()) {
    if ( is_current())
      set_current(false);
    get_current_component()->set_current();
    return true;
  }
  return false;
}

bool Menu::back() {
  if ( parent() && get_current_component()) {
    get_current_component()->set_current(false);
    return true;
  }
  return false;
}

Menu* Menu::activate(int mode) {
  MenuComponent* pComponent = get_current_component();
  if (pComponent == nullptr)
    return nullptr;

  if ( pComponent->is_focusable()) {
    if ( pComponent->select(mode)) {
      ;
    }
    return nullptr;
  }

  if ( pComponent->select(mode)) {
    return static_cast<Menu*>(pComponent);
  }
  return nullptr;
}

Menu* Menu::deactivate() {
  MenuComponent* pComponent = get_current_component();
  if (pComponent == nullptr)
    return nullptr;

  if ( pComponent->is_focusable()) {
    if ( pComponent->back()) {
      return this;
    }
    pComponent->set_current(false);
  }

  pComponent->back();
  Menu *menu = parent();
  if ( menu ) {
    menu->select(0);
    return menu;
  }
  select(0);
  return this;
}

void Menu::reset() {
  if ( get_current_component() && get_current_component()->is_current())
    get_current_component()->set_current(false);

  for (int i = 0; i < _num_components; ++i)
    _menu_components[i]->reset();

  _current_component_num = 0;
}

void Menu::add_item(MenuItem* p_item) {
  add_component((MenuComponent*) p_item);
}

void Menu::add_menu(Menu* p_menu) {
  add_component((MenuComponent*) p_menu);
  p_menu->parent(this);
}

void Menu::add_component(MenuComponent* p_component) {
  // Resize menu component list, keeping existing items.
  // If it fails, there the item is not added and the function returns.
  _menu_components = (MenuComponent**) realloc(_menu_components,
                     (_num_components + 1) * sizeof(MenuComponent*));
  if (_menu_components == nullptr) {
    _num_components = 0;
    return;
  }

  _menu_components[_num_components] = p_component;
  _num_components++;
}

MenuComponent* Menu::get_menu_component(uint8_t index) const {
  if ( index < _num_components)
    return _menu_components[index];
  return nullptr;
}



// *********************************************************
// MenuSystem
// *********************************************************
MenuSystem::MenuSystem(Menu *p_root_menu, MenuRenderer const* renderer)
  : _p_root_menu(p_root_menu),
    _p_curr_menu(_p_root_menu),
    _renderer(renderer)
{
}

bool MenuSystem::next(bool loop) {
  return _p_curr_menu->next(loop);
}

bool MenuSystem::prev(bool loop) {
  return _p_curr_menu->prev(loop);
}

void MenuSystem::select(int mode) {
  Menu* pMenu = _p_curr_menu->activate(mode);

  if (pMenu != nullptr)
    _p_curr_menu = pMenu;
}

void MenuSystem::back() {
  Menu* pMenu = _p_curr_menu->deactivate();

  if (pMenu != nullptr)
    _p_curr_menu = pMenu;
}

void MenuSystem::reset() {
  _p_curr_menu = _p_root_menu;
  _p_root_menu->reset();
  if (_p_curr_menu->get_current_component())
    _p_curr_menu->get_current_component()->set_current();
}

void MenuSystem::display() const {
  if (_p_curr_menu != nullptr && _renderer != nullptr)
    _renderer->render_menu(*_p_curr_menu);
}
