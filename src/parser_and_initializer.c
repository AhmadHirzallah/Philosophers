/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_and_initializer.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:52:04 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/29 01:51:19 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Parses command line arguments and fills table configuration
*/
void	parse_simulation_parameters(t_table *table, char **argv)
{
	table->philos_count = ft_atol(argv[1]);
	table->time_to_be_died = ft_atol(argv[2]);
	table->time_to_eat = ft_atol(argv[3]);
	table->time_to_sleep = ft_atol(argv[4]);
	if (argv[5] && argv[5][0])
		table->maximum_meals_nbr = ft_atol(argv[5]);
	else
		table->maximum_meals_nbr = -1;
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

/*
** Sets up initialization order
*/
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

/*
** Completes initialization process
*/
static int	complete_initialization(t_table *table)
{
	if (initialize_philosophers(table) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/*
** Main table initialization function - coordinates all setup steps
*/
int	table_initialization(t_table *table, char **argv)
{
	parse_simulation_parameters(table, argv);
	initialize_simulation_control(table);
	if (setup_initialization_order(table) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (complete_initialization(table) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
