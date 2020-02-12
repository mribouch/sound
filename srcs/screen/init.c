/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppetitea <ppetitea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 23:09:54 by ppetitea          #+#    #+#             */
/*   Updated: 2020/01/12 14:13:45 by ppetitea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "SDL.h"
#include "screen/screen.h"
#include "error/error.h"

t_result	initialize_screen(t_screen *self, size_t width, size_t height)
{
	if (!self)
		return (throw_error("initialize_screen", "NULL pointer provided"));
	self->size.x = width;
	self->size.y = height;
	if (!(self->pixels = (uint32_t*)malloc(sizeof(uint32_t) * width * height)))
		return (throw_error("initialize_screen", "malloc failed"));
	return (OK);
}
