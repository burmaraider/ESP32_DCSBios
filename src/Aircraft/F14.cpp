#include "Aircraft/F14.h"
#include "ESP32DCSBios.h"
#include "FastLED.h"

bool bDisplayPanelToggles[9];

#define nLightsConsoleTotal 64
uint8_t nLightConsoleIntensity = 0;
uint32_t cLightsConsoleColor = CRGB::OrangeRed;

CRGBArray<nLightsConsoleTotal> leds;

//ToggleSwitches for DISPLAY Panel
void onToggleVDI(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift)
{
    uint16_t data = ((messageDCS[1] << 8 | messageDCS[0]) & mask) >> shift;

    
    bDisplayPanelToggles[0] = data;
}

void onToggleHUD(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift)
{
    uint16_t data = ((messageDCS[1] << 8 | messageDCS[0]) & mask) >> shift;

    bDisplayPanelToggles[1] = data;
}

void onToggleHSD(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift)
{
    uint16_t data = ((messageDCS[1] << 8 | messageDCS[0]) & mask) >> shift;

    bDisplayPanelToggles[2] = data;

}

void onToggleHUDDeclutter(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift)
{
    uint16_t data = ((messageDCS[1] << 8 | messageDCS[0]) & mask) >> shift;

    bDisplayPanelToggles[3] = data;

}
void onToggleHUDModeAWL(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift)
{
    uint16_t data = ((messageDCS[1] << 8 | messageDCS[0]) & mask) >> shift;

    bDisplayPanelToggles[4] = data;

}

void onToggleVDIDisplayMode(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift)
{
    uint16_t data = ((messageDCS[1] << 8 | messageDCS[0]) & mask) >> shift;

    bDisplayPanelToggles[5] = data;

}

void onToggleVDILandingMode(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift)
{
    uint16_t data = ((messageDCS[1] << 8 | messageDCS[0]) & mask) >> shift;

    bDisplayPanelToggles[6] = data;

}

void onToggleHSDDisplayMode(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift)
{
    uint16_t data = ((messageDCS[1] << 8 | messageDCS[0]) & mask) >> shift;

    bDisplayPanelToggles[7] = data;

}

void onLHRPMChange(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift)
{
    uint16_t data = ((messageDCS[1] << 8 | messageDCS[0]) & mask) >> shift;

    Serial.printf("LHRPM: %d\n", data);

}

void onLightingConsoleIntensity(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift)
{
    uint16_t data = ((messageDCS[1] << 8 | messageDCS[0]) & mask) >> shift;

    nLightConsoleIntensity = data;
    Serial.printf("Lighting Console Intensity: %d\n", data);

}

F14::F14(ESP32DCSBios *dcs)
{
    this->_dcs = dcs;
    this->_dcs->SetModuleID(F_14B);

    //fill the vectors
    for(size_t i = 0; i < sizeof(F14_ActionsReceived) / sizeof(F14_ActionsReceived[0]); i++)
    {
        this->_ActionsReceived.push_back(F14_ActionsReceived[i]);
    }

    for(size_t i = 0; i < sizeof(F14_ActionsToDo) / sizeof(F14_ActionsToDo[0]); i++)
    {
        this->_ActionsToDo.push_back(F14_ActionsToDo[i]);
    }

}

F14::~F14()
{
    delete this->_dcs;
}

void F14::Init()
{

}

void F14::Update()
{

    this->UpdateConsoleLights();
}

void F14::UpdateConsoleLights()
{
    //Lighting Console
    if (nLightConsoleIntensity > 0)
    {
        for (int i = 0; i < nLightsConsoleTotal; i++)
        {
            leds[i] = cLightsConsoleColor;
        }

        FastLED.setBrightness(nLightConsoleIntensity << 5);
    }
    else
    {
        for (int i = 0; i < nLightsConsoleTotal; i++)
        {
            leds[i] = CRGB::Black;
        }
    }
    FastLED.show();
}

std::vector<std::string> F14::ActionsToDo()
{
    return this->_ActionsToDo;
}

std::vector<ActionReceived> F14::ActionsReceived()
{
    return this->_ActionsReceived;
}
