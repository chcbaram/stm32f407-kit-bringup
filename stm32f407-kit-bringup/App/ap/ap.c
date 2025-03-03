#include "ap.h"
#include "lwip.h"
#include "usb_host.h"


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
    MX_USB_HOST_Process();

    if (uartAvailable(HW_UART_CH_232))
    {
      uartPrintf(HW_UART_CH_232, "rx : 0x%X\n", uartRead(HW_UART_CH_232));
    }
  }
} 
