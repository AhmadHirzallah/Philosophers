#include "Philosophers.h"

void parse_values(t_table *table, char **argv)
{
	table->philos_count = ft_atol(argv[1]);
	table->time_to_be_died = ft_atol(argv[2]) * 1e3;
	table->time_to_eat = ft_atol(argv[3]) * 1e3;
	table->time_to_sleep = ft_atol(argv[4]) * 1e3;
	if (argv[5] && argv[5][0])
		table->maximum_meals_nbr = ft_atol(argv[5]);
	else
		table->maximum_meals_nbr = -1; // No limit on meals
}

void		table_initialization(t_table *table, char **argv)
{
	parse_values(table, argv);
}

