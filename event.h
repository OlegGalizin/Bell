#if !defined(_EVENT_H_)
#define _EVENT_H_

#include "stm8s.h"

#define EV_KEY_TOUCH    0x00
#define EV_KEY_PRESSED  0x20 
#define EV_KEY_LONG     0x30
#define EV_KEY_REPEATE  0x40
#define EV_KEY_REALIZED 0x50
#define EV_KEY_DOUBLE   0x60
#define EV_FUNC_FIRST   0x70
#define EV_MASK         0x70

extern uint8_t Event;  /* 0xKKKKAAAK */
/* AAA - it is event key. Event can be defined by & operation with EV_MASK*/
/* KKKKKK - keys - one bit fpr one key. Up to 5 keys */
volatile extern uint8_t EventQueue; /* for setting user event */
extern volatile uint8_t EvCounter; // Press delay counter

extern void EventKeys(void); /* This function is periodicaly called (e.g. from ISR) */

/* Event detect delays (unit is check intervals) */
#define KEY_PRESSED_VALUE 4 // Press event delay * 2ms
#define KEY_LONG_VALUE    125  // Long press event delay * 2ms
#define KEY_REPEATE_VALUE 250 // Repeate event delay KEY_REPEATE_VALUE - KEY_LONG_VALUE - repeate interval * 5ms
#define KEY_REALIZE_VALUE 8 //  Realize event detect delay * 2ms



/* The keys must be redefined by real key mapping */
#define KEY1  GPIO_PIN3
#define KEY_MASK_SYS (KEY1) /* real keys - EXTI interrupt enable mask */
#define KEY_MASK (KEY1) /* real and pseudo keys. Pseudo keus can be used as other events */

#endif /* _EVENT_H_ */
