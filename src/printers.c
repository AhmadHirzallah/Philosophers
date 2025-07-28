/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:52:17 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/28 23:52:18 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_error(const char *error)
{
	printf(RED"%s\n"RESET, error);
}

void	print_error_exit(const char *error)
{
	printf(RED"%s\n"RESET, error);
	exit(EXIT_FAILURE);
}

int	print_error_with_return(const char *error)
{
	printf(RED"%s\n"RESET, error);
	return (EXIT_FAILURE);
}
