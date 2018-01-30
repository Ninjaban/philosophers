//
// Created by Nathan on 23/01/18.
//

#include <unistd.h>
#include "library.h"
#include "types.h"
#include "error.h"
#include "philo.h"

/* -- basic check */
typedef char	t__check_for_philo_true[(TRUE == 1) ? 1 : -1];
typedef char	t__check_for_philo_false[(FALSE == 0) ? 1 : -1];
typedef char	t__check_for_philo_eat_t[(EAT_T < TIMEOUT) ? 1 : -1];
typedef char	t__check_for_philo_rest_t[(REST_T < TIMEOUT) ? 1 : -1];
typedef char	t__check_for_philo_think_t[(THINK_T < TIMEOUT) ? 1 : -1];
typedef char	t__check_for_philo_dps[(DPS < TIMEOUT) ? 1 : -1];
typedef char	t__check_for_philo_nb_philo[(NB_PHILOSOPHERS > 1) ? 1 : -1];


static void 		ft_initialize_stick(void)
{
	static uint64_t		id = 0;

	internal_context.table.list_stick[id].is_use = FALSE;
	pthread_mutex_init(&internal_context.table.list_stick[id].lock, NULL);
	id = id + 1;
	if (id < NB_PHILOSOPHERS)
		ft_initialize_stick();
}


static void 		ft_finalize_stick(void)
{
	static uint64_t		id = 0;

	internal_context.table.list_stick[id].is_use = TRUE;
	pthread_mutex_destroy(&internal_context.table.list_stick[id].lock);
	id = id + 1;
	if (id < NB_PHILOSOPHERS)
		ft_finalize_stick();
}


void				*ft_philo(void *arg)
{
	uint64_t		id;
	t_philo			*philo;
	uint64_t		actual_turn;

	id = (uint64_t)arg;
	philo = &internal_context.table.list_philo[id];
	philo->left = &internal_context.table.list_stick[id];
	philo->right = &internal_context.table.list_stick[(id != NB_PHILOSOPHERS - 1) ? id + 1 : 0];
	philo->life = MAX_LIFE;
	philo->action = THINK;

	actual_turn = 0;
	ft_wait((uint64_t *)(&internal_context.table.start), FALSE);

	while (internal_context.table.start == TRUE)
	{
		ft_wait(&internal_context.table.actual_turn, actual_turn);
		actual_turn = (internal_context.table.time && internal_context.table.start) ? internal_context.table.actual_turn : 0;
		if (philo->life == 0)
			break;
		ft_system_log_philo("%s : What can I do now?...", philo->name);

		if (philo->life <= THINK_T * 2)
			ft_try_eat(philo, &actual_turn);
		else if (ft_check_other_philo(id))
			ft_try_eat(philo, &actual_turn);
		else
			ft_think(philo, &actual_turn);
		philo->action = THINK;
	}

	return (NULL);
}


t_bool					ft_check_dead_philo(void)
{
	uint64_t		n;
	t_bool			ret;

	n = 0;
	ret = FALSE;
	while (n < NB_PHILOSOPHERS)
	{
		if (internal_context.table.list_philo[n].life == 0)
		{
			ret = TRUE;
			sprintf(internal_context.table.log_buf, "The philosopher %s is dead", internal_context.table.list_philo[n].name);
			ft_system_log(5, FALSE, internal_context.table.log_buf);
			wrefresh(internal_context.table.window);
		}
		n = n + 1;
	}
	return (ret);
}


void					ft_reduce_philo_life(void)
{
	uint64_t		n;

	n = 0;
	while (n < NB_PHILOSOPHERS)
	{
		if (internal_context.table.list_philo[n].action != EAT &&
			internal_context.table.list_philo[n].life)
			internal_context.table.list_philo[n].life -= DPS;
		n = n + 1;
	}
}


void					ft_print_philo_info(void)
{
	uint64_t		n;
	char			*str;
	uint64_t		percent;
	t_philo			philo;

	n = 0;
	wattron(internal_context.table.window, COLOR_PAIR(6));
	while (n < NB_PHILOSOPHERS)
	{
		philo = internal_context.table.list_philo[n];

		if (philo.action == EAT)
			str = "  EAT  ";
		else if (philo.action == REST)
			str = " REST  ";
		else
			str = " THINK ";
		ft_print(2, (PHILOSOPHER_BOX_X * n) - n + 4, str);

		n = n + 1;
	}
	wattron(internal_context.table.window, COLOR_PAIR(1));

	n = 0;
	while (n < NB_PHILOSOPHERS)
	{
		philo = internal_context.table.list_philo[n];
		percent = philo.life * 100 / MAX_LIFE / 10;
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
		if (percent <= 10 && percent >= 7)
			wattron(internal_context.table.window, COLOR_PAIR(3));
		else if (percent <= 6 && percent >= 3)
			wattron(internal_context.table.window, COLOR_PAIR(4));
		if (percent <= 2)
			wattron(internal_context.table.window, COLOR_PAIR(5));
		ft_print(3, (PHILOSOPHER_BOX_X * n) - n + 2, str);
		wattron(internal_context.table.window, COLOR_PAIR(1));
		n = n + 1;
	}
}


void					ft_create_philo_name(void)
{
	internal_context.table.list_philo[0].name = "Alexandre";
	internal_context.table.list_philo[1].name = "Richard";
	internal_context.table.list_philo[2].name = "Nicolas";
	internal_context.table.list_philo[3].name = "Marc";
	internal_context.table.list_philo[4].name = "Guillaume";
	internal_context.table.list_philo[5].name = "Simon";
	internal_context.table.list_philo[6].name = "Mel";
}


