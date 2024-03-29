#include "../execution.h"

t_fd_state	*last_input_redir(t_executor *exec)
{
	t_redir		*redir;
	t_fd_state	*input_fd_state;

	if (!exec || !get_current_cmd(exec))
		terminate(exec, EXIT_FAILURE, "parameter check failed");
	redir = get_current_cmd(exec)->redirs;
    if (!redir)
    {
        return (NULL);
    }
	input_fd_state = NULL;
	while (redir)
    {
        if (input_fd_state && input_fd_state->fd != STDIN_FILENO
            && (redir->redir_sym == RED_INP || redir->redir_sym == HEAR_DOC))
            close_fd(exec, input_fd_state);
        if (redir->redir_sym == RED_INP)
            input_fd_state = input_redirection(redir->redir_name);
        else if (redir->redir_sym == HEAR_DOC)
            input_fd_state = here_document(exec, redir->redir_name);
        // if (!input_fd_state)
        //     terminate(exec, EXIT_FAILURE, "couldn't redirect input");
        redir = redir->next;
    }
	return (input_fd_state);
}
