# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/06 16:14:01 by lmattern          #+#    #+#              #
#    Updated: 2024/05/13 10:45:12 by lmattern         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all, clean, FORCE, fclean, re, bash_spinner, stop_bash_spinner, \
bonus, zsh_spinner, stop_zsh_spinner

CC					:=	cc
NAME				:=	minishell
NAME_BONUS			:=	minishell_bonus
CFLAGS				:=	-Wextra -Wall -Werror
LIBFT				:=	./libft

HEADERS				:=	-I ./inc/mandatory
HEADERS_BONUS		:=	-I ./inc/bonus

LIBS				:=	-L$(LIBFT) -lft
PATHLIBS			:=	./lib/libft.a

DEPS				:=	./inc/mandatory/error_codes.h \
					./inc/mandatory/minishell.h \
					./inc/mandatory/parse.h \
					./inc/mandatory/exec.h

DEPS_BONUS			:=	./inc/bonus/error_codes_bonus.h \
					./inc/bonus/minishell_bonus.h \
					./inc/bonus/parse_bonus.h \
					./inc/bonus/exec_bonus.h

PARSEDIR			:=	./src/parsing
EXECDIR				:=	./src/exec
MAINDIR				:=	./src/

MAIN_SRC			:=	$(MAINDIR)/handle_user_input.c \
					$(MAINDIR)/update_input.c \
					$(MAINDIR)/minishell.c
			
MAIN_SRC_BONUS		:=	$(MAINDIR)/handle_user_input_bonus.c \
					$(MAINDIR)/update_input_bonus.c \
					$(MAINDIR)/minishell_bonus.c

EXEC_SRCS			:=	$(EXECDIR)/applying_redirections_utils.c \
					$(EXECDIR)/initialize_environnement.c \
					$(EXECDIR)/initialisation_env_utils.c \
					$(EXECDIR)/handling_pipeline_utils.c \
					$(EXECDIR)/applying_redirections.c \
					$(EXECDIR)/command_exec_failure.c \
					$(EXECDIR)/handling_builtins.c \
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
					$(EXECDIR)/mini_env.c \
					$(EXECDIR)/ft_echo.c \
					$(EXECDIR)/ft_exit.c \
					$(EXECDIR)/ft_pwd.c \
					$(EXECDIR)/ft_env.c \
					$(EXECDIR)/ft_cd.c

PARSE_SRCS			:=	$(PARSEDIR)/ast_utils.c \
					$(PARSEDIR)/clean.c \
					$(PARSEDIR)/parse.c \
					$(PARSEDIR)/get_io.c \
					$(PARSEDIR)/signal.c \
					$(PARSEDIR)/lexing.c \
					$(PARSEDIR)/clean2.c \
					$(PARSEDIR)/add_io.c \
					$(PARSEDIR)/signal2.c \
					$(PARSEDIR)/env_var.c \
					$(PARSEDIR)/env_var2.c \
					$(PARSEDIR)/expander.c \
					$(PARSEDIR)/io_utils.c \
					$(PARSEDIR)/del_redir.c \
					$(PARSEDIR)/heredeocc.c \
					$(PARSEDIR)/get_env_var.c \
					$(PARSEDIR)/expan_utils.c \
					$(PARSEDIR)/parse_utils.c \
					$(PARSEDIR)/lexing_utils.c \
					$(PARSEDIR)/env_var_size.c \
					$(PARSEDIR)/command_path.c \
					$(PARSEDIR)/get_token_tab.c \
					$(PARSEDIR)/env_var_utils.c \
					$(PARSEDIR)/launch_expand.c \
					$(PARSEDIR)/expander_utils.c \
					$(PARSEDIR)/heredoc_env_var.c \
					$(PARSEDIR)/lex_n_parse_util.c \
					$(PARSEDIR)/check_syntaxe_par.c \
					$(PARSEDIR)/check_syntax_redir.c \
					$(PARSEDIR)/check_syntaxe_first.c \
					$(PARSEDIR)/launch_syntaxe_check.c \
					$(PARSEDIR)/checker_syntax_quote.c

