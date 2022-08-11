#include "application.h"
#include "main.h"
#include "stm32g0xx_hal.h"
#include "myDefine.h"
#include "printf.h"

#define APP_BUTTON_NUM 3

typedef uint8_t (*__ReadPin)(void);

uint8_t BTN0_Stt() { return (uint8_t)HAL_GPIO_ReadPin(IN_BUTTON_GPIO_Port, IN_BUTTON_Pin); }
uint8_t BTN1_Stt() { return (uint8_t)HAL_GPIO_ReadPin(IN_SAFETY_GPIO_Port, IN_SAFETY_Pin); }
uint8_t BTN2_Stt() { return (uint8_t)HAL_GPIO_ReadPin(IN_JETSW_GPIO_Port, IN_JETSW_Pin); }

button_t BUTTON_arr[APP_BUTTON_NUM];
__ReadPin pinStt_arr[APP_BUTTON_NUM] = {BTN0_Stt, BTN1_Stt, BTN2_Stt};

void userButton0_event(EventButton_t event)
{

	switch (event)
	{
	case BUTTON_PRESS:
		printf("\n\r Btn0 Press");
		break;
	case BUTTON_RELEASE:
		printf("\n\r Btn0 release");
		break;
	case BUTTON_ONECLICK:
		printf("\n\r Btn0 click");
		break;
	case BUTTON_HOLD_PRESS:
		printf("\n\r Btn0 hold start");
		break;
	case BUTTON_HOLD_RELEASE:
		printf("\n\r Btn0 hold release");
		break;
	default:
		break;
	}
}

void userButton0_multiClick(uint8_t numClick)
{
	if (numClick == 2)
	{
		printf("\n\r Btn0 double click");
	}

	if (numClick == 3)
	{
		printf("\n\r Btn0 triple click");
	}

	if (numClick == 5)
	{
		printf("\n\r Btn0 click %u times", numClick);
	}
}

void userButton0_eventHold(uint32_t interval_ms)
{
	if (interval_ms == 5000)
	{
		printf("\n\r Btn0 hold %ums", interval_ms);
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
		btn_progress_loop(&BUTTON_arr[i], !pinStt_arr[i]());
	}
}

/**
 * @brief appButtonsCallBack_init() : init all callback function of user 
 * 
 * @return ** void 
 */
void appButtonsCallBack_init()
{
	BUTTON_arr[0].cb_Event = &userButton0_event;
	BUTTON_arr[0].cb_MultiClick = &userButton0_multiClick;
	BUTTON_arr[0].cb_Hold = &userButton0_eventHold;
}
