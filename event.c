#include <string.h>
#include "event.h"

uint8_t Event;     // Generated event
volatile uint8_t EvCounter; // Press delay counter

volatile uint8_t EventQueue; // Generated event
static uint8_t   PrevKey; // Previous keys pressed
static uint8_t   RealizeCounter; // Realize delay counter

void EventKeys(void)
{
  uint8_t Key;
  
  if ( (EventQueue & KEY_MASK_SYS) != 0 ) /* Previous event hasn't been handled. The key hase highiest  priority */
    return;

  Key = (~(GPIOA->IDR & GPIO_PIN3)) & GPIO_PIN3; /* Read the port */

  if ( Key == 0 ) // All keys was released
  {
    if ( PrevKey == 0 ) // no any key was pressed before
    {
      EvCounter = 0;
      goto StopTimer;
    }
    if ( EvCounter > KEY_PRESSED_VALUE )
    {
      RealizeCounter++; // increase timer counter
      if ( RealizeCounter > KEY_REALIZE_VALUE ) // expired realise timeout
      {
        if ( EvCounter != 0xFF ) /* There is no switch to new function. New function should not get previos function event */
        {
          EventQueue = EV_KEY_REALIZED; /* Realized event - the last event */
        }
        EvCounter = 0; // Reset interval timer value
        PrevKey = 0;   // Reset key pressed value
        RealizeCounter = 0;  // Reset realise counter
        goto StopTimer;
      }
    }
    else
    {
      EvCounter = 0; // Reset interval timer value
      PrevKey = 0;   // Reset key pressed value
      RealizeCounter = 0;  // Reset realise counter
      goto StopTimer;
    }
    return;

StopTimer:
    return;
  }
  else // Some keys are pressed
  {
    RealizeCounter = 0; //reset realise delay
   
    if ( EvCounter == 0xFF ) /* Locked - no realize was found */
      return; 
    
    {
      if ( EvCounter == KEY_LONG_VALUE )  // Long press timeout has expired
      {
        EventQueue = EV_KEY_LONG | PrevKey; // Generate Long press event
        EvCounter = 0xFF;
      }
      else if ( EvCounter > KEY_LONG_VALUE ) // After long press the key is stil pressed
      {
        EvCounter = KEY_LONG_VALUE; // Reset time counter for next delay
      }
      PrevKey = Key;
      EvCounter++; // Delay counter increasing
    }
  }
}


