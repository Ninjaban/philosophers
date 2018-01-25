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
			FT_DEBUG("%s : I'm eating now !", philo->name);
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
			FT_DEBUG("%s : It was a very good meal !", philo->name);
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
	FT_DEBUG("%s : It's time to sleep !", philo->name);
	while (n > 0)
	{
		ft_wait(&internal_context.table.actual_turn, *actual_turn);
		*actual_turn = (internal_context.table.time && internal_context.table.start) ? internal_context.table.actual_turn : 0;
		n -= 1;
	}
	FT_DEBUG("%s : I rested !", philo->name);
}


void				ft_think(t_philo *philo, uint64_t *actual_turn)
{
	uint64_t		n;

	philo->action = THINK;
	n = THINK_T;
	FT_DEBUG("%s : What is the life sence?...", philo->name);
	while (n > 0)
	{
		ft_wait(&internal_context.table.actual_turn, *actual_turn);
		*actual_turn = (internal_context.table.time && internal_context.table.start) ? internal_context.table.actual_turn : 0;
		n -= 1;
	}
	FT_DEBUG("%s : 42 !", philo->name);
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

	FT_DEBUG("Philosophe n°%" PRIu64 " is initialize", id);

	actual_turn = 0;
	ft_wait((uint64_t *)(&internal_context.table.start), FALSE);

	while (internal_context.table.start == TRUE)
	{
		ft_wait(&internal_context.table.actual_turn, actual_turn);
		actual_turn = (internal_context.table.time && internal_context.table.start) ? internal_context.table.actual_turn : 0;
		if (philo->life == 0)
			break;
		FT_DEBUG("%s : What can I do now?...", philo->name);

		if (philo->life <= THINK_T * 2)
			ft_try_eat(philo, &actual_turn);
		else if (ft_check_other_philo(id))
			ft_try_eat(philo, &actual_turn);
		else
			ft_think(philo, &actual_turn);
		philo->action = THINK;
	}

	FT_DEBUG("%s%s : I still had %" PRIu64 " life points.%s", COLOR_GREEN, philo->name, philo->life, COLOR_BASE);
	FT_DEBUG("Philosophe n°%" PRIu64 " is finalize", id);

	return (NULL);
}


t_bool					ft_check_dead_philo(void)
{
	uint64_t		n;

	n = 0;
	while (n < NB_PHILOSOPHERS)
	{
		if (internal_context.table.list_philo[n].life == 0)
		{
//			ft_end();
			FT_DEBUG("%sSYSTEM : Philosopher %" PRIu64 " (%s) is dead%s", COLOR_RED, n, internal_context.table.list_philo[n].name, COLOR_BASE);
			return (TRUE);
		}
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

	/* -- Initialize */
	ft_initialize_stick();
	ft_create_philo_name();

	n = 0;
	while (n < NB_PHILOSOPHERS)
	{
		if (pthread_create(&internal_context.table.list_philo_thread[n], NULL, ft_philo, (void *)n) != 0)
		{
			FT_ERROR("pthread_create() failed n %" PRIu64, n);
			return (FALSE);
		}
		n = n + 1;
	}

	sleep(1);

	/* -- Core */
	internal_context.table.start = TRUE;
	FT_DEBUG("Game start ! %s", "");

	while (internal_context.table.time > 0)
	{
		FT_DEBUG("%sTurn number %" PRIu64 " !%s", COLOR_BLUE, internal_context.table.actual_turn + 1, COLOR_BASE);
		internal_context.table.actual_turn += 1;
		if (ft_check_dead_philo())
			break;
		ft_reduce_philo_life();

		sleep(1);
		internal_context.table.time -= 1;
		if (!internal_context.table.time)
		{
//			ft_end();
		}
	}


	/* -- Finalize */
	internal_context.table.start = FALSE;
	internal_context.table.actual_turn += 1;
	n = 0;
	while (n < NB_PHILOSOPHERS)
		pthread_join(internal_context.table.list_philo_thread[n++], NULL);

	ft_finalize_stick();

	return (TRUE);
}

int						main()
{
	internal_context_initialize();

	if (!ft_init_table())
		return (1);
	return (0);
}