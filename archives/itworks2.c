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
/*
    if (ytop > HEIGHT)
        ytop = HEIGHT - 1;
    if (ybot < 1)
        ybot = 1;
    if (ybot > HEIGHT)
        ybot = HEIGHT - 1;
*/

	a.x = x;
	b.x = x;

	a.y = ytop;
	b.y = ybot;

	pp_liner(pixels, &a, &b, color);
}
/*
   void	render(uint32_t *pixels, int *heightmap, uint32_t *img, t_point player, float ag, int renderdistance)
   {
   memset(pixels, 0xFFFFFFFF, WIDTH * HEIGHT * (sizeof(uint32_t)));
   float value;
   int columny;
   int columnx;
   float dx = 60 * M_PI / 180;
   float ad;
   ad = dx / WIDTH;
   dx = ad;
   dx = 0;
   float gd;
   while(renderdistance > 0)
   {		
   for(int i = 0; i < WIDTH; i++)
   {
   columny = sin(ag + dx) * (float)renderdistance + player.y;
   columnx = cos(ag + dx) * (float)renderdistance + player.x;
   value = (float)heightmap[columny * WIDTH + columnx];
   gd = (float)heightmap[columny * WIDTH + columnx];
   value *= HEIGHT;
   value /= 255;
//draw_column(pixels, i, value, 0xFFAAAAAA - renderdistance);	
draw_column(pixels, i, value, img[(int)gd]);	
dx += ad;
}
renderdistance--;
}
}
 */


void	render2(uint32_t *pixels, int *hm, uint32_t *colormap, t_point player, float phi, int height, int horizon, int scale_height, int distance)
{
    memset(pixels, 0xFFFFFFFF, sizeof(uint32_t) * WIDTH * HEIGHT);
	int mapwidthperiod = WIDTH - 1;
	int mapheightperiod = HEIGHT - 1;

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
	while(!quit)
	{
		SDL_UpdateTexture(texture, NULL, screen, WIDTH * sizeof(uint32_t));
		while(SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) {quit = 1;}
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_SPACE)
				    height++;
				if (e.key.keysym.sym == SDLK_x)
				    height--;
				if (e.key.keysym.sym == SDLK_r)
				    horizon++;
				if (e.key.keysym.sym == SDLK_f)
				    horizon--;
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
		}
		//render(screen, hm,colormap, player, ag, 300);
		render2(screen, hm, colormap, player, ag, height, horizon, 120, 300);
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
