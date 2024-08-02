#pragma once

#include "../menudef/CmdRenderer.h"

class TFTCmdRenderer : public CmdRenderer {
	
    void render(BatteryTask const& task) const override;
	
};