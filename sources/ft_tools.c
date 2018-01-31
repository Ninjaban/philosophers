/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarra <jcarra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/31 13:37:49 by jcarra            #+#    #+#             */
/*   Updated: 2018/01/31 15:34:35 by jcarra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "philo.h"
#include "library.h"

extern void		ft_wait(const uint64_t *variable, const uint64_t value)
{
	while (*variable == value)
		;
}

extern void		ft_create_philo_name(void)
{
	g_internal_context.table.list_philo[0].name = "Alexandre";
	g_internal_context.table.list_philo[1].name = "Richard";
	g_internal_context.table.list_philo[2].name = "Nicolas";
	g_internal_context.table.list_philo[3].name = "Marc";
	g_internal_context.table.list_philo[4].name = "Guillaume";
	g_internal_context.table.list_philo[5].name = "Simon";
	g_internal_context.table.list_philo[6].name = "Mel";
}
