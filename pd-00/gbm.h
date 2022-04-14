#ifndef GBM_H
#define GBM_H

#include <stdio.h>

#define GL2D_IMPLEMENTATION
#include "gl2d.h"

typedef struct GBM GBM;

struct GBM {
	sword width,height;
	byte frames;
	byte *data;
	byte transparent;
};

GBM GBM_Load(const char *path);
int GBM_Save(const char *path,GBM gbm);
void GBM_Draw(byte *buf,GBM gbm,byte frame,sword x,sword y);

byte *hex="0123456789ABCDEF";

#ifdef GBM_IMPLEMENTATION

static sword hexval(byte data) {
	sword i,j;
	for(i=0,j=-1;i<16;i++) {
		if(data==hex[i]) {
			j=i;
			break;
		}
	}
	return j;
}

GBM GBM_Load(const char *path) {
	GBM gbm;
	FILE *fin;
	char magic[3]={0};
	sword width,height;
	byte frames;
	byte transparent;
	int data;
	sword h;
	sword n,i=0;

	fin=fopen(path,"rt");
	fscanf(fin,"%2s\n",magic);
	if(!strcmp(magic,"G1")) {
		if(fscanf(fin,"%hu %hu %hu %cu\n\n",&width,&height,&frames,&transparent)==4) {
			n=width*height*frames;
			gbm.width=width;
			gbm.height=height;
			gbm.frames=frames;
			h=hexval(transparent);
			if(h!=-1) {
				gbm.transparent=h;
				gbm.data=calloc(n,sizeof(*(gbm.data)));
				while(i<n && (data=fgetc(fin))!=EOF) {
					h=hexval(data);
					if(h!=-1) gbm.data[i++]=h;
				}
      }
    }
	}
	fclose(fin);
	return gbm;
}

int GBM_Save(const char *path,GBM gbm) {
	sword i,j,k,l;
	FILE *fout=fopen(path,"wt");
	if(fout==NULL) {
		return 1;
	}
	if(fprintf(fout,"G1\n%hu %hu %hu %cu\n\n",gbm.width,gbm.height,gbm.frames,gbm.transparent)!=EOF) {
		for(k=0;k<gbm.frames;k++) {
			for(j=0;j<gbm.height;j++) {
				for(i=0;i<gbm.width;i++) {
					l=i+j*gbm.width+k*gbm.width*gbm.height;
					fputc(hex[gbm.data[l]],fout);
				}
				fputc('\n',fout);
			}
			fputc('\n',fout);
		}
		fputc('\n',fout);
  }
	fclose(fout);
	return 0;
}

void GBM_Draw(byte *buf,GBM gbm,byte frame,sword x,sword y) {
	sword i,j,k;
	for(j=0;j<gbm.height;j++) {
		for(i=0;i<gbm.width;i++) {
			k=i+j*gbm.width+frame*gbm.width*gbm.height;
			if(gbm.data[k]!=gbm.transparent) DrawPoint(buf,x+i,y+j,gbm.data[k]);
		}
	}
}

#endif

#endif