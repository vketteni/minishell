/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddavlety <ddavlety@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:29:54 by ddavlety          #+#    #+#             */
/*   Updated: 2024/04/18 13:56:07 by ddavlety         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wildcard.h"

static void	router(char ***matches, char *letters, t_tree *shell_env);

static void	f_1(char ***matches, char *letters, t_tree *shell_env)
{
	t_tree	*child;

	child = shell_env;
	if (!child)
		return ;
	while (child)
	{
		if (child->letter == letters[0])
			router(matches, letters, child);
		child = child->next;
	}
	return ;
}

static void	f_2(char ***matches, char *letters, t_tree *shell_env)
{
	t_tree	*child;

	child = shell_env;
	if (!child)
		return ;
	while (child)
	{
		f_2(matches, letters, child->child);
		if (child->letter == letters[1])
			router(matches, &letters[1], child);
		if (letters)
			child = child->next;
	}
}

static void	f_3(char ***matches, t_tree *shell_env)
{
	t_tree	*child;

	child = shell_env;
	if (!child)
		return ;
	while (child)
	{
		f_3(matches, child->child);
		if (child->exists)
			*matches = append_arr_str(*matches, ft_strdup(child->name));
		child = child->next;
	}
}

static void	router(char ***matches, char *letters, t_tree *shell_env)
{
	while (letters[0] == '*' && letters[1] == '*')
		letters++;
	if (letters[0] == 42)
	{
		if (!letters[1])
			return (f_3(matches, shell_env));
		else
			f_2(matches, &letters[0], shell_env->child);
	}
	else if (letters[1])
	{
		if (letters[1] == 42)
			router(matches, &letters[1], shell_env->child);
		else
			f_1(matches, &letters[1], shell_env->child);
	}
	else
	{
		if (shell_env->exists)
			*matches = append_arr_str(*matches, ft_strdup(shell_env->name));
	}
}

void	find_wildcard(char ***matches, char *letters, t_tree *shell_env)
{
	*matches = (char **)ft_calloc(1, sizeof(char *));
	if (!*matches)
		return ;
	while (*letters == '*' && *(letters + 1) == '*')
		letters++;
	if (!*(letters + 1) && *letters == '*')
		f_3(matches, shell_env->child);
	else if (*letters == '*')
		f_2(matches, letters, shell_env->child);
	else
		f_1(matches, letters, shell_env->child);
}
