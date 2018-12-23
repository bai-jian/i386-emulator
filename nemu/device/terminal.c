#include "device/terminal.h"
#include <SDL/SDL.h>
#include <sys/time.h>

/* keyboard scancode set 2
 * http://www.mouseos.com/os/doc/scan_code.html */
enum
{
	UNDEF, K_ESC, K_1, K_2, K_3, K_4, K_5, K_6, 
	K_7, K_8, K_9, K_0, K_MINUS, K_EQUALS, K_BACK, K_TAB,
	K_q, K_w, K_e, K_r, K_t, K_y, K_u, K_i,
	K_o, K_p, K_LEFTBRACKET, K_RIGHTBRACKET, K_ENTER, K_LCTRL, K_a, K_s,
	K_d, K_f, K_g, K_h, K_j, K_k, K_l, K_SEMICOLON,
	K_QUOTE, K_BACKQUOTE, K_LSHIFT, K_BACKSLASH, K_z, K_x, K_c, K_v,
	K_b, K_n, K_m, K_COMMA, K_PERIOD, K_SLASH, K_RSHIFT, /*UNDEF, */
	K_LALT = 0x38, K_SPACE, K_CAPSLOCK, K_F1, K_F2, K_F3, K_F4, K_F5,
	K_F6, K_F7, K_F8, K_F9, K_F10, K_NUMLOCK, K_SCROLLOCK, K_HOME, 
	K_UP, K_PAGEUP, /*UNDEF,*/ K_LEFT = 0x4b, /*UNDEF,*/ K_RIGHT = 0x4d, /*UNDEF,*/ K_END = 0x4f, 
	K_DOWN, K_PAGEDOWN, K_INSERT, K_DELETE, /*UNDEF, UNDEF, UNDEF,*/ K_F11 = 0x57,
	K_F12
};

const static uint8_t sym2scancode[2][128] =
{
	{
		UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, 
		K_BACK , K_TAB , UNDEF, UNDEF, UNDEF /*SDLK_CLEAR*/, K_ENTER, UNDEF, UNDEF, 
		UNDEF, UNDEF, UNDEF, UNDEF /*SDLK_PAUSE*/, UNDEF, UNDEF, UNDEF, UNDEF, 
		UNDEF, UNDEF, UNDEF, K_ESC, UNDEF, UNDEF, UNDEF, UNDEF, 
		K_SPACE, UNDEF /*SDLK_EXCLAIM*/, UNDEF /*SDLK_QUOTEDBL*/, UNDEF /*SDLK_HASH*/, UNDEF /*SDLK_DOLLAR*/, UNDEF, UNDEF /*SDLK_AMPERSAND*/, K_QUOTE, 
		UNDEF /*SDLK_LEFTPAREN*/, UNDEF /*SDLK_RIGHTPAREN*/, UNDEF /*SDLK_ASTERISK*/, UNDEF /*SDLK_PLUS*/, K_COMMA, K_MINUS, K_PERIOD, K_SLASH, 
		K_0, K_1, K_2, K_3, K_4, K_5, K_6, K_7, 
		K_8, K_9, UNDEF /*SDLK_COLON*/, K_SEMICOLON, UNDEF /*SDLK_LESS*/, K_EQUALS, UNDEF /*K_GREATER*/, UNDEF /*K_QUESTION*/, 
		UNDEF /*SDLK_AT*/, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, 
		UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, 
		UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, 
		UNDEF, UNDEF, UNDEF, K_LEFTBRACKET, K_BACKSLASH, K_RIGHTBRACKET, UNDEF /*SDLK_CARET*/, UNDEF /*SDLK_UNDERSCORE*/, 
		K_BACKQUOTE, K_a, K_b, K_c, K_d, K_e, K_f, K_g, 
		K_h, K_i, K_j, K_k, K_l, K_m, K_n, K_o, 
		K_p, K_q, K_r, K_s, K_t, K_u, K_v, K_w, 
		K_x, K_y, K_z, UNDEF, UNDEF, UNDEF, UNDEF, K_DELETE
	},
	{
		UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, 
		UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, 
		/*
		SDLK_KP0, SDLK_KP1, SDLK_KP2, SDLK_KP3, SDLK_KP4, SDLK_KP5, SDLK_KP6, SDLK_KP7, 
		SDLK_KP8, SDLK_KP9, SDLK_KP_PERIOD, SDLK_KP_DIVIDE, SDLK_KP_MULTIPLY, SDLK_KP_MINUS, SDLK_KP_PLUS, SDLK_KP_ENTER, 
		*/

		UNDEF /*SDLK_KP_EQUALS*/, K_UP, K_DOWN, K_RIGHT, K_LEFT, K_INSERT, K_HOME, K_END, 
		K_PAGEUP, K_PAGEDOWN, K_F1, K_F2, K_F3, K_F4, K_F5, K_F6, 
		K_F7, K_F8, K_F9, K_F10, K_F11, K_F12, UNDEF /*SDLK_F13*/, UNDEF /*K_F14*/, 
		UNDEF /*SDLK_F15*/, UNDEF, UNDEF, UNDEF, K_NUMLOCK, K_CAPSLOCK, K_SCROLLOCK, K_RSHIFT, 
		K_LSHIFT, K_LCTRL, K_LCTRL, K_LALT, K_LALT, UNDEF /*SDLK_RMETA*/, UNDEF /*SDLK_LMETA*/, UNDEF /*SDLK_LSUPER*/, 
		UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, 
		/* SDLK_RSUPER, SDLK_MODE, UNDEF, SDLK_HELP, SDLK_PRINT, SDLK_SYSREQ, SDLK_BREAK, SDLK_MENU, */
		/* SDLK_POWER, SDLK_EURO, */
	}
};

