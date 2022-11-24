#include <Arduino.h>

#include "ESP32DCSBios.h"
#include "Aircraft/F14.h"

#include "CD4021BButtonInput.h"

const char *ssid = "SSID";
const char *password = "PASSWORD";

ESP32DCSBios _dcs;

void setup()
{
    Serial.begin(115200);

#ifdef _DEBUG
    Serial.println("Starting");
#endif

    memset(bDisplayPanelToggles, 0, sizeof(bDisplayPanelToggles));

    ConfigureButtonPins();
    ReadButtons();

    // Set up the DCSBIOS interface
    _dcs = ESP32DCSBios((char *)ssid, (char *)password);


#ifdef _DEBUG
    Serial.println("Setup done");
#endif

    F14 *f14 = new F14(&_dcs);
    f14->Init();
    _dcs.SetAirCraft(f14);
    
}

void loop()
{

    ReadButtons();

    _dcs.Update();

    // Reset all switches in the cockpit to match our switch positions in the real world.
    if (_dcs.HasBooted())
    {
        for (int i = 0; i < 8; i++)
        {
            bDisplayPanelToggles[i] = dip & (1 << i);
            // send the startup commands - todo: batch statup commands
            _dcs.SendCommand(CMD_SET, i, dip & (1 << i));
            delay(20);
        }
        _dcs.SetHasBooted(false);
    }

    // Loop through our dip switches and send commands if they have changed
    for (size_t i = 0; i < 8; i++)
    {
        if (dip & (1 << i))
        {
            if (!bDisplayPanelToggles[i])
            {
                bDisplayPanelToggles[i] = true;
                _dcs.SendCommand(CMD_SET, i, bDisplayPanelToggles[i] + (i == 7 ? 1 : 0));
            }
        }
        else
        {
            if (bDisplayPanelToggles[i])
            {
                bDisplayPanelToggles[i] = 0;
                _dcs.SendCommand(CMD_SET, i, bDisplayPanelToggles[i] + (i == 7 ? 1 : 0));
            }
        }
    }

    if (buttons != lastState)
    {
        // check if the button is pressed and was not pressed before
        if (buttons & 0x1)
        {
        }

        if (buttons & 0x2)
        {

            _dcs.SendCommand(CMD_TOGGLE, 1);
        }
        // save the current state
        lastState = buttons;
    }

    delay(1);
}