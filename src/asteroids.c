#define MAX_ASTEROIDS 10;
#define LEFT_S 0
#define RIGHT_S 1
#define DOWN_S 2
#define UP_S 3
#define UPL_S 4
#define DOWNR_S 5
#define DOWNL_S 6
#define UPR_S 7
#include "vc_vector.h"
typedef struct Asteroids{
  //on scale from 0,1,2
  unsigned char size;
  unsigned short x;
  unsigned short y;
  int velx;
  int vely;
  unsigned char visible;
}Asteroid;

enum ship_status {ALIVE, DEAD};

typedef struct _player{
  unsigned short x;
  unsigned short y;
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
  unsigned short x;
  unsigned short y;
  int velx;
  int vely;
  unsigned char visible;
} Bullet;

static Bullet b;

vc_vector* bullets; //= vc_vector_create(0, sizeof(Bullet), NULL);

enum Asteroids_State {INIT, START_SCREEN, GAME, UPDATE, RESET, GAME_OVER}; 
void update(unsigned int input);
void update_player(unsigned int input);
void update_bullets();
void update_asteroids();
void update_display();
unsigned int get_input();

int asteroids_game(int game_state)
{
  PORTA = vc_vector_size(bullets); 
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
            PORTA = 0xFF;
            break;
          case DOWN_S:
            velx = 0;
            vely = 1;
            PORTA = 0xFF;
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

        if(b.visible)
          return;
        b.x = player.x;
        b.y = player.y;
        b.vely = vely;
        b.velx = velx;
        b.visible = 1;


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
  if(b.x + b.velx < 0 || b.x + b.velx > 84)
    {
      b.visible = 0;
      b.velx = 1;
      b.vely = 1;
      b.x=0;
      b.y=0;
    }
    else if(b.y + b.vely < 0 || b.y + b.vely > 48)
    {
      b.visible = 0;
      b.velx = 1;
      b.vely = 1;
      b.x=0;
      b.y=0;
    }
    else {
      b.x += b.velx;
      b.y += b.vely;
    }
  


  // int idx =0;
  // vc_vector* idxs = vc_vector_create(0, sizeof(int), NULL);
//
  // for(void* i = vc_vector_begin(bullets);
      // i != vc_vector_end(bullets);
      // i = vc_vector_next(bullets, i),idx++)
  // {
    // int velx = (*(Bullet*)i).velx;
    // int vely = (*(Bullet*)i).vely;
//
//
    // int x = (*(Bullet*)i).x;
    // int y = (*(Bullet*)i).y;
//
    // if(x + velx < 0 || x + velx > 84)
    // {
      // vc_vector_push_back(idxs, &i);
      // continue;
    // }
    // else if(y + vely < 0 || y + vely > 48)
    // {
      // vc_vector_push_back(idxs, &i);
      // continue;
    // }
    // else {
      // (*(Bullet*)i).x += velx;
      // (*(Bullet*)i).y += vely;
    // }
  // }
  // if(vc_vector_empty(idxs))
  // {
    // vc_vector_release(idxs);
    // return;
  // }
//
  // for(void* i = vc_vector_begin(idxs);
      // i != vc_vector_end(idxs);
      // i = vc_vector_next(idxs, i))
  // {
    // vc_vector_erase(bullets, *(int*)i);
  // }
  // vc_vector_release(idxs);
}

void update_asteroids()
{

}

void update_display()
{
  nokia_lcd_clear();
  //iterate through all game objects and draw them
  if(b.visible)
  {
    nokia_lcd_set_cursor(b.x, b.y);
    nokia_lcd_write_custom(8,1);
  }
  // for(void* i = vc_vector_begin(bullets);
      // i != vc_vector_end(bullets);
      // i = vc_vector_next(bullets, i))
  // {
    // nokia_lcd_set_cursor((*(Bullet*)i).x, (*(Bullet*)i).y);
    // nokia_lcd_write_custom(8,1);
  // }

  //draw player
  nokia_lcd_set_cursor(player.x,player.y);
  nokia_lcd_write_custom(player.orientation, 2);
}
