#define MAX_ASTEROIDS 4
#define LEFT_S 0
#define RIGHT_S 1
#define DOWN_S 2
#define UP_S 3
#define UPL_S 4
#define DOWNR_S 5
#define DOWNL_S 6
#define UPR_S 7
#include "vc_vector.h"
#include <stdlib.h>
#include <stdio.h>
typedef struct Asteroids{
  //on scale from 0,1,2
  unsigned char size;
  short x;
  short y;
  int velx;
  int vely;
  unsigned char visible;
}Asteroid;

enum ship_status {ALIVE, DEAD};

typedef struct _player{
  short x;
  short y;
  int velx;
  int vely;
  int accy;
  int accx;
  int orientation;
  unsigned char player_status;

} Player; 
static Player player;

unsigned int input;


typedef struct Bullets{
  short x;
  short y;
  int velx;
  int vely;
  unsigned char visible;
} Bullet;

// static Bullet b;

vc_vector* bullets; //= vc_vector_create(0, sizeof(Bullet), NULL);
vc_vector* asteroids;
Asteroid asteroids_arr[MAX_ASTEROIDS];
static Asteroid test_a;
static int RAND_SEED = 9;
long score;

enum Asteroids_State {INIT, START_SCREEN, GAME, UPDATE, RESET, GAME_OVER}; 
void update(unsigned int input);
void update_player(unsigned int input);
void update_bullets();
void update_asteroids();
void update_display();
unsigned int get_input();

int asteroids_game(int game_state)
{
  // PORTA = vc_vector_size(asteroids);
  switch (game_state) {
    case INIT:
      game_state = START_SCREEN;
      break;

    case START_SCREEN:
      if(input) {
          game_state= GAME;
      }
      break;
    case GAME:
      if(player.player_status == DEAD)
        game_state = GAME_OVER;
      break;
    case GAME_OVER:
      if(input)
        game_state = START_SCREEN;
      break;
    default:
      game_state = INIT;
  }

  switch (game_state) {
    case START_SCREEN:
      nokia_lcd_clear();
      nokia_lcd_write_string("Press Start to continue", 1);
      break;
    case GAME:
      update(input);
      break;
    case GAME_OVER:
      nokia_lcd_clear();
      nokia_lcd_write_string("Game OVER", 1);
      break;
  }

  nokia_lcd_render();
  return game_state;
}

void update(unsigned int input)
{
  update_player(input);
  update_bullets();
  // update_asteroids();
  update_display();
}

void update_player(unsigned int input)
{
  //check if collision
  //check through all asteroids
  //check if
  //update movement according to input

  unsigned int dpad = input >> 4;
  unsigned int buttons = input & (0x0003 | 0x0300);
  unsigned int control_buttons = input & 0x000C; 

  if(buttons)
  {
    if(buttons == 0x01)
      {
        //calculate offset due to orientation
        char velx;
        char vely;
        switch (player.orientation) {
          case DOWNL_S:
            velx = -1;
            vely = 1;
            break;
          case DOWNR_S:
            velx = 1;
            vely = 1;
            break;
          case UPL_S:
            velx = -1;
            vely = -1;
            break;
          case UPR_S:
            velx = 1;
            vely = -1;
            break;
          case UP_S:
            velx = 0;
            vely = -1;
            break;
          case DOWN_S:
            velx = 0;
            vely = 1;
            break;
          case LEFT_S:
            velx = -1;
            vely = 0;
            break;
          case RIGHT_S:
            velx = 1;
            vely = 0;
            break;
        }
        // if(player.x + offsetx || player.y+offsety < LIMIT)

        // if(b.visible)
          // return;
        Bullet b;
        b.x = player.x;
        b.y = player.y;
        b.vely = vely;
        b.velx = velx;
        b.visible = 1;

        vc_vector_push_back(bullets, &b);


      }

  }

  if(dpad)
  {
    if(dpad == (0x06) )
    {
      player.orientation = DOWNL_S;
      player.x -= 1;
      player.y += 1;
    }
    else if (dpad == (0x0A))
    {
      player.orientation = DOWNR_S;
      player.x +=1;
      player.y +=1;
    }
    else if(dpad == (0x02))
    {
      player.orientation = DOWN_S;
      player.y += 1;
    }
    else if(dpad == 0x05)
    {
      player.orientation = UPL_S;
      player.y -=1;
      player.x -=1;
    }
    else if (dpad == 0x09)
    {
      player.orientation = UPR_S;
      player.y -=1;
      player.x +=1;
    }
    else if(dpad ==  0x01)
    {
      player.orientation = UP_S;
      player.y -= 1;
    }
    else if(dpad == 0x04)
    {
      player.orientation = LEFT_S;
      player.x -= 1;
    }
    else if(dpad == 0x08)
    {
      player.orientation = RIGHT_S;
      player.x += 1;
    }
    
  }

}

