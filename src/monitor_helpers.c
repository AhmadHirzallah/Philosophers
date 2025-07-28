/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 02:00:00 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/29 02:01:30 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Waits for all threads to be ready before starting monitoring
*/
void	wait_for_monitor_start(t_table *table)
{
	while (1)
	{
		if (safe_mutex_handle(&table->data_mutex, LOCK) == EXIT_FAILURE)
			return ;
		if (table->are_all_threads_ready)
		{
			safe_mutex_handle(&table->data_mutex, UNLOCK);
			break ;
		}
		safe_mutex_handle(&table->data_mutex, UNLOCK);
		usleep(100);
	}
}

/*
** Checks if simulation should end due to global flag
*/
int	should_end_simulation(t_table *table)
{
	if (safe_mutex_handle(&table->data_mutex, LOCK) == EXIT_FAILURE)
		return (1);
	if (table->is_simulation_ended)
	{
		safe_mutex_handle(&table->data_mutex, UNLOCK);
		return (1);
	}
	safe_mutex_handle(&table->data_mutex, UNLOCK);
	return (0);
}

/*
** Checks all philosophers for death
*/
int	check_philosophers_death(t_table *table)
{
	int	philo_index;

	philo_index = 0;
	while (philo_index < table->philos_count)
	{
		if (philosopher_died(&table->philosophers_arr[philo_index]))
			return (1);
		philo_index++;
	}
	return (0);
}
