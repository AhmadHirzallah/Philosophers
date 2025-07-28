#include "philo.h"

static inline int is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

static inline bool skip_spaces(const char *str, int *index )
{
	while (is_space(str[*index]))
		(*index)++;
	return (str[*index] != '\0');
}

static inline bool get_sign(const char *str, int *index, long *signal)
{
	if (str[*index] == '-')
	{
		*signal = -1;
		(*index)++;
		return (true);
	}
	else if (str[*index] == '+')
	{
		(*index)++;
		return (true);
	}
	return (false);
}

static inline bool calculate_number(const char *str, int *index, long *result)
{
	*result = 0;

	if (str[*index] < '0' || str[*index] > '9')
		return (false);
	while (str[*index] >= '0' && str[*index] <= '9')
	{
		*result = *result * 10 + (str[*index] - '0');
		(*index)++;
	}
	return true;
}

long	ft_atol(const char *str)
{
	int	i;
	long	result;
	long	signal;

	signal = 1;
	i = 0;
	if (!(skip_spaces(str, &i)))
		return (0);
	if ((str[i] == '+' || str[i] == '-'))
		get_sign(str, &i, &signal);
	if (!(calculate_number(str, &i, &result)))
		return (0);
	return (result * signal);
}
