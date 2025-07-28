/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:52:11 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/28 23:52:12 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		start_philos_dinner_sim(t_table *table)
{
	// TODO: Implement philosopher simulation
	(void)table;
	printf("Starting philosopher simulation...\n");
	return (EXIT_SUCCESS);
}

void	clean_table(t_table *table)
{
	// TODO: Implement cleanup
	if (table->philosophers_arr)
		free(table->philosophers_arr);
	if (table->forks)
		free(table->forks);
	printf("Cleaned up table resources.\n");
}

int	start_program(char *argv[])
{
	t_table	table;

	if (table_initialization(&table, argv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	start_philos_dinner_sim(&table);
	clean_table(&table);

	return (EXIT_SUCCESS);
}
