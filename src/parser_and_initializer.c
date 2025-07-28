/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_and_initializer.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:52:04 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/29 01:17:02 by ahirzall         ###   ########.fr       */
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
** Initializes simulation control variables
*/
static void	initialize_simulation_control(t_table *table)
{
	table->is_simulation_ended = false;
	table->are_all_threads_ready = false;
	table->threads_running_counter = 0;
	table->start_simulation_time = get_time();
}

/*
** Initializes all mutexes for the simulation
*/
static int	initialize_mutexes(t_table *table)
{
	if (safe_mutex_handle(&table->print_mutex, INIT) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (safe_mutex_handle(&table->data_mutex, INIT) == EXIT_FAILURE)
		return (EXIT_FAILURE);
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

/*
** Initializes all fork mutexes
*/
static int	initialize_fork_mutexes(t_table *table)
{
	int	fork_index;

	fork_index = 0;
	while (fork_index < table->philos_count)
	{
		if (safe_mutex_handle(&table->forks[fork_index].fork, INIT)
			== EXIT_FAILURE)
			return (EXIT_FAILURE);
		table->forks[fork_index].fork_id = fork_index;
		fork_index++;
	}
	return (EXIT_SUCCESS);
}

/*
** Initializes individual philosopher properties
*/
static void	initialize_philosopher_properties(t_table *table, int philo_index)
{
	table->philosophers_arr[philo_index].table_ptr = table;
	table->philosophers_arr[philo_index].id = philo_index + 1;
	table->philosophers_arr[philo_index].is_full = false;
	table->philosophers_arr[philo_index].eaten_meals_counter = 0;
	table->philosophers_arr[philo_index].last_meal_time =
		table->start_simulation_time;
	assign_forks(&table->philosophers_arr[philo_index],
		table->forks, philo_index);
}

/*
** Initializes all philosophers in the simulation
*/
static int	initialize_all_philosophers(t_table *table)
{
	int	philo_index;

	philo_index = 0;
	while (philo_index < table->philos_count)
	{
		initialize_philosopher_properties(table, philo_index);
		philo_index++;
	}
	return (EXIT_SUCCESS);
}

/*
** Main table initialization function - coordinates all setup steps
*/
int	table_initialization(t_table *table, char **argv)
{
	parse_simulation_parameters(table, argv);
	initialize_simulation_control(table);
	if (initialize_mutexes(table) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (allocate_simulation_memory(table) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (initialize_fork_mutexes(table) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (initialize_all_philosophers(table) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

