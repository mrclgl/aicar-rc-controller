#ifndef RC_CONTROLLER_TYPES
#define RC_CONTROLLER_TYPES

enum ModesOfOperation
{
    ConfirmSwitch = 0,
    Standby = 1,
    RC = 2,
    AI = 3
};

enum RGBLedColor
{
    Black   = 0b00000000,
    Red     = 0b00000001,
    Green   = 0b00000010,
    Yellow  = 0b00000011,
    Blue    = 0b00000100,
    Magenta = 0b00000101,
    Cyan    = 0b00000110,
    White   = 0b00000111,
};

#endif