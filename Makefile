# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/06 16:14:01 by lmattern          #+#    #+#              #
#    Updated: 2024/03/11 15:24:50 by lmattern         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC				:=	gcc
NAME			:=	exec
CFLAGS			:=	-Wextra -Wall -Werror
LIBFT			:=	./libft
HEADERS			:=	-I ./inc
LIBS			:=	-L$(LIBFT) -lft
PATHLIBS		:=	./lib/libft.a
DEPS			:=	./inc/exec.h ./inc/error_codes.h
SRCDIR			:=	./src/exec
SRCS			:=	$(SRCDIR)/main.c \
					$(SRCDIR)/parsing.c \
					$(SRCDIR)/printing.c \
					$(SRCDIR)/executing.c \
					$(SRCDIR)/execution_utils.c \
					$(SRCDIR)/applying_redirections.c \
					$(SRCDIR)/applying_redirections_utils.c \
					$(SRCDIR)/handling_command.c \
					$(SRCDIR)/handling_logic.c \
					$(SRCDIR)/handling_pipeline.c \
					$(SRCDIR)/handling_pipeline_utils.c \
					$(SRCDIR)/ft_echo.c \
					$(SRCDIR)/ft_cd.c \
					$(SRCDIR)/ft_pwd.c
OBJDIR			:=	./.obj
OBJS			:=	$(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(LIBFT) $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS) | $(OBJDIR)
	$(CC) $(CFLAGS) -g -o $@ -c $< $(HEADERS)

$(LIBFT)/libft.a: FORCE
	@make -C $(LIBFT)

$(NAME): $(LIBFT)/libft.a $(OBJS) | $(OBJDIR)
	$(CC) $(OBJS) $(LIBS) $(HEADERS) $(CFLAGS) -o $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

valgrind: $(NAME)
	clear
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --track-fds=yes ./$(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT)

re: fclean all

FORCE:

.PHONY: all, clean, run, valgrind, FORCE, fclean, re