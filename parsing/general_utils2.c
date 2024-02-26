#include "parsing.h"

char	**inject_string(char **to_arr, char **from_arr, ssize_t *inj_indx)
{
	char **new_arr;
	ssize_t	to_indx;
	ssize_t	indx;

	indx = -1;
	to_indx = 0;
	if (!from_arr)
		return (to_arr);
	if (!to_arr)
		return (NULL);  // catch
	new_arr = (char **)ft_calloc(ft_arr_len(to_arr) + ft_arr_len(from_arr), 1);
	if (!new_arr)
		return (error_general("wildcard parsing", NULL)); // catch
	while (++indx < *inj_indx)
		new_arr[indx] = to_arr[indx];
	while (from_arr[to_indx])
		new_arr[indx++] = from_arr[to_indx++];
	while (to_arr[++(*inj_indx)])
		new_arr[indx++] = to_arr[*inj_indx];
	free(to_arr);
	*from_arr = NULL;
	(*inj_indx)++;
	return (new_arr);
}
