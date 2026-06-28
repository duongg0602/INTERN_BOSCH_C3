#define _Base_Freq          40000    // Hz
#define _Working_Freq       100       // Hz    
//-------------------------------------------------------------------------------
#define _Sys_CLK            80000000  // Hz
#define _BaseTimer_CLK      2000000   // Hz
#define _BaseTimer_PRE      (_Sys_CLK / _BaseTimer_CLK)
#define _BaseTimer_TOP      (_BaseTimer_CLK / _Base_Freq)
#define _WrkCycle_MAX       (_Base_Freq / _Working_Freq)
//-------------------------------------------------------------------------------
hw_timer_t * BaseTimer = NULL;
portMUX_TYPE BaseTimerMux = portMUX_INITIALIZER_UNLOCKED;
unsigned int WrkCycle_Counter = _WrkCycle_MAX;
volatile bool F_Working_Cycle = false;
//-------------------------------------------------------------------------------
void IRAM_ATTR onBaseTimer()
{     
  portENTER_CRITICAL_ISR(&BaseTimerMux);    
  if (!(--WrkCycle_Counter)) 
  {
    WrkCycle_Counter = _WrkCycle_MAX;
    F_Working_Cycle = true;
  }   
  portEXIT_CRITICAL_ISR(&BaseTimerMux);  
}
void InitBaseTimer()
{
  BaseTimer = timerBegin(0, _BaseTimer_PRE, true); 
  timerAttachInterrupt(BaseTimer, &onBaseTimer, true);
  timerAlarmWrite(BaseTimer, _BaseTimer_TOP, true);
  timerAlarmEnable(BaseTimer);
}
