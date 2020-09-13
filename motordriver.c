#include "motordriver.h"
#include "main.h"

//------------------------------------------------------------------------------
// C OOP Implementation
//------------------------------------------------------------------------------

// create a struct MotorDriver (Analog to C++ Class) 
// containing control pins for each motor driver chip
// on Sparkfun Monster Motor shield
typedef struct MotorDriver {
    DriverConfiguration mode; // HALFBRIDGE or FULLBRIDGE
    MotorState state;         // state of connected motor
} MotorDriver;

////////////////////////////////////////////////////////////////////////////////
MotorDriver md = {0};
// Function to create a MotorDriver instance
void MotorDriver__init(DriverConfiguration mode)    // C constructor
{
  // initialize with variables
  md.mode = mode;
  md.state = BRAKETOGND;

  if (mode == FULLBRIDGE) {
    LL_GPIO_ResetOutputPin(outB_GPIO_Port, outB_Pin);
  }
}

////////////////////////////////////////////////////////////////////////////////

// Function to change the driver configuration (HALFBRIDGE OR FULLBRIDGE)
void MotorDriver__changeBridgeMode(DriverConfiguration mode) {
  if (MotorDriver__motorDriverIsFaulty() &&
      md.mode != mode)   // if no fault, i.e. is true and
    // different mode specified
  {
    MotorState current_state = md.state;   // save current state
    MotorDriver__turnOffMotor();    // turn off motor
    md.mode = mode;    // update mode
    MotorDriver__turnOnMotor(current_state);
  }

  // otherwise, do nothing
}

////////////////////////////////////////////////////////////////////////////////

// Function to check motor driver status
// Returns: true if fault is detected (shutdown), false if none 
bool MotorDriver__motorDriverIsFaulty() {
  // en pin is already pulling HIGH in hardware
  // but if a fault causes shutdown,
  // this pin will output LOW

  return !(LL_GPIO_ReadInputPort(motorEN_DIAG_GPIO_Port)&motorEN_DIAG_Pin);
}

////////////////////////////////////////////////////////////////////////////////

// Function to check motor current draw
// Returns: current draw value mapped from 0 to 1024
//unsigned int MotorDriver__checkMotorCurrentDraw(MotorDriver *self) {
//  return HAL_ADC_GetValue(&hadc);;
//}

////////////////////////////////////////////////////////////////////////////////

// Function to turn off motor
// Returns: nothing
void MotorDriver__turnOffMotor() {
  LL_GPIO_ResetOutputPin(outA_GPIO_Port, outA_Pin);

  if (md.mode == FULLBRIDGE) {
    LL_GPIO_ResetOutputPin(outB_GPIO_Port, outB_Pin);
  }

//  analogWrite(self->pwm, 0);
  md.state = BRAKETOGND;
}

////////////////////////////////////////////////////////////////////////////////

// Function to make motor operate in any of four states (two if HALFBRIDGE MODE)
// If returns true, the chosen state is saved to instance
// Precondition: state < CLOCKWISE || state > BRAKETOGND
//               mode = HALFBRIDGE || FULLBRIDGE
// Parameter: state - one of four(two) states - see enum motorState above
// Returns: true if operation is successful, false if state is incompatible
bool MotorDriver__turnOnMotor(MotorState state) {
  if (md.mode == HALFBRIDGE) {
    if (state == CLOCKWISE) {
      LL_GPIO_SetOutputPin(outA_GPIO_Port, outA_Pin);
    } else if (state == BRAKETOGND) {
      LL_GPIO_ResetOutputPin(outA_GPIO_Port, outA_Pin);
    } else {
      return false;
    }
  } else // if mode == FULLBRIDGE
  {
    // set GPIO states
    (state == CLOCKWISE || state == BRAKETOVCC) ?
      LL_GPIO_SetOutputPin(outA_GPIO_Port, outA_Pin) :
      LL_GPIO_ResetOutputPin(outA_GPIO_Port, outA_Pin);

    (state == COUNTERCLOCKWISE || state == BRAKETOVCC) ?
      LL_GPIO_SetOutputPin(outB_GPIO_Port, outB_Pin) :
      LL_GPIO_ResetOutputPin(outB_GPIO_Port, outB_Pin);
  }

  md.state = state;
  return true;
}

////////////////////////////////////////////////////////////////////////////////

// Function to check operating state of Motor Driver
MotorState MotorDriver__getMotorState() {
  return md.state;
}

