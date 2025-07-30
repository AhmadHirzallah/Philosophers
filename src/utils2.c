/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 01:50:00 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/30 16:27:16 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	assign_forks(t_philosopher *philo, t_fork *forks, int philosopher_index)
{
	int	total_philosophers;

	total_philosophers = philo->table_ptr->philos_count;
	if (philo->id == total_philosophers)
	{
		philo->first_fork = &forks[philosopher_index];
		philo->second_fork = &forks[(philosopher_index + 1)
			% total_philosophers];
	}
	else if (philo->id % 2 == 0)
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

long	get_time(void)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL))
		return (-1);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	ft_usleep(long sleep_time_ms)
{
	long	start_time;

	start_time = get_time();
	while ((get_time() - start_time) < sleep_time_ms)
		usleep(100);
}

void	ft_usleep_interruptible(t_philosopher *philo, long sleep_time_ms)
{
	long	start_time;

	start_time = get_time();
	while ((get_time() - start_time) < sleep_time_ms)
	{
		if (is_simulation_ended(philo))
			break ;
		usleep(50);
	}
}
