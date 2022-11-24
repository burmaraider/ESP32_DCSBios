#pragma once

//****************************
// Global definitions
//****************************
#define _DEBUG


// Protocol definitions
#define DCSBIOS_STATE_WAIT_FOR_SYNC 0
#define DCSBIOS_STATE_ADDRESS_LOW 1
#define DCSBIOS_STATE_ADDRESS_HIGH 2
#define DCSBIOS_STATE_COUNT_LOW 3
#define DCSBIOS_STATE_COUNT_HIGH 4
#define DCSBIOS_STATE_DATA_LOW 5
#define DCSBIOS_STATE_DATA_HIGH 6

//****************************
// DCS BIOS TAGS
//****************************

// Common tags String
#define MODULE_NAME 0
#define DCS_BIOS_VERSION 0x0400
#define PILOT_NAME 0x0406
#define MISSION_START_TIME 0x0438

// A-4E-C tags
#define TACAN_CHANNEL_MAJOR 34064

// Structure that contains the data required to interpretate the data received
// from DCS-Bios. It also points to the callback function
typedef struct actionReceived_t
{
    uint16_t address;
    uint16_t mask;
    uint8_t shift;
    void (*callback)(char *messageDCS, uint32_t, uint32_t, unsigned char);
} ActionReceived;

typedef enum commandType_t
{
    CMD_TOGGLE,
    CMD_INC,
    CMD_DEC,
    CMD_SET,
} CommandType;