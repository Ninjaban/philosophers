/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_info.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarra <jcarra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/31 14:50:28 by jcarra            #+#    #+#             */
/*   Updated: 2018/04/20 13:52:51 by jcarra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "philo.h"
#include "library.h"

static char		*ft_get_life_bar(uint64_t percent, t_bool life)
{
	char		*str;

	if (percent >= 10)
		str = (life) ? "IIIIIIIIII" : "----------";
	else if (percent == 9)
		str = (life) ? "IIIIIIIII " : "--------- ";
	else if (percent == 8)
		str = (life) ? "IIIIIIII  " : "--------  ";
	else if (percent == 7)
		str = (life) ? "IIIIIII   " : "-------   ";
	else if (percent == 6)
		str = (life) ? "IIIIII    " : "------    ";
	else if (percent == 5)
		str = (life) ? "IIIII     " : "-----     ";
	else if (percent == 4)
		str = (life) ? "IIII      " : "----      ";
	else if (percent == 3)
		str = (life) ? "III       " : "---       ";
	else if (percent == 2)
		str = (life) ? "II        " : "--        ";
	else if (percent == 1)
		str = (life) ? "I         " : "-         ";
	else
		str = "          ";
	return (str);
}

static void		ft_print_philo_action(void)
{
	uint64_t		n;
	char			*str;
	t_philo			philo;

	n = 0;
	wattron(g_internal_context.table.window, COLOR_PAIR(6));
	while (n < NB_PHILOSOPHERS)
	{
		philo = g_internal_context.table.list_philo[n];
		if (philo.action == EAT)
			str = "  EAT  ";
		else if (philo.action == REST)
			str = " REST  ";
		else
			str = " THINK ";
		ft_print(2, (PHILOSOPHER_BOX_X * n) - n + 4, str);
		n = n + 1;
	}
	wattron(g_internal_context.table.window, COLOR_PAIR(1));
}

static void		ft_print_philo_action_percent(void)
{
	t_philo		philo;
	char		*str;
	uint64_t	n;

	n = 0;
	while (n < NB_PHILOSOPHERS)
	{
		philo = g_internal_context.table.list_philo[n];
		str = ft_get_life_bar(philo.action_percent, FALSE);
		ft_print(3, (PHILOSOPHER_BOX_X * n) - n + 2, str);
		n = n + 1;
	}
}

static void		ft_print_philo_life(void)
{
	t_philo		philo;
	char		*str;
	uint64_t	percent;
	uint64_t	n;

	n = 0;
	while (n < NB_PHILOSOPHERS)
	{
		philo = g_internal_context.table.list_philo[n];
		percent = philo.life * 100 / MAX_LIFE / 10;
		str = ft_get_life_bar(percent, TRUE);
		if (percent <= 10 && percent >= 7)
			wattron(g_internal_context.table.window, COLOR_PAIR(3));
		else if (percent <= 6 && percent >= 3)
			wattron(g_internal_context.table.window, COLOR_PAIR(4));
		if (percent <= 2)
			wattron(g_internal_context.table.window, COLOR_PAIR(5));
		ft_print(5, (PHILOSOPHER_BOX_X * n) - n + 2, str);
		wattron(g_internal_context.table.window, COLOR_PAIR(1));
		n = n + 1;
	}
}

extern void		ft_print_philo_info(void)
{
	ft_print_philo_action();
	ft_print_philo_action_percent();
	ft_print_philo_life();
}
