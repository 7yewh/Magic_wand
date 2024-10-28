#include "CyberryPotter.h"

extern struct IMU_t IMU;
extern struct LED_t LED;
Cyberry_Potter_t Cyberry_Potter;
Module_t Module;

void Module_None_Mode0_Handler(void) {
    LED.Operate(BLINK_10HZ);
}

void Module_None_Mode1_Handler(void) {
    LED.Operate(BLINK_5HZ);
}

/// @brief Initialize required peripheral and function according to module type
void Module_Init(void) {
    typedef void (*InitFunc)();
    typedef void (*HandlerFunc)();

    InitFunc initFunctions[] = {
        NULL, // Module_Type_None
        Module0_Init, Module1_Init, Module2_Init, Module3_Init,
        Module4_Init, Module5_Init, Module6_Init, Module7_Init,
        Module8_Init, Module9_Init, Module10_Init
    };

    HandlerFunc mode0Handlers[] = {
        Module_None_Mode0_Handler,
        Module0_Mode0_Handler, Module1_Mode0_Handler, Module2_Mode0_Handler,
        Module3_Mode0_Handler, Module4_Mode0_Handler, Module5_Mode0_Handler,
        Module6_Mode0_Handler, Module7_Mode0_Handler, Module8_Mode0_Handler,
        Module9_Mode0_Handler, Module10_Mode0_Handler
    };

    HandlerFunc mode1Handlers[] = {
        Module_None_Mode1_Handler,
        Module0_Mode1_Handler, Module1_Mode1_Handler, Module2_Mode1_Handler,
        Module3_Mode1_Handler, Module4_Mode1_Handler, Module5_Mode1_Handler,
        Module6_Mode1_Handler, Module7_Mode1_Handler, Module8_Mode1_Handler,
        Module9_Mode1_Handler, Module10_Mode1_Handler
    };

    if (Module.Type < sizeof(initFunctions) / sizeof(initFunctions[0]) && initFunctions[Module.Type] != NULL) {
        initFunctions[Module.Type]();
        printf("Module %d Init\n", Module.Type);
        Module.Mode0_Handler = mode0Handlers[Module.Type];
        Module.Mode1_Handler = mode1Handlers[Module.Type];
    } else {
        Module.Mode0_Handler = &Module_None_Mode0_Handler;
        Module.Mode1_Handler = &Module_None_Mode1_Handler;
    }
}

/// @brief System initialization
void System_Init(void) {
    USART1_Init();
    LED_Init();
    Button_Init();
    IMU_Init();
    Module.Type = Module_Type_0;
    printf("Module_Type_0\n");
    Module_Init();
}

void Cyberry_Potter_System_Status_Update(void) {
    switch (Cyberry_Potter.System_Mode) {
        case SYSTEM_MODE_0:
            Cyberry_Potter.System_Mode = SYSTEM_MODE_1;
            #ifdef SERIAL_DEBUG
            printf("SYSTEM_MODE_1\n");
            #endif
            break;
        case SYSTEM_MODE_1:
            #ifdef LASER_ENABLE
            Cyberry_Potter.System_Mode = SYSTEM_MODE_2;
            #else
            Cyberry_Potter.System_Mode = SYSTEM_MODE_0;
            #endif
            #ifdef SERIAL_DEBUG
            printf("SYSTEM_MODE_0\n");
            #endif
            break;
        #ifdef LASER_ENABLE
        case SYSTEM_MODE_2:
            Cyberry_Potter.System_Mode = SYSTEM_MODE_0;
            #ifdef SERIAL_DEBUG
            printf("SYSTEM_MODE_0\n");
            #endif
            break;
        #endif
    }
}

/// @brief IMU and IR RF module interrupt handler
void EXTI9_5_IRQHandler(void) {
    static uint8_t i = 0;
    if (EXTI_GetITStatus(EXTI_Line5) == SET) {
        IMU_Get_Data(i);
        if (++i >= IMU_SEQUENCE_LENGTH_MAX) {
            i = 0;
            IMU.Sample_Stop();
            #ifdef SYSTEM_MODE_DATA_COLLECT
            Delay_ms(200);
            IMU_Data_Print();
            #endif
        }
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
}

void EXTI_Stop(void) {
    EXTI->IMR &= ~(EXTI_Line0);
}

void EXTI_Restore(void) {
    EXTI->IMR |= EXTI_Line0;
}
