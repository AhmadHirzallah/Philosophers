/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:52:11 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/29 01:44:50 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Main simulation function that coordinates thread creation and execution
*/
int	start_philos_dinner_sim(t_table *table)
{
	if (create_philosopher_threads(table) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (create_monitor_thread(table) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (signal_simulation_start(table) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (wait_for_monitor_thread(table) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (wait_for_philosopher_threads(table) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/*
** Destroys all fork mutexes and frees memory
*/
static void	cleanup_forks_and_memory(t_table *table)
{
	int	fork_index;

	if (table->forks)
	{
		fork_index = 0;
		while (fork_index < table->philos_count)
		{
			safe_mutex_handle(&table->forks[fork_index].fork, DESTROY);
			fork_index++;
		}
		free(table->forks);
	}
	if (table->philosophers_arr)
		free(table->philosophers_arr);
}

/*
** Destroys all mutexes used in the simulation
*/
static void	cleanup_mutexes(t_table *table)
{
	safe_mutex_handle(&table->print_mutex, DESTROY);
	safe_mutex_handle(&table->data_mutex, DESTROY);
}

/*
** Cleans up all resources allocated for the simulation
*/
void	clean_table(t_table *table)
{
	cleanup_forks_and_memory(table);
	cleanup_mutexes(table);
}

/*
** Main program entry point - coordinates initialization, simulation and cleanup
*/
int	start_program(char *argv[])
{
	t_table	table;

	if (table_initialization(&table, argv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (start_philos_dinner_sim(&table) == EXIT_FAILURE)
	{
		clean_table(&table);
		return (EXIT_FAILURE);
	}
	clean_table(&table);
	return (EXIT_SUCCESS);
}
