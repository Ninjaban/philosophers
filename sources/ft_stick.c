/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stick.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarra <jcarra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/31 14:38:28 by jcarra            #+#    #+#             */
/*   Updated: 2018/01/31 15:21:13 by jcarra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "philo.h"
#include "library.h"

extern void			ft_initialize_stick(void)
{
	static uint64_t		id = 0;

	g_internal_context.table.list_stick[id].is_use = FALSE;
	pthread_mutex_init(&g_internal_context.table.list_stick[id].lock, NULL);
	id = id + 1;
	if (id < NB_PHILOSOPHERS)
		ft_initialize_stick();
}

extern void			ft_finalize_stick(void)
{
	static uint64_t		id = 0;

	g_internal_context.table.list_stick[id].is_use = TRUE;
	pthread_mutex_destroy(&g_internal_context.table.list_stick[id].lock);
	id = id + 1;
	if (id < NB_PHILOSOPHERS)
		ft_finalize_stick();
}
