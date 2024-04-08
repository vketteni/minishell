#include "builtin.h"

int	builtin_echo(const char **argv, t_env *root)
{
	ssize_t	i;

	(void)root;
	i = 1;
	if (argv[i] && ft_strncmp(argv[i], "-n", 3) == 0)
		i++;
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (argv[1] == NULL || ft_strncmp(argv[1], "-n", 3))
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
