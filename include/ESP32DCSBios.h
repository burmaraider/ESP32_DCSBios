#pragma once
#include <Arduino.h>
#include "DCSModuleAddresses.h"
#include "DCSTypes.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <vector>

#include "Aircraft/Aircraft.h"

const IPAddress ipmulti(239, 255, 50, 10);
const uint32_t multi_port = 5010;
const uint32_t dcs_port = 7778;

// Mainly based on the original DCS-Bios
// https://github.com/dcs-bios

// Based on ESP32_MultiModuleDCSBios by pavidovich
// https://github.com/pavidovich/ESP32_MultiModuleDCSBios

//***Additions/Changes by @burmaraider***
// - Somewhat of a rewrite of ESP32_MultiModuleDCSBios
// - Added a basic command builder to make it easier to send commands to DCS-Bios
//   CommandTypes now include "TOGGLE", "SET", "INC", "DEC"
// - More functionality self contained inside of MultiDCSBios
// - More modularity for adding new planes, _aircraft now holds a single instance of the aircraft class
//   This will allow you to add more aircraft classes and switch between them
// - Renamed "include.h" to "DCSTypes.h"
// - Renamed "Module_Address.h" to "DCSModuleAddresses.h"
// - WiFi Connection is now handled by MultiDCSBios

class ESP32DCSBios
{
private:
    int32_t _nModuleId;
    int32_t _nLastModuleId;
    bool _bModuleChanged;

    char _szMessageDCS[512];

    bool _bInitialBoot; // This gets set as soon as we get a response from DCS-Bios
    bool _bSimStarted;

    uint32_t _nLastUpdateReceivedSeconds; // Todo: Implement timeout for DCS-Bios
    uint32_t _nUpdateTimeoutSeconds; // Todo: Implement timeout for DCS-Bios

    Aircraft *_aircraft;
    WiFiUDP *_udp;

public:
    ESP32DCSBios()
    {
        this->_nModuleId = NO_MODULE;
        this->_nLastModuleId = NO_MODULE;
        this->_bModuleChanged = false;
        memset(this->_szMessageDCS, 0x00, sizeof(this->_szMessageDCS));

        this->_nLastUpdateReceivedSeconds = 0;
        this->_nUpdateTimeoutSeconds = 30;
    }

    /// @brief Sets up MultiDCSBios with the given SSID and Password
    /// @param ssid SSID of the WiFi network
    /// @param password Password of the WiFi network
    ESP32DCSBios(char *ssid, char *password)
    {
        this->_nModuleId = NO_MODULE;
        this->_nLastModuleId = NO_MODULE;
        this->_bModuleChanged = false;
        memset(this->_szMessageDCS, 0x00, sizeof(this->_szMessageDCS));

        this->_nLastUpdateReceivedSeconds = 0;
        this->_nUpdateTimeoutSeconds = 30;

        this->_udp = new WiFiUDP();

        // setup Wifi
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED)
        {
#ifdef _DEBUG
            Serial.print(".");
#endif
            delay(250);
        }
#ifdef _DEBUG
        Serial.print("LAN OK - IP:");
        Serial.println(WiFi.localIP());
#endif

