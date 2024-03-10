#include "../execution.h"

t_fd_state	*last_input_redir(t_executor *exec, t_scope *scope)
{
	t_redir		*redir;
	t_fd_state	*input_fd_state;

	if (param_check(exec, scope) == -1)
		terminate(NULL, NULL, EXIT_FAILURE, "parameter check failed");
	input_fd_state = exec->input_fd;
	redir = current_cmd_in_execution(exec)->redirs;
	if (redir == NULL)
		return (exec->input_fd);
	while (redir)
	{
		if ((redir->redir_sym == RED_INP || redir->redir_sym == HEAR_DOC)
			&& input_fd_state->fd != STDIN_FILENO)
			close_fd(input_fd_state, exec);
		if (redir->redir_sym == RED_INP)
			input_fd_state = input_redirection(redir->redir_name);
		else if (redir->redir_sym == HEAR_DOC)
			input_fd_state = here_document(redir->redir_name,
					current_cmd_in_execution(exec)->env, scope);
		redir = redir->next;
		if (!input_fd_state)
			terminate(NULL, NULL, EXIT_FAILURE,
				"Couldn't find input redirection");
	}
	return (input_fd_state);
}
