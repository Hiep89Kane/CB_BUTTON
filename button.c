#include <button.h>
#include "timer.h"

void btn_progress_loop(button_t *btn){

	if(btn->pinStt==0)
		return;

	  //is press
	  if(btn->pinStt()){
		 switch(btn->state){
			 case _BTN_null:
				 btn->state=_BTN_init;
			      timer_set(&btn->process._timeout,_TIME_INIT_PRESS);
			      btn->process._timeBeginHold = timer_getTick();
			      break;

			case _BTN_init:
				 if (timer_expired(&btn->process._timeout))
					 btn->state=_BTN_isPress;
				 break;

			case _BTN_isPress:
				btn->state=_BTN_wait_hold;
			     timer_set(&btn->process._timeout,_TIME_HOLD_ACTIVE);

			     //callback Press
			     if(btn->cb_function){
			    	 button_typeArg_t ArgValue;
			    	 ArgValue.event = BUTTON_PRESS;
			    	 btn->cb_function(_BUTTON_FUNC_EVENT, ArgValue);
			     }

			     break;

			case _BTN_wait_hold :
				if (timer_expired(&btn->process._timeout)){
					btn->state=_BTN_isHold;
				      //callbak hold Release
				      if(btn->cb_function){
				    	  button_typeArg_t ArgValue;
				    	  ArgValue.event = BUTTON_HOLD_PRESS;
				    	  btn->cb_function(_BUTTON_FUNC_EVENT, ArgValue);
				      }
				}
				break;

			case _BTN_wait_clrMultiClick:
		    	//restart init ...
		    	if (!timer_expired(&btn->process._timeout)) {
		    		btn->state=_BTN_init;
		    		timer_set(&btn->process._timeout,_TIME_INIT_PRESS);
		    		btn->process._timeBeginHold=timer_getTick();
		    	}
		    	break;

			case _BTN_isHold:
				btn->timeHold = timer_getTick()-btn->process._timeBeginHold;

				//callback Hold on
				if(btn->cb_function){
					button_typeArg_t ArgValue;
			    	ArgValue.holdInterval_ms = 10*btn->timeHold;
					btn->cb_function(_BUTTON_FUNC_HOLD, ArgValue);
				}

				break;

			default : break;
		 }
		 return;
	  }
	  //is release
	  else
	  {
		  switch(btn->state){
			  case _BTN_wait_hold:
				  btn->state=_BTN_isRelease;
			      timer_set(&btn->process._timeout,_TIME_INIT_RELEASE);
			      break;

			  case _BTN_isRelease:
			      if (timer_expired(&btn->process._timeout)){
			    	  btn->state=_BTN_isClick;
			    	  btn->multiClick++;

					//callback every click
					if(btn->cb_function){
						button_typeArg_t ArgValue;
						ArgValue.event = BUTTON_ONECLICK;
						btn->cb_function(_BUTTON_FUNC_EVENT, ArgValue);
					}


					//callback multiClick
					if(btn->cb_function){
						button_typeArg_t ArgValue;
						ArgValue.numClick = btn->multiClick;
						//if( btn->multiClick > 2)
						btn->cb_function(_BUTTON_FUNC_MULCLICK, ArgValue);
					}
			      }

			      break;

			  case  _BTN_isHold:
				  btn->state=_BTN_hold_pass;
			      timer_stop(&btn->process._timeout);
			      btn->multiClick=0;

			      //callbak hold Release
			      if(btn->cb_function){
			    	  button_typeArg_t ArgValue;
			    	  ArgValue.event = BUTTON_HOLD_RELEASE;
			    	  btn->cb_function(_BUTTON_FUNC_EVENT, ArgValue);
			      }

			      break;

			  case _BTN_isClick:
				btn->state=_BTN_wait_clrMultiClick;
			    timer_set(&btn->process._timeout,_TIMEOUT_MULTICLICK);

			    break;

			  case _BTN_wait_clrMultiClick:
			    if (timer_expired(&btn->process._timeout)){
			    	btn->state=_BTN_null;
			    	timer_stop(&btn->process._timeout);
			    	 btn->multiClick=0;
			    }
			    break;

			  default : btn->state=_BTN_null;
		  }
	  }
}
