/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:52:11 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/29 01:27:22 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Creates and starts all philosopher threads
*/
static int	create_philosopher_threads(t_table *table)
{
	int	philo_index;

	philo_index = 0;
	while (philo_index < table->philos_count)
	{
		if (safe_thread_handle(&table->philosophers_arr[philo_index].thread_id,
			philosopher_routine, &table->philosophers_arr[philo_index],
			CREATE) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		philo_index++;
	}
	return (EXIT_SUCCESS);
}

/*
** Creates and starts the monitor thread
*/
static int	create_monitor_thread(t_table *table)
{
	if (safe_thread_handle(&table->monitor_thread, monitor_routine,
		table, CREATE) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/*
** Signals all threads to start simulation
*/
static int	signal_simulation_start(t_table *table)
{
	if (safe_mutex_handle(&table->data_mutex, LOCK) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	table->are_all_threads_ready = true;
	safe_mutex_handle(&table->data_mutex, UNLOCK);
	return (EXIT_SUCCESS);
}

/*
** Waits for all philosopher threads to complete
*/
static int	wait_for_philosopher_threads(t_table *table)
{
	int	philo_index;

	philo_index = 0;
	while (philo_index < table->philos_count)
	{
		if (safe_thread_handle(&table->philosophers_arr[philo_index].thread_id,
			NULL, NULL, JOIN) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		philo_index++;
	}
	return (EXIT_SUCCESS);
}

/*
** Waits for monitor thread to complete
*/
static int	wait_for_monitor_thread(t_table *table)
{
	if (safe_thread_handle(&table->monitor_thread, NULL, NULL, JOIN)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

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
