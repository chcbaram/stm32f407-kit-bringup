#include "button.h"


#ifdef _USE_HW_BUTTON
#include "gpio.h"
#include "cli.h"


typedef struct
{
  uint8_t     state;
  bool        pressed;
  uint16_t    pressed_cnt;
  uint32_t    pre_time;
} button_t;



typedef struct
{
  GPIO_TypeDef *port;
  uint32_t      pin;
  uint32_t      pull;
  GPIO_PinState on_state;
} button_pin_t;



#if CLI_USE(HW_BUTTON)
static void cliButton(cli_args_t *args);
#endif
static bool buttonGetPin(uint8_t ch);

static const button_pin_t button_pin[BUTTON_MAX_CH] =
    {
      {GPIOG, GPIO_PIN_2, GPIO_PULLUP, GPIO_PIN_RESET},  
      {GPIOG, GPIO_PIN_3, GPIO_PULLUP, GPIO_PIN_RESET},  
      {GPIOG, GPIO_PIN_4, GPIO_PULLUP, GPIO_PIN_RESET},  
      {GPIOG, GPIO_PIN_5, GPIO_PULLUP, GPIO_PIN_RESET},        
    };


static button_t button_tbl[BUTTON_MAX_CH];





bool buttonInit(void)
{
  bool ret = true;
  GPIO_InitTypeDef GPIO_InitStruct = {0};


  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();


  for (int i=0; i<BUTTON_MAX_CH; i++)
  {
    button_tbl[i].state          = 0;
    button_tbl[i].pressed_cnt    = 0;
    button_tbl[i].pressed        = false;
  }

  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  for (int i = 0; i < BUTTON_MAX_CH; i++)
  {
    GPIO_InitStruct.Pin = button_pin[i].pin;
    GPIO_InitStruct.Pull = button_pin[i].pull;
    HAL_GPIO_Init(button_pin[i].port, &GPIO_InitStruct);
  }


#if CLI_USE(HW_BUTTON)
  cliAdd("button", cliButton);
#endif

  return ret;
}

bool buttonGetPin(uint8_t ch)
{
  bool ret = false;

  if (ch >= BUTTON_MAX_CH)
  {
    return false;
  }

  if (HAL_GPIO_ReadPin(button_pin[ch].port, button_pin[ch].pin) == button_pin[ch].on_state)
  {
    ret = true;
  }

  return ret;
}


bool buttonGetPressed(uint8_t ch)
{
  if (ch >= BUTTON_MAX_CH)
  {
    return false;
  }

  return buttonGetPin(ch);
}

uint32_t buttonGetData(void)
{
  uint32_t ret = 0;


  for (int i=0; i<BUTTON_MAX_CH; i++)
  {
    ret |= (buttonGetPressed(i)<<i);
  }

  return ret;
}

uint8_t  buttonGetPressedCount(void)
{
  uint32_t i;
  uint8_t ret = 0;

  for (i=0; i<BUTTON_MAX_CH; i++)
  {
    if (buttonGetPressed(i) == true)
    {
      ret++;
    }
  }

  return ret;
}

#if CLI_USE(HW_BUTTON)
void cliButton(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "info"))
  {
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "show"))
  {    
    while(cliKeepLoop())
    {
      for (int i=0; i<BUTTON_MAX_CH; i++)
      {
        cliPrintf("%d", buttonGetPressed(i));
      }
      delay(50);
      cliPrintf("\r");
    }
    ret = true;
  }

  if (ret == false)
  {
    cliPrintf("button info\n");
    cliPrintf("button show\n");
  }
}
#endif



#endif