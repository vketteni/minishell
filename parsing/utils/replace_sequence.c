#include "../parsing.h"

const char	*escape_quotes(const char *literal)
{
	return (literal);
	// const char *end;
	// const char *quote;
	// const char *start;
	// while (contains_quotations(literal))
	// {
	// }
}

//              "hello 'world'" -> "hello \'world\'"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_replacer
{
	int		literal_len;
	int		seq_len;
	int		rep_len;
	int		result_len;
}			t_replacer;

int	get_replace_count(const char *literal, const char *sequence,
		t_replacer replacer)
{
	const char	*tmp;
	const char	*needle;
	int			count;

	count = 0;
	tmp = literal;
	needle = NULL;
	needle = ft_strnstr(tmp, sequence, ft_strlen(sequence));
	while (needle != NULL)
	{
		count++;
		tmp += replacer.seq_len; // Move past the sequence
		needle = ft_strnstr(tmp, sequence, ft_strlen(sequence));
	}
	return (count);
}

int	get_result_length(const char *literal, const char *sequence,
		t_replacer replacer)
{
	int	count;

	count = get_replace_count(literal, sequence, replacer);
	return (replacer.literal_len - count * replacer.seq_len + count
		* replacer.rep_len);
}

t_replacer	initialize_replacer(const char *literal, const char *sequence,
		const char *replacement)
{
	t_replacer	replacer;

	replacer.literal_len = strlen(literal);
	replacer.seq_len = strlen(sequence);
	replacer.rep_len = strlen(replacement);
	replacer.result_len = get_result_length(literal, sequence, replacer);
	return (replacer);
}



const char	*replace_loop(const char *literal, const char *sequence,
		const char *replacement, t_replacer replacer)
{
	int			head_len;
	const char	*tmp;
	const char	*current;
	char	*result;
	char	*result_ptr;

	result = (char *)ft_calloc(replacer.result_len + 1, 1);
	if (result == NULL)
        return (NULL);
    result_ptr = result;
	current = literal;
	tmp = ft_strnstr(current, sequence, ft_strlen(sequence));
	while (tmp != NULL)
	{
		head_len = tmp - current;
		ft_memcpy(result_ptr, current, head_len);
		result_ptr += head_len;
		ft_memcpy(result_ptr, replacement, replacer.rep_len);
		result_ptr += replacer.rep_len;
		current = tmp + replacer.seq_len;
		tmp = ft_strnstr(current, sequence, ft_strlen(sequence));
	}
	strcpy(result_ptr, current);
    return (result);
}

const char	*replace_sequence(const char *literal, const char *sequence,
		const char *replacement)
{
	t_replacer	replacer;

	if (!literal || !sequence || !replacement)
        return (NULL);
	replacer = initialize_replacer(literal, sequence, replacement);
    return (replace_loop(literal, sequence, replacement, replacer));
}
