#include "module0.h"
#include "new_study_IR.h"
#include "protocol.h"

extern volatile Model_Output_t model_output;
uint16_t bufLen;
uint8_t buf[128], i;

// ����ģ������
const char* model_names[] = {
    "RightAngle", "SharpAngle", "Lightning", "Triangle", 
    "Letter_h", "Letter_R", "Letter_W", "Letter_phi", 
    "Circle", "UpAndDown", "Horn", "Wave"
};

// ����ģ�Ͷ�Ӧ������ֵ
const uint8_t model_indices[] = {
    0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 1, 1
};

// ��ӡģ������
void print_model_name(Model_Output_t model) {
    if (model < sizeof(model_names) / sizeof(model_names[0])) {
        printf("%s", model_names[model]);
    } else {
        printf("Unrecognized Model");
    }
}

// ���ģ�Ͳ����ؽ��
Model_Output_t check_model(uint8_t* model_index) {
    Model_Output_t ret = model_output;
    
    if (ret < sizeof(model_indices) / sizeof(model_indices[0])) {
        *model_index = model_indices[ret];
    } else {
        ret = Unrecognized;
    }
    
    print_model_name(ret);
    return ret;
}

void handle_ir_signal(uint8_t mode) {
    uint8_t model_index;
    
    if (check_model(&model_index) != Unrecognized) {
				#ifdef SERIAL_DEBUG
				printf(" * %d * ", model_index);
				#endif //SERIAL_DEBUG
		bufLen = (mode == 0)?IR_Send_Pack(buf,model_index):IR_Learn_Pack(buf,model_index);
        Uart_Send(buf, bufLen);
        LED.Operate(BLINK_5HZ);
    } else {
        LED.Operate(BLINK_2HZ);
    }
}

void Module0_Init(void) {
    USART2_Config();
}

// ģʽ0�������ⷢ�䣩
void Module0_Mode0_Handler(void) {
    handle_ir_signal(0);
}

// ģʽ1��������ѧϰ��
void Module0_Mode1_Handler(void) {
    handle_ir_signal(1);
}
