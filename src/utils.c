/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 01:50:00 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/29 02:17:09 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Assigns fork pointers to philosopher based on even/odd strategy to prevent
** deadlock
*/
void	assign_forks(t_philosopher *philo, t_fork *forks, int philosopher_index)
{
	int	total_philosophers;

	total_philosophers = philo->table_ptr->philos_count;
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &forks[philosopher_index];
		philo->second_fork = &forks[(philosopher_index + 1)
			% total_philosophers];
	}
	else
	{
		philo->first_fork = &forks[(philosopher_index + 1)
			% total_philosophers];
		philo->second_fork = &forks[philosopher_index];
	}
}

/*
** Gets current time in milliseconds
*/
long	get_time(void)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL))
		return (-1);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

/*
** Precise sleep function using microsleep in a loop
*/
void	ft_usleep(long sleep_time_ms)
{
	long	start_time;

	start_time = get_time();
	while ((get_time() - start_time) < sleep_time_ms)
		usleep(100);
}

/*
** Waits for all threads to be ready before starting philosopher routine
*/
void	wait_for_simulation_start(t_philosopher *philo)
{
	while (1)
	{
		if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK)
			== EXIT_FAILURE)
			return ;
		if (philo->table_ptr->are_all_threads_ready)
		{
			safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
			break ;
		}
		safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
		usleep(100);
	}
}

/*
** Checks if simulation has ended
*/
int	is_simulation_ended(t_philosopher *philo)
{
	int	ended;

	if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
		return (1);
	ended = philo->table_ptr->is_simulation_ended;
	safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
	return (ended);
}