union color_t
{
	uint32_t val;
	struct
	{
		uint8_t r, g, b, a;
	};
};

static union color_t palette[] =
{
	{0x000000}, {0x0000a8}, {0x00a800}, {0x00a8a8}, {0xa80000}, {0xa800a8}, {0xa85400}, {0xa8a8a8}, 
	{0x545454}, {0x5454fc}, {0x54fc54}, {0x54fcfc}, {0xfc5454}, {0xfc54fc}, {0xfcfc54}, {0xfcfcfc}, 
	{0x000000}, {0x141414}, {0x202020}, {0x2c2c2c}, {0x383838}, {0x444444}, {0x505050}, {0x606060}, 
	{0x707070}, {0x808080}, {0x909090}, {0xa0a0a0}, {0xb4b4b4}, {0xc8c8c8}, {0xe0e0e0}, {0xfcfcfc}, 
	{0x0000fc}, {0x4000fc}, {0x7c00fc}, {0xbc00fc}, {0xfc00fc}, {0xfc00bc}, {0xfc007c}, {0xfc0040}, 
	{0xfc0000}, {0xfc4000}, {0xfc7c00}, {0xfcbc00}, {0xfcfc00}, {0xbcfc00}, {0x7cfc00}, {0x40fc00}, 
	{0x00fc00}, {0x00fc40}, {0x00fc7c}, {0x00fcbc}, {0x00fcfc}, {0x00bcfc}, {0x007cfc}, {0x0040fc}, 
	{0x7c7cfc}, {0x9c7cfc}, {0xbc7cfc}, {0xdc7cfc}, {0xfc7cfc}, {0xfc7cdc}, {0xfc7cbc}, {0xfc7c9c}, 
	{0xfc7c7c}, {0xfc9c7c}, {0xfcbc7c}, {0xfcdc7c}, {0xfcfc7c}, {0xdcfc7c}, {0xbcfc7c}, {0x9cfc7c}, 
	{0x7cfc7c}, {0x7cfc9c}, {0x7cfcbc}, {0x7cfcdc}, {0x7cfcfc}, {0x7cdcfc}, {0x7cbcfc}, {0x7c9cfc}, 
	{0xb4b4fc}, {0xc4b4fc}, {0xd8b4fc}, {0xe8b4fc}, {0xfcb4fc}, {0xfcb4e8}, {0xfcb4d8}, {0xfcb4c4}, 
	{0xfcb4b4}, {0xfcc4b4}, {0xfcd8b4}, {0xfce8b4}, {0xfcfcb4}, {0xe8fcb4}, {0xd8fcb4}, {0xc4fcb4}, 
	{0xb4fcb4}, {0xb4fcc4}, {0xb4fcd8}, {0xb4fce8}, {0xb4fcfc}, {0xb4e8fc}, {0xb4d8fc}, {0xb4c4fc}, 
	{0x000070}, {0x1c0070}, {0x380070}, {0x540070}, {0x700070}, {0x700054}, {0x700038}, {0x70001c}, 
	{0x700000}, {0x701c00}, {0x703800}, {0x705400}, {0x707000}, {0x547000}, {0x387000}, {0x1c7000}, 
	{0x007000}, {0x00701c}, {0x007038}, {0x007054}, {0x007070}, {0x005470}, {0x003870}, {0x001c70}, 
	{0x383870}, {0x443870}, {0x543870}, {0x603870}, {0x703870}, {0x703860}, {0x703854}, {0x703844}, 
	{0x703838}, {0x704438}, {0x705438}, {0x706038}, {0x707038}, {0x607038}, {0x547038}, {0x447038}, 
	{0x387038}, {0x387044}, {0x387054}, {0x387060}, {0x387070}, {0x386070}, {0x385470}, {0x384470}, 
	{0x505070}, {0x585070}, {0x605070}, {0x685070}, {0x705070}, {0x705068}, {0x705060}, {0x705058}, 
	{0x705050}, {0x705850}, {0x706050}, {0x706850}, {0x707050}, {0x687050}, {0x607050}, {0x587050}, 
	{0x507050}, {0x507058}, {0x507060}, {0x507068}, {0x507070}, {0x506870}, {0x506070}, {0x505870}, 
	{0x000040}, {0x100040}, {0x200040}, {0x300040}, {0x400040}, {0x400030}, {0x400020}, {0x400010}, 
	{0x400000}, {0x401000}, {0x402000}, {0x403000}, {0x404000}, {0x304000}, {0x204000}, {0x104000}, 
	{0x004000}, {0x004010}, {0x004020}, {0x004030}, {0x004040}, {0x003040}, {0x002040}, {0x001040}, 
	{0x202040}, {0x282040}, {0x302040}, {0x382040}, {0x402040}, {0x402038}, {0x402030}, {0x402028}, 
	{0x402020}, {0x402820}, {0x403020}, {0x403820}, {0x404020}, {0x384020}, {0x304020}, {0x284020}, 
	{0x204020}, {0x204028}, {0x204030}, {0x204038}, {0x204040}, {0x203840}, {0x203040}, {0x202840}, 
	{0x2c2c40}, {0x302c40}, {0x342c40}, {0x3c2c40}, {0x402c40}, {0x402c3c}, {0x402c34}, {0x402c30}, 
	{0x402c2c}, {0x40302c}, {0x40342c}, {0x403c2c}, {0x40402c}, {0x3c402c}, {0x34402c}, {0x30402c}, 
	{0x2c402c}, {0x2c4030}, {0x2c4034}, {0x2c403c}, {0x2c4040}, {0x2c3c40}, {0x2c3440}, {0x2c3040}, 
	{0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, 
};

#define SCREEN_ROW 400
#define SCREEN_COL 640
#define TIMER_HZ 100
#define VGA_HZ 25

static SDL_Surface *sdl_real_screen;
static SDL_Surface *sdl_screen;
static uint8_t (*sdl_pixels) [SCREEN_COL];
struct itimerval sdl_timer;

static void _terminal_update(int signum)
{
	terminal.timer_irq();

	static uint64_t jiffy = 0;
	jiffy ++;
	if (jiffy % (TIMER_HZ / VGA_HZ) == 0 && *terminal.vga_vmem_dirty)
	{
		int i, j;
		for (i = 0; i < CTR_ROW; i ++)
		{
			if (!terminal.vga_vmem_line_dirty[i])
				continue;
			for (j = 0; j < CTR_COL; j ++)
			{
				uint8_t color_idx = terminal.vga_vmem[i * CTR_COL + j];
				sdl_pixels[2 * i][2 * j] = color_idx;
				sdl_pixels[2 * i][2 * j + 1] = color_idx;
				sdl_pixels[2 * i + 1][2 * j] = color_idx;
				sdl_pixels[2 * i + 1][2 * j + 1] = color_idx;
			}

			SDL_Rect rect;
			rect.x = 0;
			rect.y = i * 2;
			rect.w = CTR_COL * 2;
			rect.h = 2;
			SDL_BlitSurface(sdl_screen, &rect, sdl_real_screen, &rect);
		}
		SDL_Flip(sdl_real_screen);

		*terminal.vga_vmem_dirty = false;
		memset(terminal.vga_vmem_line_dirty, false, CTR_ROW);
	}

	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		uint32_t sym = event.key.keysym.sym;
		if (event.type == SDL_KEYDOWN)
			*terminal.keyboard_scancode = sym2scancode[sym >> 8][sym & 0xff];
		else if (event.type == SDL_KEYUP)
			*terminal.keyboard_scancode = sym2scancode[sym >> 8][sym & 0xff] | 0x80;
		terminal.keyboard_irq();

		// If the user has Xed out the window
		if (event.type == SDL_QUIT)
			exit(0);
	}

