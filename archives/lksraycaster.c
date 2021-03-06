#include "SDL.h"
#include <stdio.h>
#include "libft.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitmap/bitmap.h"
#include "error/error.h"

#define RATIO 16 / 9
#define HEIGHT 180
#define WIDTH (HEIGHT * RATIO) 
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

int pp_putpixel(uint32_t *pixel, int x, int y, SDL_Color color)
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
	pixel[y * WIDTH + x] = c; 
	return (0);
}

void	pp_liner(uint32_t *pixel, t_point *a, t_point *b, SDL_Color color)
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
	while(height--)
		pixels[(HEIGHT - height - 1) * WIDTH + x] = color;
}

void	render(uint32_t *pixels, int *heightmap, uint32_t *img, t_point player, float ag, int renderdistance)
{
	(void)pixels;
	(void)heightmap;
	(void)player.x;
	(void)ag;
	(void)renderdistance;
	memset(pixels, 0xFFFFFFFF, WIDTH * HEIGHT * (sizeof(uint32_t)));

	float scale;
	(void) scale;
	float cosag = cos(ag);
	float sinag = sin(ag);
	(void) cosag;
	(void) sinag;
	float value;
	int columny;
	int columnx;
	float dx = 90 * M_PI / 180;
	float ad;
	ad = dx / WIDTH;
	dx = ad;
	float gd;
	while(renderdistance > 10)
	{		
		for(int i = 0; i < WIDTH; i++)
		{
			columny = sin(ag + dx) * (float)renderdistance;
			columnx = cos(ag + dx) * (float)renderdistance;
			value = (float)heightmap[columny * WIDTH + columnx];
			gd = (float)heightmap[columny * WIDTH + columnx];
			value *= HEIGHT;
			value /= 255;
			//la couleur c pas ouf la
			////verifie la valeur de img[gd] stp	
			draw_column(pixels, i, value, img[(int)gd]);	
			dx += ad;
		}
		renderdistance--;
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

	if (argc != 2)
		return (0);
	t_bitmap_texture *img = load_bmp(argv[1]);
	uint32_t *map = img->pixels;

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

	for(int i = 0; i < WIDTH * HEIGHT; i++)
		hm[i] = get_blue(map[i]);	
	//for(int i = 0; i < WIDTH * HEIGHT; i++)
	//	printf("%d\n",hm[i]);	
	float ag = 0;
	while(!quit)
	{
		SDL_UpdateTexture(texture, NULL, screen, WIDTH * sizeof(uint32_t));
		while(SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) {quit = 1;}
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_SPACE)
					memset(screen, 0x00000000, WIDTH * HEIGHT * sizeof(uint32_t));
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
		render(screen, hm,img->pixels, player, ag, 30);
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
