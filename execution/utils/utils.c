#include "../execution.h"

void	free_arr2d(void **arr2d)
{
	int		i;
	char	**arr;

	arr = (char **)arr2d;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	msg_error(char *err)
{
	perror(err);
	exit(EXIT_FAILURE);
}

void	exit_handler(int status)
{
	if (status == 0)
		exit(0);
	else
		exit(status);
}

void	terminate(t_executor *exec, t_scope *scope, int status, char *msg)
{
	if (exec)
		free(exec);
	if (scope)
	{
		close_fd(scope->pipe->read, exec);
		free(scope->pipe->read);
		close_fd(scope->pipe->write, exec);
		free(scope->pipe->write);
		free(scope->pipe);
		free(scope);
	}
    if (msg)
        msg_error(msg);
	exit_handler(status);
}

int	is_last(t_executor *exec)
{
	if (exec->cmds[exec->command_index + 1] == NULL)
		return (1);
	return (0);
}

int	execution_has_finished(t_executor *exec)
{
	t_cmd	*cmd;

	cmd = current_cmd_in_execution(exec);
	if (cmd == NULL)
		return (1);
	return (0);
}

char	*get_name(char *cmd_path)
{
	char	*last;

	last = NULL;
	while (cmd_path)
	{
		last = cmd_path;
		cmd_path = ft_strchr(cmd_path, '/');
	}
	return (last);
}

int	is_builtin(t_executor *exec)
{
	char	*cmd;

	cmd = current_cmd_in_execution(exec)->com;
	if (ft_strchr(cmd, '/'))
	{
		cmd = get_name(cmd);
		if (!cmd)
			return (0);
	}
	if (ft_strncmp(cmd, "cd", ft_strlen(cmd) == 0) || ft_strncmp(cmd, "echo",
			ft_strlen(cmd) == 0) || ft_strncmp(cmd, "env", ft_strlen(cmd) == 0)
		|| ft_strncmp(cmd, "exit", ft_strlen(cmd) == 0) || ft_strncmp(cmd,
			"export", ft_strlen(cmd) == 0) || ft_strncmp(cmd, "pwd",
			ft_strlen(cmd) == 0) || ft_strncmp(cmd, "unset",
			ft_strlen(cmd) == 0))
		return (1);
	return (0);
}

int	is_inside_scope(t_cmd *cmd, int scope_id)
{
	int		i;

    if (scope_id < 1)
        terminate(NULL, NULL, EXIT_FAILURE, "is_inside_scope: invalid scope_id");
    i = 0;
    if (cmd)
    {
        while (cmd->scope_stack[i] && cmd->scope_stack[i] != scope_id)
            i++;
        if (cmd->scope_stack[i] == scope_id)
            return (1);
    }
	return (0);
}

int	has_nested_scope(t_executor *exec, t_scope *scope)
{
	t_cmd	*cmd;
	int		i;

    if (param_check(exec, scope) == -1)
        terminate(NULL, NULL, EXIT_FAILURE, "has_nested_scope: parameter check failed");
	i = 0;
	cmd = current_cmd_in_execution(exec);
	while (cmd->scope_stack[i])
		i++;
	if (cmd->scope_stack[i - 1] != scope->scope_id)
		return (1);
	return (0);
}

int scope_length(t_executor *exec, int scope_id)
{
    t_cmd   *cmd;
    int     len;

    if (!exec || !exec->cmds || scope_id == 0)
        terminate(NULL, NULL, EXIT_FAILURE, "scope_length: parameter check failed");
    len = 1;
    cmd = next_cmd_in_execution(exec);
    while (is_inside_scope(cmd, scope_id))
    {
        len++;
        cmd = next_cmd(exec, cmd);
    }
    return (len);
}

int	param_check(t_executor *exec, t_scope *scope)
{
	if (!exec || !exec->cmds  || !scope)
		return (-1);
	return (0);
}

int	get_nested_id(t_executor *exec, int scope_id)
{
	int		i;
    t_cmd   *cmd;

	i = 0;
    cmd = current_cmd_in_execution(exec);
	while (cmd->scope_stack[i] != scope_id)
		i++;
	if (cmd->scope_stack[i] && cmd->scope_stack[i + 1])
		return (cmd->scope_stack[i + 1]);
	return (0);
}

int	arr_len(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

t_cmd	*current_cmd_in_execution(t_executor *exec)
{
	if (!exec || !exec->cmds)
		terminate(NULL, NULL, EXIT_FAILURE, "current_cmd_in_execution: missing or incomplete exec");
	return (exec->cmds[exec->command_index]);
}

t_cmd	*previous_cmd_in_execution(t_executor *exec)
{
	if (!exec || !exec->cmds)
		terminate(NULL, NULL, EXIT_FAILURE, "previous_cmd_in_execution: missing or incomplete exec");
	if (exec->command_index == 0)
		return (NULL);
	return (exec->cmds[exec->command_index - 1]);
}

t_cmd	*next_cmd_in_execution(t_executor *exec)
{
	if (!exec || !exec->cmds)
		terminate(NULL, NULL, EXIT_FAILURE, "next_cmd_in_execution: missing or incomplete exec");
	if (current_cmd_in_execution(exec) == NULL)
		return (NULL);
	return (exec->cmds[exec->command_index + 1]);
}

int get_scope(t_cmd *cmd)
{
    int i;

    if (!cmd)
        return (-1);
    i = 0;
    while (cmd->scope_stack[i])
        i++;
    return (cmd->scope_stack[i - 1]);
}

int get_outside_scope(t_cmd *cmd)
{
    int i;

    i = 0;
    while (cmd->scope_stack[i])
        i++;
    return (cmd->scope_stack[i - 2]);
}

t_cmd	*final_cmd_in_scope(t_executor *exec, t_scope *scope)
{
	t_cmd *cmd;
	t_cmd *next;

	if (param_check(exec, scope) == -1)
		terminate(NULL, NULL, EXIT_FAILURE, "parameter check failed");
	cmd = current_cmd_in_execution(exec);
    next = next_cmd(exec, cmd);
	while (get_scope(next) == scope->scope_id)
    {
        cmd = next;
        next = next_cmd(exec, cmd);
    }
	return (cmd);
}

t_cmd   *next_cmd(t_executor *exec, t_cmd *cmd)
{
    int i;

    if (!exec || !exec->cmds)
        terminate(NULL, NULL, EXIT_FAILURE, "next_cmd: missing or incomplete exec");
    i = 0;
    if (cmd == NULL)
        return (NULL);
    while (exec->cmds[i] != cmd && exec->cmds[i + 1] != NULL)
        i++;
    return (exec->cmds[i + 1]);
}

void close_fd(t_fd_state *fd_state, t_executor *exec)
{
    if (!fd_state)
        terminate(NULL, NULL, EXIT_FAILURE, "close_fd: couldn't close file descriptor");
    if (fd_state->is_open == 0)
    {
        ft_putstr_fd("DEBUG::fd: ", 2);
        ft_putstr_fd(ft_itoa(fd_state->fd), 2);
        ft_putstr_fd("::already closed", 2);
        // ft_putstr_fd("::argv:", 2);
        // ft_putstr_fd((char *)exec->cmds[exec->command_index]->argv, 2);
        ft_putstr_fd("::command_index:", 2);
        ft_putendl_fd(ft_itoa(exec->command_index), 2);
        // terminate(NULL, NULL, EXIT_FAILURE, "close_fd: file descriptor already closed");
    }
    else if (close(fd_state->fd) == -1)
    {
        ft_putstr_fd("DEBUG::close_fd:command_index::", 2);
        ft_putendl_fd(ft_itoa(exec->command_index), 2);
        msg_error(ft_itoa(fd_state->fd));
        terminate(NULL, NULL, EXIT_FAILURE, "Couldn't close file descriptor");
    }
    fd_state->is_open = 0;
}

char **argv(t_executor *exec)
{
    t_cmd   *cmd;
    
    if (!exec || !exec->cmds)
        return (NULL);
    cmd = current_cmd_in_execution(exec);
    return (cmd->argv);
}

char **envp(t_executor *exec)
{
    t_cmd   *cmd;
    
    if (!exec || !exec->cmds)
        return (NULL);
    cmd = current_cmd_in_execution(exec);
    return (cmd->env->envp);
}