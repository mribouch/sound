/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppetitea <ppetitea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 01:17:28 by ppetitea          #+#    #+#             */
/*   Updated: 2020/01/12 01:22:50 by ppetitea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game/game.h"

void	handle_keyboard_events(t_game *game, SDL_Keycode key)
{
	if (key == SDLK_ESCAPE)
		game->is_running = FALSE;
}
