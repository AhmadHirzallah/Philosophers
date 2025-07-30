/* *************************	return (EXIT_SUCCESS);
}

int	initialize_forks(t_table *table)****************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 02:00:00 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/29 01:51:19 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	initialize_simulation_control(t_table *table)
{
	table->is_simulation_ended = false;
	table->are_all_threads_ready = false;
	table->threads_running_counter = 0;
	table->start_simulation_time = get_time();
}

int	initialize_mutexes(t_table *table)
{
	if (safe_mutex_handle(&table->print_mutex, INIT) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (safe_mutex_handle(&table->data_mutex, INIT) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	initialize_fork_mutexes(t_table *table)
{
	int	fork_index;

	fork_index = 0;
	while (fork_index < table->philos_count)
	{
		if (safe_mutex_handle(&table->forks[fork_index].fork, INIT)
			== EXIT_FAILURE)
		{
			return (EXIT_FAILURE);
		}
		table->forks[fork_index].fork_id = fork_index;
		fork_index++;
	}
	return (EXIT_SUCCESS);
}

void	initialize_philosopher_properties(t_table *table, int philo_index)
{
	table->philosophers_arr[philo_index].table_ptr = table;
	table->philosophers_arr[philo_index].id = philo_index + 1;
	table->philosophers_arr[philo_index].is_full = false;
	table->philosophers_arr[philo_index].eaten_meals_counter = 0;
	table->philosophers_arr[philo_index].last_meal_time
		= table->start_simulation_time;
	assign_forks(&table->philosophers_arr[philo_index],
		table->forks, philo_index);
}

int	initialize_philosophers(t_table *table)
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
