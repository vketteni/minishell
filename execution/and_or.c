/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_or.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vketteni <vketteni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:16:54 by vketteni          #+#    #+#             */
/*   Updated: 2024/04/18 14:48:14 by vketteni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	is_first_operant(t_cmd2 *cmd)
{
	t_oper2	operation;

	cmd_check(cmd);
	operation = cmd->execution->operation;
	if (operation == AND_ || operation == OR_)
		return (1);
	return (0);
}

int	is_second_operant(t_cmd2 *cmd)
{
	t_cmd2	*prev;

	cmd_check(cmd);
	prev = get_previous_cmd(cmd);
	if (prev && (prev->execution->operation == AND_
			|| prev->execution->operation == OR_))
		return (1);
	return (0);
}

int	is_logic_operation(t_cmd2 *cmd)
{
	cmd_check(cmd);
	if (is_first_operant(cmd) || is_second_operant(cmd))
		return (1);
	return (0);
}

int	evaluate_logic_operation(t_cmd2 *cmd)
{
	int		exit_status;
	t_oper2	operation;

	cmd_check(cmd);
	operation = cmd->execution->operation;
	exit_status = cmd->execution->exit_status;
	if (is_first_operant(cmd) && (operation == OR_
			&& exit_status == EXIT_SUCCESS))
		return (1);
	else if (is_first_operant(cmd) && (operation == AND_ && exit_status))
		return (1);
	if (is_second_operant(cmd) && (operation == OR_ && exit_status))
		return (1);
	else if (is_second_operant(cmd) && (operation == AND_ && exit_status))
		return (1);
	return (0);
}
