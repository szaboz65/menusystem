#define MAIN
#include "my_config.h" 
#include "my_settings.h" 

#include "btnctrl/btnctrl.cpp"
#include "taskman/TaskMan.cpp"
#include "menusystem/MenuSystem.cpp"
#include "menusystem/MenuFields.cpp"
#include "renderer/TFTMenuRenderer.cpp"
#include "menunav/MenuNav.cpp"
#include "menunav/MenuNavTask.cpp"
#include "menudef/MenuDef.cpp"
#include "menudef/MenuCmds.cpp"

/****************************************/
//eeprom data
#define EEPROM_SIZE 256
Settings<WFMSettings> Setting;


/****************************************/
//renderers
TFTMenuRenderer menu_renderer;
TFTCmdRenderer cmd_renderer;

//menusystem
MenuSystem main_menusystem(&mainmenu);
MenuSystem admin_menusystem(&adminmenu);

//control
MenuNav menunav;
MenuNavTask menunavtask;

//menu tasks
BatteryTask batterytask;

//****************************************
//global menu navigator functions
void MenuNav_Suspend() {
  menunav.end();
}
void MenuNav_Resume() {
  menunav.begin();
}
void MenuNav_SetLoop(bool l) {
  menunav.loop(l);
}

void set_MainMenu() {
	main_menusystem.reset();
	menunav.setMenuSystem(&main_menusystem);
}
void set_AdminMenu() {
	admin_menusystem.reset();
	menunav.setMenuSystem(&admin_menusystem);
}

void TFT_SetBrightness(uint8_t percent) {
	menu_renderer.brightness(percent);
}

//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(10);

  //device info
  Serial.println();
  Serial.print("Model:"); Serial.println(MODEL);
  Serial.println();
  Serial.print("Version:"); Serial.println(VERSION);
  Serial.print("Build:"); Serial.println(BUILDDATE_STR);
  Serial.println();

  //EEPROM storage init
  EEPROM.begin(EEPROM_SIZE);
  if ( !Setting.begin())
  {
    Serial.println("EEPROM error");
    return;
  }
  Setting.data.dump();
  
  taskman.begin(8,millis());

  menu_renderer.begin();
  int val = GetBrightness(Setting.data.brightness);
  TFT_SetBrightness(val ? val : 100);

  Build_Menu();
  batterytask.setrenderer(&cmd_renderer);
  main_menusystem.setrenderer(&menu_renderer);
  admin_menusystem.setrenderer(&menu_renderer);
  
  set_MainMenu();
  MenuNav_SetLoop(Setting.data.loop);
  
  menunavtask.setmenunav(&menunav);
  menunavtask.begin();	
  
  //select mainscreen
//  menunav.doAction(MenuNav::Actions::SELECT);
}

void loop() {
  uint32_t now = millis();
  taskman.process(now);
}
