//
// Created by Nathan on 30/01/18.
//

#include "error.h"
#include "types.h"
#include "library.h"
#include "philo.h"

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

