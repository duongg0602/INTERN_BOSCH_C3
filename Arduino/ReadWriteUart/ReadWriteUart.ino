#include "arduino_serial_lib.h"
#include "arduino_timer_lib.h"    //use for arduino
//#include "esp32_timer_lib.h"    //use for esp32
//-------------------------------------------------------------------------------
float pri_press = 6542.321, sec_press = 987.332;
uint16_t pri_speed = 23421;
char* p_pri_press = (char*)&pri_press; 
char* p_sec_press = (char*)&sec_press; 
char* p_pri_speed = (char*)&pri_speed; 
//-------------------------------------------------------------------------------
void setup()
{
  cli();
  onBaseTimer();        //use for arduino
  // InitBaseTimer();    //use for esp32
  Serial.begin(115200);
  sei();
}

void loop()
{
  if (F_Working_Cycle)
  {
    F_Working_Cycle = false;
    send_num_float(pri_press, "pp");    
    send_num_float(sec_press, "sp");
    send_num_u16(pri_speed, "ps");
    pri_press += 10;
  }
  if (Serial.available() >= 7)
  {
    switch (Serial.read())
    {
      case 'P':
      {
        switch (Serial.read())
        {
          case 'S':
          {
            Serial.readBytes(read_buffer, 2); 
            if (Serial.read() == ';')
            {
              *p_pri_speed = (char)read_buffer[1];
              *(p_pri_speed + 1) = (char)read_buffer[0];
            } 
            break;
          }
          case 'P':
          {
            Serial.readBytes(read_buffer, 4); 
            if (Serial.read() == ';')
            {              
              *p_pri_press = (char)read_buffer[3];
              *(p_pri_press + 1) = (char)read_buffer[2];
              *(p_pri_press + 2) = (char)read_buffer[1];
              *(p_pri_press + 3) = (char)read_buffer[0];
            }        
            break;
          }
        }
        break;
      }
      case 'S':
      {
        if (Serial.read() == 'P')
          Serial.readBytes(read_buffer, 4); 
        if (Serial.read() == ';')
        {
          *p_sec_press = (char)read_buffer[3];
          *(p_sec_press + 1) = (char)read_buffer[2];
          *(p_sec_press + 2) = (char)read_buffer[1];
          *(p_sec_press + 3) = (char)read_buffer[0];
        } 
        break;
      }
    }
  }
}
