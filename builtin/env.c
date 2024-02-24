#include "builtin.h"

int	builtin_env(char **argv, t_env *root)
{
	uint32_t	i;

	i = 0;
	if (argv[1])
		return (builtin_err_gen("env", \
		"option(s) or argument(s) are not allowed", NULL));
	while (root->envp[i])
		ft_putendl_fd(root->envp[i++], 1);
	return (0);
}
