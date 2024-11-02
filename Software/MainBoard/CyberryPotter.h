#include "config.h"

//********************************************************************************
#ifndef _CYBERRY_POTTER_H_
#define _CYBERRY_POTTER_H_

#include "Delay.h"
#include "IMU.h"
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include "button.h"
#include "LED.h"
#include "USART.h"

// Include module headers
#include "module0.h"
#include "module1.h"
#include "module2.h"
#include "module3.h"
#include "module4.h"
#include "module5.h"
#include "module6.h"
#include "module7.h"
#include "module8.h"
#include "module9.h"
#include "module10.h"

// System mode enumeration
typedef enum eSystem_Mode {
    SYSTEM_MODE_0 = 0,
    SYSTEM_MODE_1 = 1,
    #ifdef LASER_ENABLE
    SYSTEM_MODE_2 = 2
    #endif
} eSystem_Mode;  

// Module type enumeration
typedef enum eModule_Type {
    Module_Type_None = -1,
    Module_Type_0,
    Module_Type_1,
    Module_Type_2,
    Module_Type_3,
    Module_Type_4,
    Module_Type_5,
    Module_Type_6,
    Module_Type_7,
    Module_Type_8,
    Module_Type_9,
    Module_Type_10
} eModule_Type;

// Model output enumeration
typedef enum eModel_Output {
    Unrecognized = -1,
    RightAngle,
    SharpAngle,
    Lightning,
    Triangle,
    Letter_h,
    letter_R,
    letter_W,
    letter_phi,
    Circle,
    UpAndDown,
    Horn,
    Wave,
    NoMotion
} eModel_Output;

// Cyberry Potter structure
typedef struct Cyberry_Potter_t {
    eSystem_Mode System_Mode;
    void (*System_Handler)(void);
} Cyberry_Potter_t;

// Module structure
typedef struct Module_t {
    volatile eModule_Type Type;
    void (*Mode0_Handler)(void);
    void (*Mode1_Handler)(void);
} Module_t;

typedef int8_t Model_Output_t;

// External variable declarations
extern struct Module_t Module;
extern struct Cyberry_Potter_t Cyberry_Potter;

// Function declarations
void System_Init(void);
void Cyberry_Potter_System_Status_Update(void);
void EXTI_Stop(void);
void EXTI_Restore(void);

#ifdef Signal_DEBUG
void Signal_Data_Reset(void);
void Signal_Print(void);
#endif // Signal_DEBUG

#endif // _CYBERRY_POTTER_H_
