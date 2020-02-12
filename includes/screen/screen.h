/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screen.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppetitea <ppetitea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 22:55:08 by ppetitea          #+#    #+#             */
/*   Updated: 2020/01/12 23:33:18 by ppetitea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef screen_H
# define screen_H

# include "libft.h"
# include "SDL.h"
# include "bitmap/bitmap.h"

typedef struct			s_screen
{
	t_usize				size;
	uint32_t			*pixels;
}						t_screen;

t_result	initialize_screen(t_screen *self, size_t width, size_t height);
void		fill_screen(t_vec2i offset, t_screen screen, t_bitmap_texture bmp);

#endif