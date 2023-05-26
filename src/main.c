#include <avm_core.h>

int main() {
	// #ifdef DEBUG_BLDC
	// 	//Brushless motor auto init
	//     MOTOR_SETTING();
	// #endif
	//
    // uart_sendStr("Config MPU6050...");
    // UART_CR();
    // MPU_init();
    // uart_sendStr("MPU6050 Connect Success!");
    // UART_CR();
	avm_core_module_st.init_module(NULL);
	// avm_uart_module_st.init_module(avm_uart_module_st.confs);
	// uart_init(72, 115200);

	while(1) {
	}
}
//333333333东方不败
