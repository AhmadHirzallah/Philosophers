#include "philo.h"

void	print_error(const char *error)
{
	printf(RED"%s\n"RESET, error);
}

void	print_error_exit(const char *error)
{
	printf(RED"%s\n"RESET, error);
	exit(EXIT_FAILURE);
}

int	print_error_with_return(const char *error)
{
	printf(RED"%s\n"RESET, error);
	return (EXIT_FAILURE);
}
