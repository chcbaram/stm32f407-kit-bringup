#include "buzzer.h"


#ifdef _USE_HW_BUZZER
#include "cli.h"



static bool is_init = false;
static bool is_on = false;
static uint16_t on_time_cnt = 0;
extern TIM_HandleTypeDef htim14;


#if CLI_USE(HW_BUZZER)
static void cliBuzzer(cli_args_t *args);
#endif



void buzzerISR(void *arg)
{
  (void)arg;

  if (is_on && on_time_cnt > 0)
  {
    on_time_cnt--;

    if (on_time_cnt == 0)
    {
      buzzerOff();
    }
  }
}




bool buzzerInit(void)
{
  bool ret = true;

  
#if CLI_USE(HW_BUZZER)
  cliAdd("buzzer", cliBuzzer);
#endif  

  logPrintf("[%s] buzzerInit()\n", ret ? "OK":"NG");

  is_init = ret;
  return ret;
}

void buzzerOn(uint32_t freq_hz, uint16_t time_ms)
{
  uint32_t freq_cmp;

  freq_cmp = (1000000/freq_hz) - 1;

  if (freq_hz >= 20 && freq_hz <= 500000)
  {
    htim14.Instance->ARR = freq_cmp/2;

    HAL_TIM_OC_Start(&htim14, TIM_CHANNEL_1);
  }
  is_on = true;
  on_time_cnt = time_ms;
}

void buzzerOff(void)
{
  HAL_TIM_OC_Stop(&htim14, TIM_CHANNEL_1);

  is_on = false;
}

#if CLI_USE(HW_BUZZER)
void cliBuzzer(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 3 && args->isStr(0, "on"))
  {
    uint16_t freq_hz;
    uint16_t time_ms;

    freq_hz = args->getData(1);
    time_ms = args->getData(2);

    buzzerOn(freq_hz, time_ms);
    ret = true;
  }
  
  if (args->argc == 1 && args->isStr(0, "off"))
  {
    buzzerOff();
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "test"))
  {
    uint16_t freq_tbl[8] = {261, 293, 329, 349, 391, 440, 493, 523};
    uint8_t freq_i;

    for (int i=0; i<16; i++)
    {
      if (i/8 == 0)
      {
        freq_i = i%8;
      }
      else
      {
        freq_i = 7 - (i%8);
      }
      buzzerOn(freq_tbl[freq_i], 150);
      cliPrintf("%dHz, %dms\n", freq_tbl[freq_i], 100);
      delay(300);
    }
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "melody"))
  {
    uint16_t melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
    int note_durations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

    for (int i=0; i<8; i++) 
    {
      int note_duration = 1000 / note_durations[i];

      buzzerOn(melody[i], note_duration);
      delay(note_duration * 1.30);    
    }
    ret = true;
  }
  if (ret != true)
  {
    cliPrintf("buzzer on freq(32~500000) time_ms\n");
    cliPrintf("buzzer off\n");
    cliPrintf("buzzer test\n");
    cliPrintf("buzzer melody\n");
  }
}
#endif

#endif