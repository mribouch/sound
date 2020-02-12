/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_init.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppetitea <ppetitea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 23:51:42 by ppetitea          #+#    #+#             */
/*   Updated: 2020/01/12 21:29:55 by ppetitea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SDL_INIT_H
# define SDL_INIT_H

# include "SDL.h"
# include "libft.h"

// # define SDL_OK 0
# define SDL_ERROR -1

typedef struct			s_sdl
{
	SDL_Window			*window;
	SDL_Renderer		*renderer;
	SDL_Texture			*texture;
	SDL_Event			event;
}						t_sdl;

t_result				initialize_sdl(t_sdl *self, size_t width, size_t height);

#endif