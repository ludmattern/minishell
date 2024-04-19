# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/06 16:14:01 by lmattern          #+#    #+#              #
#    Updated: 2024/04/19 15:57:38 by lmattern         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC				:=	cc
NAME			:=	minishell
CFLAGS			:=	-Wextra -Wall -Werror
LIBFT			:=	./libft
HEADERS			:=	-I ./inc
LIBS			:=	-L$(LIBFT) -lft
PATHLIBS		:=	./lib/libft.a

DEPS			:=	./inc/error_codes.h \
					./inc/minishell.h \
					./inc/parse.h \
					./inc/exec.h

PARSEDIR		:=	./src/parsing
EXECDIR			:=	./src/exec
MAINDIR			:=	./src/

MAIN_SRC		:=	$(MAINDIR)/minishell.c \
					$(MAINDIR)/launch2.c \
					$(MAINDIR)/launch1.c

EXEC_SRCS		:=	$(EXECDIR)/applying_redirections_utils.c \
					$(EXECDIR)/initialize_environnement.c \
					$(EXECDIR)/handling_pipeline_utils.c \
					$(EXECDIR)/applying_redirections.c \
					$(EXECDIR)/command_exec_failure.c \
					$(EXECDIR)/handling_pipeline.c \
					$(EXECDIR)/handling_command.c \
					$(EXECDIR)/ft_export_utils.c \
					$(EXECDIR)/execution_utils.c \
					$(EXECDIR)/handling_logic.c \
					$(EXECDIR)/duplicate_env.c \
					$(EXECDIR)/ft_add_local.c \
					$(EXECDIR)/printing_env.c \
					$(EXECDIR)/handling_env.c \
					$(EXECDIR)/executing.c \
					$(EXECDIR)/ft_export.c \
					$(EXECDIR)/env_utils.c \
					$(EXECDIR)/ft_unset.c \
					$(EXECDIR)/ft_echo.c \
					$(EXECDIR)/ft_exit.c \
					$(EXECDIR)/ft_pwd.c \
					$(EXECDIR)/ft_env.c \
					$(EXECDIR)/ft_cd.c

PARSE_SRCS		:=	$(PARSEDIR)/launch_syntaxe_check.c \
					$(PARSEDIR)/heredoc_env_var.c \
					$(PARSEDIR)/syntaxe_checker.c \
					$(PARSEDIR)/expander_utils.c \
					$(PARSEDIR)/get_token_tab.c \
					$(PARSEDIR)/launch_expand.c \
					$(PARSEDIR)/lexing_utils.c \
					$(PARSEDIR)/command_path.c \
					$(PARSEDIR)/get_env_var.c \
					$(PARSEDIR)/parse_utils.c \
					$(PARSEDIR)/printfunc.c \
					$(PARSEDIR)/del_redir.c \
					$(PARSEDIR)/expander.c \
					$(PARSEDIR)/io_utils.c \
					$(PARSEDIR)/env_var.c \
					$(PARSEDIR)/lexing.c \
					$(PARSEDIR)/signal.c \
					$(PARSEDIR)/get_io.c \
					$(PARSEDIR)/clean2.c \
					$(PARSEDIR)/add_io.c \
					$(PARSEDIR)/clean.c \
					$(PARSEDIR)/parse.c

OBJDIR			:=	./.obj

MAIN_OBJ		:=	$(MAIN_SRC:$(MAINDIR)/%.c=$(OBJDIR)/%.o)
EXEC_OBJS		:=	$(EXEC_SRCS:$(EXECDIR)/%.c=$(OBJDIR)/%.o) 
PARSE_OBJS		:=	$(PARSE_SRCS:$(PARSEDIR)/%.c=$(OBJDIR)/%.o)

OBJS			:=	$(MAIN_OBJ) $(EXEC_OBJS) $(PARSE_OBJS)

all: $(LIBFT) $(NAME)

$(OBJDIR)/%.o: $(EXECDIR)/%.c $(DEPS) | $(OBJDIR)
	@$(CC) $(CFLAGS) -g -o $@ -c $< $(HEADERS)

$(OBJDIR)/%.o: $(PARSEDIR)/%.c $(DEPS) | $(OBJDIR)
	@$(CC) $(CFLAGS) -g -o $@ -c $< $(HEADERS)

$(OBJDIR)/%.o: $(MAINDIR)/%.c $(DEPS) | $(OBJDIR)
	@$(CC) $(CFLAGS) -g -o $@ -c $< $(HEADERS)

$(LIBFT)/libft.a: FORCE
	@make -C $(LIBFT) -s

$(NAME): spinner $(LIBFT)/libft.a $(OBJS) | $(OBJDIR) stop_spinner
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) $(CFLAGS) -lreadline -o $(NAME)

spinner:
	@echo "\033[1;32mCompiling minishell ....\c\033[0m"
	@while :; do \
		for s in / - \\ \|; do \
			printf "\b$$s"; \
			sleep 0.1; \
		done; \
	done & echo $$! > spinner_pid.txt

stop_spinner:
	@kill `cat spinner_pid.txt` 2>/dev/null || true
	@rm -f spinner_pid.txt
	@echo "\nCompilation of minishell sucessful."
	@echo "\n\033[34m   ▄▄▄▄███▄▄▄▄    ▄█  ███▄▄▄▄    ▄█     ▄████████ \
	   ▄█    █▄       ▄████████  ▄█        ▄█       "
	@sleep 0.1;
	@echo "\033[35m ▄██▀▀▀███▀▀▀██▄ ███  ███▀▀▀██▄ ███    ███    ███  \
	 ███    ███     ███    ███ ███       ███       "
	@sleep 0.1;
	@echo "\033[36m ███   ███   ███ ███▌ ███   ███ ███▌   ███    █▀    \
	███    ███     ███    █▀  ███       ███       "
	@sleep 0.1;
	@echo "\033[33m ███   ███   ███ ███▌ ███   ███ ███▌   ███         \
	▄███▄▄▄▄███▄▄  ▄███▄▄▄     ███       ███       "
	@sleep 0.1;
	@echo "\033[34m ███   ███   ███ ███▌ ███   ███ ███▌ ▀███████████ \
	▀▀███▀▀▀▀███▀  ▀▀███▀▀▀     ███       ███       "
	@sleep 0.1;
	@echo "\033[35m ███   ███   ███ ███  ███   ███ ███           ███  \
	 ███    ███     ███    █▄  ███       ███       "
	@sleep 0.1;
	@echo "\033[36m ███   ███   ███ ███  ███   ███ ███     ▄█    ███  \
	 ███    ███     ███    ███ ███▌    ▄ ███▌    ▄ "
	@sleep 0.1;
	@echo "\033[33m  ▀█   ███   █▀  █▀    ▀█   █▀  █▀    ▄████████▀    \
	███    █▀      ██████████ █████▄▄██ █████▄▄██ "
	@sleep 0.1;
	@echo "\033[34m                                                    \
	                               ▀▀        ▀▀   "
	@echo "\033[1;32m\nReady to take off ! 🚀\033[0m"

$(OBJDIR):
	@mkdir -p $(OBJDIR)

valgrind: $(NAME)
	clear
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --track-fds=yes --suppressions=./mask_readline_leaks.supp ./$(NAME)

valenv: $(NAME)
	clear
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --track-fds=yes --suppressions=./mask_readline_leaks.supp env -i ./$(NAME)

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFT) -s

re: fclean all

FORCE:

.PHONY: all, clean, run, valgrind, valenv, FORCE, fclean, re