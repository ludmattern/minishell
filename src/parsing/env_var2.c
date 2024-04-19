/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:32:05 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/19 12:41:34 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"
#include "../../inc/exec.h"

typedef struct s_var_context {
    char *res;
    int k;
    t_g_data *data;
    t_env *mini_env;
    bool dquotes;
} t_var_context;

char *initialize_result(char *tkn, t_g_data *data, bool *dquotes) {
    t_envsize s = get_mal_size2(tkn, 0, 0, 0, data->mini_env, data, *dquotes);
    if (s.size == 1)
        return NULL;
    char *res = malloc(s.size);
    if (!res)
        fail_exit_shell(data);
    ft_bzero(res, s.size);
    return res;
}


void process_variable(char **tkn, int *i, int *k, char **res, t_g_data *data, bool dquotes) {
    int start = *i;
    int env_length, j = 0;
    char *env_val;

    while (ft_isalnum((*tkn)[*i])) (*i)++;
    env_length = *i - start;
    char *tmp_env = malloc(env_length + 1);
    if (!tmp_env) fail_exit_shell(data);
    ft_strncpy(tmp_env, *tkn + start, env_length);
    tmp_env[env_length] = '\0';
    env_val = ft_get_env3(tmp_env, data->mini_env, data, dquotes);
    free(tmp_env);
    if (!env_val) 
    {
        env_val = malloc(2);
        env_val[0] = -1;
        env_val[1] = '\0';
    }
    while (env_val[j])
        (*res)[(*k)++] = env_val[j++];
    free(env_val);
}

char *get_env_var2(char *tkn, int i, int k, int j, t_g_data *data, bool dquotes) 
{
    char *res = initialize_result(tkn, data, &dquotes);
    if (!res) return NULL;
    (void)j;
    while (tkn[i] != '\0') {
        if (tkn[i] == '$' && tkn[i + 1] && tkn[i + 1] != ' ' && tkn[i + 1] != '$' && tkn[i + 1] != '"')
        {
            i++;
            process_variable(&tkn, &i, &k, &res, data, dquotes);
        } 
        else 
        {
            res[k++] = tkn[i];
        }
        i++;
    }
    res[k] = '\0';
    return res;
}
