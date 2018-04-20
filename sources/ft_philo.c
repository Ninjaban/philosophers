/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarra <jcarra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/31 15:35:48 by jcarra            #+#    #+#             */
/*   Updated: 2018/04/20 13:53:21 by jcarra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "philo.h"
#include "library.h"

static void		ft_init_philo(t_philo *philo, uint64_t id)
{
	philo->left = &g_internal_context.table.list_stick[id];
	philo->right = &g_internal_context.table.list_stick[(id !=
											NB_PHILOSOPHERS - 1) ? id + 1 : 0];
	philo->life = MAX_LIFE;
	philo->action = THINK;
	philo->action_percent = 0;
}

static void		ft_philo_do(t_philo *philo, uint64_t *actual_turn, uint64_t id)
{
	ft_system_log_philo("%s : What can I do now?...", philo->name);
	if (philo->life <= THINK_T * 2)
		ft_try_eat(philo, actual_turn);
	else if (ft_check_other_philo(id))
		ft_try_eat(philo, actual_turn);
	else
		ft_think(philo, actual_turn);
	philo->action = THINK;
}

extern void		*ft_philo(void *arg)
{
	uint64_t		id;
	t_philo			*philo;
	uint64_t		actual_turn;

	id = (uint64_t)arg;
	philo = &g_internal_context.table.list_philo[id];
	ft_init_philo(philo, id);
	actual_turn = 0;
	ft_wait((uint64_t *)(&g_internal_context.table.start), FALSE);
	while (g_internal_context.table.start == TRUE)
	{
		ft_wait(&g_internal_context.table.actual_turn, actual_turn);
		actual_turn = (g_internal_context.table.time &&
						g_internal_context.table.start) ?
						g_internal_context.table.actual_turn : 0;
		if (philo->life == 0)
			break ;
		ft_philo_do(philo, &actual_turn, id);
	}
	return (NULL);
}
