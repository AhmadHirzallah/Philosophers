/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:54:52 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/29 01:17:02 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Assigns fork pointers to philosopher based on even/odd strategy to prevent deadlock
*/
void	assign_forks(t_philosopher *philo, t_fork *forks, int philosopher_index)
{
	int	total_philosophers;

	total_philosophers = philo->table_ptr->philos_count;
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &forks[philosopher_index];
		philo->second_fork = &forks[(philosopher_index + 1) % total_philosophers];
	}
	else
	{
		philo->first_fork = &forks[(philosopher_index + 1) % total_philosophers];
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
		usleep(500);
}

/*
** Determines appropriate color based on action type
*/
static char	*get_action_color(char *action)
{
	if (strstr(action, "fork"))
		return (COLOR_FORK);
	else if (strstr(action, "eating"))
		return (COLOR_EAT);
	else if (strstr(action, "sleeping"))
		return (COLOR_SLEEP);
	else if (strstr(action, "thinking"))
		return (COLOR_THINK);
	else
		return (WHITE);
}

/*
** Thread-safe printing with timestamp and colors
*/
void	safe_print(t_philosopher *philo, char *action)
{
	long	elapsed_time;
	char	*action_color;

	if (safe_mutex_handle(&philo->table_ptr->print_mutex, LOCK) == EXIT_FAILURE)
		return;
	if (!philo->table_ptr->is_simulation_ended)
	{
		elapsed_time = get_time() - philo->table_ptr->start_simulation_time;
		action_color = get_action_color(action);
		printf(COLOR_TIME"%ld"RESET" "COLOR_ID"%d"RESET" %s%s"RESET"\n",
			   elapsed_time, philo->id, action_color, action);
	}
	safe_mutex_handle(&philo->table_ptr->print_mutex, UNLOCK);
}

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
** Prints death message with proper formatting and colors
*/
static void	print_death_message(t_philosopher *philo)
{
	if (safe_mutex_handle(&philo->table_ptr->print_mutex, LOCK) == EXIT_FAILURE)
		return;
	printf(COLOR_TIME"%ld"RESET" "COLOR_ID"%d"RESET" "COLOR_DEATH"died"RESET"\n",
		   get_time() - philo->table_ptr->start_simulation_time, philo->id);
	safe_mutex_handle(&philo->table_ptr->print_mutex, UNLOCK);
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

/*
** Waits for all threads to be ready before starting simulation
*/
static void	wait_for_simulation_start(t_philosopher *philo)
{
	while (1)
	{
		if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
			return;
		if (philo->table_ptr->are_all_threads_ready)
		{
			safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
			break;
		}
		safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
		usleep(10);
	}
}

/*
** Checks if simulation has ended
*/
static int	is_simulation_ended(t_philosopher *philo)
{
	int	ended;

	if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
		return (1);
	ended = philo->table_ptr->is_simulation_ended;
	safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
	return (ended);
}

/*
** Handles special case for single philosopher
*/
static int	handle_single_philosopher(t_philosopher *philo)
{
	if (philo->table_ptr->philos_count == 1)
	{
		ft_usleep(philo->table_ptr->time_to_be_died);
		safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
		return (1);
	}
	return (0);
}

/*
** Attempts to acquire first fork with safety checks
*/
static int	acquire_first_fork(t_philosopher *philo)
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

/*
** Attempts to acquire second fork with safety checks
*/
static int	acquire_second_fork(t_philosopher *philo)
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
static void	update_meal_data(t_philosopher *philo)
{
	if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
		return;
	philo->last_meal_time = get_time();
	philo->eaten_meals_counter++;
	safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
}

/*
** Handles eating process including fork acquisition and meal tracking
*/
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

/*
** Checks if philosopher has eaten enough meals
*/
static void	check_meal_completion(t_philosopher *philo)
{
	if (philo->table_ptr->maximum_meals_nbr != -1 &&
		philo->eaten_meals_counter >= philo->table_ptr->maximum_meals_nbr)
	{
		if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
			return;
		philo->is_full = true;
		safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
	}
}

/*
** Handles sleeping process with safety checks
*/
static int	philosopher_sleep(t_philosopher *philo)
{
	if (is_simulation_ended(philo))
		return (0);
	safe_print(philo, "is sleeping");
	ft_usleep(philo->table_ptr->time_to_sleep);
	return (1);
}

/*
** Handles thinking process with safety checks
*/
static int	philosopher_think(t_philosopher *philo)
{
	if (is_simulation_ended(philo))
		return (0);
	safe_print(philo, "is thinking");
	return (1);
}

/*
** Main philosopher routine - coordinates eating, sleeping, thinking cycle
*/
void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	wait_for_simulation_start(philo);
	if (philo->id % 2 == 0)
		ft_usleep(philo->table_ptr->time_to_eat / 2);
	while (!is_simulation_ended(philo))
	{
		if (!philosopher_eat(philo))
			break;
		check_meal_completion(philo);
		if (!philosopher_sleep(philo))
			break;
		if (!philosopher_think(philo))
			break;
	}
	return (NULL);
}

/*
** Waits for monitor thread simulation start
*/
static void	wait_for_monitor_start(t_table *table)
{
	while (1)
	{
		if (safe_mutex_handle(&table->data_mutex, LOCK) == EXIT_FAILURE)
			return;
		if (table->are_all_threads_ready)
		{
			safe_mutex_handle(&table->data_mutex, UNLOCK);
			break;
		}
		safe_mutex_handle(&table->data_mutex, UNLOCK);
		usleep(10);
	}
}

/*
** Checks if all philosophers have finished eating required meals
*/
static int	are_all_philosophers_full(t_table *table)
{
	int	philo_index;
	int	all_full;

	all_full = 1;
	philo_index = 0;
	while (philo_index < table->philos_count)
	{
		if (safe_mutex_handle(&table->data_mutex, LOCK) == EXIT_FAILURE)
			return (0);
		if (!table->philosophers_arr[philo_index].is_full)
			all_full = 0;
		safe_mutex_handle(&table->data_mutex, UNLOCK);
		philo_index++;
	}
	return (all_full && table->maximum_meals_nbr != -1);
}

/*
** Ends simulation by setting the end flag
*/
static void	end_simulation(t_table *table)
{
	if (safe_mutex_handle(&table->data_mutex, LOCK) == EXIT_FAILURE)
		return;
	table->is_simulation_ended = true;
	safe_mutex_handle(&table->data_mutex, UNLOCK);
}

/*
** Monitor routine - watches for death conditions and meal completion
*/
void	*monitor_routine(void *arg)
{
	t_table	*table;
	int		philo_index;

	table = (t_table *)arg;
	wait_for_monitor_start(table);
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
		philo_index = 0;
		while (philo_index < table->philos_count)
		{
			if (philosopher_died(&table->philosophers_arr[philo_index]))
				return (NULL);
			philo_index++;
		}
		if (are_all_philosophers_full(table))
		{
			end_simulation(table);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

