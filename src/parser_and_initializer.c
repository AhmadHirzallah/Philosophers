/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_and_initializer.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:52:04 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/28 23:52:05 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void parse_values(t_table *table, char **argv)
{
	table->philos_count = ft_atol(argv[1]);
	table->time_to_be_died = ft_atol(argv[2]) * 1e3;
	table->time_to_eat = ft_atol(argv[3]) * 1e3;
	table->time_to_sleep = ft_atol(argv[4]) * 1e3;
	if (argv[5] && argv[5][0])
		table->maximum_meals_nbr = ft_atol(argv[5]);
	else
		table->maximum_meals_nbr = -1; // No limit on meals
}

static int initialize_philosophers(t_table *table)
{
	int i;

	i = 0;
	while (i < table->philos_count)
	{
		table->philosophers_arr[i].table_ptr = table;
		table->philosophers_arr[i].id = i + 1;
		table->philosophers_arr[i].is_full = false;
		table->philosophers_arr[i].eaten_meals_counter = 0;
		table->philosophers_arr[i].last_meal_passed_time = 0;
		assign_forks(&table->philosophers_arr[i], table->forks, i);
		i++;
	}
	return (EXIT_SUCCESS);
}

int		table_initialization(t_table *table, char **argv)
{
	int i;

	i = 0;
	parse_values(table, argv);
	table->is_simulation_ended = false;
	table->are_all_threads_ready = false;
	table->threads_running_counter = 0;
	table->philosophers_arr = safe_malloc(sizeof(t_philosopher) * table->philos_count);
	if (!(table->philosophers_arr))
		return (EXIT_FAILURE);
	table->forks = safe_malloc(sizeof(t_fork) * table->philos_count);
	if (!(table->forks))
		return (EXIT_FAILURE);
	table->philosophers_arr = safe_malloc(sizeof(t_philosopher) * table->philos_count);
	if (!(table->philosophers_arr))
		return (EXIT_FAILURE);
	table->forks = safe_malloc(sizeof(t_fork) * table->philos_count);
	if (!(table->forks))
		return (EXIT_FAILURE);
	while (i < table->philos_count)
	{
		safe_mutex_handle(&table->forks[i].fork, INIT);
		table->forks[i].fork_id = i;
	}


	return (EXIT_SUCCESS);
}

