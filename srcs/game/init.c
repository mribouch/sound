/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppetitea <ppetitea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 22:36:09 by ppetitea          #+#    #+#             */
/*   Updated: 2020/01/12 21:36:38 by ppetitea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game/game.h"
#include "screen/screen.h"
#include "sdl/sdl_init.h"
#include "error/error.h"

t_game	*initialize_game()
{
	t_game	*self;
	t_usize	size;

	size = ft_usize(640, 480);
	if (!(self = (t_game*)malloc(sizeof(t_game))))
		return(throw_null("initialize_game", "malloc failed"));
	if (!(initialize_sdl(&self->sdl, size.x, size.y)))
		return(throw_null("initialize_game", "sdl initialization failed"));
	if (!initialize_screen(&self->screen, size.x, size.y))
		return(throw_null("initialize_game", "screen initialization failed"));
	self->is_running = FALSE;
	return (self);
}