void update_bullets()
{
  unsigned char all_off = 1;
  for(void* i = vc_vector_begin(bullets);
      i != vc_vector_end(bullets);
      i = vc_vector_next(bullets, i))
  {
    int velx = (*(Bullet*)i).velx;
    int vely = (*(Bullet*)i).vely;


    int x = (*(Bullet*)i).x;
    int y = (*(Bullet*)i).y;

    if(x + velx < 0 || x + velx > 84)
    {
      // vc_vector_push_back(idxs, &i);
      (*(Bullet*)i).visible = 0;
      continue;
    }
    else if(y + vely < 0 || y + vely > 48)
    {
      // vc_vector_push_back(idxs, &i);
      (*(Bullet*)i).visible = 0;
      continue;
    }
    else {
      all_off = 0;
      (*(Bullet*)i).x += velx;
      (*(Bullet*)i).y += vely;
    }
  }
  if(all_off)
  {
    vc_vector_release(bullets);
    bullets = vc_vector_create(0,sizeof(Bullet), NULL);
    return;
  }

  // for(void* i = vc_vector_begin(idxs);
      // i != vc_vector_end(idxs);
      // i = vc_vector_next(idxs, i))
  // {
    // vc_vector_erase(bullets, *(int*)i);
  // }
  // vc_vector_release(idxs);
}

#define VEL_RANGE 3
#define LOCX_RANGE 84
#define LOCY_RANGE 48
#define A_RAND 3
#define C_RAND 2

// void rand(int mod, int a, int c, int* seed)
// {
  // *seed = (a * (*seed) + c) % (*seed);
// }

