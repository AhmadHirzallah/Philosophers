/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 01:30:00 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/30 07:37:25 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_philosopher_threads(t_table *table)
{
	int	philo_index;

	philo_index = 0;
	while (philo_index < table->philos_count)
	{
		if (safe_thread_handle(&table->philosophers_arr[philo_index].thread_id,
				philosopher_routine, &table->philosophers_arr[philo_index],
				CREATE) == EXIT_FAILURE)
		{
			return (EXIT_FAILURE);
		}
		philo_index++;
	}
	return (EXIT_SUCCESS);
}

int	create_monitor_thread(t_table *table)
{
	if (safe_thread_handle(&table->monitor_thread, monitor_routine,
			table, CREATE) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	signal_simulation_start(t_table *table)
{
	if (safe_mutex_handle(&table->data_mutex, LOCK) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	table->are_all_threads_ready = true;
	safe_mutex_handle(&table->data_mutex, UNLOCK);
	return (EXIT_SUCCESS);
}

int	wait_for_philosopher_threads(t_table *table)
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

int	wait_for_monitor_thread(t_table *table)
{
	if (safe_thread_handle(&table->monitor_thread, NULL, NULL, JOIN)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
