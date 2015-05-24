/*
    gcc slifepl.c -o life.exe -s -lmingw32 -lSDLmain -lSDL -static-libgcc -fomit-frame-pointer -ffast-math -msse2 -O3
*/
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <SDL/SDL.h>

#define width 100
#define SCAL 5
#define BIRTHRULE (nn == 3 || nn == 6)
#define DEATHRULE (nn > 3 || nn < 2)
#define LIVE 4
#define PILLAR 7
//#define BIRTHRULE (nn == 3)

SDL_Surface *sdl_scr;
int *scr;
static unsigned int seed=1000; inline void _srand(int s) {seed=s;}
__inline int _rand() {seed=(214013*seed+2531011); return((seed>>16)&0x7FFF);}

__inline void rot2d(float sf, float cf, float *x, float *y) {
    float tx = (*x) * cf - (*y) * sf; *y = (*y) * cf + (*x) * sf; *x = tx; 
}

char* life() {
  const int m = width;
  static char map[width*width], nei[width*width];
  static int init = 0;
  int x,y;
  if (!init) for (x = 0; x < m*m; x++) { map[x] = 0; nei[x] = 0; }
  init = 1;
    int q,w,n;
      for (q = 1; q < m-1; q++) {
        for (w = 1; w < m-1; w++) {
          n = 0;
          n += map[q-1 + (w-1) * m]?1:0;
          n += map[q+1 + (w+1) * m]?1:0;
          n += map[q-1 + (w+1) * m]?1:0;
          n += map[q+1 + (w-1) * m]?1:0;
          n += map[q-1 + (w-0) * m]?1:0;
          n += map[q+1 + (w+0) * m]?1:0;
          n += map[q-0 + (w-1) * m]?1:0;
          n += map[q+0 + (w+1) * m]?1:0;
          nei[q + w * m] = n;}}
      for (q = 1; q < m-1; q++) {
        for (w = 1; w < m-1; w++) {
          int nn  = nei[q + w * m];
          if (map[q + w * m] != 0) {
            if DEATHRULE map[q + w * m] = 0; else map[q+w*m]-=1; }
          else {
            if BIRTHRULE map[q + w * m] = LIVE;}}}
	return map;
}

int main(int argc, char **argv)
{
    sdl_scr = SDL_SetVideoMode(width*SCAL, width*SCAL, 32, SDL_HWSURFACE);
    scr = calloc(width * width * SCAL*SCAL, 4);
    int playing = 0;
	int stepms = 50;
	int leftdn = 0;
	char* map = life();
    for(;;) {
        SDL_PumpEvents();
		
		int q,w,col;
		
		for (q = 0; q < width; q+=PILLAR) {
        for (w = 0; w < width; w+=PILLAR) {
			col = map[q + w * width] = 1000;
		}
		}
		

		for (q = 0; q < width; q++) {
			map[q] = 0;
			map[q+(width-1)*width]=0;
			map[q*width] = 0;
			map[q*width+width-1] = 0;
		}
		
		int leftmb = 0, rightmb = 0;
		int mx,my,mbtns;
		mbtns = SDL_GetMouseState(&mx, &my);
		leftmb = mbtns & SDL_BUTTON(SDL_BUTTON_LEFT);
		rightmb = mbtns & SDL_BUTTON(SDL_BUTTON_RIGHT);
        static unsigned lt = 0; float d = SDL_GetTicks() - lt; lt += d; float dt = d<1?1:d;
		if (playing)
        if (dt<stepms) { SDL_Delay(stepms - dt); dt = stepms; }
		else
		if (dt<16) { SDL_Delay(16 - dt); dt = 16; }
        unsigned char *key = SDL_GetKeyState(NULL);
        if (key[SDLK_ESCAPE]) return;
        if (key[SDLK_SPACE]) { playing = !playing; SDL_Delay(250); }
		if (key[SDLK_KP_PLUS]) stepms++;
		if (key[SDLK_KP_MINUS]) stepms--;
		if (!leftdn & key[SDLK_RIGHT]) { life(); leftdn = 1; }
		else if (!key[SDLK_RIGHT]) leftdn = 0;
		
		if (key[SDLK_r]) {
			int x;
			for (x = 0; x < width*width; x++) { map[x] = _rand() > 32767/2 ? LIVE : 0; }
			SDL_Delay(250);
		}
		
		if (key[SDLK_z]) {
			int x;
			for (x = 0; x < width*width; x++) { map[x] = _rand() > 32767/2 ? map[x] : (_rand() < 32767/8 ? LIVE : 0); }
			SDL_Delay(250);
		}
		
		if (key[SDLK_e]) {
			int x;
			for (x = 0; x < width*width; x++) { map[x] = 0; }
			SDL_Delay(250);
		}
		
		if (playing) life();
		mx /= SCAL; my /= SCAL;
		if (leftmb) {
			map[(mx % width) + (my % width) * width] = 1;
		} else if (rightmb) {
			map[(mx % width) + (my % width) * width] = 0;
		}
		
						
		for (q = 0; q < width; q+=PILLAR) {
        for (w = 0; w < width; w+=PILLAR) {
			col = map[q + w * width] = 1000;
		}
		}

		for (q = 0; q < width; q++) {
        for (w = 0; w < width; w++) {
			col = map[q + w * width] ? 0xffffff : 0;
			if (scr[(q*SCAL) + (w*SCAL) * width * SCAL] != 0 && col == 0) 
				if (scr[(q*SCAL) + (w*SCAL) * width * SCAL] == 0xffffff) col = 0x113344;
				else col = scr[(q*SCAL) + (w*SCAL) * width * SCAL] / 2;
				
			int scx, scy;
			for (scx = 0; scx < SCAL; scx++)
			for (scy = 0; scy < SCAL; scy++)
			{
				scr[(q*SCAL+scx) + (w*SCAL+scy) * width * SCAL] = col;
			}			
		}
		}

        if(SDL_MUSTLOCK(sdl_scr)) if(SDL_LockSurface(sdl_scr)<0) return;
        memcpy(sdl_scr->pixels, scr, width*width*SCAL*SCAL*4);
        if(SDL_MUSTLOCK(sdl_scr)) SDL_UnlockSurface(sdl_scr); 
        SDL_Flip(sdl_scr);
        char buf[256]; sprintf(buf, "+/-:step(%d), space:play/stop, esc:exit, z/r/e - cells", stepms, 1000/dt); SDL_WM_SetCaption(buf, NULL);
    }
}
