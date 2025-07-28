/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 01:52:00 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/29 02:11:08 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Checks if enough time has passed since last meal to consider philosopher dead
*/
static int	has_philosopher_died(t_philosopher *philo)
{
	long	time_since_last_meal;

	time_since_last_meal = get_time() - philo->last_meal_time;
	return (time_since_last_meal >= philo->table_ptr->time_to_be_died);
}

/*
** Checks if philosopher has died and handles death logic
*/
int	philosopher_died(t_philosopher *philo)
{
	if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
		return (1);
	if (philo->table_ptr->is_simulation_ended)
	{
		safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
		return (1);
	}
	if (philo->is_full)
	{
		safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
		return (0);
	}
	if (has_philosopher_died(philo))
	{
		philo->table_ptr->is_simulation_ended = true;
		safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
		print_death_message(philo);
		return (1);
	}
	safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
	return (0);
}
