/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:52:11 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/29 01:04:46 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:51:54 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/28 23:51:55 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		start_philos_dinner_sim(t_table *table)
{
	int		i;

	// Create philosopher threads
	i = 0;
	while (i < table->philos_count)
	{
		if (safe_thread_handle(&table->philosophers_arr[i].thread_id,
			philosopher_routine, &table->philosophers_arr[i], CREATE) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		i++;
	}

	// Create monitor thread
	if (safe_thread_handle(&table->monitor_thread, monitor_routine, table, CREATE) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	// Signal all threads to start
	if (safe_mutex_handle(&table->data_mutex, LOCK) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	table->are_all_threads_ready = true;
	safe_mutex_handle(&table->data_mutex, UNLOCK);

	// Wait for monitor thread
	if (safe_thread_handle(&table->monitor_thread, NULL, NULL, JOIN) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	// Wait for all philosopher threads
	i = 0;
	while (i < table->philos_count)
	{
		if (safe_thread_handle(&table->philosophers_arr[i].thread_id, NULL, NULL, JOIN) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		i++;
	}

	return (EXIT_SUCCESS);
}

void	clean_table(t_table *table)
{
	int	i;

	if (table->forks)
	{
		i = 0;
		while (i < table->philos_count)
		{
			safe_mutex_handle(&table->forks[i].fork, DESTROY);
			i++;
		}
		free(table->forks);
	}

	if (table->philosophers_arr)
		free(table->philosophers_arr);

	safe_mutex_handle(&table->print_mutex, DESTROY);
	safe_mutex_handle(&table->data_mutex, DESTROY);
}

int	start_program(char *argv[])
{
	t_table	table;

	if (table_initialization(&table, argv) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	print_simulation_start(table.philos_count);

	if (start_philos_dinner_sim(&table) == EXIT_FAILURE)
	{
		clean_table(&table);
		return (EXIT_FAILURE);
	}

	print_simulation_end();
	clean_table(&table);
	return (EXIT_SUCCESS);
}
