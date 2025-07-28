/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:51:52 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/29 01:04:46 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PHILO_H
# define	PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

// Professional Color Scheme for Philosophers
# define RESET "\033[0m"
# define BOLD "\033[1m"
# define DIM "\033[2m"
# define ITALIC "\033[3m"
# define UNDERLINE "\033[4m"

// Primary Colors
# define RED "\033[31m"           // Errors and death
# define GREEN "\033[32m"         // Success and valid input
# define YELLOW "\033[33m"        // Warnings and thinking
# define BLUE "\033[34m"          // Information and actions
# define MAGENTA "\033[35m"       // Philosopher IDs
# define CYAN "\033[36m"          // Time stamps
# define WHITE "\033[37m"         // General text

// Bright Colors for Better Visibility
# define BRIGHT_RED "\033[91m"    // Critical errors
# define BRIGHT_GREEN "\033[92m"  // Success messages
# define BRIGHT_YELLOW "\033[93m" // Thinking state
# define BRIGHT_BLUE "\033[94m"   // Eating state
# define BRIGHT_MAGENTA "\033[95m"// Sleeping state
# define BRIGHT_CYAN "\033[96m"   // Fork actions
# define BRIGHT_WHITE "\033[97m"  // Timestamps

// Semantic Color Definitions for Actions
# define COLOR_FORK BRIGHT_CYAN     // Fork taking
# define COLOR_EAT BRIGHT_BLUE      // Eating
# define COLOR_SLEEP BRIGHT_MAGENTA // Sleeping
# define COLOR_THINK BRIGHT_YELLOW  // Thinking
# define COLOR_DEATH BRIGHT_RED     // Death
# define COLOR_TIME CYAN            // Timestamps
# define COLOR_ID MAGENTA           // Philosopher IDs
# define COLOR_SUCCESS BRIGHT_GREEN // Success messages
# define COLOR_ERROR BRIGHT_RED     // Error messages

typedef enum e_operation_code
{
	INIT,
	DESTROY,
	LOCK,
	UNLOCK,
	CREATE,
	JOIN,
	DETACH
}			t_operation_code;

typedef	struct s_table			t_table;
typedef	struct s_philosopher	t_philosopher;

typedef	struct	s_fork
{
	pthread_mutex_t	fork;
	int				fork_id;
}							t_fork;

typedef struct s_philosopher
{
	int			id;
	long		eaten_meals_counter;
	bool		is_full;
	long		last_meal_time;
	t_fork		*first_fork;
	t_fork		*second_fork;
	pthread_t	thread_id;
	t_table		*table_ptr;

}				t_philosopher;

typedef	struct s_table
{
	long			philos_count;
	long			time_to_be_died;
	long			time_to_eat;
	long			time_to_sleep;
	long			maximum_meals_nbr;
	long			start_simulation_time;
	bool			is_simulation_ended;
	bool			are_all_threads_ready;
	long			threads_running_counter;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	data_mutex;
	pthread_t		monitor_thread;
	t_fork			*forks;
	t_philosopher	*philosophers_arr;
}					t_table;


int	validate_input(int argc);
void	print_error(const char *error);
void	print_error_exit(const char *error);
void	print_success(const char *message);
void	print_info(const char *message);
void	print_simulation_start(long philos_count);
void	print_simulation_end(void);
int	start_program(char *argv[]);
long	ft_atol(const char *str);
int		table_initialization(t_table *table, char **argv);
void	*safe_malloc(size_t size);
int	print_error_with_return(const char *error);
int	safe_mutex_handle(pthread_mutex_t *mutex, t_operation_code opcode);
int	safe_thread_handle(pthread_t *thread, void *(*foo)(void *), void *data, t_operation_code opcode);
int		start_philos_dinner_sim(t_table *table);
void	clean_table(t_table *table);
void	assign_forks(t_philosopher *philo, t_fork *forks, int i);
long	get_time(void);
void	ft_usleep(long time);
void	safe_print(t_philosopher *philo, char *action);
int		philosopher_died(t_philosopher *philo);
void	*philosopher_routine(void *arg);
void	*monitor_routine(void *arg);


#endif
