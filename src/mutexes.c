/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutexes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:52:01 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/28 23:52:02 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int handle_mutex_error(int status, t_operation_code opcode)
{
	if (0 == status)
		return (EXIT_SUCCESS);
	if (EINVAL == status && (LOCK == opcode || UNLOCK == opcode))
		return (print_error_with_return("The value specified by mutex is invalid."));
	else if (EINVAL == status && INIT == opcode)
		return (print_error_with_return("The value specified by attr is invalid."));
	else if (EDEADLK == status)
		return (print_error_with_return("A deadlock would occur if the thread blocked waiting for mutex."));
	else if (EPERM == status)
		return (print_error_with_return("The current thread does not hold a lock on mutex."));
	else if (ENOMEM == status)
		return (print_error_with_return("The process cannot allocate enough memory to create another mutex."));
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

