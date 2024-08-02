#include "../my_settings.h"
#include "MenuDef.h"
#include "MenuCmds.h"


//**********************************************
// Menu Listeners for save Setting

class TextEditListener: public MenuListener {
    char *_dest;
  public:
    TextEditListener(FieldEdit &edit, char *dest) : MenuListener(edit), _dest(dest) {}
    void EventHandler(Events theevent) override {
      if(theevent == Events::Load) {
          static_cast<FieldEdit&>(component()).text(_dest);
      } else if(theevent == Events::Save) {
          strcpy(_dest, static_cast<FieldEdit&>(component()).text().c_str());
          Setting.save();
      }
    }
};

template<class T>
class IntEditListener: public MenuListener {
    T *_dest;
  public:
    IntEditListener(FieldIntEdit &edit, T *dest) : MenuListener(edit), _dest(dest) {}
    void EventHandler(Events theevent) override {
      if(theevent == Events::Load) {
          static_cast<FieldIntEdit&>(component()).value((int)(*_dest));
      } else if(theevent == Events::Save) {
          *_dest = (T)(static_cast<FieldIntEdit&>(component()).value());
          Setting.save();
      }
    }
};

template<class T>
class ListListener: public MenuListener {
    T *_dest;
  public:
    ListListener(FieldList &list, T *dest) : MenuListener(list), _dest(dest) {}
    void EventHandler(Events theevent) override {
      if(theevent == Events::Load) {
          static_cast<FieldList&>(component()).selected((int)(*_dest));
      } else if(theevent == Events::Save) {
          *_dest = (T)(static_cast<FieldList&>(component()).selected());
          Setting.save();
      }
    }
};

class LoopListener: public ListListener<char> {
  public:
	  LoopListener(FieldList &list, char *dest) : ListListener<char>(list,dest) {}
    void EventHandler(Events theevent) override {
      if(theevent == Events::Save) {
        int sel = static_cast<FieldList&>(component()).selected();
        MenuNav_SetLoop(sel!=0);
	  }
	  ListListener<char>::EventHandler(theevent);
	}
};

class BrightnessListener: public ListListener<char> {
  public:
	  BrightnessListener(FieldList &list, char *dest) : ListListener<char>(list,dest) {}
    void EventHandler(Events theevent) override {
      if(theevent == Events::Changed) {
        int val = GetBrightness(static_cast<FieldList&>(component()).selected());
        TFT_SetBrightness(val);
	  } else ListListener<char>::EventHandler(theevent);
	}
};

//-------------------------------------------------
// MenuListeners for actions
class MainScreenListener: public MenuListener {
    BatteryTask &_task;
  public:
    MainScreenListener(MenuItem &menu, BatteryTask &task) : MenuListener(menu), _task(task) {}
    void EventHandler(Events theevent) override {
      if(theevent == Events::Enter) {
		MenuNav_Suspend();
        _task.enter();
      } else if(theevent == Events::Exit) {
        _task.exit();
		MenuNav_Resume();
      }
    }
};

class LoginListener: public MenuListener {
  public:
    LoginListener(FieldEdit &edit) : MenuListener(edit) {}
    void EventHandler(Events theevent) override {
      if(theevent == Events::Load) {
          static_cast<FieldEdit&>(component()).text("");
      } else if(theevent == Events::Save) {
          if(strcmp(static_cast<FieldEdit&>(component()).text().c_str(), Setting.data.mpass) == 0)
			  set_AdminMenu();
      }
    }
};

class ExitListener: public MenuListener {
  public:
    ExitListener(MenuItem &menu) : MenuListener(menu) {}
    void EventHandler(Events theevent) override {
      if(theevent == Events::Enter) {
		set_MainMenu();
      }
    }
};

/*****************************************/
// MENU definition

Menu mainmenu("Main menu");
Menu aboutmenu("About...");

Menu adminmenu("Admin menu");
Menu aboutmenu2("About...");
Menu setmenu("Setting menu");
Menu wifimenu("Wifi Setting");
Menu hostmenu("Host Setting");
Menu hwmenu("HW Setting");
Menu menumenu("Menu Setting");

FocusableMenuItem mi_exit("Exit");
FocusableMenuItem mi_mscreen("Main screen");
MenuItem mi_model("Model " MODEL);
MenuItem mi_version("Version " VERSION_STR);
MenuItem mi_built("Built " BUILDDATE_STR);

FieldAlphaNumEdit mi_sn("S/N  ", 11);
FieldAlphaNumEdit mi_ssid("SSID ", 15);
FieldAlphaNumEdit mi_pass("Pass ", 15);
FieldUrlEdit mi_host("Host ", 31);
FieldIntEdit mi_param("Param", 1,999);

const char *list_onoff[] = {
  "Off",
  "On",
  nullptr
};

const char *list_bright[] = {
  "2 %",
  "4 %",
  "6 %",
  "8 %",
  "10 %",
  "20 %",
  "30 %",
  "40 %",
  "50 %",
  "60 %",
  "70 %",
  "80 %",
  "90 %",
  "100 %",
  nullptr
};


FieldList mi_loop("Menu Loop", list_onoff);
FieldList mi_bright("Brightness", list_bright);
FieldIntEdit mi_mpass("Password", 0,999999);
FieldIntEdit mi_login("Login", 0,999999);

MainScreenListener mscreen_list(mi_mscreen, batterytask);

TextEditListener sn_list(mi_sn, Setting.data.sn);
TextEditListener ssid_list(mi_ssid, Setting.data.ssid);
TextEditListener pass_list(mi_pass, Setting.data.pass);
TextEditListener host_list(mi_host, Setting.data.host);
IntEditListener<uint16_t> param_list(mi_param, &Setting.data.param);

LoopListener loop_list(mi_loop, &Setting.data.loop);
BrightnessListener bright_list(mi_bright, &Setting.data.brightness);
TextEditListener mpass_list(mi_mpass, Setting.data.mpass);
LoginListener login_list(mi_login);
ExitListener exit_list(mi_exit);

//**********************************************
// Menu functions

void Build_Menu() {
  mi_login.set_password(true);
	
  mainmenu.add_item(&mi_mscreen);
  mainmenu.add_menu(&aboutmenu);
  mainmenu.add_item(&mi_login);
  aboutmenu.add_item(&mi_model);
  aboutmenu.add_item(&mi_version);
  aboutmenu.add_item(&mi_built);


  adminmenu.add_menu(&setmenu);
  adminmenu.add_menu(&aboutmenu2);
  adminmenu.add_item(&mi_exit);

  aboutmenu2.add_item(&mi_model);
  aboutmenu2.add_item(&mi_version);
  aboutmenu2.add_item(&mi_built);
  
  setmenu.add_menu(&wifimenu);
  setmenu.add_menu(&hostmenu);
  setmenu.add_menu(&hwmenu);
  setmenu.add_menu(&menumenu);

  wifimenu.add_item(&mi_ssid);
  wifimenu.add_item(&mi_pass);

  hostmenu.add_item(&mi_host);
  
  hwmenu.add_item(&mi_sn);
  hwmenu.add_item(&mi_param);

  menumenu.add_item(&mi_bright);
  menumenu.add_item(&mi_loop);
  menumenu.add_item(&mi_mpass);

}

uint8_t GetBrightness(uint8_t selected) {
	size_t size = sizeof(list_bright)/sizeof(list_bright[0]);
	if(selected < size) {
		return atoi(list_bright[selected]);
	}
	return 0;
}