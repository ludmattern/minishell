# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/06 16:14:01 by lmattern          #+#    #+#              #
#    Updated: 2024/04/19 17:25:27 by lmattern         ###   ########.fr        #
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

PARSE_SRCS		:=	$(PARSEDIR)/clean.c $(PARSEDIR)/command_path.c $(PARSEDIR)/env_var.c \
					$(PARSEDIR)/expander_utils.c $(PARSEDIR)/expander.c $(PARSEDIR)/get_io.c \
					$(PARSEDIR)/get_token_tab.c $(PARSEDIR)/lexing.c \
					$(PARSEDIR)/parse_utils.c $(PARSEDIR)/parse.c $(PARSEDIR)/printfunc.c \
					$(PARSEDIR)/checker_syntax_quote.c $(PARSEDIR)/signal.c $(PARSEDIR)/clean2.c \
					$(PARSEDIR)/lexing_utils.c $(PARSEDIR)/launch_expand.c $(PARSEDIR)/del_redir.c \
					$(PARSEDIR)/launch_syntaxe_check.c $(PARSEDIR)/get_env_var.c \
					$(PARSEDIR)/heredoc_env_var.c $(PARSEDIR)/add_io.c $(PARSEDIR)/io_utils.c \
					$(PARSEDIR)/env_var2.c $(PARSEDIR)/env_var_size.c $(PARSEDIR)/signal2.c \
					$(PARSEDIR)/heredeocc.c $(PARSEDIR)/check_syntaxe_par.c \
					$(PARSEDIR)/check_syntaxe_first.c $(PARSEDIR)/check_syntax_redir.c

OBJDIR			:=	./.obj

MAIN_OBJ		:=	$(MAIN_SRC:$(MAINDIR)/%.c=$(OBJDIR)/%.o)
EXEC_OBJS		:=	$(EXEC_SRCS:$(EXECDIR)/%.c=$(OBJDIR)/%.o) 
PARSE_OBJS		:=	$(PARSE_SRCS:$(PARSEDIR)/%.c=$(OBJDIR)/%.o)

OBJS			:=	$(MAIN_OBJ) $(EXEC_OBJS) $(PARSE_OBJS)

all : SYSTEM = BASH
all : normal_spinner $(LIBFT) $(NAME) stop_normal_spinner

bash : SYSTEM = BASH
bash: bash_spinner $(LIBFT) $(NAME) stop_bash_spinner

zsh : SYSTEM = ZSH
zsh: zsh_spinner $(LIBFT) $(NAME) stop_zsh_spinner

$(OBJDIR)/%.o: $(EXECDIR)/%.c $(DEPS) | $(OBJDIR)
	@$(CC) $(CFLAGS) -g -o $@ -c $< $(HEADERS)

$(OBJDIR)/%.o: $(PARSEDIR)/%.c $(DEPS) | $(OBJDIR)
	@$(CC) $(CFLAGS) -g -o $@ -c $< $(HEADERS)

$(OBJDIR)/%.o: $(MAINDIR)/%.c $(DEPS) | $(OBJDIR)
	@$(CC) $(CFLAGS) -g -o $@ -c $< $(HEADERS)

$(LIBFT)/libft.a: FORCE
	@make -C $(LIBFT) -s

$(NAME): $(LIBFT)/libft.a $(OBJS) | $(OBJDIR)
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) $(CFLAGS) -DSYSTEM=\"$(SYSTEM)\" -lreadline -o $(NAME)

normal_spinner:
	@echo "\033[32mCompiling mandatory minishell ....\c\033[0m"
	@while :; do \
		for s in / - \\ \|; do \
			printf "\b$$s"; \
			sleep 0.1; \
		done; \
	done & echo $$! > spinner_pid.txt

stop_normal_spinner:
	@kill `cat spinner_pid.txt` 2>/dev/null || true
	@rm -f spinner_pid.txt
	@echo "\nCompilation of mandatory minishell sucessful."


bash_spinner:
	@echo "\033[1;32mCompiling minibash ....\c\033[0m"
	@while :; do \
		for s in / - \\ \|; do \
			printf "\b$$s"; \
			sleep 0.1; \
		done; \
	done & echo $$! > spinner_pid.txt

stop_bash_spinner:
	@kill `cat spinner_pid.txt` 2>/dev/null || true
	@rm -f spinner_pid.txt
	@echo "\nCompilation of minibash sucessful."
	@sleep 0.1; 
	@echo "\033[32m  __  __  _         _       _           _  _ "
	@sleep 0.1; 
	@echo "\033[32m |  \/  |(_) _ __  (_) ___ | |__   __  | || |"
	@sleep 0.1; 
	@echo "\033[32m | |\/| || || '_ \ | |/ _ /|  _ \ / _ \| || |"
	@sleep 0.1; 
	@echo "\033[32m | |  | || || | | || |\__ \| | | |  __/| || |"
	@sleep 0.1; 
	@echo "\033[32m |_|  |_||_||_| |_||_||___/|_| |_|\___||_||_|"
	@sleep 0.1; 
	@echo "\033[1;32m\nReady to go ! 🚀\033[0m"


zsh_spinner:
	@echo "\033[1;32mCompiling minizshell ....\c\033[0m"
	@while :; do \
		for s in / - \\ \|; do \
			printf "\b$$s"; \
			sleep 0.1; \
		done; \
	done & echo $$! > spinner_pid.txt

stop_zsh_spinner:
	@kill `cat spinner_pid.txt` 2>/dev/null || true
	@rm -f spinner_pid.txt
	@echo "\nCompilation of minizsh sucessful."
	@sleep 0.1; 
	@echo "\033[32m                                                                    "
	@sleep 0.1; 
	@echo "\033[32m ███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     "
	@sleep 0.1; 
	@echo "\033[32m ████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     "
	@sleep 0.1; 
	@echo "\033[32m ██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     "
	@sleep 0.1; 
	@echo "\033[32m ██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     "
	@sleep 0.1; 
	@echo "\033[32m ██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗"
	@sleep 0.1;
	@echo "\033[32m ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝"
	@sleep 0.1;
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