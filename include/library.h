/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   library.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarra <jcarra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/31 14:46:47 by jcarra            #+#    #+#             */
/*   Updated: 2018/01/31 14:47:35 by jcarra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBRARY_H
# define LIBRARY_H

/*
** -- library_configuration
*/
typedef struct	s_internal_context
{
	t_bool		initialized;

	t_bool		debug_mode;

	t_table		table;
}				t_internal_context;

extern t_internal_context		g_internal_context;

/*
**	-- Function
**
**	library.c
*/
t_bool			internal_context_is_initialize (void);
void			internal_context_initialize (void);

#endif
