#include "CyberryPotter.h"
#include "weights.h"
#include "nnom.h"

#define QUANTIFICATION_SCALE (pow(2, INPUT_1_OUTPUT_DEC))
#define OUTPUT_THRESHOLD 63

void model_feed_data(void);
Model_Output_t model_get_output(void);

#ifdef NNOM_USING_STATIC_MEMORY
    uint8_t static_buf[1024 * 10];
#endif 

nnom_model_t* model;
volatile Model_Output_t model_output = -1;

int main(void) {
    System_Init();
    LED.Operate(BLINK_10HZ);

    #ifdef NNOM_USING_STATIC_MEMORY
        nnom_set_static_buf(static_buf, sizeof(static_buf)); 
    #endif 

    model = nnom_model_create();

    while (1) {
        if (Button.status == BUTTON_HOLD && IMU.status == IMU_Idle) {
            IMU.Sample_Start();
            EXTI_Stop();
            LED.Operate(OFF);

            while (IMU.status != IMU_Sampled);
            LED.Operate(ON);

            #ifndef SYSTEM_MODE_DATA_COLLECT
                model_output = model_get_output();
                
                if (model_output != Unrecognized) {
                    switch (Cyberry_Potter.System_Mode) {
                        case SYSTEM_MODE_0:
                            Module.Mode0_Handler();
                            break;
                        case SYSTEM_MODE_1:
                            Module.Mode1_Handler();
                            break;
                        default:
                            break;
                    }
                }
            #endif

            IMU.status = IMU_Idle;
            Button.status_clear();
            EXTI_Restore();
        } else if (Button.status == BUTTON_HOLD_LONG) {
            printf("BUTTON_HOLD_LONG\n");
            LED.Operate(BLINK_5HZ);
            Cyberry_Potter_System_Status_Update();
            Button.status_clear();
        }
    }
}

void model_feed_data(void) {
    const double scale = QUANTIFICATION_SCALE;
    
    for (uint16_t i = 0; i < IMU_SEQUENCE_LENGTH_MAX; i++) {
        nnom_input_data[i * 3]     = (int8_t)round(IMU.gyro[i][Roll] * scale);
        nnom_input_data[i * 3 + 1] = (int8_t)round(IMU.gyro[i][Pitch] * scale);
        nnom_input_data[i * 3 + 2] = (int8_t)round(IMU.gyro[i][Yaw] * scale);
    }
}

Model_Output_t model_get_output(void) {
    model_feed_data();
    model_run(model);

    int8_t max_output = -128;
    Model_Output_t ret = Unrecognized;

    for (uint8_t i = 0; i < 13; i++) {
        #ifdef SERIAL_DEBUG
            printf("Output[%d] = %.2f %%\n", i, (nnom_output_data[i] / 127.0) * 100);
        #endif

        if (nnom_output_data[i] > max_output) {
            max_output = nnom_output_data[i];
            ret = i;
        }
    }

    if (max_output < OUTPUT_THRESHOLD) {
        ret = Unrecognized;
    }

    #ifdef SERIAL_DEBUG
    const char* gesture_names[] = {
        "Unrecognized", "RightAngle", "SharpAngle", "Lightning", "Triangle", 
        "Letter_h", "Letter_R", "Letter_W", "Letter_phi", "Circle", 
        "UpAndDown", "Horn", "Wave", "NoMotion"
    };
    printf("%s\n", gesture_names[ret]);
    #endif

    return ret;
}
