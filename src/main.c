#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "snes.h"
#include "scheduler.h"
#include "lcd_display.h"
#include "asteroids.c"
#include "vc_vector.h"


enum INPUT_STATE {ACCEPT, LOCK} input_state;
int update_input(int state)
{

  switch (state) {
    case ACCEPT:
      input = get_input();
      PORTA = input;
      state = LOCK;
      break;
    case LOCK:
      input = 0x0000;
      state = ACCEPT;
      break;
    default:
      state = ACCEPT;
  }
  return state;
}

int main(void)
{
  DDRC = 0x0F;
  DDRA = 0xFF;
  snes_init();
  nokia_lcd_init();       //LCD initialization
  nokia_lcd_clear();

  static task game, input_t;//, display;
  task *tasks[2] = {&game, &input_t};//, &display};
  game.period = 1;
  game.elapsedTime = game.period;
  game.TickFct = &asteroids_game;
  game.state = -1;


  input_t.period = 1;
  input_t.elapsedTime = input_t.period;
  input_t.TickFct = &update_input;
  input_t.state = -1;


  //display.period = 2;
  //display.elapsedTime = display.peroid;
  //display.TickFct =  

  player.player_status = ALIVE;
  player.x=45;
  player.y=35;
  player.orientation = 3; 
  b.visible = 0;
  // bullets = vc_vector_create(0, sizeof(Bullet), NULL);
  // LCD_draw_custom(DOWNR_S,10,10);
  TimerSet(findGCD(game.period, input_t.period));
  TimerOn();
  while(1)
  {
    for(unsigned i = 0 ; i < 2; i++)
    {
      if(tasks[i]->elapsedTime == tasks[i]->period)
      {
        tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
        tasks[i]->elapsedTime = 0;
      }
      tasks[i]->elapsedTime++;
    }
    while(!TimerFlag);
    TimerFlag = 0;
  }

}
