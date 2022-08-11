#include <button.h>
#include "timer.h"

void btn_progress_loop(button_t *btn, uint8_t _HW_PinStt)
{

	// is press
	if (_HW_PinStt)
	{
		switch (btn->state)
		{
		case _BTN_null:
			btn->state = _BTN_init;
			timer_set(&btn->process._timeout, _TIME_INIT_PRESS);
			btn->process._timeBeginHold = timer_getTick();
			break;

		case _BTN_init:
			if (timer_expired(&btn->process._timeout))
				btn->state = _BTN_isPress;
			break;

		case _BTN_isPress:
			btn->state = _BTN_wait_hold;
			timer_set(&btn->process._timeout, _TIME_HOLD_ACTIVE);

			// callback Press
			if (btn->cb_Event)
				btn->cb_Event(BUTTON_PRESS);

			break;

		case _BTN_wait_hold:
			if (timer_expired(&btn->process._timeout))
			{
				btn->state = _BTN_isHold;
				// callbak hold Release
				if (btn->cb_Event)
					btn->cb_Event(BUTTON_HOLD_PRESS);
			}
			break;

		case _BTN_wait_clrMultiClick:
			// restart init ...
			if (!timer_expired(&btn->process._timeout))
			{
				btn->state = _BTN_init;
				timer_set(&btn->process._timeout, _TIME_INIT_PRESS);
				btn->process._timeBeginHold = timer_getTick();
			}
			break;

		case _BTN_isHold:
			btn->timeHold = timer_getTick() - btn->process._timeBeginHold;

			// callback Hold on
			if (btn->cb_Hold)
			{
				btn->cb_Hold(btn->timeHold * 10);
			}

			break;

		default:
			break;
		}
		return;
	}
	// is release
	else
	{
		switch (btn->state)
		{
		case _BTN_wait_hold:
			btn->state = _BTN_isRelease;
			timer_set(&btn->process._timeout, _TIME_INIT_RELEASE);
			break;

		case _BTN_isRelease:
			if (timer_expired(&btn->process._timeout))
			{
				btn->state = _BTN_isClick;
				btn->multiClick++;

				// callback every click
				if (btn->cb_Event)
					btn->cb_Event(BUTTON_ONECLICK);

				// callback multiClick
				if (btn->cb_MultiClick)
					// if( btn->multiClick > 2)
					btn->cb_MultiClick(btn->multiClick);
			}

			break;

		case _BTN_isHold:
			btn->state = _BTN_hold_pass;
			timer_stop(&btn->process._timeout);
			btn->multiClick = 0;

			// callbak hold Release
			if (btn->cb_Event)
				btn->cb_Event(BUTTON_HOLD_RELEASE);

			break;

		case _BTN_isClick:
			btn->state = _BTN_wait_clrMultiClick;
			timer_set(&btn->process._timeout, _TIMEOUT_MULTICLICK);

			break;

		case _BTN_wait_clrMultiClick:
			if (timer_expired(&btn->process._timeout))
			{
				btn->state = _BTN_null;
				timer_stop(&btn->process._timeout);
				btn->multiClick = 0;
			}
			break;

		default:
			btn->state = _BTN_null;
		}
	}
}
