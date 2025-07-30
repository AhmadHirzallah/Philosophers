/* *************************************************	return (1);
}

void	update_meal_tracking(t_philosopher *philo)****************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 01:53:00 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/29 01:57:15 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	handle_single_philosopher(t_philosopher *philo)
{
	if (philo->table_ptr->philos_count == 1)
	{
		ft_usleep(philo->table_ptr->time_to_be_died);
		safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
		return (1);
	}
	return (0);
}

int	acquire_first_fork(t_philosopher *philo)
{
	if (safe_mutex_handle(&philo->first_fork->fork, LOCK) == EXIT_FAILURE)
		return (0);
	if (is_simulation_ended(philo))
	{
		safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
		return (0);
	}
	safe_print(philo, "has taken a fork");
	return (1);
}

int	acquire_second_fork(t_philosopher *philo)
{
	if (safe_mutex_handle(&philo->second_fork->fork, LOCK) == EXIT_FAILURE)
	{
		safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
		return (0);
	}
	if (is_simulation_ended(philo))
	{
		safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
		safe_mutex_handle(&philo->second_fork->fork, UNLOCK);
		return (0);
	}
	safe_print(philo, "has taken a fork");
	return (1);
}

/*
** Updates philosopher's meal data
*/
void	update_meal_data(t_philosopher *philo)
{
	if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
		return ;
	philo->last_meal_time = get_time();
	philo->eaten_meals_counter++;
	if ((philo->table_ptr->maximum_meals_nbr != -1)
		&& (philo->eaten_meals_counter >= philo->table_ptr->maximum_meals_nbr))
	{
		philo->is_full = true;
	}
	safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
}
