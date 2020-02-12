/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppetitea <ppetitea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 23:31:27 by ppetitea          #+#    #+#             */
/*   Updated: 2020/01/12 23:32:04 by ppetitea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "screen/screen.h"
#include "bitmap/bitmap.h"

void	fill_screen(t_vec2i offset, t_screen screen, t_bitmap_texture bmp)
{
	size_t	x_start;
	size_t	y_start;
	size_t	x;
	size_t	y;

	x_start = (offset.x >= 0) ? 0 : -offset.x;
	y_start = (offset.y >= 0) ? 0 : -offset.y;
	y = y_start;
	while (offset.y + y < screen.size.y && y < (size_t)bmp.head.height)
	{
		x = x_start;
		while (offset.x + x < screen.size.x && x < (size_t)bmp.head.width)
		{
			screen.pixels[(offset.x + x) + (offset.y + y) * screen.size.x]
				= bmp.pixels[x + y * bmp.head.width];
			x++;
		}
		y++;
	}
}
