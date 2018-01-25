//
// Created by Nathan on 23/01/18.
//

#include "types.h"
#include "error.h"
#include "libft.h"

#include "philo.h"

/* -- basic check */
typedef char __check_for_philo_true[ (TRUE == 1) ? 1:-1 ];
typedef char __check_for_philo_false[ (FALSE == 0) ? 1:-1 ];
typedef char __check_for_philo_eat_t[ (EAT_T < TIMEOUT) ? 1:-1 ];
typedef char __check_for_philo_rest_t[ (REST_T < TIMEOUT) ? 1:-1 ];
typedef char __check_for_philo_think_t[ (THINK_T < TIMEOUT) ? 1:-1 ];
typedef char __check_for_philo_dps[ (DPS < TIMEOUT) ? 1:-1 ];


/* -- library_configuration */
static struct
{
	t_bool	initialized;

	t_bool	debug_mode;

	t_table		table;
} internal_context = {
		.initialized	= FALSE,

		.debug_mode	= FALSE
};


/**
 *		internal_context_is_initialize
 *
 *		@param	none
 *
 *		@error	none
 *
 *		@return	TRUE in success, FALSE otherwise
 */
static t_bool		internal_context_is_initialize (void)
{
	return (internal_context.initialized);
}


/* === initialisation function === */
/**
 *		internal_context_initialize
 *
 *		@param	none
 *
 *		@error	none
 */
static void			internal_context_initialize (void)
{
	internal_context.initialized = TRUE;

	internal_context.debug_mode = DEBUG_MODE;

	internal_context.table.time = TIMEOUT;
	internal_context.table.start = FALSE;
	internal_context.table.actual_turn = 0;
}

static void			ft_print(int y, int x, char *format)
{
	pthread_mutex_lock(&internal_context.table.print);
	wmove(internal_context.table.window, y, x);
	wprintw(internal_context.table.window, format);
	pthread_mutex_unlock(&internal_context.table.print);
}


static void			ft_system_log_clear(const uint64_t nb_log)
{
	uint64_t		n;

	n = 0;
	while (n < nb_log)
	{
		ft_print(PHILOSOPHER_BOX_Y - 1 + 2 + n, 2, LOG_CLEAR);
		n = n + 1;
	}
}


static void			ft_system_log(uint64_t color, t_bool new_turn, char *format)
{
	static uint64_t	nb_log = 0;

	pthread_mutex_lock(&internal_context.table.log);
	if (new_turn)
	{
		ft_system_log_clear(nb_log);
		nb_log = 0;
	}
	if (nb_log < SYSTEM_BOX_Y - 4)
	{
		wattron(internal_context.table.window, COLOR_PAIR(color));
		ft_print(PHILOSOPHER_BOX_Y - 1 + 2 + nb_log, 2, format);
		nb_log += 1;
		wattron(internal_context.table.window, COLOR_PAIR(1));
	}
	pthread_mutex_unlock(&internal_context.table.log);
}


static void			ft_system_log_philo(char *format, char *name)
{
	pthread_mutex_lock(&internal_context.table.log_philo);
	sprintf(internal_context.table.log_philo_buf, format, name);
	ft_system_log(1, FALSE, internal_context.table.log_philo_buf);
	wrefresh(internal_context.table.window);
	pthread_mutex_unlock(&internal_context.table.log_philo);
}


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


void				ft_wait(const uint64_t *variable, const uint64_t value)
{
	while (*variable == value)
		;
}


t_bool				ft_eat(t_philo *philo, uint64_t *actual_turn)
{
	uint64_t		n;

	if (philo->left->is_use == FALSE && philo->right->is_use == FALSE)
	{
		if (!pthread_mutex_trylock(&philo->left->lock) &&
			!pthread_mutex_trylock(&philo->right->lock))
		{
			philo->action = EAT;
			n = EAT_T;
			ft_system_log_philo("%s : I'm eating now!", philo->name);
			while (n > 0)
			{
				ft_wait(&internal_context.table.actual_turn, *actual_turn);
				*actual_turn = (internal_context.table.time && internal_context.table.start) ? internal_context.table.actual_turn : 0;
				n -= 1;
			}
			philo->life = (philo->life + RESTORED_LIFE > MAX_LIFE) ? MAX_LIFE : philo->life + RESTORED_LIFE;
			philo->action = REST;
			pthread_mutex_unlock(&philo->left->lock);
			pthread_mutex_unlock(&philo->right->lock);
			philo->left->is_use = FALSE;
			philo->right->is_use = FALSE;
			ft_system_log_philo("%s : It was a very good meal!", philo->name);
			return (TRUE);
		}
		pthread_mutex_unlock(&philo->left->lock);
		pthread_mutex_unlock(&philo->right->lock);
	}
	return (FALSE);
}


void				ft_rest(t_philo *philo, uint64_t *actual_turn)
{
	uint64_t		n;

	philo->action = REST;
	n = REST_T;
	ft_system_log_philo("%s : It's time to sleep!", philo->name);
	while (n > 0)
	{
		ft_wait(&internal_context.table.actual_turn, *actual_turn);
		*actual_turn = (internal_context.table.time && internal_context.table.start) ? internal_context.table.actual_turn : 0;
		n -= 1;
	}
	ft_system_log_philo("%s : I rested!", philo->name);
}


void				ft_think(t_philo *philo, uint64_t *actual_turn)
{
	uint64_t		n;

	philo->action = THINK;
	n = THINK_T;
	ft_system_log_philo("%s : What is the life sence?...", philo->name);
	while (n > 0)
	{
		ft_wait(&internal_context.table.actual_turn, *actual_turn);
		*actual_turn = (internal_context.table.time && internal_context.table.start) ? internal_context.table.actual_turn : 0;
		n -= 1;
	}
	ft_system_log_philo("%s : 42!", philo->name);
}


void				ft_try_eat(t_philo *philo, uint64_t *actual_turn)
{
	if (ft_eat(philo, actual_turn))
		ft_rest(philo, actual_turn);
	else
		ft_think(philo, actual_turn);
}


t_bool				ft_check_other_philo(uint64_t id)
{
	if (internal_context.table.list_philo[(id != 0) ? id - 1 : NB_PHILOSOPHERS - 1].action == EAT)
		return (FALSE);
	if (internal_context.table.list_philo[(id != NB_PHILOSOPHERS - 1) ? id + 1 : 0].action == EAT)
		return (FALSE);
	if (internal_context.table.list_philo[(id != 0) ? id - 1 : NB_PHILOSOPHERS - 1].life < THINK_T * 2)
		return (FALSE);
	if (internal_context.table.list_philo[(id != NB_PHILOSOPHERS - 1) ? id + 1 : 0].life < THINK_T * 2)
		return (FALSE);
	return (TRUE);
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

	n = 0;
	while (n < NB_PHILOSOPHERS)
	{
		if (internal_context.table.list_philo[n].life == 0)
			return (TRUE);
		n = n + 1;
	}
	return (FALSE);
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
	internal_context.table.list_philo[6].name = "Eric";
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
		internal_context.table.actual_turn += 1;
		if (ft_check_dead_philo())
			break;

		sleep(SLEEP_TIME);
		wrefresh(internal_context.table.window);
		ft_reduce_philo_life();
		ft_print_philo_info();
		internal_context.table.time -= 1;
	}

	/* -- End print */
	sprintf(internal_context.table.log_buf, "Turn number %" PRIu64, internal_context.table.actual_turn);
	ft_system_log(2, TRUE, internal_context.table.log_buf);
	if (!internal_context.table.time)
		ft_system_log(1, FALSE, TIMEOUT_TXT);
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