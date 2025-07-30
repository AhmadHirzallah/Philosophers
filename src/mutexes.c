/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutexes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:52:01 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/30 07:30:49 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	handle_mutex_error(int status, t_operation_code opcode)
{
	if (0 == status)
		return (EXIT_SUCCESS);
	if (EINVAL == status && (LOCK == opcode || UNLOCK == opcode))
		return (print_error_with_return("Invalid mutex value."));
	else if (EINVAL == status && INIT == opcode)
		return (print_error_with_return("Invalid mutex attr value."));
	else if (EDEADLK == status)
		return (print_error_with_return("Deadlock mutex would occur."));
	else if (EPERM == status)
		return (print_error_with_return("Thread mutex does not hold lock."));
	else if (ENOMEM == status)
		return (print_error_with_return("Mutex Cannot allocate memory."));
	else if (EBUSY == status)
		return (print_error_with_return("Mutex is locked"));
	else
		return (print_error_with_return("Unknown mutex error"));
}

int	safe_mutex_handle(pthread_mutex_t *mutex, t_operation_code opcode)
{
	if (LOCK == opcode)
		return (handle_mutex_error(pthread_mutex_lock(mutex), opcode));
	else if (UNLOCK == opcode)
		return (handle_mutex_error(pthread_mutex_unlock(mutex), opcode));
	else if (INIT == opcode)
		return (handle_mutex_error(pthread_mutex_init(mutex, NULL), opcode));
	else if (DESTROY == opcode)
		return (handle_mutex_error(pthread_mutex_destroy(mutex), opcode));
	else
		return (print_error_with_return("Wrong opcode for mutex handle"));
}
