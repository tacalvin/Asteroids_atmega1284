#ifndef SNES_H
#define SNES_H

#define Bbtn 0x01
#define Ybtn 0x02
#define SELECT 0x04
#define START 0x08
#define UP 0x0010
#define DOWN 0x0020
#define LEFT 0x0040
#define RIGHT 0x0080
#define Abtn 0x0100
#define Xbtn 0x0200
#define Lbtn 0x0400
#define Rbtn 0x0800
#define LATCH_TIME .012

void snes_init();
unsigned int get_input();

#endif
