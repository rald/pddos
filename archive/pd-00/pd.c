#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>

#define GL2D_IMPLEMENTATION
#include "gl2d.h"

#define GBM_IMPLEMENTATION
#include "gbm.h"

#define MOUSE_IMPLEMENTATION
#include "mouse.h"

#define BUTTON_IMPLEMENTATION
#include "button.h"

static sword show=0;

void btnOK_OnClick(Button *button) {
	show=1-show;
}

int main(void) {

	byte palette[]={
			0,   0,   0,
			0,   6,  28,
			3,  34,   0,
			5,  46,  51,
		 28,   2,   2,
		 26,   6,  38,
		 43,  19,   5,
		 45,  43,  41,
		 18,  17,  16,
			2,  24,  48,
		 38,  50,   0,
		 28,  60,  52,
		 57,  38,   0,
		 62,  30,  53,
		 62,  59,  20,
		 62,  62,  62
	};

	byte npalette=16;

	int i,j,k;

	int key;

	sword dx,dy;

	byte *dbf=calloc(SCREEN_SIZE,sizeof(*dbf));

	GBM gbmMouse;
	GBM gbmOK;

	Mouse mouse;
	Button btnOK;

	srand(time(NULL));

	SetMode(VGA_256_COLOR_MODE);

	SetPalette(palette,npalette);

	gbmMouse=GBM_Load("mouse.gbm");
	mouse=Mouse_Create(gbmMouse,SCREEN_WIDTH/2,SCREEN_HEIGHT/2);

	if(Mouse_Reset(&mouse)==0) {
		printf("No mouse found"); getch();
		goto terminate;
	}

	gbmOK=GBM_Load("ok.gbm");
	btnOK=Button_Create(gbmOK,0,0);
	btnOK.OnClick=btnOK_OnClick;

	while(1) {

		if(kbhit()) {
			key=getch();
			if(key==0) key=getch()+256;
			if(key==27) break;
		}

		Mouse_Update(&mouse);

		Button_HandleEvents(&btnOK,mouse);

		memset(dbf,0,SCREEN_SIZE);

		if(show) {
			for(i=0;i<16;i++) {
				FillRect(dbf,(i%4*8)+(SCREEN_WIDTH-4*8),i/4*8,8,8,i);
			}
		}

		Button_Draw(dbf,btnOK);
		Mouse_Draw(dbf,mouse);

		memcpy(vga,dbf,SCREEN_SIZE);

	}

terminate:

	SetMode(VGA_256_COLOR_MODE);

	return 0;
}
