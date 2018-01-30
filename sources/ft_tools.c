//
// Created by Nathan on 30/01/18.
//

#include "types.h"

extern void		ft_wait(const uint64_t *variable, const uint64_t value)
{
	while (*variable == value)
		;
}
