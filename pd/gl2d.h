#ifndef GL2D
#define GL2D

#include <stddef.h>
#include <dos.h>

#define SCREEN_WIDTH        320
#define SCREEN_HEIGHT       200
#define SCREEN_SIZE         (SCREEN_WIDTH*SCREEN_HEIGHT)
#define NUM_COLORS          256

#define VIDEO_INT           0x10
#define SET_MODE            0x00
#define VGA_256_COLOR_MODE  0x13
#define TEXT_MODE           0x03

#define PALETTE_INDEX       0x03c8
#define PALETTE_DATA        0x03c9
#define INPUT_STATUS        0x03da
#define VRETRACE            0x08

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;
typedef char  sbyte;
typedef short sword;

byte* vga=(byte*)0xA0000000L;

void SetMode(byte mode);
void SetPalette(byte *palette,word npalette);
void VWait(void);

void DrawPoint(byte *buf,sword x,sword y,byte color);
void DrawLine(byte *buf,sword x1,sword y1,sword x2,sword y2,byte color);
void DrawRect(byte *buf,sword x1,sword y1,sword x2,sword y2,byte color);
void FillRect(byte *buf,sword x1,sword y1,sword x2,sword y2,byte color);

#ifdef GL2D_IMPLEMENTATION

static sword sgn(sword x) {
	return x<0?-1:x>0?1:0;
}

void SetMode(byte mode) {
	union REGS regs;

	regs.h.ah = SET_MODE;
	regs.h.al = mode;
	int86(VIDEO_INT, &regs, &regs);
}

void SetPalette(byte *palette,word npalette) {
	word i;

	outp(PALETTE_INDEX,0);              /* tell the VGA that palette data
																				 is coming. */
	for(i=0;i<npalette;i++) {
		outp(PALETTE_DATA,palette[i*3+0]);
		outp(PALETTE_DATA,palette[i*3+1]);
		outp(PALETTE_DATA,palette[i*3+2]);
	}

	for(i=0;i<256-npalette;i++) {
		outp(PALETTE_DATA,0);
		outp(PALETTE_DATA,0);
		outp(PALETTE_DATA,0);
	}

}

void VWait(void) {
		/* wait until done with vertical retrace */
		while  ((inp(INPUT_STATUS) & VRETRACE));
		/* wait until done refreshing */
		while (!(inp(INPUT_STATUS) & VRETRACE));
}

void DrawPoint(byte *buf,sword x,sword y,byte color) {
	if(x>=0 && x<SCREEN_WIDTH && y>=0 && y<SCREEN_HEIGHT) {
		buf[y*SCREEN_WIDTH+x]=color;
	}
}

void DrawLine(byte *buf,sword x1,sword y1,sword x2,sword y2, byte color) {
	int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;

	dx = x2 - x1;
	dy = y2 - y1;
	dxabs = abs(dx);
	dyabs = abs(dy);
	sdx = sgn(dx);
	sdy = sgn(dy);
	x = dyabs >> 1;
	y = dxabs >> 1;
	px = x1;
	py = y1;

	DrawPoint(buf, px, py, color);
	if (dxabs >= dyabs) {
		for (i = 0; i < dxabs; i++) {
			y += dyabs;
			if (y >= dxabs) {
				y -= dxabs;
				py += sdy;
			}
			px += sdx;
			DrawPoint(buf, px, py, color);
		}
	} else {
		for (i = 0; i < dyabs; i++) {
			x += dxabs;
			if (x >= dyabs) {
				x -= dyabs;
				px += sdx;
			}
			py += sdy;
			DrawPoint(buf, px, py, color);
		}
	}
}

void DrawRect(byte *buf,sword x,sword y,sword w,sword h,byte color) {
	int i,j;
	for(i=x;i<x+w;i++) {
		DrawLine(buf,i,y,i,y,color);
		DrawLine(buf,i,y+h-1,i,y+h-1,color);
	}
	for(j=y;j<y+h;j++) {
		DrawLine(buf,x,j,x,j,color);
		DrawLine(buf,x+w-1,j,x+w-1,j,color);
	}
}

void FillRect(byte *buf,sword x,sword y,sword w,sword h,byte color) {
	int i,j,k;
	for(j=y;j<y+h;j++) {
		for(i=x;i<x+w;i++) {
			DrawPoint(buf,i,j,color);
		}
	}
}

#endif

#endif
