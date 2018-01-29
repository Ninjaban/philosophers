#  **************************************************************************  #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jcarra <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/01/01 00:00:00 by jcarra            #+#    #+#              #
#    Updated: 2017/12/12 15:20:21 by jcarra           ###   ########.fr        #
#                                                                              #
#  **************************************************************************  #

NAME		=   philo

SRC 		=   ft_philo.c

DIRSRC  	=	sources/
DIRINC		=	include/

SRCS		=	$(SRC:%=$(DIRSRC)%)

OBJS		=	$(SRCS:.c=.o)

CFLAGS		=	-Wall -Wextra -Werror -I./$(DIRINC)
LFLAGS		=   -lpthread -lncurses

CC	    	=	gcc
RM		    =	rm -f
ECHO		=	printf
MAKE		=	make -C


all		:	$(NAME)
			@$(ECHO) '\033[32m>\033[0m $(NAME) : [\033[34mAll\033[0m] ->\033[32m\tReady\n\033[0m'

$(NAME)	:	.libft .hide
			@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LFLAGS)

%.o		:	%.c
			@$(CC) -o $@ -c $< $(CFLAGS)
			@$(ECHO) '\-\> \033[36m$@\033[0m\n'

.hide	:	$(OBJS)
			@touch .hide
			@chmod 755 .hide
			@$(ECHO) '\033[32m>\033[0m $(NAME) : [\033[33m$(NAME)\033[0m]    ->\033[32m	Ready\n\033[0m'

.libft		:
			@touch .libft
			@chmod 755 .libft

clean		:
			@if [ -e $(OBJS) ]; \
			then \
				$(RM) $(OBJS) .hide .libft; \
				$(ECHO) '\r\033[38;5;202m✗ $(NAME)\033[0m\033[0m\n'; \
			fi;

fclean		:	clean
			@if [ -e $(NAME) ]; \
			then \
				$(RM) $(NAME); \
				$(ECHO) "\r\033[38;5;196m✗ $(NAME)\033[0m\033[0m\n"; \
			fi;

re		:	fclean all

.PHONY		:	all clean fclean re

