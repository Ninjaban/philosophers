/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarra <jcarra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/31 15:48:42 by jcarra            #+#    #+#             */
/*   Updated: 2018/01/31 15:56:46 by jcarra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <curses.h>
# include <pthread.h>
# include <sys/types.h>

/*
**	-- default macros
**	Macros define by the subject
**
**	-> MAX_LIFE is the number of life point for each philosophers
**	-> EAT_T is the time to eat for a philosopher in second
**	-> REAT_T is the rest time for a philosopher in second
**	-> THINK_T is the thinking time for a philosopher in second
**	-> TIMEOUT is the duration of the game in second
*/

# define MAX_LIFE			(uint64_t)10
# define EAT_T				(uint64_t)2
# define REST_T				(uint64_t)1
# define THINK_T			(uint64_t)2
# define TIMEOUT			(uint64_t)30

/*
**	-- internal macros
**	Extracted macros from the subject
**
**	-> RESTORED_LIFE is the number of life point restore when a philosopher rest
**	-> DPS is the number of life point lost every second
**	-> NB_PHILOSOPHERS is the number of philospoher arond the table
**	-> TIMEOUT_TXT is the displayed text at the end of the game
*/

# define RESTORED_LIFE		MAX_LIFE
# define DPS				(uint64_t)1
# define NB_PHILOSOPHERS	(uint32_t)7
# define TIMEOUT_TXT		"Now, it is time... To DAAAAAAAANCE!!!"
# define SLEEP_TIME			1

/*
**	-- internal graphique macros
*/
# define WINDOW_SIZE_X		94
# define WINDOW_SIZE_Y		30

# define PHILOSOPHER_BOX_X	14
# define PHILOSOPHER_BOX_Y	6

# define SYSTEM_BOX_X		92
# define SYSTEM_BOX_Y		22

typedef enum e_action	t_action;
enum	e_action
{
	EAT,
	REST,
	THINK
};

/*
**	-- structure
*/
typedef struct	s_stick {
	pthread_mutex_t		lock;
	t_bool				is_use;
}				t_stick;

typedef struct	s_philo {
	t_stick				*left;
	t_stick				*right;
	uint64_t			life;
	t_action			action;
	t_pchar				name;
}				t_philo;

typedef struct	s_table {
	t_stick				list_stick[NB_PHILOSOPHERS];
	t_philo				list_philo[NB_PHILOSOPHERS];
	pthread_t			list_philo_thread[NB_PHILOSOPHERS];
	uint64_t			time;
	t_bool				start;
	uint64_t			actual_turn;
	WINDOW				*window;
	pthread_mutex_t		print;
	pthread_mutex_t		log;
	pthread_mutex_t		log_philo;
	char				log_buf[SYSTEM_BOX_X - 4];
	char				log_philo_buf[SYSTEM_BOX_X - 4];
}				t_table;

/*
**	-- Function
**	ft_tools.c
*/
extern void		ft_wait(const uint64_t *variable, const uint64_t value);
extern void		ft_create_philo_name(void);

/*
**	ft_stick.c
*/
extern void		ft_initialize_stick(void);
extern void		ft_finalize_stick(void);

/*
**	ft_ncurses
*/
extern void		ft_init_ncurses(void);
extern void		ft_draw_box_system(void);
extern void		ft_draw_box_philo(uint64_t n);

/*
**	ft_log.c
*/
extern void		ft_print(int y, int x, char *format);
extern void		ft_system_log(uint64_t color, t_bool new_turn, char *format);
extern void		ft_system_log_philo(char *format, char *name);
extern void		ft_print_turn(void);

/*
**	ft_philo_action.c
*/
extern void		ft_think(t_philo *philo, uint64_t *actual_turn);
extern void		ft_try_eat(t_philo *philo, uint64_t *actual_turn);
extern t_bool	ft_check_other_philo(uint64_t id);

/*
**	ft_philo_info
*/
extern void		ft_print_philo_info(void);

/*
**	ft_philo
*/
extern void		*ft_philo(void *arg);

/*
**	ft_system
*/
extern t_bool	ft_init_system(void);

#endif
