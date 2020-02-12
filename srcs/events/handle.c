/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppetitea <ppetitea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 01:13:53 by ppetitea          #+#    #+#             */
/*   Updated: 2020/01/12 01:23:14 by ppetitea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SDL.h"
#include "game/game.h"
#include "events/keyboard.h"

void	handle_events(t_game *game)
{
	if (game->sdl.event.type == SDL_QUIT)
		game->is_running = FALSE;
	if (game->sdl.event.type == SDL_KEYDOWN)
		handle_keyboard_events(game, game->sdl.event.key.keysym.sym);
}
