/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppetitea <ppetitea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 22:07:04 by ppetitea          #+#    #+#             */
/*   Updated: 2020/01/12 23:33:22 by ppetitea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"index.h"
#include	"libft.h"
#include	<stdio.h>
#include	<stdlib.h>

t_result	debug(t_game *game)
{
	static	t_bool			init = TRUE;
	static t_bitmap_texture	*bmp;

	if (init)
	{
		if (!(bmp = load_bmp("resources/textures/tmp/pepe2.bmp")))
			return (throw_error("debug", "failed to load bmp"));
		init = FALSE;
	}
	fill_screen(ft_vec2i(0, 0), game->screen, *bmp);
	return (OK);
}
