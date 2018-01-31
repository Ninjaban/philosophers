/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_info.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarra <jcarra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/31 14:50:28 by jcarra            #+#    #+#             */
/*   Updated: 2018/01/31 15:20:06 by jcarra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "philo.h"
#include "library.h"

static char		*ft_get_life_bar(uint64_t percent)
{
	char		*str;

	if (percent >= 10)
		str = "IIIIIIIIII";
	else if (percent == 9)
		str = "IIIIIIIII ";
	else if (percent == 8)
		str = "IIIIIIII  ";
	else if (percent == 7)
		str = "IIIIIII   ";
	else if (percent == 6)
		str = "IIIIII    ";
	else if (percent == 5)
		str = "IIIII     ";
	else if (percent == 4)
		str = "IIII      ";
	else if (percent == 3)
		str = "III       ";
	else if (percent == 2)
		str = "II        ";
	else if (percent == 1)
		str = "I         ";
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
		str = ft_get_life_bar(percent);
		if (percent <= 10 && percent >= 7)
			wattron(g_internal_context.table.window, COLOR_PAIR(3));
		else if (percent <= 6 && percent >= 3)
			wattron(g_internal_context.table.window, COLOR_PAIR(4));
		if (percent <= 2)
			wattron(g_internal_context.table.window, COLOR_PAIR(5));
		ft_print(3, (PHILOSOPHER_BOX_X * n) - n + 2, str);
		wattron(g_internal_context.table.window, COLOR_PAIR(1));
		n = n + 1;
	}
}

extern void		ft_print_philo_info(void)
{
	ft_print_philo_action();
	ft_print_philo_life();
}
