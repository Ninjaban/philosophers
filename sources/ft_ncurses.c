/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ncurses.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarra <jcarra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/31 15:01:19 by jcarra            #+#    #+#             */
/*   Updated: 2018/01/31 15:19:20 by jcarra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "philo.h"
#include "library.h"

extern void		ft_init_ncurses(void)
{
	initscr();
	start_color();
	g_internal_context.table.window = newwin(WINDOW_SIZE_Y, WINDOW_SIZE_X,
												0, 0);
	cbreak();
	noecho();
	curs_set(0);
	wrefresh(g_internal_context.table.window);
	pthread_mutex_init(&g_internal_context.table.print, NULL);
	pthread_mutex_init(&g_internal_context.table.log, NULL);
	pthread_mutex_init(&g_internal_context.table.log_philo, NULL);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(5, COLOR_RED, COLOR_BLACK);
	init_pair(6, COLOR_BLACK, COLOR_WHITE);
}

extern void		ft_draw_box_system(void)
{
	wmove(g_internal_context.table.window, PHILOSOPHER_BOX_Y - 1, 0);
	whline(g_internal_context.table.window, '*', SYSTEM_BOX_X);
	wmove(g_internal_context.table.window, PHILOSOPHER_BOX_Y - 1, 0);
	wvline(g_internal_context.table.window, '*', SYSTEM_BOX_Y);
	wmove(g_internal_context.table.window, PHILOSOPHER_BOX_Y - 1,
														SYSTEM_BOX_X - 1);
	wvline(g_internal_context.table.window, '*', SYSTEM_BOX_Y);
	wmove(g_internal_context.table.window, PHILOSOPHER_BOX_Y + SYSTEM_BOX_Y - 2,
														0);
	whline(g_internal_context.table.window, '*', SYSTEM_BOX_X);
	wattron(g_internal_context.table.window, COLOR_PAIR(2));
	ft_print(PHILOSOPHER_BOX_Y - 1, 3, "System");
	wattron(g_internal_context.table.window, COLOR_PAIR(1));
	wrefresh(g_internal_context.table.window);
}

extern void		ft_draw_box_philo(uint64_t n)
{
	wmove(g_internal_context.table.window, 0, (PHILOSOPHER_BOX_X * n) - n);
	whline(g_internal_context.table.window, '*', PHILOSOPHER_BOX_X);
	wmove(g_internal_context.table.window, 0, (PHILOSOPHER_BOX_X * n) - n);
	wvline(g_internal_context.table.window, '*', PHILOSOPHER_BOX_Y);
	wmove(g_internal_context.table.window, 0,
			(PHILOSOPHER_BOX_X * (n + 1)) - (n + 1));
	wvline(g_internal_context.table.window, '*', PHILOSOPHER_BOX_Y);
	wattron(g_internal_context.table.window, COLOR_PAIR(2));
	ft_print(0, (PHILOSOPHER_BOX_X * n) - n + 3,
			g_internal_context.table.list_philo[n].name);
	wattron(g_internal_context.table.window, COLOR_PAIR(1));
	ft_print(2, (PHILOSOPHER_BOX_X * n) - n + 2, ">");
	wrefresh(g_internal_context.table.window);
}
