#include "main.h"
#include "application.h"

int main(void)
{
  // GPIO_Config();
  // Uart_Config();

  appButtonsCallBack_init();

  while (1)
  {

    appButtonsProgress_loop();
  }
  /* USER CODE END 3 */
}

/**
 * @brief  SYSTICK callback : IRQ system tick 1ms
 * @retval None
 */
void HAL_SYSTICK_Callback(void)
{
  static uint8_t tx10;
  // 10ms
  if (++tx10 > 10)
  {
    tx10 = 0;
    timer_periodic_poll();
  }
}
