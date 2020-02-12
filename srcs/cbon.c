#include "SDL.h"
#include <stdio.h>
#include "libft.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitmap/bitmap.h"
#include "error/error.h"

//#define RATIO 16 / 9
#define HEIGHT 1024
#define WIDTH 1024
//#define WIDTH (HEIGHT * RATIO) 
#include <math.h>

typedef struct s_point
{
	int x;
	int y;
} t_point;

uint32_t	pp_get_SDLcolor(SDL_Color color)
{
	uint32_t c;
	c = 0xFFFFFFFF;
	c = c | color.a;
	c = c << 8;
	c = c | color.r;
	c = c << 8;
	c = c | color.g;
	c = c << 8;
	c = c | color.b;
	return (c);
}

int pp_putpixel(uint32_t *pixel, int x, int y, uint32_t color)
{
	if(y < 0 || y > HEIGHT)
		return (0);
	if (x < 0 || x > WIDTH)
		return (0);
	pixel[y * WIDTH + x] = color; 
	return (0);
}

void	pp_liner(uint32_t *pixel, t_point *a, t_point *b, uint32_t color)
{
	int tab[10];

	tab[0] = a->x;
	tab[1] = b->x;
	tab[2] = a->y;
	tab[3] = b->y;
	tab[4] = abs(tab[1] - tab[0]);
	tab[5] = tab[0] < tab[1] ? 1 : -1;
	tab[6] = abs(tab[3] - tab[2]);
	tab[7] = tab[2] < tab[3] ? 1 : -1;
	tab[8] = (tab[4] > tab[6] ? tab[4] : -tab[6]) / 2;
	while (!(tab[0] == tab[1] && tab[2] == tab[3]))
	{
		pp_putpixel(pixel, tab[0], tab[2], color);
		tab[9] = tab[8];
		tab[8] -= (tab[9] > -tab[4]) ? tab[6] : 0;
		tab[0] += (tab[9] > -tab[4]) ? tab[5] : 0;
		tab[8] += (tab[9] < tab[6]) ? tab[4] : 0;
		tab[2] += (tab[9] < tab[6]) ? tab[7] : 0;
	}
}

