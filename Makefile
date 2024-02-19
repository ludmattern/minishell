# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/06 16:14:01 by lmattern          #+#    #+#              #
#    Updated: 2024/02/17 22:23:10 by lmattern         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC				:=	cc
NAME			:=	pipex
CFLAGS			:=	-Wextra -Wall -Werror
LIBFT			:=	./lib/libft
LIBFTPRINTF		:=	./lib/ft_printf
HEADERS			:=	-I ./inc
LIBS			:=	-L$(LIBFTPRINTF) -lftprintf -L$(LIBFT) -lft
PATHLIBS		:=	./lib/libftprintf.a ./lib/libft.a
DEPS			:=	./inc/pipex.h
SRCDIR			:=	./src
SRCS			:=	$(SRCDIR)/main.c \
					$(SRCDIR)/handle_files.c \
					$(SRCDIR)/handle_cmds.c \
					$(SRCDIR)/handle_cmds2.c \
					$(SRCDIR)/handle_errors.c \
					$(SRCDIR)/utils.c \
					$(SRCDIR)/parse_cmd.c \
					$(SRCDIR)/parse_cmd_2.c \
					$(SRCDIR)/parse_cmd_3.c \
					$(SRCDIR)/count_args.c \
					$(SRCDIR)/handle_process.c
OBJDIR			:=	./obj
OBJS			:=	$(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(LIBFTPRINTF) $(LIBFT) $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS) | $(OBJDIR)
	$(CC) $(CFLAGS) -g -o $@ -c $< $(HEADERS)

$(LIBFT)/libft.a: FORCE
	make -C $(LIBFT)

$(LIBFTPRINTF)/libftprintf.a: FORCE
	make -C $(LIBFTPRINTF)

$(NAME): $(LIBFTPRINTF)/libftprintf.a $(LIBFT)/libft.a $(OBJS) | $(OBJDIR)
	$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFTPRINTF)
	make fclean -C $(LIBFT)

re: clean all

FORCE:

.PHONY: all, clean, FORCE, fclean, re