//update on slower period as seprate sm
int update_at(int state)
{
  update_asteroids();
  return 1;
}
void update_asteroids()
{

  // if(test_a.visible == 0)
  // {
    // test_a.x =0;
    // test_a.y =15;
    // test_a.velx = 1;
    // test_a.visible = 1;
  // }
  // else {
    // test_a.x += test_a.velx;
  // }
  // return;
//
  // for(void* i = vc_vector_begin(asteroids);
      // i != vc_vector_end(asteroids);
      // i = vc_vector_next(asteroids, i))
  // {
    // int velx = (*(Asteroid*)i).velx;
    // int vely = (*(Asteroid*)i).vely;
//
//
    // int x = (*(Asteroid*)i).x;
    // int y = (*(Asteroid*)i).y;
    // if((*(Asteroid*)i).visible == 0)
    // {
        // if(rand() > .5)
        // {
          // (*(Asteroid*)i).velx = rand() % (VEL_RANGE + 1 - (-1 * VEL_RANGE)) +(-1*VEL_RANGE);
          // (*(Asteroid*)i).vely = 0;
        // }
        // else {
         // (*(Asteroid*)i).vely = rand() % (VEL_RANGE + 1 - (-1 * VEL_RANGE)) +(-1*VEL_RANGE);
         // (*(Asteroid*)i).velx = 0;
        // }
        // (*(Asteroid*)i).x = rand()%LOCX_RANGE;
        // (*(Asteroid*)i).y = rand()%LOCY_RANGE;
        // (*(Asteroid*)i).visible = 1;
    // }
    // else if(x + velx < 0 )
    // {
      // (*(Asteroid*)i).x = 83;
    // }
    // else if(x + velx > 84)
    // {
      // (*(Asteroid*)i).x = 1;
    // }
    // else if(y + vely < 0)
    // {
      // (*(Asteroid*)i).y = 46;
    // }
    // else if(y + vely > 48)
    // {
      // (*(Asteroid*)i).y = 1;
    // }
    // else {
      // (*(Asteroid*)i).x += velx;
      // (*(Asteroid*)i).y += vely;
    // }
  // }
//
  for(unsigned i =0; i < MAX_ASTEROIDS; i++)
  {
    int velx = asteroids_arr[i].velx;
    int vely = asteroids_arr[i].vely;


    int x = asteroids_arr[i].x;
    int y = asteroids_arr[i].y;
    if(asteroids_arr[i].visible == 0)
    {
      if(rand() > .5)
        {
          asteroids_arr[i].velx = rand() % (VEL_RANGE + 1 - (-1 * VEL_RANGE)) +(-1*VEL_RANGE);
          asteroids_arr[i].vely = 0;//rand() % (VEL_RANGE + 1 - (-1 * VEL_RANGE)) +(-1*VEL_RANGE);
        }
        else {
         asteroids_arr[i].velx = rand() % (VEL_RANGE + 1 - (-1 * VEL_RANGE)) +(-1*VEL_RANGE);
         asteroids_arr[i].vely = rand() % (VEL_RANGE + 1 - (-1 * VEL_RANGE)) +(-1*VEL_RANGE);
        }
        asteroids_arr[i].velx = 0;//rand() % (VEL_RANGE + 1 - (-1 * VEL_RANGE)) +(-1*VEL_RANGE);
        asteroids_arr[i].vely = rand() % (VEL_RANGE + 1 - (-1 * VEL_RANGE)) +(-1*VEL_RANGE);

        asteroids_arr[i].x = rand()%LOCX_RANGE;
        asteroids_arr[i].y = rand()%LOCY_RANGE;
        asteroids_arr[i].visible = 1;
    }
    else if(x + velx < 0 )
    {
      asteroids_arr[i].x = 83;
    }
    else if(x + velx > 84)
    {
      asteroids_arr[i].x = 0;
    }
    else if(y + vely < 0)
    {
      asteroids_arr[i].y = 47;
    }
    else if(y + vely > 48)
    {
      asteroids_arr[i].y = 0;
    }
    else {
      asteroids_arr[i].x += velx;
      asteroids_arr[i].y += vely;
    }
  }
    // vc_vector* temp = asteroids;
    // asteroids = vc_vector_create(0,sizeof(Asteroid), NULL);
    // vc_vector_release(temp);
    // spawn_asteroids();
  return;
}
//

void update_display()
{
  nokia_lcd_clear();
  //iterate through all game objects and draw them
  // if(b.visible)
  // {
    // nokia_lcd_set_cursor(b.x, b.y);
    // nokia_lcd_write_custom(8,1);
  // }
  for(void* i = vc_vector_begin(bullets);
      i != vc_vector_end(bullets);
      i = vc_vector_next(bullets, i))
  {
    if((*(Bullet*)i).visible == 0)
      continue;
    nokia_lcd_set_cursor((*(Bullet*)i).x, (*(Bullet*)i).y);
    nokia_lcd_write_custom(8,1);
  }


  // if(test_a.visible)
  // {
    // nokia_lcd_set_cursor(test_a.x, test_a.y);
    // nokia_lcd_write_custom(11,2);
  // }
  for(unsigned i = 0; i < MAX_ASTEROIDS; i++)
  {
    if(asteroids_arr[i].visible == 0)
      continue;
    nokia_lcd_set_cursor(asteroids_arr[i].x, asteroids_arr[i].y);
    nokia_lcd_write_custom(11,1);
  }
//
  // for(void* i = vc_vector_begin(asteroids);
      // i != vc_vector_end(asteroids);
      // i = vc_vector_next(asteroids, i))
  // {
    // if((*(Asteroid*)i).visible == 0)
      // continue;
    // nokia_lcd_set_cursor((*(Asteroid*)i).x, (*(Asteroid*)i).y);
    // nokia_lcd_write_custom(11,1);
  // }

  //draw player
  nokia_lcd_set_cursor(player.x,player.y);
  nokia_lcd_write_custom(player.orientation, 2);
}
