#pragma once

#ifdef MAIN
extern Menu mainmenu;
extern Menu adminmenu;
#endif

//Build menusystem
void Build_Menu();

//control menus
void set_MainMenu();
void set_AdminMenu();

//control menunav
void MenuNav_Suspend();
void MenuNav_Resume();
void MenuNav_SetLoop(bool l);

//control commands
uint8_t GetBrightness(uint8_t selected);
void TFT_SetBrightness(uint8_t percent);