/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddavlety <ddavlety@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:31:00 by ddavlety          #+#    #+#             */
/*   Updated: 2024/04/18 13:31:00 by ddavlety         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_cmd2	*parser(t_token *tokens, t_env *shell_env)
{
	t_cmd2	*cmds;

	if (parse_check(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	cmds = initialize_commands(tokens, shell_env);
	if (!cmds)
	{
		terminate_parsing(tokens, shell_env, NULL,
			"minishell: failed to parse command line");
		return (NULL);
	}
	process_quotations(cmds, shell_env);
	expand_wildcards(cmds, shell_env);
	return (cmds);
}
