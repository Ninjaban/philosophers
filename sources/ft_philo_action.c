/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_action.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarra <jcarra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/31 13:37:44 by jcarra            #+#    #+#             */
/*   Updated: 2018/01/31 13:37:54 by jcarra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "philo.h"
#include "library.h"

static t_bool		ft_eat(t_philo *philo, uint64_t *actual_turn, uint64_t n)
{
	if (philo->left->is_use == FALSE && philo->right->is_use == FALSE)
	{
		if (!pthread_mutex_trylock(&philo->left->lock) &&
			!pthread_mutex_trylock(&philo->right->lock))
		{
			philo->action = EAT;
			philo->left->is_use = TRUE;
			philo->right->is_use = TRUE;
			ft_system_log_philo("%s : I'm eating now!", philo->name);
			while (--n > 0)
			{
				ft_wait(&internal_context.table.actual_turn, *actual_turn);
				*actual_turn = (internal_context.table.time &&
		internal_context.table.start) ? internal_context.table.actual_turn : 0;
			}
			philo->life = (philo->life + RESTORED_LIFE > MAX_LIFE) ?
							MAX_LIFE : philo->life + RESTORED_LIFE;
			philo->left->is_use = FALSE;
			philo->right->is_use = FALSE;
			ft_system_log_philo("%s : It was a very good meal!", philo->name);
		}
		pthread_mutex_unlock(&philo->left->lock);
		pthread_mutex_unlock(&philo->right->lock);
	}
	return ((philo->action == EAT) ? TRUE : FALSE);
}

static void			ft_rest(t_philo *philo, uint64_t *actual_turn)
{
	uint64_t		n;

	philo->action = REST;
	n = REST_T;
	ft_system_log_philo("%s : It's time to sleep!", philo->name);
	while (n > 0)
	{
		ft_wait(&internal_context.table.actual_turn, *actual_turn);
		*actual_turn = (internal_context.table.time &&
						internal_context.table.start) ?
						internal_context.table.actual_turn : 0;
		n -= 1;
	}
	ft_system_log_philo("%s : I rested!", philo->name);
}

extern void			ft_think(t_philo *philo, uint64_t *actual_turn)
{
	uint64_t		n;

	philo->action = THINK;
	n = THINK_T;
	ft_system_log_philo("%s : What is the life sence?...", philo->name);
	while (n > 0)
	{
		ft_wait(&internal_context.table.actual_turn, *actual_turn);
		*actual_turn = (internal_context.table.time &&
						internal_context.table.start) ?
						internal_context.table.actual_turn : 0;
		n -= 1;
	}
	ft_system_log_philo("%s : 42!", philo->name);
}

extern void			ft_try_eat(t_philo *philo, uint64_t *actual_turn)
{
	if (ft_eat(philo, actual_turn, EAT_T + 1))
		ft_rest(philo, actual_turn);
	else
		ft_think(philo, actual_turn);
}

extern t_bool		ft_check_other_philo(uint64_t id)
{
	if (internal_context.table.list_philo[(id != 0) ?
							id - 1 : NB_PHILOSOPHERS - 1].action == EAT)
		return (FALSE);
	if (internal_context.table.list_philo[(id != NB_PHILOSOPHERS - 1) ?
							id + 1 : 0].action == EAT)
		return (FALSE);
	if (internal_context.table.list_philo[(id != 0) ?
							id - 1 : NB_PHILOSOPHERS - 1].life < THINK_T * 2)
		return (FALSE);
	if (internal_context.table.list_philo[(id != NB_PHILOSOPHERS - 1) ?
							id + 1 : 0].life < THINK_T * 2)
		return (FALSE);
	return (TRUE);
}
