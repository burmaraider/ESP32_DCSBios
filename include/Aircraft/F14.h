#pragma once
#include "Arduino.h"
#include "DCSTypes.h"
#include "DCSModuleAddresses.h"
#include "vector"

#include "Aircraft.h"
#include "../ESP32DCSBios.h"

extern bool bDisplayPanelToggles[9];

// ToggleSwitches for DISPLAY Panel
void onToggleVDI(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift);
void onToggleHUD(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift);
void onToggleHSD(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift);
void onToggleHUDDeclutter(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift);
void onToggleHUDModeAWL(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift);
void onToggleVDIDisplayMode(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift);
void onToggleVDILandingMode(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift);
void onToggleHSDDisplayMode(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift);

// Lighting
void onLightingConsoleIntensity(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift);

// F14 Engine RPM
void onLHRPMChange(char *messageDCS, uint32_t len, uint32_t mask, unsigned char shift);

ActionReceived const F14_ActionsReceived[] = {
    {0x121c, 0x8000, 15, onToggleVDI},                // VDI POWER
    {0x1222, 0x0001, 0, onToggleHUD},                 // HUD POWER
    {0x1222, 0x0002, 1, onToggleHSD},                 // HSD POWER
    {0x1272, 0x4000, 14, onToggleHUDDeclutter},       // HUD DeClutter
    {0x1272, 0x8000, 15, onToggleHUDModeAWL},         // HUD AWL
    {0x1272, 0x1000, 12, onToggleVDIDisplayMode},     // VDI DISP
    {0x1272, 0x2000, 13, onToggleVDILandingMode},     // VDI LANDING MODE DISPLAY
    {0x1222, 0x0180, 7, onToggleHSDDisplayMode},      // HSD DISPLAY MODE
    {0x121e, 0xf000, 12, onLightingConsoleIntensity}, // CONSOLE LIGHTING INTENSITY
    {0x130a, 0xffff, 0, onLHRPMChange}                // LH Engine RPM

};

const char *const F14_ActionsToDo[]{
    "PLT_VDI_PW_SW",     // VDI POWER
    "PLT_HUD_PW_SW",     // HUD POWER
    "PLT_HSD_PW_SW",     // HSD POWER
    "PLT_HUD_DECLUTTER", // HUD DECLUTTER
    "PLT_HUD_MODE_AWL",  // HUD MODE AWL
    "PLT_VDI_MODE_DISP", // VDI MODE DISP
    "PLT_VDI_MODE_LAND", // VDI MODE LAND
    "PLT_HSD_DIS_MODE",  // HSD DIS MODE
};

class F14 : public Aircraft
{
private:
    ESP32DCSBios *_dcs;

public:
    F14(ESP32DCSBios *dcs);
    ~F14();

    void Init();
    void Update();
    void UpdateConsoleLights();

    std::vector<std::string> ActionsToDo();
    std::vector<ActionReceived> ActionsReceived();

    std::vector<ActionReceived> _ActionsReceived;
    std::vector<std::string> _ActionsToDo;
};