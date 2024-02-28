# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/06 16:14:01 by lmattern          #+#    #+#              #
#    Updated: 2024/02/28 11:00:32 by lmattern         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC				:=	gcc -g
NAME			:=	exec
CFLAGS			:=	-Wextra -Wall -Werror
LIBFT			:=	./libft
HEADERS			:=	-I ./inc
LIBS			:=	-L$(LIBFT) -lft
PATHLIBS		:=	./lib/libft.a
DEPS			:=	./inc/exec.h
SRCDIR			:=	./src/exec
SRCS			:=	$(SRCDIR)/main.c \
					$(SRCDIR)/parsing.c \
					$(SRCDIR)/printing.c \
					$(SRCDIR)/executing.c
OBJDIR			:=	./.obj
OBJS			:=	$(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(LIBFT) $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS) | $(OBJDIR)
	$(CC) $(CFLAGS) -g -o $@ -c $< $(HEADERS)

$(LIBFT)/libft.a: FORCE
	make -C $(LIBFT)

$(NAME): $(LIBFT)/libft.a $(OBJS) | $(OBJDIR)
	$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT)

re: clean all

FORCE:

.PHONY: all, clean, FORCE, fclean, re