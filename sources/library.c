/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   library.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarra <jcarra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/31 13:37:53 by jcarra            #+#    #+#             */
/*   Updated: 2018/01/31 14:46:21 by jcarra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "error.h"
#include "types.h"
#include "philo.h"
#include "library.h"

t_internal_context			g_internal_context = {
	.initialized = FALSE,
	.debug_mode = FALSE
};

/*
**	internal_context_is_initialize()
**
**	@param	none
**
**	@error	none
**
**	@return	TRUE in success, FALSE otherwise
*/

extern t_bool		internal_context_is_initialize(void)
{
	return (g_internal_context.initialized);
}

/*
**	internal_context_initialize()
**
**	@param	none
**
**	@error	none
*/

extern void			internal_context_initialize(void)
{
	g_internal_context.initialized = TRUE;
	g_internal_context.debug_mode = DEBUG_MODE;
	g_internal_context.table.time = TIMEOUT;
	g_internal_context.table.start = FALSE;
	g_internal_context.table.actual_turn = 0;
}