int cast_ray(uint32_t *pixels, t_point *a, float angle)
{
    int x0 = a->x;
    int x1 = a->x + 5000 * cos(angle);
    int y0 = a->y;
    int y1 = a->y - 5000 * sin(angle);

    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = (dx>dy ? dx : -dy)/2, e2;

    while(!(x0==x1 && y0==y1))
    {
        if (pixels[y0 * WIDTH + x0] == 1)
        {
            int adj = x0 - a->x;
            int opp = y0 - a->y;
            return(sqrt((adj * adj) + (opp * opp)));
        }
        e2 = err;
        if (e2 >-dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
    return(0);
}

int	get_blue(uint32_t color)
{
	int c;
	color = color << 24;
	color = color >> 24;
	c = (int) color;
	return (c);
}

void	draw_column(uint32_t *pixels, int x, int height, uint32_t color)
{
	if (height > HEIGHT)
		height = HEIGHT;
	if (height < 0)
		height = 0;
	while(height--)
		pixels[(HEIGHT - height - 1) * WIDTH + x] = color;
}

void	draw_vertical_line(uint32_t *pixels, int x, int ytop, int ybot, uint32_t color)
{
	t_point a;
	t_point b;


	if (ytop > ybot)
		return ;
	if (ytop < 0)
		ytop = 0;
	a.x = x;
	b.x = x;
	a.y = ytop;
	b.y = ybot;

	pp_liner(pixels, &a, &b, color);
}

uint32_t nocturne(uint32_t c)
{
	return((c & 255) << 8);
}

unsigned char get_r(uint32_t c)
{
	return(c & 0xFF);
}


unsigned char get_g(uint32_t c)
{
	return((c >> 8) & 0xFF);
}


unsigned char get_b(uint32_t c)
{
	return((c >> 16) & 0xFF);
}

void	draw_bg(uint32_t *pixels)
{
	uint32_t c;

	c = 0xFF181818;
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
			pixels[i * WIDTH + j] = c;
}

void	render2(uint32_t *pixels, int *hm, uint32_t *colormap, t_point player, float phi, int height, int horizon, int scale_height, int distance)
{
	memset(pixels, 0xFFFFFFFF, sizeof(uint32_t) * WIDTH * HEIGHT);
	draw_bg(pixels);
	int mapwidthperiod = WIDTH - 1;
	int mapheightperiod = HEIGHT - 1;

	(void)scale_height;

	float sinang = sin(phi);
	float cosang = cos(phi);

	uint32_t hiddeny[WIDTH];

	for (int i = 0; i < WIDTH; i++)
		hiddeny[i] = 0;

	for(int i = 0; i < WIDTH; i++)
		hiddeny[i] = HEIGHT;

	float deltaz = 1;

	for(float z=1; z < distance; z += deltaz)
	{
		float plx =  -cosang * z - sinang * z;
		float ply =   sinang * z - cosang * z;
		float prx =   cosang * z - sinang * z;
		float pry =  -sinang * z - cosang * z;

		float dx = (prx - plx) / WIDTH;
		float dy = (pry - ply) / WIDTH;
		plx += player.x;
		ply += player.y;
		float invz = 1 / z * 240;
		for(int i=0; i< WIDTH; i++)
		{
			int mapoffset = (((int)floorf(ply) & (int)mapwidthperiod) << 10) + (((int)floorf(plx)) & ((int)mapheightperiod));
			float heightonscreen = (height - hm[mapoffset]) * invz + horizon;
			draw_vertical_line(pixels, i, heightonscreen, hiddeny[i], colormap[mapoffset]);
			if (heightonscreen < hiddeny[i]) hiddeny[i] = heightonscreen;
			plx += dx;
			ply += dy;
		}
		deltaz += 0.005;
	}
}

static const char *arrow[] = {
  /* width height num_colors chars_per_pixel */
  "    32    32        3            1",
  /* colors */
  "X c #00FF00",
  ". c #000000",
  "  c None",
  /* pixels */
  "X                               ",
  "XX                              ",
  "X.X                             ",
  "X..X                            ",
  "X...X                           ",
  "X....X                          ",
  "X.....X                         ",
  "X......X                        ",
  "X.......X                       ",
  "X........X                      ",
  "X.....XXXXX                     ",
  "X..X..X                         ",
  "X.X X..X                        ",
  "XX  X..X                        ",
  "X    X..X                       ",
  "     X..X                       ",
  "      X..X                      ",
  "      X..X                      ",
  "       XX                       ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "0,0"
};

static SDL_Cursor *init_system_cursor(const char *image[])
{
  int i, row, col;
  Uint8 data[4*32];
  Uint8 mask[4*32];
  int hot_x, hot_y;

  i = -1;
  for (row=0; row<32; ++row) {
    for (col=0; col<32; ++col) {
      if (col % 8) {
        data[i] <<= 1;
        mask[i] <<= 1;
      } else {
        ++i;
        data[i] = mask[i] = 0;
      }
      switch (image[4+row][col]) {
        case 'X':
          data[i] |= 0x01;
          mask[i] |= 0x01;
          break;
        case '.':
          mask[i] |= 0x01;
          break;
        case ' ':
          break;
      }
    }
  }
  sscanf(image[4+row], "%d,%d", &hot_x, &hot_y);
  return SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
}

int main(int argc, char **argv)
{
	SDL_Event e;
	int quit = 0;
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	int statut = EXIT_FAILURE;
	SDL_Texture *texture = NULL;

	if (argc != 3)
		return (0);
	t_bitmap_texture *img = load_bmp(argv[1]);
	t_bitmap_texture *texturemap = load_bmp(argv[2]);
	uint32_t *map = img->pixels;
	uint32_t *colormap = texturemap->pixels;

	if(0 != SDL_Init(SDL_INIT_VIDEO))
		goto Quit;
	window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if(NULL == window)
		goto Quit;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if(NULL == renderer)
		goto Quit;
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, WIDTH, HEIGHT);

	//SDL_ShowCursor(SDL_DISABLE);
	SDL_SetCursor(init_system_cursor(arrow));
	SDL_RenderClear(renderer);

	uint32_t *screen;
	screen = malloc(sizeof(uint32_t) * HEIGHT * WIDTH );
	//memset(screen, 255, WIDTH * HEIGHT * (sizeof(uint32_t)));

	int *hm;
	hm = malloc(sizeof(int) * HEIGHT * WIDTH);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);

	memset(screen, 0x00000000, WIDTH * HEIGHT * sizeof(uint32_t));
	memset(hm, 0, WIDTH * HEIGHT * sizeof(int));

	t_point player;
	player.x = WIDTH/2;
	player.y = HEIGHT/2;

	(void)player.x;

	for(int i = 0; i < WIDTH * HEIGHT - 1; i++)
		hm[i] = get_blue(map[i]);	
	float ag = 0;
	int height = 50;
	int horizon = 120;
	
	float incx = 0;
	int incy = 0;
	(void)incx;
	(void)incy;

	while(!quit)
	{
		SDL_WarpMouseInWindow(window, WIDTH / 2, HEIGHT / 2);
		SDL_UpdateTexture(texture, NULL, screen, WIDTH * sizeof(uint32_t));
		while(SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) {quit = 1;}
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_SPACE)
					height += 5;
				if (e.key.keysym.sym == SDLK_x)
					height -= 5;
				if (e.key.keysym.sym == SDLK_r)
					horizon += 5;
				if (e.key.keysym.sym == SDLK_f)
					horizon -= 5;
				if (e.key.keysym.sym == SDLK_UP)
					player.y--;
				if (e.key.keysym.sym == SDLK_DOWN)
					player.y++;
				if (e.key.keysym.sym == SDLK_LEFT)
					player.x--;
				if (e.key.keysym.sym == SDLK_RIGHT)
					player.x++;
				if (e.key.keysym.sym == SDLK_ESCAPE)
					quit = 1;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
					ag += 0.1;
				if (e.button.button == SDL_BUTTON_RIGHT)
					ag -= 0.1;
			}
			if (e.type == SDL_MOUSEMOTION)
			{
				if (e.button.x > WIDTH / 2)
					ag -= 0.01 * (e.button.x - WIDTH / 2);
				if (e.button.x < WIDTH / 2)
					ag += 0.01 * (WIDTH/2 - e.button.x );
				if (e.button.y > HEIGHT / 2)
					horizon -= 10;
				if (e.button.y < HEIGHT / 2)
					horizon += 10;
			}
		}
		//render(screen, hm,colormap, player, ag, 300);
		render2(screen, hm, colormap, player, ag, height, horizon, 120, 3000);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	statut = EXIT_SUCCESS;

Quit:
	if(NULL != renderer)
		SDL_DestroyRenderer(renderer);
	if(NULL != texture)
		SDL_DestroyTexture(texture);
	if(NULL != window)
		SDL_DestroyWindow(window);
	SDL_Quit();
	return statut;
}
