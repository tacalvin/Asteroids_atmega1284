#include "snes.h"
#include "bit.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "snes.h"

#define DATA_DDR  DDRB
#define DATA_PORT PORTB //Where controller is
#define DATA_LATCH_PIN 0 // data latch pin
#define DATA_CLOCK_PIN 1 // clock pin
#define DATA_SERIAL_PIN 2 // serial output
#define NUM_CYCLES 12


//map inputs
const unsigned int input_map[NUM_CYCLES] = 
{
  Bbtn,Ybtn,SELECT,START,UP,DOWN,LEFT,RIGHT,Abtn,Xbtn,Lbtn,Rbtn
};
void snes_init()
{
    unsigned char out = 0x00;
    out = SetBit(out, DATA_LATCH_PIN, 1);
    out = SetBit(out, DATA_CLOCK_PIN, 1);
    DATA_DDR |= out;
}
unsigned int get_input()
{
  int input_state = 0x00;
  DATA_PORT = 0x03;
  _delay_ms(LATCH_TIME);
  
  for(unsigned i = 0; i < NUM_CYCLES; i++)
  {
    DATA_PORT = 0x02;
    _delay_ms(LATCH_TIME);
    unsigned char input = ~PINB & 0x04;
    if(input)
        input_state |= input_map[i];//SetBit(input_state,i,input);
    DATA_PORT = 0x00;
    
    
  }
  return input_state;
}
