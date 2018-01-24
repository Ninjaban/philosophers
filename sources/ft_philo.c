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


void				*ft_philo(void *arg)
{
	uint64_t		id;
	t_philo			*philo;

	id = (uint64_t)arg;
	philo = &internal_context.table.list_philo[id];
	philo->left = &internal_context.table.list_stick[id];
	philo->right = &internal_context.table.list_stick[(id != NB_PHILOSOPHERS - 1) ? id + 1 : 0];
	philo->life = MAX_LIFE;
	philo->action = THINK;

	FT_DEBUG("Philosophe n°%" PRIu64 " is initialize", id);

	while (internal_context.table.start == FALSE)
		;


	while (internal_context.table.start == TRUE)
		sleep(1);

	FT_DEBUG("Philosophe n°%" PRIu64 " is finalize", id);

	return (NULL);
}


void					ft_check_dead_philo(void)
{
	uint64_t		n;

	n = 0;
	while (n < NB_PHILOSOPHERS)
	{
		if (internal_context.table.list_philo[n].life == 0)
//			ft_end();
		n = n + 1;
	}
}


t_bool					ft_init_table(void)
{
	uint64_t		n;

	if (!internal_context_is_initialize())
	{
		FT_ERROR("internal_context not initialize %s", "");
		return (FALSE);
	}

	/* -- Initialize */
	ft_initialize_stick();

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


	/* -- Core */
	internal_context.table.start = TRUE;
	FT_DEBUG("Game start ! %s", "");

	while (internal_context.table.time > 0)
	{
		ft_check_dead_philo();
		sleep(1);
		internal_context.table.time += -1;
	}


	/* -- Finalize */
	internal_context.table.start = FALSE;
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