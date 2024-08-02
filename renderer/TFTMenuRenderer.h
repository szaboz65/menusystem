/*
	TFT renderer for the menusystem
*/

#pragma once

#include "../menusystem/MenuRenderer.h"

class TFTMenuRenderer : public MenuRenderer {
	public:
		virtual void render_menu(Menu const& menu) const;
		virtual void render(Menu const& menu) const;
		virtual void render(MenuItem const& menu_item) const;

		virtual void render(FieldList const& list) const;
		virtual void render(FieldEdit const& edit) const;
		
		void begin();
		void brightness(uint8_t percent);
};
