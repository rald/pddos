#ifndef BUTTON_H
#define BUTTON_H

#define GBM_IMPLEMENTATION
#include "gbm.h"

#define MOUSE_IMPLEMENTATION
#include "mouse.h"

typedef struct Button Button;
typedef void OnClickCallback(Button *button);


struct Button {
	sword x,y;
	GBM gbm;
	sword pressed;
	OnClickCallback *OnClick;
};

Button Button_Create(GBM gbm,sword x,sword y);
void Button_Draw(byte *buf,Button button);
void Button_HandleEvents(Button *button,Mouse mouse);


#ifdef BUTTON_IMPLEMENTATION

static sword inrect(sword x,sword y,sword rx,sword ry,sword rw,sword rh);

static sword inrect(sword x,sword y,sword rx,sword ry,sword rw,sword rh) {
	return x>=rx && x<rx+rw && y>=ry && y<ry+rh;
}

Button Button_Create(GBM gbm,sword x,sword y) {
	Button button;
	button.gbm=gbm;
	button.x=x;
	button.y=y;
	button.pressed=0;
	button.OnClick=NULL;
	return button;
}

void Button_Draw(byte *buf,Button button) {
	if(button.pressed==1) {
		GBM_Draw(buf,button.gbm,1,button.x,button.y);
	} else {
		GBM_Draw(buf,button.gbm,0,button.x,button.y);
	}
}

void Button_HandleEvents(Button *button,Mouse mouse) {
	if(inrect(
			mouse.x,mouse.y,
			button->x,button->y,
			button->gbm.width,button->gbm.height)) {
		if(Mouse_IsPressed(LEFT_BUTTON)==1) {
			button->pressed=1;
		}
		if(Mouse_IsReleased(LEFT_BUTTON)==1){
			if(button->pressed==1) button->OnClick(button);
			button->pressed=0;
		}
	} else {
		if(Mouse_IsReleased(LEFT_BUTTON)==1){
			button->pressed=0;
		}
	}
}

#endif

#endif
