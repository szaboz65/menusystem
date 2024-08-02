/*
	TFT renderer for the menusystem
*/
#include "../my_config.h"
#include "Arduino.h"
#include "TFTMenuRenderer.h"
#include "TFTCmdRenderer.h"

#include <SPI.h>

#include <TFT_eSPI.h>       // Hardware-specific library

#define COLOR_MAIN  TFT_LIGHTGREY
#define COLOR_BASIC TFT_DARKCYAN
#define COLOR_CURRENT TFT_GREENYELLOW
#define COLOR_ADJUST TFT_WHITE

#define COLOR_MAIN_BG  TFT_BLACK
#define COLOR_BASIC_BG TFT_BLACK
#define COLOR_CURRENT_BG TFT_NAVY

#define MODE_MENU 1
#define MODE_BAT  2

//control tft
// the number of the BACKLIGHT pin
const int ledPin = TFT_BL;

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library

const char *_lasthead = nullptr;
void printHead(const char *str) {
  if( _lasthead != str) {
    _lasthead = str;
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(COLOR_MAIN, COLOR_MAIN_BG);
    tft.setCursor(0, 0, 4);
    tft.println(str);
    tft.drawFastHLine(0, tft.fontHeight() - 1, TFT_HEIGHT, COLOR_MAIN);
  } else {
    int h = tft.fontHeight();
    tft.fillRect(0, h, tft.width(), tft.height()-h, TFT_BLACK);
    tft.setCursor(0, h, 4);  
  }
}

void TFTMenuRenderer::begin() {
  _lasthead = nullptr;

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  
  // configure LED PWM functionalities
  pinMode(ledPin, OUTPUT);
}

void TFTMenuRenderer::brightness(uint8_t percent) {
	int dutyCycle = percent % 100;
	dutyCycle = 255*percent/100;
	analogWrite(ledPin, dutyCycle);
}

void TFTMenuRenderer::render_menu(Menu const& menu) const {
  printHead(menu.name());
  
  for (int i = 0; i < menu.get_num_components(); ++i) {
    MenuComponent const* cp_m_comp = menu.get_menu_component(i);
    if (cp_m_comp->is_current())
      tft.setTextColor(COLOR_CURRENT, COLOR_CURRENT_BG);
    else
      tft.setTextColor(COLOR_BASIC, COLOR_MAIN_BG);

    cp_m_comp->render(*this);
    tft.println("");
  }
}

void TFTMenuRenderer::render(Menu const& menu) const {
  tft.print(menu.name());
}

void TFTMenuRenderer::render(MenuItem const& menu_item) const {
  tft.print(menu_item.name());
}

void TFTMenuRenderer::render(FieldList const& list) const {
  tft.print(list.name());

  String s;
  if (list.has_focus()) {
    tft.setTextColor(COLOR_ADJUST, COLOR_CURRENT_BG);
    s += '<';
  }
  else
    s += ' ';

  const char *v = list.value();
  if (v)
    s += v;
  else
    s += "---";

  if (list.has_focus())
    s += '>';
  else
    s += ' ';

  tft.print(s);
}

void TFTMenuRenderer::render(FieldEdit const& edit) const {
  if (edit.is_current())
    tft.setTextColor(COLOR_CURRENT, COLOR_CURRENT_BG);
  else
    tft.setTextColor(COLOR_BASIC, COLOR_MAIN_BG);
  tft.print(edit.name());

  String text = edit.text();
  int length = edit.length();
  int index = edit.current_index();

  String s(' ');

  if (edit.has_focus()) {
    s += '[';
    if ( index > 0) {
	  if (edit.is_password()) {
		for( int i= 0; i++ < index; s += '*');
	  } 
	  else
		s += text.substring(0, index);
	}
    tft.print(s);

    tft.setTextColor(COLOR_ADJUST, COLOR_CURRENT_BG);
    tft.print(text[index]);

    tft.setTextColor(COLOR_CURRENT, COLOR_CURRENT_BG);
	if (edit.is_password()) {
		s="";
		for( int i= index+1; i++ < text.length(); s += '*');
	} 
	else
		s = text.substring(index + 1);
    length -= text.length();
    while (length--) s += ' ';
    s += ']';
  }
  else {
    s += ' ';
    s += text;
  }

  tft.print(s);
}


void TFTCmdRenderer::render(BatteryTask const& task) const {
  printHead("Battery level");

  String voltage(task.voltage());
  voltage += 'V';
  tft.setTextDatum(MC_DATUM);
  tft.drawString(voltage,  tft.width() / 2, tft.height() / 2 );
}