        // multicast listener
        this->_udp->beginMulticast(ipmulti, multi_port);
    }
    
    ~ESP32DCSBios()
    {
        delete this->_udp;
        delete this->_aircraft;
    }

    void SetAirCraft(Aircraft *aircraft)
    {
        this->_aircraft = aircraft;
    }

    Aircraft *GetAirCraft()
    {
        return this->_aircraft;
    }

    /// @brief This is the main MultiDCS loop that should be called in the main loop()
    void Update()
    {
        char dataRead;

        if (this->_udp->parsePacket())
        {
            while (this->_udp->available())
            {
                dataRead = (char)this->_udp->read();
                this->ProcessChar(dataRead);
            }
        }

        this->_aircraft->Update();
    }

    /// @brief This will send a command to DCS-Bios
    /// @param cmd command types are "TOGGLE", "SET", "INC", "DEC"
    /// @param nAction action number
    /// @param set set value to send to DCS-Bios
    /// @return
    bool SendCommand(CommandType cmd, uint32_t nAction, uint8_t set = 0)
    {
        bool bSetCommand = false;
        static char szCommandType[8];
        memset(szCommandType, 0, 8);

        if (cmd == CMD_TOGGLE)
            strcpy(szCommandType, "TOGGLE\n");

        else if (cmd == CMD_INC)
            strcpy(szCommandType, "INC\n");

        else if (cmd == CMD_DEC)
            strcpy(szCommandType, "DEC\n");

        else if (cmd == CMD_SET)
            bSetCommand = true;
        else
            return false; // No valid command, return

        static char szCommand[1024];
        memset(szCommand, 0, 1024);

        // Sanity Check and bail out.
        if (_udp->beginPacket(_udp->remoteIP(), dcs_port) == 0)
            return false;

        if (bSetCommand)
            snprintf(szCommand, 1024, "%s %d\n", _aircraft->ActionsToDo()[nAction].c_str(), set);
        else
            snprintf(szCommand, 1024, "%s %s\n", _aircraft->ActionsToDo()[nAction].c_str(), szCommandType);

        this->_udp->write((const uint8_t *)szCommand, strlen(szCommand));

#ifdef _DEBUG
        Serial.printf("Sending: %s\n", szCommand);
#endif

        if (this->_udp->endPacket())
            return true;

        return false;
    }

    /// @brief Has the simulation started?
    /// @return true if simulation has started
    bool HasSimStarted()
    {
        return this->_bSimStarted;
    }

    /// @brief Has the module recieved a response from DCS-Bios?
    /// @return true if module has recieved a response from DCS-Bios
    bool HasBooted()
    {
        return this->_bInitialBoot;
    }

    /// @brief Set's the boot flag to true or false.
    /// @param bBooted
    void SetHasBooted(bool bBooted = true)
    {
        this->_bInitialBoot = bBooted;
    }

    unsigned int GetModuleId()
    {
        return this->_nModuleId;
    }

    void SetModuleID(unsigned int nModuleId)
    {
        this->_nModuleId = nModuleId;
    }

    bool ModuleChanged()
    {
        // If changed the flag is reset when asked about it
        if (this->_bModuleChanged)
        {
            this->_bModuleChanged = false;
            return true;
        }
        return false;
    }

    void SetMessage(char *dataIn)
    {
        memset(this->_szMessageDCS, 0x00, sizeof(this->_szMessageDCS));
        strncpy(this->_szMessageDCS, dataIn, sizeof(this->_szMessageDCS));
    }

    void ProcessChar(unsigned char c)
    {
        static uint32_t state = DCSBIOS_STATE_WAIT_FOR_SYNC;
        static uint32_t sync_byte_count = 0;
        static uint32_t len = 0;
        static uint32_t count = 0;
        static uint32_t address = 0;

        switch (state)
        {
        case DCSBIOS_STATE_WAIT_FOR_SYNC:
            break;

        case DCSBIOS_STATE_ADDRESS_LOW:
            address = (uint32_t)c;
            state = DCSBIOS_STATE_ADDRESS_HIGH;
            break;

        case DCSBIOS_STATE_ADDRESS_HIGH:
            address = (c << 8) | address;
            if (address != 0x5555)
            {
                state = DCSBIOS_STATE_COUNT_LOW;
            }
            else
            {
                state = DCSBIOS_STATE_WAIT_FOR_SYNC;
                address = 0;
                len = 0;
            }
            break;

        case DCSBIOS_STATE_COUNT_LOW:
            count = (uint32_t)c;
            state = DCSBIOS_STATE_COUNT_HIGH;
            break;

        case DCSBIOS_STATE_COUNT_HIGH:
            count = (c << 8) | count;
            state = DCSBIOS_STATE_DATA_LOW;
            break;

        case DCSBIOS_STATE_DATA_LOW:
            // data = (unsigned int)c;
            count--;
            this->_szMessageDCS[len] = c;
            len++;
            state = DCSBIOS_STATE_DATA_HIGH;
            break;

        case DCSBIOS_STATE_DATA_HIGH:
            // data = (c << 8) | data;
            count--;
            this->_szMessageDCS[len] = c;
            len++;

            if (count == 0)
            {
                this->ProcessData(address, len);
                len = 0;
                address = 0;
                state = DCSBIOS_STATE_ADDRESS_LOW;
            }
            else
            {
                state = DCSBIOS_STATE_DATA_LOW;
            }
            break;
        } // END switch(state)

        if (c == 0x55)
            sync_byte_count++;
        else
            sync_byte_count = 0;

        if (sync_byte_count == 4)
        {
            state = DCSBIOS_STATE_ADDRESS_LOW; // Start processing data
            sync_byte_count = 0;
            address = 0;
            len = 0;
        }
    }

    void ProcessData(uint32_t address, uint32_t len)
    {
        std::map<std::string, uint32_t>::const_iterator it;

        switch (address)
        {
        case MODULE_NAME:

            it = ModuleList.find(_szMessageDCS);
            if (it != ModuleList.end())
            {
                this->_nModuleId = it->second;
            }

            if (_nModuleId != _nLastModuleId)
            {
                _bModuleChanged = true;
                _nLastModuleId = _nModuleId;
            }

            break;
        // Some special commands to take into consideration
        case DCS_BIOS_VERSION:
            // _dcsBiosVersion = atoi(_messageDCS);
            break;
        case PILOT_NAME:
            // strncpy(_pilotName, _messageDCS, sizeof(_pilotName));
            break;
        case MISSION_START_TIME:
            // packet received update the last update time
            break;
        default:
            this->ExecuteAircraftAction(address, len);
            break;
        }
    }

    void ExecuteAircraftAction(uint32_t address, uint32_t len)
    {
        // setup intial boot to send startup commands
        if (!this->_bInitialBoot && !this->_bSimStarted)
        {
            this->_bInitialBoot = true;
            this->_bSimStarted = true;
        }

        if (this->_nModuleId != NO_MODULE)
        {
            for (size_t i = 0; i < this->_aircraft->ActionsReceived().size(); i++)
            {
                if (this->_aircraft->ActionsReceived()[i].address == address)
                {
                    this->_aircraft->ActionsReceived()[i].callback(this->_szMessageDCS,
                                                                   len,
                                                                   this->_aircraft->ActionsReceived()[i].mask,
                                                                   this->_aircraft->ActionsReceived()[i].shift);
                }
            }
        }
    }
};