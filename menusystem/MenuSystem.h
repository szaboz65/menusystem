/*
	OS independent menusystem
*/

#pragma once

#include <inttypes.h>


class MenuRenderer;

class MenuComponent;

class MenuListener {
  public:
    //! \brief define events
    //!
    enum class Events {
      Activate,
      Deactivate,
      Enter,
      Exit,
      Changed,
      Load,
      Save
    };

    MenuListener(MenuComponent &component);
    virtual ~MenuListener() {}
    virtual void EventHandler(MenuListener::Events theevent) = 0;

    MenuComponent &component() const {return _component; }
    
  private:
    MenuComponent &_component;
};


// *********************************************************
// MenuComponent
// *********************************************************
//! \brief Abstract base class that represents a component in the menu
//!
//! This is the abstract base class for the main components used to build a
//! menu structure: Menu and MenuItem.
//!
//! MenuComponent should not be used as a base class in clients. Instead use
//! either Menu or MenuItem.
//!
//! MenuComponent is the `Component` part of the composite design pattern (see:
//! https://en.wikipedia.org/wiki/Composite_pattern).
//!
//! Based on the Observer pattern the button model notify the only one listener object.
//! This implementation has only one listener.
//! https://en.wikipedia.org/wiki/Observer_pattern
//!
//! \see Menu
//! \see MenuItem
class MenuComponent {
public:
    //! \brief Construct a MenuComponent
    //! \param[in] name The name of the menu component that is displayed in
    //!                 clients.
    MenuComponent(const char* aname);
    virtual ~MenuComponent() {};
	
    //! \brief Set the component's name
    //! \param[in] name The name of the menu component that is displayed in
    //!                 clients.
    void name(const char* aname) { _name = aname; }

    //! \brief Gets the component's name
    //! \returns The component's name.
    const char* name() const { return _name; }

    //! \brief Returns true if this component has focus; false otherwise
    //!
    //! This bool registers if the component has the focus.
    //!
    //! \returns true if this component has focus, false otherwise.
    //! \see MenuComponent::set_focus
    bool has_focus() const { return _has_focus; }

    //! \brief Returns true if this is the current component; false otherwise
    //!
    //! This bool registers if the component is the current selected component.
    //!
    //! \returns true if this component is the current component, false otherwise.
    //! \see MenuComponent::set_current
    bool is_current() const { return _is_current; }

    //! \brief Set the focus of the component
    //!
    //! \param has_current true if this component has the focus; false otherwise.
    //! Send Activate or Deactivate Events
    //! \see has_current
    void set_focus(bool has_focus=true);

    //! \brief Set the current state of the component
    //!
    //! \param is_current true if this component is the current one; false otherwise.
    //! Send Enter or Exit Events	
    //! \see is_current
    void set_current(bool is_current=true);

    //! \brief is_focusable is true if the component can grab the focus
    virtual bool is_focusable() const { return false; }

    //! \brief Renders the component using the given MenuComponentRenderer
    //!
    //! This is the `accept` method in the visitor design pattern. It should
    //! call the appropriate method on the given MenuComponentRenderer for
    //! rendering this type of component. This method should be called from the
    //! MenuComponent::render method.
    //!
    //! \param[in] renderer The MenuComponentRenderer to use to render this
    //!                     component
    //!
    //! \see MenuComponentRenderer
    virtual void render(MenuRenderer const& renderer) const = 0;

    //! \brief Processes the next action
    //!
    //! \param [in] loop if true the action should loop around to the beginning
    //!             for finite ranges; otherwise overflow should be prevented.
    //! \returns true if the component processed the action, false otherwise.
    //!
    //! \see MenuComponent::prev
    //! \see MenuComponent::has_focus
    virtual bool next(bool loop=false) = 0;

    //! \brief Processes the prev action
    //!
    //! \param [in] loop if true the action should loop around to the end
    //!             for finite ranges; otherwise overflow should be prevented.
    //! \returns true if the component processed the action, false otherwise.
    //!
    //! \see MenuComponent::next
    //! \see MenuComponent::has_focus
    virtual bool prev(bool loop=false) = 0;

    //! \brief Processes the select action
    //!
    //! \param [in] mode mode can extend the select process, 
	  //!             ex. click, doubleclick, longclick
    //! \returns true if the component processed the action, false otherwise.
    //!
    //! \see MenuComponent::back
    //! \see MenuComponent::has_focus
    virtual bool select(int mode=0) = 0;

    //! \brief Processes the back action
    //!
    //! \returns true if the component processed the action, false otherwise.
    //!
    //! \see MenuComponent::select
    //! \see MenuComponent::has_focus
    virtual bool back() = 0;

    //! \brief Resets the component to its initial state
    virtual void reset() = 0;

    //! \brief Add the event listener to the component 
    void addlistener(MenuListener *listener) {
      _listener = listener;
    }
    //! \brief Notify the event listener about the event
    void notify(MenuListener::Events theevent);


private:
    MenuListener *_listener = nullptr;
    const char* _name;
    bool _has_focus = false;
    bool _is_current = false;
};

// *********************************************************
// MenuItem
// *********************************************************
//! \brief A MenuComponent that calls the EventHandler function.
//!
//! MenuItem represents the `Leaf` in the composite design pattern (see:
//! https://en.wikipedia.org/wiki/Composite_pattern). 
//!
//! \see MenuComponent
class MenuItem : public MenuComponent {
public:
    //! \brief Construct a MenuItem
    //! \param[in] name The name of the menu component that is displayed in
    //!                 clients.
    MenuItem(const char* name);

