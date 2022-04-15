#ifndef MOUSE_H
#define MOUSE_H

#include <dos.h>

#define GL2D_IMPLEMENTATION
#include "gl2d.h"

#define GBM_IMPLEMENTATION
#include "gbm.h"

#define MOUSE_INT           0x33
#define MOUSE_RESET         0x00
#define MOUSE_GETPRESS      0x05
#define MOUSE_GETRELEASE    0x06
#define MOUSE_GETMOTION     0x0B
#define LEFT_BUTTON         0x00
#define RIGHT_BUTTON        0x01
#define MIDDLE_BUTTON       0x02

typedef struct Mouse Mouse;

struct Mouse {
	sword x,y;
	GBM gbm;
};

Mouse Mouse_Create(GBM gbm,sword x,sword y);
void Mouse_Draw(byte *buf,Mouse mouse);
sword Mouse_Reset(Mouse *mouse);
void Mouse_GetMotion(sword *dx, sword *dy);
sword Mouse_IsPressed(sword button);
sword Mouse_IsReleased(sword button);

#ifdef MOUSE_IMPLEMENTATION

Mouse Mouse_Create(GBM gbm,sword x,sword y) {
	Mouse mouse;
	mouse.gbm=gbm;
	mouse.x=x;
	mouse.y=y;
	return mouse;
}

void Mouse_Draw(byte *buf,Mouse mouse) {
	GBM_Draw(buf,mouse.gbm,0,mouse.x,mouse.y);
}

void Mouse_Update(Mouse *mouse) {
	sword dx,dy;

	Mouse_GetMotion(&dx,&dy);

	mouse->x+=dx/2;
	mouse->y+=dy/2;

	if(mouse->x<0) mouse->x=0;
	if(mouse->y<0) mouse->y=0;
	if(mouse->x>=SCREEN_WIDTH) mouse->x=SCREEN_WIDTH-1;
	if(mouse->y>=SCREEN_HEIGHT) mouse->y=SCREEN_HEIGHT-1;

}

void Mouse_GetMotion(sword *dx, sword *dy) {
  union REGS regs;
  regs.x.ax = MOUSE_GETMOTION;
  int86(MOUSE_INT, &regs, &regs);
  *dx=regs.x.cx;
  *dy=regs.x.dx;
}

sword Mouse_IsPressed(sword button) {
  union REGS regs;
  regs.x.ax = MOUSE_GETPRESS;
  regs.x.bx = button;
  int86(MOUSE_INT, &regs, &regs);
  return regs.x.bx;
}

sword Mouse_IsReleased(sword button) {
	union REGS regs;
	regs.x.ax = MOUSE_GETRELEASE;
  regs.x.bx = button;
	int86(MOUSE_INT, &regs, &regs);
  return regs.x.bx;
}

sword Mouse_Reset(Mouse *mouse) {
  union REGS regs;
	mouse->x=SCREEN_WIDTH/2;
	mouse->y=SCREEN_HEIGHT/2;
  regs.x.ax = MOUSE_RESET;
  int86(MOUSE_INT, &regs, &regs);
	return regs.x.ax;
}

#endif

#endif
