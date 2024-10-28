#ifndef _CONFIG_H_
#define _CONFIG_H_

// This file contains compile-time configurations for Cyberry Potter magical wand internal system.


#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define running mode.
// NOTE: You can change the wand into different mode by the following defines.
//      When you change into another mode,you should comment the define of previous mode. 
// CYBERRY_POTTER_MODE_NORMAL: Normal mode.
// CYBERRY_POTTER_MODE_DATA_COLLECTOR: Running at data collecting mode to collect 
//      data from IMU and then send those data to serial port.Those data are used
//      to training motion identify model.

//Define system configuration
#define SY STEM_FREQUENCY (72000000)
#define SERIAL_DEBUG
//#define SYSTEM_MODE_DATA_COLLECT

//Define CNN
//The out put of model must bigger than this value unless it will give a unrecognized message

#endif  //_CONFIG_H_
