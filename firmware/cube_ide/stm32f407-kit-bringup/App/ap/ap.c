#include "ap.h"
#include "lwip.h"
#include "usb_host.h"


void apInit(void)
{
  cliOpen(HW_UART_CH_CLI, 115200);
  cliLogo();      

  for (int i = 0; i < 32; i += 1)
  {
    lcdClearBuffer(black);
    lcdPrintfResize(0, 40 - i, green, 16, "  -- BARAM --");
    lcdDrawRect(0, 0, LCD_WIDTH, LCD_HEIGHT, white);
    lcdUpdateDraw();
    delay(10);
  }
  delay(500);
  lcdClear(black);
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

    if (uartAvailable(HW_UART_CH_485))
    {
      uartPrintf(HW_UART_CH_485, "rx : 0x%X\n", uartRead(HW_UART_CH_485));
    }
  }
} 
