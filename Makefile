# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/15 23:52:19 by nvasilev          #+#    #+#              #
#    Updated: 2022/05/16 06:05:20 by nvasilev         ###   ########.fr        #
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
				./srcs/get_cmds.c \
				./srcs/free_args.c \
				./srcs/close_stdio.c \
				./srcs/print_err.c \
				./srcs/exit_failure.c \
				./srcs/init_args.c \
				./srcs/is_dir.c \
				./srcs/exec_cmd.c \
				./srcs/child_procs.c \
				./srcs/pipex.c \
				./srcs/heredoc.c \
				./srcs/exit_status.c

UTILS =			./utils/ft_split.c \
				./utils/ft_strlen.c \
				./utils/ft_strdup.c \
				./utils/ft_strncmp.c \
				./utils/ft_strjoin.c \
				./utils/ft_putchar_fd.c \
				./utils/ft_putstr_fd.c \
				./utils/ft_strchr.c \
				./utils/ft_putnbr_fd.c \
				./utils/ft_substr.c \
				./utils/get_next_line.c

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

bonus:	all

.PHONY: all clean fclean re
