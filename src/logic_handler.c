/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:54:52 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/29 01:04:46 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	assign_forks(t_philosopher *philo, t_fork *forks, int i)
{
	int	philo_nbr;

	philo_nbr = philo->table_ptr->philos_count;
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &forks[i];
		philo->second_fork = &forks[(i + 1) % philo_nbr];
	}
	else
	{
		philo->first_fork = &forks[(i + 1) % philo_nbr];
		philo->second_fork = &forks[i];
	}
}

long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long time)
{
	long	start;

	start = get_time();
	while ((get_time() - start) < time)
		usleep(500);
}

void	safe_print(t_philosopher *philo, char *action)
{
	long	time;
	char	*color;

	if (safe_mutex_handle(&philo->table_ptr->print_mutex, LOCK) == EXIT_FAILURE)
		return;

	if (!philo->table_ptr->is_simulation_ended)
	{
		time = get_time() - philo->table_ptr->start_simulation_time;

		// Choose color based on action
		if (strstr(action, "fork"))
			color = COLOR_FORK;
		else if (strstr(action, "eating"))
			color = COLOR_EAT;
		else if (strstr(action, "sleeping"))
			color = COLOR_SLEEP;
		else if (strstr(action, "thinking"))
			color = COLOR_THINK;
		else
			color = WHITE;

		printf(COLOR_TIME"%ld"RESET" "COLOR_ID"%d"RESET" %s%s"RESET"\n",
			   time, philo->id, color, action);
	}

	safe_mutex_handle(&philo->table_ptr->print_mutex, UNLOCK);
}

int	philosopher_died(t_philosopher *philo)
{
	long	time_since_last_meal;

	if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
		return (1);

	if (philo->table_ptr->is_simulation_ended)
	{
		safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
		return (1);
	}

	time_since_last_meal = get_time() - philo->last_meal_time;

	if (time_since_last_meal >= philo->table_ptr->time_to_be_died)
	{
		philo->table_ptr->is_simulation_ended = true;
		safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);

		// Print death message with print mutex protection and colors
		if (safe_mutex_handle(&philo->table_ptr->print_mutex, LOCK) == EXIT_FAILURE)
			return (1);
		printf(COLOR_TIME"%ld"RESET" "COLOR_ID"%d"RESET" "COLOR_DEATH"died"RESET"\n",
			   get_time() - philo->table_ptr->start_simulation_time, philo->id);
		safe_mutex_handle(&philo->table_ptr->print_mutex, UNLOCK);

		return (1);
	}

	safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
	return (0);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;

	// Synchronize start with proper mutex protection
	while (1)
	{
		if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
			return (NULL);
		if (philo->table_ptr->are_all_threads_ready)
		{
			safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
			break;
		}
		safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
		usleep(10);
	}

	// Stagger even philosophers to avoid deadlock
	if (philo->id % 2 == 0)
		ft_usleep(philo->table_ptr->time_to_eat / 2);

	while (1)
	{
		// Check if simulation ended
		if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
			break;
		if (philo->table_ptr->is_simulation_ended)
		{
			safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
			break;
		}
		safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);

		// Take forks
		if (safe_mutex_handle(&philo->first_fork->fork, LOCK) == EXIT_FAILURE)
			break;
		if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
		{
			safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
			break;
		}
		if (philo->table_ptr->is_simulation_ended)
		{
			safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
			safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
			break;
		}
		safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
		safe_print(philo, "has taken a fork");

		if (philo->table_ptr->philos_count == 1)
		{
			ft_usleep(philo->table_ptr->time_to_be_died);
			safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
			break;
		}

		if (safe_mutex_handle(&philo->second_fork->fork, LOCK) == EXIT_FAILURE)
		{
			safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
			break;
		}
		if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
		{
			safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
			safe_mutex_handle(&philo->second_fork->fork, UNLOCK);
			break;
		}
		if (philo->table_ptr->is_simulation_ended)
		{
			safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
			safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
			safe_mutex_handle(&philo->second_fork->fork, UNLOCK);
			break;
		}
		safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
		safe_print(philo, "has taken a fork");

		// Eat
		safe_print(philo, "is eating");

		if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
		{
			safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
			safe_mutex_handle(&philo->second_fork->fork, UNLOCK);
			break;
		}
		philo->last_meal_time = get_time();
		philo->eaten_meals_counter++;
		safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);

		ft_usleep(philo->table_ptr->time_to_eat);

		// Release forks
		safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
		safe_mutex_handle(&philo->second_fork->fork, UNLOCK);

		// Check if finished eating
		if (philo->table_ptr->maximum_meals_nbr != -1 &&
			philo->eaten_meals_counter >= philo->table_ptr->maximum_meals_nbr)
		{
			if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
				break;
			philo->is_full = true;
			safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
		}

		// Sleep
		if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
			break;
		if (!philo->table_ptr->is_simulation_ended)
		{
			safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
			safe_print(philo, "is sleeping");
			ft_usleep(philo->table_ptr->time_to_sleep);
		}
		else
		{
			safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
			break;
		}

		// Think
		if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
			break;
		if (!philo->table_ptr->is_simulation_ended)
		{
			safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
			safe_print(philo, "is thinking");
		}
		else
		{
			safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
			break;
		}
	}

	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_table	*table;
	int		i;
	int		all_full;

	table = (t_table *)arg;

	// Synchronize start with proper mutex protection
	while (1)
	{
		if (safe_mutex_handle(&table->data_mutex, LOCK) == EXIT_FAILURE)
			return (NULL);
		if (table->are_all_threads_ready)
		{
			safe_mutex_handle(&table->data_mutex, UNLOCK);
			break;
		}
		safe_mutex_handle(&table->data_mutex, UNLOCK);
		usleep(10);
	}

	while (1)
	{
		if (safe_mutex_handle(&table->data_mutex, LOCK) == EXIT_FAILURE)
			return (NULL);
		if (table->is_simulation_ended)
		{
			safe_mutex_handle(&table->data_mutex, UNLOCK);
			return (NULL);
		}
		safe_mutex_handle(&table->data_mutex, UNLOCK);

		all_full = 1;

		for (i = 0; i < table->philos_count; i++)
		{
			if (philosopher_died(&table->philosophers_arr[i]))
				return (NULL);

			if (safe_mutex_handle(&table->data_mutex, LOCK) == EXIT_FAILURE)
				return (NULL);

			if (!table->philosophers_arr[i].is_full)
				all_full = 0;

			safe_mutex_handle(&table->data_mutex, UNLOCK);
		}

		if (all_full && table->maximum_meals_nbr != -1)
		{
			if (safe_mutex_handle(&table->data_mutex, LOCK) == EXIT_FAILURE)
				return (NULL);
			table->is_simulation_ended = true;
			safe_mutex_handle(&table->data_mutex, UNLOCK);
			return (NULL);
		}

		usleep(1000);
	}

	return (NULL);
}

