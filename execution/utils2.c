/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 16:47:24 by nettalha          #+#    #+#             */
/*   Updated: 2023/06/22 14:10:54 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_error(char *input, char *message, int errnb)
{
	input = ft_strjoin(input, ": ");
	ft_putstr_fd("minishell: ", 2);
	if (input != NULL)
		ft_putstr_fd(input, 2);
	ft_putendl_fd(message, 2);
	free(input);
	g_glb.exit_status = errnb;
}

void	free_string(void *str)
{
	free(str);
}

void	ft_free(char **s)
{
	int	i;

	if (!s)
		return ;
	else
	{
		i = 0;
		while (s[i])
		{
			free(s[i]);
			i++;
		}
	}
	free(s);
}

void	free_cmd_gpt_version(t_cmd *ll) {
    t_cmd *current;

    while (ll != NULL) {
        current = ll;
        ll = ll->next;
        ft_free(current->cmd);
        ft_free(current->red);
        ft_free(current->file);
        ft_free(current->delimiter);
        free(current);
    }
}

void	free_cmd(t_cmd *ll)
{
    t_cmd    *pnt;

    while (ll)
    {
        pnt = ll->next;
        ft_free(ll->cmd);
        ft_free(ll->red);
        ft_free(ll->file);
        ft_free(ll->delimiter);
        free(ll);
        ll = pnt;
    }
}
