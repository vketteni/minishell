#include "../execution.h"

void    cmds_check(t_cmd **cmds)
{
	if (!cmds)
		terminate(NULL, EXIT_FAILURE, NULL);
	if (cmds[0] == NULL)
		terminate(NULL, EXIT_SUCCESS, NULL);
}