EXEC_SRCS_BONUS		:=	$(EXECDIR)/applying_redirections_utils_bonus.c \
					$(EXECDIR)/initialize_environnement_bonus.c \
					$(EXECDIR)/initialisation_env_utils_bonus.c \
					$(EXECDIR)/handling_pipeline_utils_bonus.c \
					$(EXECDIR)/applying_redirections_bonus.c \
					$(EXECDIR)/command_exec_failure_bonus.c \
					$(EXECDIR)/handling_builtins_bonus.c \
					$(EXECDIR)/handling_pipeline_bonus.c \
					$(EXECDIR)/handling_command_bonus.c \
					$(EXECDIR)/ft_export_utils_bonus.c \
					$(EXECDIR)/execution_utils_bonus.c \
					$(EXECDIR)/handling_logic_bonus.c \
					$(EXECDIR)/duplicate_env_bonus.c \
					$(EXECDIR)/ft_add_local_bonus.c \
					$(EXECDIR)/printing_env_bonus.c \
					$(EXECDIR)/handling_env_bonus.c \
					$(EXECDIR)/executing_bonus.c \
					$(EXECDIR)/ft_export_bonus.c \
					$(EXECDIR)/env_utils_bonus.c \
					$(EXECDIR)/ft_unset_bonus.c \
					$(EXECDIR)/mini_env_bonus.c \
					$(EXECDIR)/ft_echo_bonus.c \
					$(EXECDIR)/ft_exit_bonus.c \
					$(EXECDIR)/ft_pwd_bonus.c \
					$(EXECDIR)/ft_env_bonus.c \
					$(EXECDIR)/ft_cd_bonus.c

PARSE_SRCS_BONUS	:=	$(PARSEDIR)/ast_utils_bonus.c \
					$(PARSEDIR)/clean_bonus.c \
					$(PARSEDIR)/parse_bonus.c \
					$(PARSEDIR)/get_io_bonus.c \
					$(PARSEDIR)/signal_bonus.c \
					$(PARSEDIR)/lexing_bonus.c \
					$(PARSEDIR)/clean2_bonus.c \
					$(PARSEDIR)/add_io_bonus.c \
					$(PARSEDIR)/signal2_bonus.c \
					$(PARSEDIR)/env_var_bonus.c \
					$(PARSEDIR)/env_var2_bonus.c \
					$(PARSEDIR)/expander_bonus.c \
					$(PARSEDIR)/io_utils_bonus.c \
					$(PARSEDIR)/del_redir_bonus.c \
					$(PARSEDIR)/heredeocc_bonus.c \
					$(PARSEDIR)/get_env_var_bonus.c \
					$(PARSEDIR)/expan_utils_bonus.c \
					$(PARSEDIR)/parse_utils_bonus.c \
					$(PARSEDIR)/lexing_utils_bonus.c \
					$(PARSEDIR)/env_var_size_bonus.c \
					$(PARSEDIR)/command_path_bonus.c \
					$(PARSEDIR)/get_token_tab_bonus.c \
					$(PARSEDIR)/env_var_utils_bonus.c \
					$(PARSEDIR)/launch_expand_bonus.c \
					$(PARSEDIR)/expander_utils_bonus.c \
					$(PARSEDIR)/heredoc_env_var_bonus.c \
					$(PARSEDIR)/lex_n_parse_util_bonus.c \
					$(PARSEDIR)/check_syntaxe_par_bonus.c \
					$(PARSEDIR)/check_syntax_redir_bonus.c \
					$(PARSEDIR)/check_syntaxe_first_bonus.c \
					$(PARSEDIR)/launch_syntaxe_check_bonus.c \
					$(PARSEDIR)/checker_syntax_quote_bonus.c

OBJDIR				:=	./.obj

MAIN_OBJ			:=	$(MAIN_SRC:$(MAINDIR)/%.c=$(OBJDIR)/%.o)
EXEC_OBJS			:=	$(EXEC_SRCS:$(EXECDIR)/%.c=$(OBJDIR)/%.o) 
PARSE_OBJS			:=	$(PARSE_SRCS:$(PARSEDIR)/%.c=$(OBJDIR)/%.o)

MAIN_OBJ_BONUS		:=	$(MAIN_SRC_BONUS:$(MAINDIR)/%.c=$(OBJDIR)/%.o)
EXEC_OBJS_BONUS		:=	$(EXEC_SRCS_BONUS:$(EXECDIR)/%.c=$(OBJDIR)/%.o) 
PARSE_OBJS_BONUS	:=	$(PARSE_SRCS_BONUS:$(PARSEDIR)/%.c=$(OBJDIR)/%.o)

