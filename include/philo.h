//
// Created by Nathan on 23/01/18.
//

#ifndef PHILOSOPHERS_PHILO_H
#define PHILOSOPHERS_PHILO_H

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
#define		EAT_T		(uint64_t)3
#define		REST_T		(uint64_t)2
#define		THINK_T		(uint64_t)3
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

typedef enum	e_action t_action;
enum e_action {
	EAT,
	REST,
	THINK
};


/* -- structure */
typedef struct	s_stick {
	pthread_mutex_t		lock;
	t_bool				is_use;
}				t_stick;

typedef struct	s_philo {
	t_stick				*left;
	t_stick				*right;
	uint64_t			life;
	t_action			action;
}				t_philo;

typedef struct	s_table {
	t_stick				list_stick[NB_PHILOSOPHERS];
	t_philo				list_philo[NB_PHILOSOPHERS];
	pthread_t			list_philo_thread[NB_PHILOSOPHERS];
	uint64_t			time;
	t_bool				start;
}				t_table;

#endif //PHILOSOPHERS_PHILO_H
