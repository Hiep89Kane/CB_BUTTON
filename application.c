#include "application.h"
#include "main.h"
#include "stm32g0xx_hal.h"
#include "myDefine.h"
#include "printf.h"

#define APP_BUTTON_NUM 3
uint8_t BTN0_Stt() { return (uint8_t)HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1); }
uint8_t BTN1_Stt() { return (uint8_t)HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5); }
uint8_t BTN2_Stt() { return (uint8_t)HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7); }

button_t BUTTON_arr[APP_BUTTON_NUM];
void btn0_cb_Handle(button_functionCb_t typeFunction, button_typeArg_t agr){

	switch(typeFunction){
		case _BUTTON_FUNC_EVENT:		
			if(agr.event == BUTTON_PRESS){
				debug_msg("\n\r Btn0 press");
			}
			
			if(agr.event == BUTTON_RELEASE){
				debug_msg("\n\r Btn0 press");
			}
			
			if(agr.event == BUTTON_ONECLICK){
				debug_msg("\n\r Btn0 a click");
			}
			
			if(agr.event == BUTTON_HOLD_RELEASE){
				debug_msg("\n\r Btn0 aldready holded and release");
			}			
			
			break;

		case _BUTTON_FUNC_MULCLICK:
			if(agr.numClick==2)
				debug_msg("\n\r Btn0 DoubleClick");
			if(agr.numClick==3)
				debug_msg("\n\r Btn0 TripleClick");
			break;

		case _BUTTON_FUNC_HOLD:
			if(agr.holdInterval_ms == 3000/*3000ms*/){
				debug_msg("\n\r Btn0 hold %ums", agr);
			}		
			if(agr.holdInterval_ms == 5000){
				debug_msg("\n\r Btn0 hold %ums", agr);
			}
			if(agr.holdInterval_ms == 10000){
				debug_msg("\n\r Btn0 hold %ums", agr);
			}			
			break;
		default:break;
	}

}

/**
 * @brief appButtonsProgress_loop :  excuse all Button struct , put in while(1)
 * 
 * @return ** void 
 */
void appButtonsProgress_loop()
{

	for (uint8_t i = 0; i < APP_BUTTON_NUM; i++)
	{
		btn_progress_loop(&BUTTON_arr[i]);
	}
}

/**
 * @brief appButtonsCallBack_init() : init all callback function of user 
 * 
 * @return ** void 
 */
void appButtonsCallBack_init()
{
	//add the PinStt of Hardware
	BUTTON_arr[0].pinStt = &BTN0_Stt;
	BUTTON_arr[1].pinStt = &BTN1_Stt;
	BUTTON_arr[2].pinStt = &BTN2_Stt;
	
	//init callback of event button0
	BUTTON_arr[0].cb_Event = &userButton0_event;
}
