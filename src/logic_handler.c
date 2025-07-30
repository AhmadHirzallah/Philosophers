/* ************************	return (1);
}

static void	philosopher_sleep(t_philosopher *philo)******************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:54:52 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/29 02:17:09 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	philosopher_eat(t_philosopher *philo)
{
	if (!acquire_first_fork(philo))
		return (0);
	if (handle_single_philosopher(philo))
		return (0);
	if (!acquire_second_fork(philo))
		return (0);
	safe_print(philo, "is eating");
	update_meal_data(philo);
	ft_usleep(philo->table_ptr->time_to_eat);
	safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
	safe_mutex_handle(&philo->second_fork->fork, UNLOCK);
	return (1);
}

static int	philosopher_sleep(t_philosopher *philo)
{
	if (is_simulation_ended(philo))
		return (0);
	safe_print(philo, "is sleeping");
	ft_usleep(philo->table_ptr->time_to_sleep);
	return (1);
}

static int	philosopher_think(t_philosopher *philo)
{
	if (is_simulation_ended(philo))
		return (0);
	safe_print(philo, "is thinking");
	return (1);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	wait_for_simulation_start(philo);
	if (philo->id % 2 == 0)
		ft_usleep(philo->table_ptr->time_to_eat / 2);
	while (!is_simulation_ended(philo))
	{
		if (philo->is_full)
		{
			ft_usleep(100);
			continue ;
		}
		if (!philosopher_eat(philo))
			break ;
		if (!philosopher_sleep(philo))
			break ;
		if (!philosopher_think(philo))
			break ;
	}
	return (NULL);
}
