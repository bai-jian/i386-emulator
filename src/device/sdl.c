#include "common.h"
#include "sdl.h"
#include "vga.h"

#include <sys/time.h>
#include <signal.h>

extern uint8_t fontdata_8x16[128][16];
SDL_Surface *real_screen;
SDL_Surface *screen;
uint8_t (*pixel_buf) [SCREEN_COL];

#define TIMER_HZ 100

static uint64_t jiffy = 0;
static struct itimerval it;
extern void timer_intr();
extern void keyboard_intr();
extern void update_screen();
static void device_update(int signum) {
	jiffy ++;
	timer_intr();
	if(jiffy % (TIMER_HZ / VGA_HZ) == 0) {
		update_screen();
	}

	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		// If a key was pressed

		uint32_t sym = event.key.keysym.sym;
		if( event.type == SDL_KEYDOWN ) {
			keyboard_intr(sym2scancode[sym >> 8][sym & 0xff]);
		}
		else if( event.type == SDL_KEYUP ) {
			keyboard_intr(sym2scancode[sym >> 8][sym & 0xff] | 0x80);
		}

		// If the user has Xed out the window
		if( event.type == SDL_QUIT ) {
			//Quit the program
			exit(0);
		}
	}

	int ret = setitimer(ITIMER_VIRTUAL, &it, NULL);
	assert(ret == 0);
}

void sdl_clear_event_queue() {
	SDL_Event event;
	while(SDL_PollEvent(&event));
}

void init_sdl() {
	int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
	assert(ret == 0);

	real_screen = SDL_SetVideoMode(640, 400, 8, 
			SDL_HWSURFACE | SDL_HWPALETTE | SDL_HWACCEL | SDL_ASYNCBLIT);

	screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 400, 8,
			real_screen->format->Rmask, real_screen->format->Gmask,
			real_screen->format->Bmask, real_screen->format->Amask);
	pixel_buf = screen->pixels;

	SDL_SetPalette(real_screen, SDL_LOGPAL | SDL_PHYSPAL, (void *)&palette, 0, 256);
	SDL_SetPalette(screen, SDL_LOGPAL, (void *)&palette, 0, 256);

	SDL_WM_SetCaption("NEMU", NULL);

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	struct sigaction s;
	memset(&s, 0, sizeof(s));
	s.sa_handler = device_update;
	ret = sigaction(SIGVTALRM, &s, NULL);
	assert(ret == 0);

	it.it_value.tv_sec = 0;
	it.it_value.tv_usec = 1000000 / TIMER_HZ;
	ret = setitimer(ITIMER_VIRTUAL, &it, NULL);
	assert(ret == 0);
}
