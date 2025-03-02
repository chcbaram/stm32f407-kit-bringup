#include "ap.h"

extern void MX_LWIP_Process(void);

void apInit(void)
{
  cliOpen(HW_UART_CH_CLI, 115200);
  cliLogo();      
}


void apMain(void)
{
  uint32_t pre_time;


  pre_time = millis();
  while(1)
  {  
    if (millis() - pre_time >= 500)
    {
      pre_time = millis();

      ledToggle(_DEF_LED1);
    }
    cliMain();
    sdUpdate();

    MX_LWIP_Process();
  }
} 
