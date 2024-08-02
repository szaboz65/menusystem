#pragma once

#include "../taskman/TaskMan.h"

class CmdRenderer;

//------------------------------------------------------------
//Menu commands
class BatteryTask : public Task {
  public:
    BatteryTask(CmdRenderer *renderer = nullptr);
    void setrenderer(CmdRenderer *renderer);
    void enter();
    void exit();
    void render();
    float voltage() const {
      return _voltage;
    }

    void run() override;

  private:
    CmdRenderer *_renderer;
    float _voltage = 0.0;
    float _lastvoltage = -1.0;
    int _vref = 1100;

    void readAdc();
    void setVref();
};

extern class BatteryTask batterytask;
