#include "timer.h"

clock_time_t _unitTick;

/*---------------------------------------------------------------------------*/
/**
 * get uTick of my timer system.
 */
clock_time_t timer_getTick()
{
  return _unitTick;
}

/*---------------------------------------------------------------------------*/
/**
 * count the uTick .
 * This function is must be call in a timer interrupt
 */
void timer_periodic_poll(void)
{
  _unitTick++;
}

/*---------------------------------------------------------------------------*/
/**
 * Set a timer of user
 *
 * This function is used to set a timer for a time sometime in the
 * future. The function timer_expired() will evaluate to true after
 * the timer has expired.
 *
 * \param t A pointer to the timer
 * \param interval The interval before the timer expires.
 *
 */
void timer_set(timer_t *t, clock_time_t interval)
{
  t->status = _timer_on;
  t->start = _unitTick;
  t->interval = interval;
  t->left_time = 0;
}

/*---------------------------------------------------------------------------*/
/**
 * Reset the timer with the same interval.
 *
 * This function resets the timer with the same interval that was
 * given to the timer_set() function. The start point of the interval
 * is the exact time that the timer last expired. Therefore, this
 * function will cause the timer to be stable over time, unlike the
 * timer_rester() function.
 *
 * \param t A pointer to the timer.
 *
 * \sa timer_restart()
 */
void timer_reset(timer_t *t)
{
  t->start += t->interval;
  t->status = _timer_on;
}

/*---------------------------------------------------------------------------*/
/**
 * Restart the timer from the current point in time
 *
 * This function restarts a timer with the same interval that was
 * given to the timer_set() function. The timer will start at the
 * current time.
 *
 * \note A periodic timer will drift if this function is used to reset
 * it. For preioric timers, use the timer_reset() function instead.
 *
 * \param t A pointer to the timer.
 *
 * \sa timer_reset()
 */
void timer_restart(timer_t *t)
{
  t->start = _unitTick;
  t->status = _timer_on;
}

/*---------------------------------------------------------------------------*/
/**
 * Check if a timer has expired.
 *
 * This function tests if a timer has expired and returns true or
 * false depending on its status.
 *
 * \param t A pointer to the timer
 *
 * \return Non-zero if the timer has expired, zero otherwise.
 *
 */
uint8_t timer_expired(timer_t *t)
{
  if (t->status == _timer_over)
    return 1;

  if ((clock_time_t)(_unitTick - t->start) >= (clock_time_t)t->interval)
  {
    t->status = _timer_over;
    return 1;
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
/**
 * Stop the timer
 * \param t A pointer to the timer
 *
 * \return Non-zero if the timer has expired, zero otherwise.
 *
 */
void timer_stop(timer_t *t)
{
  t->status = _timer_off;
  t->left_time = 0;
}

/*---------------------------------------------------------------------------*/
/**
 * make the timer has expired by the intention of developer
 * \param t A pointer to the timer
 *
 * \return Non-zero if the timer has expired, zero otherwise.
 *
 */
void timer_makeExpired(timer_t *t)
{
  t->start = _unitTick - t->interval;
  t->status = _timer_over;
}

/*---------------------------------------------------------------------------*/
/**
 * pause the time and save the left time
 * \param t A pointer to the timer
 * \return Non-zero if the timer has expired, zero otherwise.
 */
void timer_pause(timer_t *t)
{
  if (t->status == _timer_on)
  {
    t->status = _timer_off;
    t->left_time = t->interval - (_unitTick - t->start);
  }
}

/*---------------------------------------------------------------------------*/
/**
 * continue working with the timer which new interval is the left time have been saved before
 * \param t A pointer to the timer
 * \return Non-zero if the timer has expired, zero otherwise.
 *
 */
void timer_continue(timer_t *t)
{
  if (t->left_time > 0)
  {
    t->status = _timer_on;
    t->start = _unitTick;
    t->interval = t->left_time;
    t->left_time = 0;
  }
}

/*---------------------------------------------------------------------------*/
/**
 * return the remaining time of timer when is working
 * \param t A pointer to the timer
 * \return Non-zero if the timer has expired, zero otherwise.
 *
 */
clock_time_t timer_remaining(timer_t *t)
{
  return (t->start + t->interval - _unitTick);
}
