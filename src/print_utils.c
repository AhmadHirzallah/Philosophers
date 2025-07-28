/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 01:51:00 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/29 02:01:30 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		return ;
	if (!philo->table_ptr->is_simulation_ended)
	{
		elapsed_time = get_time() - philo->table_ptr->start_simulation_time;
		action_color = get_action_color(action);
		printf(COLOR_TIME "%ld" RESET " " COLOR_ID "%d" RESET " %s%s" RESET
			"\n", elapsed_time, philo->id, action_color, action);
	}
	safe_mutex_handle(&philo->table_ptr->print_mutex, UNLOCK);
}

/*
** Prints death message with proper formatting and colors
*/
void	print_death_message(t_philosopher *philo)
{
	if (safe_mutex_handle(&philo->table_ptr->print_mutex, LOCK) == EXIT_FAILURE)
		return ;
	printf(COLOR_TIME "%ld" RESET " " COLOR_ID "%d" RESET " " COLOR_DEATH
		"died" RESET "\n", get_time() - philo->table_ptr->start_simulation_time,
		philo->id);
	safe_mutex_handle(&philo->table_ptr->print_mutex, UNLOCK);
}
