# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/06 16:14:01 by lmattern          #+#    #+#              #
#    Updated: 2024/04/09 11:41:04 by lmattern         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC				:=	cc
NAME			:=	minishell
CFLAGS			:=	-Wextra -Wall -Werror
LIBFT			:=	./libft
HEADERS			:=	-I ./inc
LIBS			:=	-L$(LIBFT) -lft
PATHLIBS		:=	./lib/libft.a
DEPS			:=	./inc/exec.h ./inc/error_codes.h ./inc/parse.h ./inc/minishell.h

EXECDIR			:=	./src/exec
PARSEDIR		:=	./src/parsing
MAINDIR			:=	./src/

MAIN_SRC		:=	$(MAINDIR)/minishell.c $(MAINDIR)/shell_signals.c 

EXEC_SRCS		:=	$(EXECDIR)/executing.c $(EXECDIR)/execution_utils.c \
					$(EXECDIR)/applying_redirections.c \
					$(EXECDIR)/applying_redirections_utils.c \
					$(EXECDIR)/handling_command.c \
					$(EXECDIR)/handling_logic.c \
					$(EXECDIR)/handling_pipeline.c \
					$(EXECDIR)/handling_pipeline_utils.c \
					$(EXECDIR)/ft_echo.c $(EXECDIR)/ft_cd.c \
					$(EXECDIR)/ft_pwd.c $(EXECDIR)/ft_export.c \
					$(EXECDIR)/handling_env.c $(EXECDIR)/ft_unset.c \
					$(EXECDIR)/ft_env.c $(EXECDIR)/ft_export_utils.c \
					$(EXECDIR)/duplicate_env.c $(EXECDIR)/ft_exit.c \
					$(EXECDIR)/command_exec_failure.c \
					$(EXECDIR)/ft_add_local.c \
					$(EXECDIR)/initialize_environnement.c

PARSE_SRCS		:=	$(PARSEDIR)/clean.c $(PARSEDIR)/command_path.c $(PARSEDIR)/env_var.c \
					$(PARSEDIR)/expander_utils.c $(PARSEDIR)/expander.c $(PARSEDIR)/get_io.c \
					$(PARSEDIR)/get_token_tab.c $(PARSEDIR)/lexing.c \
					$(PARSEDIR)/parse_utils.c $(PARSEDIR)/parse.c $(PARSEDIR)/printfunc.c \
					$(PARSEDIR)/syntaxe_checker.c

OBJDIR			:=	./.obj

MAIN_OBJ		:=	$(MAIN_SRC:$(MAINDIR)/%.c=$(OBJDIR)/%.o)
EXEC_OBJS		:=	$(EXEC_SRCS:$(EXECDIR)/%.c=$(OBJDIR)/%.o) 
PARSE_OBJS		:=	$(PARSE_SRCS:$(PARSEDIR)/%.c=$(OBJDIR)/%.o)

OBJS			:=	$(MAIN_OBJ) $(EXEC_OBJS) $(PARSE_OBJS)

all: $(LIBFT) $(NAME)

$(OBJDIR)/%.o: $(EXECDIR)/%.c $(DEPS) | $(OBJDIR)
	$(CC) $(CFLAGS) -g -o $@ -c $< $(HEADERS)

$(OBJDIR)/%.o: $(PARSEDIR)/%.c $(DEPS) | $(OBJDIR)
	$(CC) $(CFLAGS) -g -o $@ -c $< $(HEADERS)

$(OBJDIR)/%.o: $(MAINDIR)/%.c $(DEPS) | $(OBJDIR)
	$(CC) $(CFLAGS) -g -o $@ -c $< $(HEADERS)

$(LIBFT)/libft.a: FORCE
	@make -C $(LIBFT)

$(NAME): $(LIBFT)/libft.a $(OBJS) | $(OBJDIR)
	$(CC) $(OBJS) $(LIBS) $(HEADERS) $(CFLAGS) -lreadline -o $(NAME)

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