    //! \copydoc MenuComponent::render
    virtual void render(MenuRenderer const& renderer) const override;

    //! \copydoc MenuComponent::next
    //!
    //! This method does nothing in MenuItem.
    virtual bool next(bool loop=false) override;

    //! \copydoc MenuComponent::prev
    //!
    //! This method does nothing in MenuItem.
    virtual bool prev(bool loop=false) override;

    //! \copydoc MenuComponent::select
    //!
    //! This method does nothing in MenuItem.
    virtual bool select(int mode=0) override;

    //! \copydoc MenuComponent::back
    //!
    //! This method does nothing in MenuItem.
    virtual bool back() override;

    //! \copydoc MenuComponent::reset
    //!
    //! This method does nothing in MenuItem.
    virtual void reset() override;

};


// *********************************************************
// Focusable MenuItem
// *********************************************************
class FocusableMenuItem : public MenuItem {
  public:
    FocusableMenuItem(const char *name);
    bool is_focusable() const override { return true; }
    bool select(int param=0) override;
    bool back() override;
};

// *********************************************************
// Menu
// *********************************************************
//! \brief A MenuComponent that can contain other MenuComponents.
//!
//! Menu represents the branch in the composite design pattern (see:
//! https://en.wikipedia.org/wiki/Composite_pattern). 
//!
//! \see MenuComponent
//! \see MenuItem
class Menu : public MenuComponent {
public:
    Menu(const char* name);

    //! \brief Adds a MenuItem to the Menu
    void add_item(MenuItem* p_item);

    //! \brief Adds a Menu to the Menu
    void add_menu(Menu* p_menu);

    MenuComponent* get_menu_component(uint8_t index) const;
    MenuComponent* get_current_component() const { return get_menu_component(_current_component_num);}

    uint8_t get_num_components() const { return _num_components; }
    uint8_t get_current_component_num() const { return _current_component_num; }

    void parent(Menu* p_parent) { _p_parent = p_parent; }
    Menu * parent() const { return _p_parent; }

    //! \brief Activates the current selection
    //!
    //! When a client makes a selection, activate is called on the current menu
    //! which in turn calls the menu's current item's callback.
    Menu* activate(int mode=0);
    Menu* deactivate();


    //! \copydoc MenuComponent::render
    virtual void render(MenuRenderer const& renderer) const override;

    //! \brief Processes the next action
    //!
    //! The behaviour of this function can differ depending on whether
    //! MenuComponent::has_focus returns true or false.
    //!
    //! When MenuComponent::has_focus returns true, this method should change
    //! some state in the component; when it returns false, this method should
    //! navigate the menu structure.
    //!
    //! \param [in] loop if true the action should loop around to the beginning
    //!                 for finite ranges; otherwise overflow should be prevented.
    virtual bool next(bool loop=false) override;

    //! \brief Processes the prev action
    //!
    //! The behaviour of this function can differ depending on whether
    //! MenuComponent::has_focus returns true or false.
    //!
    //! When MenuComponent::has_focus returns true, this method should change
    //! some state in the component; when it returns false, this method should
    //! navigate the menu structure.
    //!
    //! \param [in] loop if true the action should loop around to the end
    //!                 for finite ranges; otherwise overflow should be
    //!                 prevented.
    //! \returns true if the component processed the action, false otherwise.
    virtual bool prev(bool loop=false) override;

    //! \copydoc MenuComponent::select
    //!
    virtual bool select(int mode=0) override;

    //! \copydoc MenuComponent::back
    //!
    virtual bool back() override;

    //! \copydoc MenuComponent::reset
    virtual void reset() override;


protected:
    void add_component(MenuComponent* p_component);

private:
    MenuComponent** _menu_components;
    Menu* _p_parent;
    uint8_t _num_components;
    uint8_t _current_component_num;
};

// *********************************************************
// MenuSystem
// *********************************************************
class MenuSystem {
public:
    MenuSystem(Menu *p_root_menu, MenuRenderer const* renderer=nullptr);

    //! \brief Processes the next action
    //! \param [in] loop if true the action should loop around to the end
    //!                 for finite ranges; otherwise overflow should be
    //!                 prevented.
    //! \returns true if the component processed the action, false otherwise.
    bool next(bool loop=false);

    //! \brief Processes the prev action
    //! \param [in] loop if true the action should loop around to the end
    //!                 for finite ranges; otherwise overflow should be
    //!                 prevented.
    //! \returns true if the component processed the action, false otherwise.
    bool prev(bool loop=false);
	
    //! \brief Resets the component to its initial state
    void reset();

    //! \brief Activate the current component
    void select(int mode=0);
    //! \brief Deactivate the current component
    void back();
    //! \brief Display the menusystem
    void display() const;

    Menu& get_root_menu() const { return *_p_root_menu; }
    Menu const* get_current_menu() const { return _p_curr_menu;}
	void setrenderer(MenuRenderer const* renderer) { _renderer = renderer; }
	
private:
    Menu* _p_root_menu;
    Menu* _p_curr_menu;
    MenuRenderer const* _renderer;
};