t_bool					ft_init_table(void)
{
	uint64_t		n;

	/* -- Basic test */
	if (!internal_context_is_initialize())
	{
		FT_ERROR("internal_context not initialize %s", "");
		return (FALSE);
	}

	/* -- Graphique Initialize */
	initscr();
	start_color();
	internal_context.table.window = newwin(WINDOW_SIZE_Y, WINDOW_SIZE_X, 0, 0);
	cbreak();
	noecho();
	curs_set(0);
	wrefresh(internal_context.table.window);
	pthread_mutex_init(&internal_context.table.print, NULL);
	pthread_mutex_init(&internal_context.table.log, NULL);
	pthread_mutex_init(&internal_context.table.log_philo, NULL);

	/* -- Init all color pair */
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(5, COLOR_RED, COLOR_BLACK);
	init_pair(6, COLOR_BLACK, COLOR_WHITE);

	/* -- Draw System Box*/
	wmove(internal_context.table.window, PHILOSOPHER_BOX_Y - 1, 0);
	whline(internal_context.table.window, '*', SYSTEM_BOX_X);
	wmove(internal_context.table.window, PHILOSOPHER_BOX_Y - 1, 0);
	wvline(internal_context.table.window, '*', SYSTEM_BOX_Y);
	wmove(internal_context.table.window, PHILOSOPHER_BOX_Y - 1, SYSTEM_BOX_X - 1);
	wvline(internal_context.table.window, '*', SYSTEM_BOX_Y);
	wmove(internal_context.table.window, PHILOSOPHER_BOX_Y + SYSTEM_BOX_Y - 2, 0);
	whline(internal_context.table.window, '*', SYSTEM_BOX_X);
	wattron(internal_context.table.window, COLOR_PAIR(2));
	ft_print(PHILOSOPHER_BOX_Y - 1, 3, "System");
	wattron(internal_context.table.window, COLOR_PAIR(1));
	wrefresh(internal_context.table.window);

	/* -- Initialize */
	ft_initialize_stick();
	ft_create_philo_name();

	n = 0;
	while (n < NB_PHILOSOPHERS)
	{
		/* -- Draw Philosopher Box*/
		wmove(internal_context.table.window, 0, (PHILOSOPHER_BOX_X * n) - n);
		whline(internal_context.table.window, '*', PHILOSOPHER_BOX_X);
		wmove(internal_context.table.window, 0, (PHILOSOPHER_BOX_X * n) - n);
		wvline(internal_context.table.window, '*', PHILOSOPHER_BOX_Y);
		wmove(internal_context.table.window, 0, (PHILOSOPHER_BOX_X * (n + 1)) - (n + 1));
		wvline(internal_context.table.window, '*', PHILOSOPHER_BOX_Y);
		wattron(internal_context.table.window, COLOR_PAIR(2));
		ft_print(0, (PHILOSOPHER_BOX_X * n) - n + 3, internal_context.table.list_philo[n].name);
		wattron(internal_context.table.window, COLOR_PAIR(1));
		ft_print(2, (PHILOSOPHER_BOX_X * n) - n + 2, ">");
		wrefresh(internal_context.table.window);

		if (pthread_create(&internal_context.table.list_philo_thread[n], NULL, ft_philo, (void *)n) != 0)
		{
			FT_ERROR("pthread_create() failed n %" PRIu64, n);
			return (FALSE);
		}
		n = n + 1;
	}

	ft_system_log(5, TRUE, "Game starts in :");
	wrefresh(internal_context.table.window);
	sleep(1);
	ft_print_philo_info();
	ft_system_log(5, FALSE, "3...");
	wrefresh(internal_context.table.window);
	sleep(1);
	ft_system_log(5, FALSE, "2...");
	wrefresh(internal_context.table.window);
	sleep(1);
	ft_system_log(5, FALSE, "1...");
	wrefresh(internal_context.table.window);
	sleep(1);

	/* -- Core */
	internal_context.table.start = TRUE;

	while (internal_context.table.time > 0)
	{
		sprintf(internal_context.table.log_buf, "Turn number %" PRIu64, internal_context.table.actual_turn);
		ft_system_log(2, TRUE, internal_context.table.log_buf);
		wrefresh(internal_context.table.window);
		if (ft_check_dead_philo())
			break;
		internal_context.table.actual_turn += 1;

		sleep(SLEEP_TIME);
		wrefresh(internal_context.table.window);
		ft_reduce_philo_life();
		ft_print_philo_info();
		internal_context.table.time -= 1;
	}

	/* -- End print */
	if (!internal_context.table.time)
	{
		sprintf(internal_context.table.log_buf, "Turn number %" PRIu64, internal_context.table.actual_turn);
		ft_system_log(2, TRUE, internal_context.table.log_buf);
		ft_system_log(1, FALSE, TIMEOUT_TXT);
	}
	wrefresh(internal_context.table.window);
	wgetch(internal_context.table.window);

	/* -- Finalize */
	internal_context.table.start = FALSE;
	internal_context.table.actual_turn += 1;
	n = 0;
	while (n < NB_PHILOSOPHERS)
		pthread_join(internal_context.table.list_philo_thread[n++], NULL);

	ft_finalize_stick();

	/* -- Graphique Finalize*/
	delwin(internal_context.table.window);
	endwin();

	return (TRUE);
}

int						main()
{
	internal_context_initialize();

	if (!ft_init_table())
		return (1);
	return (0);
}
