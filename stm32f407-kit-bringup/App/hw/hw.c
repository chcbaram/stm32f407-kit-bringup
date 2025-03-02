#include "hw.h"




bool hwInit(void)
{
  cliInit();
  ledInit();
  gpioInit();  
  uartInit();
  for (int i=0; i<UART_MAX_CH; i++)
  {
    uartOpen(i, 115200);
  }

  logPrintf("\r\n[ Firmware Begin... ]\r\n");
  logPrintf("Booting..Clock\t\t: %d Mhz\r\n", (int)HAL_RCC_GetSysClockFreq()/1000000);
  logPrintf("\n");

  rtcInit();
  i2cInit();
  spiInit();  
  eepromInit();
  spiFlashInit();
  buttonInit();
  buzzerInit();
  sdInit();

  return true;
}

void delay(uint32_t ms)
{
  HAL_Delay(ms);
}

uint32_t millis(void)
{
  return HAL_GetTick();
}
