/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:52:17 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/30 15:41:11 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_error(const char *error)
{
	printf(COLOR_ERROR"%s"RESET"\n", error);
}

int	print_error_with_return(const char *error)
{
	printf(COLOR_ERROR"%s"RESET"\n", error);
	return (EXIT_FAILURE);
}

void	print_success(const char *message)
{
	printf(COLOR_SUCCESS"%s"RESET"\n", message);
}

void	print_info(const char *message)
{
	printf(BLUE"%s"RESET"\n", message);
}
