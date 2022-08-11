#ifndef __TIMER_H__
#define __TIMER_H__
//#pragma used+
#include "stdint.h"

typedef uint32_t clock_time_t;
#define CLOCK_SECOND (100UL)
#define CLOCK_MINUTE (60 * CLOCK_SECOND)
#define CLOCK_HOUR (60 * CLOCK_MINUTE)

/**
 * A timer.
 *
 * This structure is used for declaring a timer. The timer must be set
 * with timer_set() before it can be used.
 *
 * \hideinitializer
 */

typedef enum
{
	_timer_off,
	_timer_on,
	_timer_over // over time
} timerStt_t;

typedef struct
{
	timerStt_t status;
	clock_time_t start;
	clock_time_t interval;
	clock_time_t left_time;
} timer_t;

clock_time_t timer_getTick(void);
void timer_periodic_poll(void);

void timer_set(timer_t *t, clock_time_t interval);
void timer_reset(timer_t *t);
void timer_restart(timer_t *t);
void timer_stop(timer_t *t);
uint8_t timer_expired(timer_t *t);
void timer_makeExpired(timer_t *t);
void timer_pause(timer_t *t);
void timer_continue(timer_t *t);
clock_time_t timer_remaining(timer_t *t);

//#pragma used-
#endif /* __TIMER_H__ */
