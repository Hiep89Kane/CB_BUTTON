#ifndef SRC_BUTTON_BUTTON_H_
#define SRC_BUTTON_BUTTON_H_

#include "stdint.h"
#include "timer.h"

#define _TIME_INIT_PRESS 5	   /*50 ms*/
#define _TIME_INIT_RELEASE 5   /*50 ms*/
#define _TIME_HOLD_ACTIVE 100  /*1s */
#define _TIMEOUT_MULTICLICK 50 /*500 ms*/

/*Note : explain events
*
* "PRESS and HOLD"
* 			HIGH____________																					  ______
* 							|																				     |
* 							|.....(5)____________________________________________________ LOW (hold continues)___|
*
* status:			 --0--->1----> 2<----------3--------->4----------------------------------------------------->9<----0---
*					   null |init  |	 wait hold (1s)   |  isHold  				^				   			 |
*					   			   ^ 											    ^							 ^
* user event:					 press                	 	 			(isHold + _timeHold)			hold release

* "PRESS and CLICK"
* 			HIGH____________				    .....(5)___________________________________________HIGH (ide)
* 							|				   |
* 							|.....(5)__________|
*
*			    -------0--->1----->2<----3----><---5--><----------------7--------------->0---------------->
* status:		      null  | init | wait hold |release|     wait multi-click (0.5s)     | null
*                                  ^				   ^								 ^
* user event:    				 press               click							timeout 0.5s
*												(_multiClick++)					(_multiClick == 0)

*"PRESS , CLICK and Multi-CLICK"
* 			HIGH____________				     .....(5)_____________________                    	 .....(5)________detect multi-click continues
* 							|				    |                      		  |					 	|
* 							|.....(5)___________|					          |........(5)__________|
*
* status		-------0--->1----->2<----3---->----5--->6<--------7----------->----8---->2<----3---->---5--->
*					  null  |init  | wait hold |release |   wait multi-click  |multi init|			   		|
*					               ^					^								 ^					|
* user event:					 press				 click  						   press				|
*       										(_multiClick++)												|
*                                                       ^                                                   |
*                                                       |___________________________________________________|
*										(_multiClick=1 -> single click)
*										(_multiClick=2 -> double click)
*										(_multiClick=3 -> triple click)
*										(... )
*										(_multiClick=n -> n click)
*/
typedef enum
{
	_BTN_null = 0,
	_BTN_init = 1,
	_BTN_isPress = 2, /*(user event)*/
	_BTN_wait_hold = 3,
	_BTN_isHold = 4, /*(user event) && (_timeHold==n_second)*/
	_BTN_isRelease = 5,
	_BTN_isClick = 6,			 /*(user event) && (_multiClick==user_times)*/
	_BTN_wait_clrMultiClick = 7, // wait timeout clear multi-click
	_BTN_multi_init = 8,		 // begin the multi-click
	_BTN_hold_pass = 9
} BTN_Stt_t;
typedef enum
{
	BUTTON_UNKNOWN = 0,
	BUTTON_PRESS,
	BUTTON_RELEASE,
	BUTTON_ONECLICK,
	BUTTON_HOLD_PRESS,
	BUTTON_HOLD_RELEASE,
} EventButton_t;

typedef void (*button_cbEvent)(EventButton_t event);
typedef void (*button_cbEventMultiClick)(uint8_t cntClick);
typedef void (*button_cbEventHold)(uint32_t interval_ms);

typedef enum
{
	_BTN_null = 0,
	_BTN_init = 1,
	_BTN_isPress = 2, /*(user event)*/
	_BTN_wait_hold = 3,
	_BTN_isHold = 4, /*(user event) && (_timeHold==n_second)*/
	_BTN_isRelease = 5,
	_BTN_isClick = 6,			 /*(user event) && (_multiClick==user_times)*/
	_BTN_wait_clrMultiClick = 7, // wait timeout clear multi-click
	_BTN_multi_init = 8,		 // begin the multi-click
	_BTN_hold_pass = 9
} BTN_Stt_t;

typedef struct
{
	timer_t _timeout;
	uint32_t _timeBeginHold;
} private_process_t;

typedef struct
{
	BTN_Stt_t state;
	uint8_t multiClick;
	uint32_t timeHold;

	// CallBack functions
	button_cbEvent cb_Event;
	button_cbEventMultiClick cb_MultiClick;
	button_cbEventHold cb_Hold;

	// PRIVATE
	private_process_t process;

} button_t;

void btn_progress_loop(button_t *btn, uint8_t _HW_PinStt);

#endif /* SRC_BUTTON_H_ */