OBJS				:=	$(MAIN_OBJ) $(EXEC_OBJS) $(PARSE_OBJS)
OBJS_BONUS			:=	$(MAIN_OBJ_BONUS) $(EXEC_OBJS_BONUS) $(PARSE_OBJS_BONUS)

all : bash_spinner $(LIBFT) $(NAME) stop_bash_spinner

bonus : zsh_spinner $(LIBFT) $(NAME_BONUS) stop_zsh_spinner

$(OBJDIR)/%.o: $(EXECDIR)/%.c $(DEPS) | $(OBJDIR)
	@$(CC) $(CFLAGS) -g -o $@ -c $< $(HEADERS)

$(OBJDIR)/%.o: $(PARSEDIR)/%.c $(DEPS) | $(OBJDIR)
	@$(CC) $(CFLAGS) -g -o $@ -c $< $(HEADERS)

$(OBJDIR)/%.o: $(MAINDIR)/%.c $(DEPS) | $(OBJDIR)
	@$(CC) $(CFLAGS) -g -o $@ -c $< $(HEADERS)

$(OBJDIR)/%.o: $(EXECDIR)/%.c $(DEPS_BONUS) | $(OBJDIR)
	@$(CC) $(CFLAGS) -g -o $@ -c $< $(HEADERS_BONUS)

$(OBJDIR)/%.o: $(PARSEDIR)/%.c $(DEPS_BONUS) | $(OBJDIR)
	@$(CC) $(CFLAGS) -g -o $@ -c $< $(HEADERS_BONUS)

$(OBJDIR)/%.o: $(MAINDIR)/%.c $(DEPS_BONUS) | $(OBJDIR)
	@$(CC) $(CFLAGS) -g -o $@ -c $< $(HEADERS_BONUS)

$(LIBFT)/libft.a: FORCE
	@make -C $(LIBFT) -s

$(NAME): $(LIBFT)/libft.a $(OBJS) | $(OBJDIR)
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) $(CFLAGS) -lreadline -o $(NAME)

$(NAME_BONUS): $(LIBFT)/libft.a $(OBJS_BONUS) | $(OBJDIR)
	@$(CC) $(OBJS_BONUS) $(LIBS) $(HEADERS_BONUS) $(CFLAGS) -lreadline -o $(NAME_BONUS)

bash_spinner:
	@echo "\033[1;32mCompiling mandatory minishell ....\c\033[0m"
	@while :; do \
		for s in / - \\ \|; do \
			printf "\b$$s"; \
			sleep 0.1; \
		done; \
	done & echo $$! > spinner_pid.txt

stop_bash_spinner:
	@kill `cat spinner_pid.txt` 2>/dev/null || true
	@rm -f spinner_pid.txt
	@echo "\nCompilation of mandatory minishell successful."
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
	@echo "\033[1;32mCompiling bonus minishell ....\c\033[0m"
	@while :; do \
		for s in / - \\ \|; do \
			printf "\b$$s"; \
			sleep 0.1; \
		done; \
	done & echo $$! > spinner_pid.txt

stop_zsh_spinner:
	@kill `cat spinner_pid.txt` 2>/dev/null || true
	@rm -f spinner_pid.txt
	@echo "\nCompilation of bonus minishell sucessful."
	@sleep 0.1; 
	@echo "\033[32m                                                            \
	        "
	@sleep 0.1; 
	@echo "\033[32m ███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     \
	██╗     "
	@sleep 0.1; 
	@echo "\033[32m ████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     \
	██║     "
	@sleep 0.1; 
	@echo "\033[32m ██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     \
	██║     "
	@sleep 0.1; 
	@echo "\033[32m ██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     \
	██║     "
	@sleep 0.1; 
	@echo "\033[32m ██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗\
	███████╗"
	@sleep 0.1;
	@echo "\033[32m ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝\
	╚══════╝"
	@sleep 0.1;
	@echo "\033[1;32m\nReady to take off ! 🚀\033[0m"

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@rm -f $(OBJS) $(OBJS_BONUS)

fclean: clean
	@rm -f $(NAME) $(NAME_BONUS)
	@make fclean -C $(LIBFT) -s

re: fclean all

FORCE:
