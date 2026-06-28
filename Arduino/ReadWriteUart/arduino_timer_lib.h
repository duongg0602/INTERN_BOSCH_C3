#define _Base_Freq          80000     // Hz    
#define _Working_Freq       100       // Hz    
//-------------------------------------------------------------------------------
#define _Sys_CLK            16000000  // Hz    
#define _BaseTimer_CLK      16000000  // Hz   
#define _BaseTimer_TOP      (((_BaseTimer_CLK / _Base_Freq)) - 1)   
#define _WrkCycle_MAX       (_Base_Freq / _Working_Freq)         
//-------------------------------------------------------------------------------
uint32_t WrkCycle_Counter = _WrkCycle_MAX;      
volatile bool F_Working_Cycle = false;        
//-------------------------------------------------------------------------------
void onBaseTimer()           
{
  TCCR0A = 0; TCCR0B = 0; TIMSK0 = 0;   
  TCCR0A |= (1 << WGM01); 
  TIMSK0 |= (1 << OCIE0A);  
  OCR0A = _BaseTimer_TOP;   
  TCCR0B |= (1 << CS00);   
}
//-------------------------------------------------------------------------------
ISR (TIMER0_COMPA_vect)      
{
  if (!(--WrkCycle_Counter))    
  {
    WrkCycle_Counter = _WrkCycle_MAX;
    F_Working_Cycle = true;
  }
}
