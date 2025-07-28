/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:52:20 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/28 23:52:21 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int handle_thread_error(int status, t_operation_code opcode)
{
	if (0 == status)
		return (EXIT_SUCCESS);
	if (EAGAIN == status)
		return (print_error_with_return("No resources to create another thread"));
	else if (EPERM == status)
		return (print_error_with_return("The caller does not have appropriate permission"));
	else if (EINVAL == status && CREATE == opcode)
		return (print_error_with_return("The value specified by attr is invalid."));
	else if (EINVAL == status && (JOIN == opcode || DETACH == opcode))
		return (print_error_with_return("The value specified by thread is not joinable"));
	else if (ESRCH == status)
		return (print_error_with_return("No thread could be found corresponding to that specified by the given thread ID"));
	else if (EDEADLK == status)
		return (print_error_with_return("A deadlock was detected or the value of thread specifies the calling thread"));
	else
		return (print_error_with_return("Unknown thread error"));
}

int	safe_thread_handle(pthread_t *thread, void *(*foo)(void *),
				void *data, t_operation_code opcode)
{
	if (CREATE == opcode)
		return (handle_thread_error(pthread_create(thread, NULL, foo, data), opcode));
	else if (JOIN == opcode)
		return (handle_thread_error(pthread_join(*thread, NULL), opcode));
	else if (DETACH == opcode)
		return (handle_thread_error(pthread_detach(*thread), opcode));
	else
		return (print_error_with_return("Wrong opcode for thread handle: use <CREATE> <JOIN> <DETACH>"));
}
