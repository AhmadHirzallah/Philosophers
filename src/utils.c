/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 01:50:00 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/30 15:59:26 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		ft_usleep_interruptible(philo, time_to_think);
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
	if (safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (ended);
}
