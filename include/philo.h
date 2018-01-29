//
// Created by Nathan on 23/01/18.
//

#ifndef PHILOSOPHERS_PHILO_H
#define PHILOSOPHERS_PHILO_H

#include <curses.h>
#include <pthread.h>
#include <sys/types.h>
#include "types.h"

/* -- default macros */
/*
**	Macros define by the subject
**
**	-> MAX_LIFE is the number of life point for each philosophers
**	-> EAT_T is the time to eat for a philosopher in second
**	-> REAT_T is the rest time for a philosopher in second
**	-> THINK_T is the thinking time for a philosopher in second
**	-> TIMEOUT is the duration of the game in second
*/

#define		MAX_LIFE	(uint64_t)10
#define		EAT_T		(uint64_t)2
#define		REST_T		(uint64_t)1
#define		THINK_T		(uint64_t)2
#define		TIMEOUT		(uint64_t)30


/* -- internal macros */
/*
**	Extracted macros from the subject
**
**	-> RESTORED_LIFE is the number of life point restore when a philosopher rest
**	-> DPS is the number of life point lost every second
**	-> NB_PHILOSOPHERS is the number of philospoher arond the table
**	-> TIMEOUT_TXT is the displayed text at the end of the game
*/

#define		RESTORED_LIFE	MAX_LIFE
#define		DPS				(uint64_t)1
#define		NB_PHILOSOPHERS	(uint32_t)7
#define		TIMEOUT_TXT		"Now, it is time... To DAAAAAAAANCE!!!"
#define		SLEEP_TIME		1


/* -- internal graphique macros */
#define		WINDOW_SIZE_X		94
#define		WINDOW_SIZE_Y		30

#define		PHILOSOPHER_BOX_X	14
#define		PHILOSOPHER_BOX_Y	6

#define		SYSTEM_BOX_X		92
#define		SYSTEM_BOX_Y		22

#define		LOG_CLEAR			"                                                                                        "


typedef enum	e_action t_action;
enum e_action {
	EAT,
	REST,
	THINK
};


/* -- structure */
typedef struct	s_stick {
	/* -- core */
	pthread_mutex_t		lock;
	t_bool				is_use;
	/* -- graphique */
	/* -- bonus */
}				t_stick;

typedef struct	s_philo {
	/* -- core */
	t_stick				*left;
	t_stick				*right;
	uint64_t			life;
	t_action			action;
	/* -- graphique */
	/* -- bonus */
	t_pchar				name;
}				t_philo;

typedef struct	s_table {
	/* -- core */
	t_stick				list_stick[NB_PHILOSOPHERS];
	t_philo				list_philo[NB_PHILOSOPHERS];
	pthread_t			list_philo_thread[NB_PHILOSOPHERS];
	uint64_t			time;
	t_bool				start;
	uint64_t			actual_turn;
	/* -- graphique */
	WINDOW				*window;
	pthread_mutex_t		print;
	pthread_mutex_t		log;
	pthread_mutex_t		log_philo;
	char				log_buf[SYSTEM_BOX_X - 4];
	char				log_philo_buf[SYSTEM_BOX_X - 4];
	/* -- bonus */
}				t_table;

#endif //PHILOSOPHERS_PHILO_H