	int ret = setitimer(ITIMER_VIRTUAL, &sdl_timer, NULL);
	assert(ret == 0);
}

void terminal_init()
{
	int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
	assert(ret == 0);

	sdl_real_screen = SDL_SetVideoMode(640, 400, 8, SDL_HWSURFACE | SDL_HWPALETTE | SDL_HWACCEL | SDL_ASYNCBLIT);
	sdl_screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 400, 8,
			sdl_real_screen->format->Rmask,
			sdl_real_screen->format->Gmask,
			sdl_real_screen->format->Bmask,
			sdl_real_screen->format->Amask);
	sdl_pixels = sdl_screen->pixels;

	SDL_SetPalette(sdl_real_screen, SDL_LOGPAL | SDL_PHYSPAL, (void *)&palette, 0, 256);
	SDL_SetPalette(sdl_screen, SDL_LOGPAL, (void *)&palette, 0, 256);

	SDL_WM_SetCaption("NEMU", NULL);

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	struct sigaction s;
	memset(&s, 0, sizeof(s));
	s.sa_handler = _terminal_update;
	ret = sigaction(SIGVTALRM, &s, NULL);
	assert(ret == 0);

	sdl_timer.it_value.tv_sec = 0;
	sdl_timer.it_value.tv_usec = 1000000 / TIMER_HZ;
	ret = setitimer(ITIMER_VIRTUAL, &sdl_timer, NULL);
	assert(ret == 0);	
}

void terminal_clear()
{
	SDL_Event event;
	while(SDL_PollEvent(&event));
}

