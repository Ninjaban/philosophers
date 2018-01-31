/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   library.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarra <jcarra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/31 13:37:53 by jcarra            #+#    #+#             */
/*   Updated: 2018/01/31 13:37:54 by jcarra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "types.h"
#include "philo.h"
#include "library.h"

/*
**	internal_context_is_initialize()
**
**	@param	none
**
**	@error	none
**
**	@return	TRUE in success, FALSE otherwise
*/
extern t_bool		internal_context_is_initialize (void)
{
	return (internal_context.initialized);
}


/*
**	internal_context_initialize()
**
**	@param	none
**
**	@error	none
*/
extern void			internal_context_initialize (void)
{
	internal_context.initialized = TRUE;

	internal_context.debug_mode = DEBUG_MODE;

	internal_context.table.time = TIMEOUT;
	internal_context.table.start = FALSE;
	internal_context.table.actual_turn = 0;
}

