#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

//------------------------------------------------------------------------------
// enums
//------------------------------------------------------------------------------

// Two modes as defined in VNH2SP30 datasheet. 
typedef enum driverConfiguration {
    HALFBRIDGE,
    FULLBRIDGE
} DriverConfiguration;


// Four states as defined in VNH2SP30 datasheet.
// If configured as half bridge, the only available
// modes are CLOCKWISE and BRAKETOGND  
typedef enum motorState {
    CLOCKWISE,
    COUNTERCLOCKWISE,
    BRAKETOVCC,
    BRAKETOGND  // this will be the default case
} MotorState;


//------------------------------------------------------------------------------
// function declarations
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct MotorDriver MotorDriver;

void MotorDriver__init(DriverConfiguration mode);   // C constructor

// member functions
void MotorDriver__changeBridgeMode(DriverConfiguration mode);

bool MotorDriver__motorDriverIsFaulty();

unsigned int MotorDriver__checkMotorCurrentDraw();

void MotorDriver__turnOffMotor();

bool MotorDriver__turnOnMotor(MotorState state);

MotorState MotorDriver__getMotorState();

#ifdef __cplusplus
}
#endif


#endif // MOTORDRIVER_H
