/*
	CommandRenderer for the menu commands based on the visitor pattern
*/

#pragma once

#include "MenuCmds.h"


// *********************************************************
// CmdRenderer
// *********************************************************
//! \brief Abstract base class that renders the component in the menu
//! https://en.wikipedia.org/wiki/Visitor_pattern
//!
//! This is the `accept` method in the visitor design pattern. It should
//! call the appropriate method on the given MenuRenderer for rendering 
//! this type of component. This method should be called from the
//! MenuComponent::render method.
//!
//! \param[in] renderer The CmdRenderer to use to render this component
//!
//! \see CmdRenderer
class CmdRenderer {
public:
    //! battery command
    virtual void render(BatteryTask const& task) const = 0;
};
