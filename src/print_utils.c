/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 01:51:00 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/30 10:54:13 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static char	*get_action_color(char *action)
{
	if (ft_strstr(action, "fork"))
		return (COLOR_FORK);
	else if (ft_strstr(action, "eating"))
		return (COLOR_EAT);
	else if (ft_strstr(action, "sleeping"))
		return (COLOR_SLEEP);
	else if (ft_strstr(action, "thinking"))
		return (COLOR_THINK);
	else
		return (WHITE);
}

void	safe_print(t_philosopher *philo, char *action)
{
	long	elapsed_time;
	char	*action_color;

	if (safe_mutex_handle(&philo->table_ptr->print_mutex, LOCK) == EXIT_FAILURE)
		return ;
	if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
	{
		safe_mutex_handle(&philo->table_ptr->print_mutex, UNLOCK);
		return ;
	}
	if (!philo->table_ptr->is_simulation_ended)
	{
		elapsed_time = get_time() - philo->table_ptr->start_simulation_time;
		safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
		action_color = get_action_color(action);
		printf(COLOR_TIME "%ld" RESET " " COLOR_ID "%d" RESET " %s%s" RESET
			"\n", elapsed_time, philo->id, action_color, action);
	}
	else
		safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
	safe_mutex_handle(&philo->table_ptr->print_mutex, UNLOCK);
}

void	print_death_message(t_philosopher *philo)
{
	long	elapsed_time;

	if (safe_mutex_handle(&philo->table_ptr->print_mutex, LOCK) == EXIT_FAILURE)
		return ;
	if (safe_mutex_handle(&philo->table_ptr->data_mutex, LOCK) == EXIT_FAILURE)
	{
		safe_mutex_handle(&philo->table_ptr->print_mutex, UNLOCK);
		return ;
	}
	elapsed_time = get_time() - philo->table_ptr->start_simulation_time;
	safe_mutex_handle(&philo->table_ptr->data_mutex, UNLOCK);
	printf(COLOR_TIME "%ld" RESET " " COLOR_ID "%d" RESET " " COLOR_DEATH
		"died" RESET "\n", elapsed_time, philo->id);
	safe_mutex_handle(&philo->table_ptr->print_mutex, UNLOCK);
}
