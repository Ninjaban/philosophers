/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_log.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarra <jcarra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/31 13:37:40 by jcarra            #+#    #+#             */
/*   Updated: 2018/01/31 15:34:43 by jcarra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "types.h"
#include "philo.h"
#include "library.h"

extern void		ft_print(int y, int x, char *format)
{
	pthread_mutex_lock(&g_internal_context.table.print);
	wmove(g_internal_context.table.window, y, x);
	wprintw(g_internal_context.table.window, format);
	pthread_mutex_unlock(&g_internal_context.table.print);
}

static void		ft_system_log_clear(const uint64_t nb_log)
{
	char			log_clear[SYSTEM_BOX_X - 4 + 1];
	uint64_t		n;

	n = 0;
	while (n < SYSTEM_BOX_X - 4)
		log_clear[n++] = ' ';
	log_clear[n] = '\0';
	n = 0;
	while (n < nb_log)
	{
		ft_print(PHILOSOPHER_BOX_Y - 1 + 2 + n, 2, log_clear);
		n = n + 1;
	}
}

extern void		ft_system_log(uint64_t color, t_bool new_turn, char *format)
{
	static uint64_t	nb_log = 0;

	pthread_mutex_lock(&g_internal_context.table.log);
	if (new_turn)
	{
		ft_system_log_clear(nb_log);
		nb_log = 0;
	}
	if (nb_log < SYSTEM_BOX_Y - 4)
	{
		wattron(g_internal_context.table.window, COLOR_PAIR(color));
		ft_print(PHILOSOPHER_BOX_Y - 1 + 2 + nb_log, 2, format);
		nb_log += 1;
		wattron(g_internal_context.table.window, COLOR_PAIR(1));
	}
	pthread_mutex_unlock(&g_internal_context.table.log);
}

extern void		ft_system_log_philo(char *format, char *name)
{
	pthread_mutex_lock(&g_internal_context.table.log_philo);
	sprintf(g_internal_context.table.log_philo_buf, format, name);
	ft_system_log(1, FALSE, g_internal_context.table.log_philo_buf);
	wrefresh(g_internal_context.table.window);
	pthread_mutex_unlock(&g_internal_context.table.log_philo);
}

extern void		ft_print_turn(void)
{
	sprintf(g_internal_context.table.log_buf, "Turn number %" PRIu64,
			g_internal_context.table.actual_turn);
	ft_system_log(2, TRUE, g_internal_context.table.log_buf);
	wrefresh(g_internal_context.table.window);
}
