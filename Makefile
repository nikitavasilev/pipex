# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/15 23:52:19 by nvasilev          #+#    #+#              #
#    Updated: 2022/05/02 21:16:50 by nvasilev         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
#                                     CONFIG                                   #
################################################################################

NAME = 			pipex

CC = 			cc
CFLAGS =		-Wall -Wextra -Werror -g3
RM =			rm -f

INCLUDES = -I ./includes/

################################################################################
#                                 PROGRAM'S SRCS                               #
################################################################################

SRCS =			./srcs/main.c \
				./srcs/extract_path.c \
				./srcs/get_cmds.c

UTILS =			./utils/ft_split.c \
				./utils/ft_strlen.c \
				./utils/ft_substr.c \
				./utils/ft_strdup.c \
				./utils/ft_strncmp.c \
				./utils/ft_strjoin.c \
				./utils/ft_putchar_fd.c \
				./utils/ft_putstr_fd.c

################################################################################
#                                     OBJECTS                                  #
################################################################################

OBJS_SRCS =		$(SRCS:%.c=%.o)
OBJS =			$(UTILS:%.c=%.o)

################################################################################
#                                     RULES                                    #
################################################################################

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS) $(OBJS_SRCS)
	$(CC) $(OBJS) $(OBJS_SRCS) $(INCLUDES) -o $@

clean:
	$(RM) $(OBJS) $(OBJS_SRCS)

fclean:	clean
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re
