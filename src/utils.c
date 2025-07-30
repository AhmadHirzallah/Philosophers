/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 01:50:00 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/30 09:33:01 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

char	*ft_strstr(const char *haystack, const char *needle)
{
	int	i;
	int	j;

	if (!haystack || !needle)
		return (NULL);
	if (*needle == '\0')
		return ((char *)haystack);
	i = 0;
	while (haystack[i])
	{
		j = 0;
		while (needle[j] && (haystack[i + j] == needle[j]))
			j++;
		if (needle[j] == '\0')
			return ((char *)&haystack[i]);
		i++;
	}
	return (NULL);
}

void	think_time(t_philosopher *philo)
{
	long	time_to_think;
	long	time_to_eat;
	long	time_to_sleep;

	time_to_eat = philo->table_ptr->time_to_eat;
	time_to_sleep = philo->table_ptr->time_to_sleep;
	if (philo->table_ptr->philos_count % 2 == 0)
		return ;
	time_to_think = (time_to_eat * 2) - time_to_sleep;
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think > 600)
		time_to_think = 200;
	if (time_to_think > 0)
		ft_usleep(time_to_think);
}

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

int	is_simulation_ended(t_philosopher *philo)
{
	int	ended;

	if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	ended = philo->table_ptr->is_simulation_ended;
	if (safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (ended);
}
