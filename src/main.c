#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "timer.h"
#include "snes.h"
#include "scheduler.h"
#include "lcd_display.h"
#include "asteroids.c"
#include "vc_vector.h"

void ADC_init() 
{
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
    // ADEN: setting this bit enables analog-to-digital conversion.
    // ADSC: setting this bit starts the first conversion.
    // ADATE: setting this bit enables auto-triggering. Since we are
    //        in Free Running Mode, a new conversion will trigger whenever
    //        the previous conversion completes.
}

enum INPUT_STATE {ACCEPT, LOCK} input_state;
int update_input(int state)
{

  switch (state) {
    case ACCEPT:
      input = get_input();
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
  DDRA = 0xFE;
  snes_init();
  nokia_lcd_init();       //LCD initialization
  nokia_lcd_clear();
  ADC_init();
  srand(ADC);

  static task game, input_t, asteroid_t;//, display;
  task *tasks[3] = {&game, &input_t, &asteroid_t};//, &display};
  game.period = 1;
  game.elapsedTime = game.period;
  game.TickFct = &asteroids_game;
  game.state = -1;

  input_t.period = 1;
  input_t.elapsedTime = input_t.period;
  input_t.TickFct = &update_input;
  input_t.state = -1;

  asteroid_t.period = 3;
  asteroid_t.elapsedTime = input_t.period;
  asteroid_t.TickFct = &update_at;
  asteroid_t.state = -1;
  

  //display.period = 2;
  //display.elapsedTime = display.peroid;
  //display.TickFct =  

  player.player_status = ALIVE;
  player.x=45;
  player.y=35;
  player.orientation = 3; 
  // b.visible = 0;
  bullets = vc_vector_create(0, sizeof(Bullet), NULL);
  // asteroids = vc_vector_create(0, sizeof(Asteroid), NULL);
  // for(unsigned int i =0; i < 4; i++)
    // {
      // Asteroid a;
      // if(rand() > .5)
      // {
        // a.velx = rand() % (VEL_RANGE + 1 - (-1 * VEL_RANGE)) +(-1*VEL_RANGE);
        // a.vely = 0;
      // }
      // else {
         // a.vely = rand() % (VEL_RANGE + 1 - (-1 * VEL_RANGE)) +(-1*VEL_RANGE);
         // a.velx = 0;
      // }
      // a.x = rand()%LOCX_RANGE;
      // a.y = rand()%LOCY_RANGE;
      // a.visible = 1;
      // vc_vector_push_back(asteroids, &a);
    // }

  // LCD_draw_custom(DOWNR_S,10,10);
  //
  //
  // nokia_lcd_write_custom(9,1);
  // nokia_lcd_write_custom(10,2);
  // nokia_lcd_write_custom(11,2);
//
  // nokia_lcd_render();
  TimerSet(1);
  TimerOn();
  while(1)
  {
    for(unsigned i = 0 ; i < 3; i++)
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
