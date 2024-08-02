/*
	MenuRenderer for the menusystem based on the visitor pattern
*/

#pragma once

#include "MenuSystem.h"
#include "MenuFields.h"

// *********************************************************
// MenuRenderer
// *********************************************************
//! \brief Abstract base class that renders the component in the menu
//! https://en.wikipedia.org/wiki/Visitor_pattern
//!
//! This is the `accept` method in the visitor design pattern. It should
//! call the appropriate method on the given MenuRenderer for rendering 
//! this type of component. This method should be called from the
//! MenuComponent::render method.
//!
//! \param[in] renderer The MenuRenderer to use to render this component
//!
//! \see MenuRenderer
class MenuRenderer {
public:
    //! complette base menu
    virtual void render_menu(Menu const& menu) const = 0;
    //! individual menu 
    virtual void render(Menu const& menu) const = 0;
    //! individual menuitem
    virtual void render(MenuItem const& menu_item) const = 0;

    //! List field
    virtual void render(FieldList const& list) const = 0;
    //! Edit field
    virtual void render(FieldEdit const& edit) const = 0;
};
