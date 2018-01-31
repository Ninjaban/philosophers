/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_system.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarra <jcarra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/31 15:24:06 by jcarra            #+#    #+#             */
/*   Updated: 2018/01/31 15:35:17 by jcarra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "types.h"
#include "error.h"
#include "philo.h"
#include "library.h"

/*
**	-- basic check
*/
typedef char	t__check_for_philo_true[(TRUE == 1) ? 1 : -1];
typedef char	t__check_for_philo_false[(FALSE == 0) ? 1 : -1];
typedef char	t__check_for_philo_eat_t[(EAT_T < TIMEOUT) ? 1 : -1];
typedef char	t__check_for_philo_rest_t[(REST_T < TIMEOUT) ? 1 : -1];
typedef char	t__check_for_philo_think_t[(THINK_T < TIMEOUT) ? 1 : -1];
typedef char	t__check_for_philo_dps[(DPS < TIMEOUT) ? 1 : -1];
typedef char	t__check_for_philo_nb_philo[(NB_PHILOSOPHERS > 1) ? 1 : -1];

static t_bool		ft_check_dead_philo(void)
{
	uint64_t		n;
	t_bool			ret;

	n = 0;
	ret = FALSE;
	while (n < NB_PHILOSOPHERS)
	{
		if (g_internal_context.table.list_philo[n].life == 0)
		{
			ret = TRUE;
			sprintf(g_internal_context.table.log_buf,
					"The philosopher %s is dead",
					g_internal_context.table.list_philo[n].name);
			ft_system_log(5, FALSE, g_internal_context.table.log_buf);
			wrefresh(g_internal_context.table.window);
		}
		n = n + 1;
	}
	return (ret);
}

static void			ft_reduce_philo_life(void)
{
	uint64_t		n;

	n = 0;
	while (n < NB_PHILOSOPHERS)
	{
		if (g_internal_context.table.list_philo[n].action != EAT &&
			g_internal_context.table.list_philo[n].life)
			g_internal_context.table.list_philo[n].life -= DPS;
		n = n + 1;
	}
}

static void			ft_system_start(void)
{
	ft_system_log(5, TRUE, "Game starts in :");
	wrefresh(g_internal_context.table.window);
	sleep(1);
	ft_print_philo_info();
	ft_system_log(5, FALSE, "3...");
	wrefresh(g_internal_context.table.window);
	sleep(1);
	ft_system_log(5, FALSE, "2...");
	wrefresh(g_internal_context.table.window);
	sleep(1);
	ft_system_log(5, FALSE, "1...");
	wrefresh(g_internal_context.table.window);
	sleep(1);
	g_internal_context.table.start = TRUE;
}

static void			ft_system_core(void)
{
	while (g_internal_context.table.time > 0)
	{
		ft_print_turn();
		if (ft_check_dead_philo())
			break ;
		g_internal_context.table.actual_turn += 1;
		sleep(SLEEP_TIME);
		wrefresh(g_internal_context.table.window);
		ft_reduce_philo_life();
		ft_print_philo_info();
		g_internal_context.table.time -= 1;
	}
	if (!g_internal_context.table.time)
	{
		ft_print_turn();
		ft_system_log(1, FALSE, TIMEOUT_TXT);
	}
	wrefresh(g_internal_context.table.window);
	wgetch(g_internal_context.table.window);
	g_internal_context.table.start = FALSE;
	g_internal_context.table.actual_turn += 1;
}

extern t_bool		ft_init_system(void)
{
	uint64_t		n;

	if (!internal_context_is_initialize())
		return (FALSE);
	ft_init_ncurses();
	ft_draw_box_system();
	ft_initialize_stick();
	ft_create_philo_name();
	n = 0;
	while (n < NB_PHILOSOPHERS)
	{
		if (pthread_create(&g_internal_context.table.list_philo_thread[n], NULL,
							ft_philo, (void *)n) != 0)
			return (FALSE);
		ft_draw_box_philo(n++);
	}
	ft_system_start();
	ft_system_core();
	n = 0;
	while (n < NB_PHILOSOPHERS)
		pthread_join(g_internal_context.table.list_philo_thread[n++], NULL);
	ft_finalize_stick();
	delwin(g_internal_context.table.window);
	endwin();
	return (TRUE);
}
