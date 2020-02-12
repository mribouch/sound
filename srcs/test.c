/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mribouch <mribouch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 15:54:55 by mribouch          #+#    #+#             */
/*   Updated: 2020/02/11 16:37:58 by mribouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SDL.h"
#include "SDL_mixer.h"
#include <stdio.h>
#include "libft.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitmap/bitmap.h"
#include "error/error.h"

#define HEIGHT 1024
#define WIDTH 1024
#define SOUND_PATH "resources/son/"
#define NUM_CHAN 32
#include <math.h>

int count = 1;

// void		play_sound(Mix_Chunk *sound)
// {
// 	int	i;

// 	i = 1;
// 	while (i < NUM_CHAN)
// 	{
// 		if (Mix_Playing(i) == 0 || Mix_GetChunk(i) == sound)
// 		{
// 			Mix_PlayChannel(i, sound, 0);
// 			i = NUM_CHAN;
// 		}
// 		i++;
// 	}
// }

// void		resume_sound(Mix_Chunk *sound)
// {
// 	int	i;

// 	i = 1;
// 	while (i < NUM_CHAN)
// 	{
// 		if (Mix_Paused(i) == 1 || Mix_GetChunk(i) == sound)
// 		{
// 			Mix_Resume(i);
// 			i = NUM_CHAN;
// 		}
// 		i++;
// 	}
// }

// void		pause_sound(Mix_Chunk *sound)
// {
// 	int	i;

// 	i = 1;
// 	while (i < NUM_CHAN)
// 	{
// 		if (Mix_Playing(i) == 1 || Mix_GetChunk(i) == sound)
// 		{
// 			Mix_Pause(i);
// 			i = NUM_CHAN;
// 		}
// 		i++;
// 	}
// }

void		play_sound(Mix_Chunk *sound, int volume, int rep)
{
	Mix_VolumeChunk(sound, MIX_MAX_VOLUME / volume);
	Mix_PlayChannel(-1, sound, rep);
}

Mix_Chunk	*init_sound(char *name)
{
	Mix_Chunk	*ret;
	if ((ret = Mix_LoadWAV(name)) == NULL)
	{
		printf("Ereur de chargement du son");
		return (NULL);
	}
	// Mix_VolumeChunk(ret, MIX_MAX_VOLUME / volume);
	// free(path);
	//ft_putendl("ou ca");
	return (ret);
}

void	missile(Mix_Chunk *button, Mix_Chunk *launch, Mix_Chunk *explosion)
{
	play_sound(button, 1, 0);
	SDL_Delay(500);
	play_sound(launch, 3, 0);
	SDL_Delay(2500);
	play_sound(explosion, 2, 0);
}

int main(int argc, char **argv)
{
	SDL_Event e;
	SDL_Window		*window = NULL;
	SDL_Renderer	*renderer = NULL;
	int				quit = 0;
	int				statut = EXIT_FAILURE;
	SDL_Texture		*texture = NULL;
	// SDL_AudioSpec	wavSpec;
	// Uint32			wavLength;
	// Uint8			*wavBuffer;
	// Uint8			*tmpbuffer;
	if (0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
		goto Quit;
	if (argc != 2)
		return (0);
	t_bitmap_texture	*img = load_bmp(argv[1]);
	window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (NULL == window)
		goto Quit;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (NULL == renderer)
		goto Quit;
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, WIDTH, HEIGHT);
	// if(SDL_LoadWAV("srcs/gun_fire.wav", &wavSpec, &wavBuffer, &wavLength) == NULL)
	// {
	// 	ft_putendl("AH");
	// 	// return 1;
	// }
	// SDL_AudioDeviceID	deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

	u_int32_t	*screen;
	screen = malloc(sizeof(int) * HEIGHT * WIDTH);
	memset(screen, 0x00000000, WIDTH * HEIGHT * sizeof(uint32_t));
	ft_putendl("on arrive la ?");
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
	{
		printf("%s", Mix_GetError());
	}
	// Mix_Music		*song;
	// if ((song = Mix_LoadMUS("resources/son/doum_noukoum.mp3")) == NULL)
	// {
	// 	printf("failed to load song\n");
	// }
	// Mix_PlayMusic(song, -1); //Jouer infiniment la musique

	Mix_AllocateChannels(NUM_CHAN); //Allouer 32 canaux
	// Mix_Volume(1, MIX_MAX_VOLUME); //Mettre à mi-volume le post 1
	Mix_Chunk	*gunfire;
	Mix_Chunk	*heli;
	Mix_Chunk	*button;
	Mix_Chunk	*launch;
	Mix_Chunk	*explosion;
	Mix_Chunk	*machinegun;
	Mix_Chunk	*music;
	Mix_Chunk	*monster;

	gunfire = init_sound("resources/son/gun_fire.wav");
	heli = init_sound("resources/son/helicopter.wav");
	button = init_sound("resources/son/missile_button.wav");
	launch = init_sound("resources/son/launch_missile2.wav");
	explosion = init_sound("resources/son/explosion.wav");
	machinegun = init_sound("resources/son/machine_gun.wav");
	music = init_sound("resources/son/doum_noukoum.wav");
	monster = init_sound("resources/son/monster/active_monster.wav");
	ft_putendl("avant allocation channels");
	// Mix_VolumeChunk(heli, MIX_MAX_VOLUME);
	// Mix_VolumeChunk(music, MIX_MAX_VOLUME);
	play_sound(music, 1, -1);
	play_sound(heli, 1, -1);
	// Mix_PlayChannel(-1, music, -1);
	// Mix_FadeInChannel(-1, heli, -1, 5000);
	while(!quit)
	{
		// SDL_WarpMouseInWindow(window, WIDTH / 2, HEIGHT / 2);
		SDL_UpdateTexture(texture, NULL, img->pixels, WIDTH * sizeof(uint32_t));
		while(SDL_PollEvent(&e))
		{
			printf("%d channels are playing\n", Mix_Playing(-1));
			if (Mix_Paused(6) != 1)
				Mix_Pause(6);
			if (e.type == SDL_QUIT) {quit = 1;}

			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_SPACE)
				{
					// Mix_PlayChannel(1, gunfire, 0);
					play_sound(gunfire, 1, 0);
					printf("wola\n");
				}
				if (e.key.keysym.sym == SDLK_x)
				{
					// if (Mix_PausedMusic() == 1)
					// 	Mix_ResumeMusic();
					// else if (Mix_PlayingMusic() == 1)
					// 	Mix_PauseMusic();
					if (Mix_Paused(28) == 1)
						Mix_Resume(28);
					else if (Mix_Playing(28) == 1)
						Mix_Pause(28);
				}
				if (e.key.keysym.sym == SDLK_r)
					count++;
				if (e.key.keysym.sym == SDLK_f)
					play_sound(monster, count, 0);
				if (e.key.keysym.sym == SDLK_UP)
					;
				if (e.key.keysym.sym == SDLK_DOWN)
					;
				if (e.key.keysym.sym == SDLK_LEFT)
					;
				if (e.key.keysym.sym == SDLK_RIGHT)
					;
				if (e.key.keysym.sym == SDLK_ESCAPE)
					quit = 1;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					missile(button, launch, explosion);
				}
				if (e.button.button == SDL_BUTTON_RIGHT)
				{
					if (Mix_Paused(6) == 1)
						resume_sound(machinegun);
					play_sound(machinegun, 1, -1);
				}
			}
			if (e.type == SDL_MOUSEMOTION)
			{
				if (e.button.x > WIDTH / 2)
					;
				if (e.button.x < WIDTH / 2)
					;
				if (e.button.y > HEIGHT / 2)
					;
				if (e.button.y < HEIGHT / 2)
					;
			}
		}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
Quit:
	if(NULL != renderer)
		SDL_DestroyRenderer(renderer);
	if(NULL != texture)
		SDL_DestroyTexture(texture);
	if(NULL != window)
		SDL_DestroyWindow(window);
	Mix_CloseAudio();
	SDL_Quit();
	return statut;
}
