# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jcarra <jcarra@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/04/17 13:07:37 by jcarra            #+#    #+#              #
#    Updated: 2018/04/19 15:17:53 by nathan           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	philo

SRC		=	ft_log.c		\
			ft_ncurses.c		\
			ft_philo.c		\
			ft_philo_action.c	\
			ft_philo_info.c		\
			ft_stick.c		\
			ft_system.c		\
			ft_tools.c		\
			library.c		\
			main.c

DIRSRC		=	sources/
DIRINC		=	include/
DIRLIB		=	library/
DIROBJ		=	objects/

SRCS		=	$(SRC:%=$(DIRSRC)%)
LIBS		=	$(LIB:%=$(DIRLIB)%)

OBJS		=	$(addprefix $(DIROBJ), $(SRC:.c=.o))

CC		=	gcc -Wall -Wextra -Werror -I $(DIRINC)
RM		=	rm -rf
ECHO		=	printf
MAKE		=	make -C

LFLAGS		=	-lncurses -lpthread

CLEAN		=	"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         "
LEN		=	$$(/bin/echo -n $(NAME) | wc -c)
DELTA		=	$$(echo "$$(tput cols)-31-$(LEN)"|bc)
NB		=	$(words $(SRC))
INDEX		=	0
SHELL		=	/bin/bash

all		:	$(NAME)

$(NAME)		:	$(DIROBJ) $(OBJS)
	@$(eval OBJ_LEN=$(shell echo "$$(echo "$(NAME)" | sed 's/^.*\///')" | wc -c))
	@$(eval CLEAN_LEN=$(shell echo "$$(tput cols)-6-$(OBJ_LEN)"|bc))
	@$(CC) -o $(NAME) $(OBJS) $(LFLAGS)
	@$(ECHO) "\r\033[38;5;040m✓ MAKE $(NAME)%.*s\033[0m\033[0m\n" $(CLEAN_LEN) $(CLEAN)

$(DIROBJ)%.o	:	$(DIRSRC)%.c | $(DIROBJ)
	@$(eval DONE=$(shell echo $$(($(INDEX)*20/$(NB)))))
	@$(eval PERCENT=$(shell echo $$(($(INDEX)*100/$(NB)))))
	@$(eval TO_DO=$(shell echo $$((20-$(INDEX)*20/$(NB) - 1))))
	@$(eval COLOR=$(shell list=(20 21 26 27 32 33 38 39 44 45 50 51); index=$$(($(PERCENT) * $${#list[@]} / 100)); echo "$${list[$$index]}"))
	@$(ECHO) "\r\033[38;5;%dm⌛ [%s]: %2d%% `printf '█%.0s' {0..$(DONE)}`%*s❙%*.*s\033[0m\033[0m" $(COLOR) $(NAME) $(PERCENT) $(TO_DO) "" $(DELTA) $(DELTA) "$(shell echo "$@" | sed 's/^.*\///')"
	@$(CC) -o $@ -c $<
	@$(eval OBJ_LEN=$(shell echo "$$(echo "$@" | sed 's/^.*\///')" | wc -c))
	@$(eval CLEAN_LEN=$(shell echo "$$(tput cols)-1-$(OBJ_LEN)"|bc))
	@$(ECHO) "\r\033[38;5;%dm✓ %s\033[0m\033[0m%.*s\n\033[38;5;%dm⌛ [%s]: %2d%% `printf '█%.0s' {0..$(DONE)}`%*s❙%*.*s\033[0m\033[0m"  $(COLOR) "$(shell echo "$@" | sed 's/^.*\///')"  $(CLEAN_LEN) $(CLEAN) $(COLOR) $(NAME) $(PERCENT) $(TO_DO) "" $(DELTA) $(DELTA) "$(shell echo "$@" | sed 's/^.*\///')"
	@$(eval INDEX=$(shell echo $$(($(INDEX)+1))))

$(DIROBJ)	:
	@mkdir -p $(DIROBJ)

clean		:
	@if [ -e $(DIROBJ) ];						\
	then								\
		$(RM) $(DIROBJ);					\
		$(ECHO) "\033[38;5;202m✗ $(NAME)\033[0m\033[0m\n";	\
	fi;

fclean		:	clean
	@if [ -e $(NAME) ];						\
	then								\
		$(RM) $(NAME);						\
		$(ECHO) "\033[38;5;196m✗ $(NAME)\033[0m\033[0m\n";	\
	fi;

re		:	fclean all

.PHONY		:	all fclean clean re
