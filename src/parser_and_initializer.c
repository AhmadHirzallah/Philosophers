/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_and_initializer.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:52:04 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/30 15:52:13 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Parses command line arguments and fills table configuration
*/
int	parse_simulation_parameters(t_table *table, char **argv)
{
	table->philos_count = ft_atol(argv[1]);
	table->time_to_be_died = ft_atol(argv[2]);
	table->time_to_eat = ft_atol(argv[3]);
	table->time_to_sleep = ft_atol(argv[4]);
	if (argv[5] && argv[5][0])
		table->maximum_meals_nbr = ft_atol(argv[5]);
	else
		table->maximum_meals_nbr = -1;
	if (table->philos_count == 0 || table->time_to_be_died == 0
		|| table->time_to_eat == 0 || table->time_to_sleep == 0
		|| table->maximum_meals_nbr == 0)
	{
		print_error_with_return("Wrong Input! Not Positive Numbers\n"
			COLOR_SUCCESS"Enter Only Positive ( > 0 ) Numbers"RESET);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/*
** Allocates memory for philosophers and forks arrays
*/
static int	allocate_simulation_memory(t_table *table)
{
	table->philosophers_arr = safe_malloc(sizeof(t_philosopher)
			* table->philos_count);
	if (!table->philosophers_arr)
		return (EXIT_FAILURE);
	table->forks = safe_malloc(sizeof(t_fork) * table->philos_count);
	if (!table->forks)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	setup_initialization_order(t_table *table)
{
	if (initialize_mutexes(table) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (allocate_simulation_memory(table) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (initialize_fork_mutexes(table) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	complete_initialization(t_table *table)
{
	if (initialize_philosophers(table) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	table_initialization(t_table *table, char **argv)
{
	if (parse_simulation_parameters(table, argv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	initialize_simulation_control(table);
	if (setup_initialization_order(table) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (complete_initialization(table) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
