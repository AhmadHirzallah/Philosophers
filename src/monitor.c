/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 01:40:00 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/29 02:17:09 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Checks if all philosophers have eaten required number of meals
*/
static int	are_all_philosophers_full(t_table *table)
{
	int	philo_index;

	if (table->maximum_meals_nbr == -1)
		return (0);
	if (safe_mutex_handle(&table->data_mutex, LOCK) == EXIT_FAILURE)
		return (0);
	philo_index = 0;
	while (philo_index < table->philos_count)
	{
		if (!table->philosophers_arr[philo_index].is_full)
		{
			safe_mutex_handle(&table->data_mutex, UNLOCK);
			return (0);
		}
		philo_index++;
	}
	safe_mutex_handle(&table->data_mutex, UNLOCK);
	return (1);
}

/*
** Ends simulation by setting the global flag
*/
static void	end_simulation(t_table *table)
{
	if (safe_mutex_handle(&table->data_mutex, LOCK) == EXIT_FAILURE)
		return ;
	table->is_simulation_ended = true;
	safe_mutex_handle(&table->data_mutex, UNLOCK);
}

/*
** Monitor routine that watches for death and completion
*/
void	*monitor_routine(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	wait_for_monitor_start(table);
	while (1)
	{
		if (should_end_simulation(table))
			return (NULL);
		if (check_philosophers_death(table))
			return (NULL);
		if (are_all_philosophers_full(table))
		{
			end_simulation(table);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
