# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/15 23:52:19 by nvasilev          #+#    #+#              #
#    Updated: 2022/04/15 23:53:22 by nvasilev         ###   ########.fr        #
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
				./srcs/create_node.c \
				./srcs/del_list.c \
				./srcs/alloc_struct.c \
				./srcs/swap.c \
				./srcs/rotate.c \
				./srcs/rrotate.c \
				./srcs/print_list.c \
				./srcs/parser.c \
				./srcs/fill_stack.c \
				./srcs/check_dups.c \
				./srcs/push.c \
				./srcs/sort.c

UTILS =			./utils/ft_putchar_fd.c \
				./utils/ft_putstr_fd.c \
				./utils/ft_error.c \
				./utils/ft_isdigit.c \
				./utils/ft_isspace.c \
				./utils/ft_issign.c \
				./utils/ft_split.c \
				./utils/ft_strlen.c \
				./utils/ft_strdup.c \
				./utils/atoi_err_overflow.c \
				./utils/strjoin_free_s1.c \
				./utils/ft_putnbr_fd.c

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
