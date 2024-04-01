#include "../parsing2.h"

int	is_pipe_token(t_token *token)
{
	if (!token)
	{
		ft_putendl_fd("missing token", STDERR_FILENO);
		return (0);
	}
	if (ft_strncmp(token->literal, "|", ft_strlen(token->literal) + 1) == 0)
		return (1);
	return (0);
}
