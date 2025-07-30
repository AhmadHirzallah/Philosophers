/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:51:52 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/30 15:59:26 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>
# include <errno.h>

# define RESET "\033[0m"
# define BOLD "\033[1m"
# define DIM "\033[2m"
# define ITALIC "\033[3m"
# define UNDERLINE "\033[4m"

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"

# define BRIGHT_RED "\033[91m"
# define BRIGHT_GREEN "\033[92m"
# define BRIGHT_YELLOW "\033[93m"
# define BRIGHT_BLUE "\033[94m"
# define BRIGHT_MAGENTA "\033[95m"
# define BRIGHT_CYAN "\033[96m"
# define BRIGHT_WHITE "\033[97m"

# define COLOR_FORK BRIGHT_CYAN
# define COLOR_EAT BRIGHT_BLUE
# define COLOR_SLEEP BRIGHT_MAGENTA
# define COLOR_THINK BRIGHT_YELLOW
# define COLOR_DEATH BRIGHT_RED
# define COLOR_TIME CYAN
# define COLOR_ID MAGENTA
# define COLOR_SUCCESS BRIGHT_GREEN
# define COLOR_ERROR BRIGHT_RED

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

typedef struct s_table			t_table;
typedef struct s_philosopher	t_philosopher;

typedef struct s_fork
{
	pthread_mutex_t	fork;
	int				fork_id;
}	t_fork;

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
}	t_philosopher;

typedef struct s_table
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
}	t_table;

/*
** Validates input arguments: count and format
*/
int		validate_input(int argc);
/*
** Prints error message in red
*/
void	print_error(const char *error);
/*
** Prints error message and exits program
*/
void	print_error_exit(const char *error);
/*
** Prints success message in green
*/
void	print_success(const char *message);
/*
** Prints info message in cyan
*/
void	print_info(const char *message);
/*
** Main program entry point - coordinates initialization, simulation and cleanup
*/
int		start_program(char *argv[]);
/*
** Converts string to long with overflow protection
*/
long	ft_atol(const char *str);
/*
** Main table initialization function - coordinates all setup steps
*/
int		table_initialization(t_table *table, char **argv);
/*
** Parses command line arguments and fills table configuration
*/
int		parse_simulation_parameters(t_table *table, char **argv);
/*
** Safe malloc wrapper with error handling
*/
void	*safe_malloc(size_t size);
/*
** Prints error and returns EXIT_FAILURE
*/
int		print_error_with_return(const char *error);
/*
** Safe mutex operations wrapper with error handling
*/
int		safe_mutex_handle(pthread_mutex_t *mutex, t_operation_code opcode);
/*
** Safe thread operations wrapper with error handling
*/
int		safe_thread_handle(pthread_t *thread, void *(*foo)(void *), void *data,
			t_operation_code opcode);
/*
** Main simulation function that coordinates thread creation and execution
*/
int		start_philos_dinner_sim(t_table *table);
/*
** Cleans up all resources allocated for the simulation
*/
void	clean_table(t_table *table);
/**
 * DEADLOCK PREVENTION STRATEGY - Even/Odd Fork Assignment
 *
 * PROBLEM:
 * 	- Without strategy, all philosophers pick forks in same order → DEADLOCK
 *
 * SOLUTION:
 *	- Alternate fork acquisition order based on philosopher ID parity
 *
 * CIRCULAR TABLE LAYOUT (4 philosophers example):
 *
 *                    Fork[0]
 *         Philo[0]============Philo[1]
 *         (ID=1)              (ID=2)
 *            ||                  ||
 *            ||                  ||
 *       Fork[3]                Fork[1]
 *            ||                  ||
 *            ||                  ||
 *         Philo[3]============Philo[2]
 *         (ID=4)              (ID=3)
 *                    Fork[2]
 *
 * Table arrangement: Each philosopher sits between two forks
 * - Philo[0] sits between Fork[0] and Fork[1]
 * - Philo[1] sits between Fork[1] and Fork[2]
 * - Philo[2] sits between Fork[2] and Fork[3]
 * - Philo[3] sits between Fork[3] and Fork[0]
 *
 * FORK ASSIGNMENT ALGORITHM:
 * if (philo->id % 2 == 0) // EVEN ID philosophers
 *     first_fork = &forks[philosopher_index];  	           // Own fork
 *     second_fork = &forks[(philosopher_index + 1) % total];  // Next fork
 * else // ODD ID philosophers
 *     first_fork = &forks[(philosopher_index + 1) % total];   // Next fork
 *     second_fork = &forks[philosopher_index];                // Own fork
 *
 * DETAILED FORK ASSIGNMENTS (4 philosophers):
 * - Philosopher ID=1 (index=0, ODD) :  first_fork=Fork[1], second_fork=Fork[0]
 * - Philosopher ID=2 (index=1, EVEN):  first_fork=Fork[1], second_fork=Fork[2]
 * - Philosopher ID=3 (index=2, ODD) :  first_fork=Fork[3], second_fork=Fork[2]
 * - Philosopher ID=4 (index=3, EVEN):  first_fork=Fork[3], second_fork=Fork[0]
 *
 * ACQUISITION ORDER SUMMARY:
 * - Philosopher ID=1 (ODD):  Fork[1] → Fork[0]
 * - Philosopher ID=2 (EVEN): Fork[1] → Fork[2]
 * - Philosopher ID=3 (ODD):  Fork[3] → Fork[2]
 * - Philosopher ID=4 (EVEN): Fork[3] → Fork[0]
 *
 * DEADLOCK PREVENTION IN ACTION:
 * Step 1: First fork acquisition:اكتساب attempts
 *   - Philo ID=1 & ID=2 both want Fork[1] → CONFLICT (one waits)
 *   - Philo ID=3 & ID=4 both want Fork[3] → CONFLICT (one waits)
 *   - Let's say ID=1 gets Fork[1] and ID=3 gets Fork[3]
 *
 * Step 2: Winners proceed to second fork
 *   - Philo ID=1: has Fork[1], wants Fork[0] (available!)
 *   - Philo ID=3: has Fork[3], wants Fork[2] (available!)
 *   - Both can eat simultaneously - NO DEADLOCK!
 *
 * Step 3: After eating, forks are released
 *   - Philo ID=1 releases Fork[1] and Fork[0]
 *   - Philo ID=3 releases Fork[3] and Fork[2]
 *
 * Step 4: Waiting philosophers can now proceed
 *   - Philo ID=2 can acquire Fork[1] then Fork[2]
 *   - Philo ID=4 can acquire Fork[3] then Fork[0]
 *
 * KEY INSIGHT: By alternating acquisition:اكتساب order,
 *  we break the circular wait
 * condition that causes deadlock. At least one pair of philosophers can
 * always make progress, ensuring the system never reaches a complete standstill.
 * DEADLOCK PREVENTION STRATEGY - Even/Odd Fork Assignment (5 Philosophers)
 *
 * PROBLEM:
 * 	- With odd numbers, more complex patterns but same deadlock risk
 *
 * SOLUTION:
 *	- Same alternating fork acquisition strategy works for odd numbers
 *
 * CIRCULAR TABLE LAYOUT (5 philosophers example):
 *
 *                     Fork[0]
 *         Philo[0]================Philo[1]
 *         (ID=1)                  (ID=2)
 *            ||                      ||
 *            ||                      ||
 *       Fork[4]                   Fork[1]
 *            ||                      ||
 *            ||         X            ||
 *         Philo[4]   Philo[2]     Fork[2]
 *         (ID=5)     (ID=3)          ||
 *            ||         ||           ||
 *            ||      Fork[3]         ||
 *            ||         ||           ||
 *            ||========================||
 *                   Philo[3]
 *                   (ID=4)
 *
 * Table arrangement: Each philosopher sits between two forks
 * - ID 1: Philo[0] sits between Fork[0] and Fork[1]
 * - ID 2: Philo[1] sits between Fork[1] and Fork[2]
 * - ID 3: Philo[2] sits between Fork[2] and Fork[3]
 * - ID 4: Philo[3] sits between Fork[3] and Fork[4]
 * - ID 5: Philo[4] sits between Fork[4] and Fork[0]
 *
 * FORK ASSIGNMENT ALGORITHM (same as 4 philosophers):
 * if (philo->id % 2 == 0) // EVEN ID philosophers
 *     first_fork = &forks[philosopher_index];  	           // Own fork
 *     second_fork = &forks[(philosopher_index + 1) % total];  // Next fork
 * else // ODD ID philosophers
 *     first_fork = &forks[(philosopher_index + 1) % total];   // Next fork
 *     second_fork = &forks[philosopher_index];                // Own fork
 *
 * DETAILED FORK ASSIGNMENTS (5 philosophers):
 * - Philosopher ID=1 (index=0, ODD) :  first_fork=Fork[1], second_fork=Fork[0]
 * - Philosopher ID=2 (index=1, EVEN):  first_fork=Fork[1], second_fork=Fork[2]
 * - Philosopher ID=3 (index=2, ODD) :  first_fork=Fork[3], second_fork=Fork[2]
 * - Philosopher ID=4 (index=3, EVEN):  first_fork=Fork[3], second_fork=Fork[4]
 * - Philosopher ID=5 (index=4, ODD) :  first_fork=Fork[0], second_fork=Fork[4]
 *
 * ACQUISITION ORDER SUMMARY:
 * - Philosopher ID=1 (ODD):  Fork[1] → Fork[0]
 * - Philosopher ID=2 (EVEN): Fork[1] → Fork[2]
 * - Philosopher ID=3 (ODD):  Fork[3] → Fork[2]
 * - Philosopher ID=4 (EVEN): Fork[3] → Fork[4]
 * - Philosopher ID=5 (ODD):  Fork[0] → Fork[4]
 *
 * DEADLOCK PREVENTION IN ACTION (5 philosophers):
 * Step 1: First fork acquisition attempts
 *   - Philo ID=1 & ID=2 both want Fork[1] → CONFLICT (one waits)
 *   - Philo ID=3 & ID=4 both want Fork[3] → CONFLICT (one waits)
 *   - Philo ID=5 wants Fork[0] → NO CONFLICT (gets it immediately)
 *   - Let's say ID=1 gets Fork[1], ID=3 gets Fork[3], ID=5 gets Fork[0]
 *
 * Step 2: Winners proceed to second fork
 *   - Philo ID=1: has Fork[1], wants Fork[0] (but ID=5 has it) → WAITS
 *   - Philo ID=3: has Fork[3], wants Fork[2] (available!) → GETS IT
 *   - Philo ID=5: has Fork[0], wants Fork[4] (available!) → GETS IT
 *   - Result: ID=3 and ID=5 can eat, ID=1 waits
 *
 * Step 3: After eating, forks are released
 *   - Philo ID=3 releases Fork[3] and Fork[2]
 *   - Philo ID=5 releases Fork[0] and Fork[4]
 *
 * Step 4: New opportunities arise
 *   - Philo ID=1: can now get Fork[0] (released by ID=5) → EATS
 *   - Philo ID=2: can now compete for Fork[1] or Fork[2]
 *   - Philo ID=4: can now compete for Fork[3] or Fork[4]
 *
 * KEY INSIGHT FOR ODD NUMBERS: Even with odd philosophers, deadlock is still
 * prevented because the alternating pattern ensures at least one philosopher
 * can always make progress.
 *  More complex waiting chains but never total deadlock.
 */
void	assign_forks(t_philosopher *philo, t_fork *forks, int i);
/*
** Gets current time in milliseconds
*/
long	get_time(void);
/*
** Precise sleep function using microsleep in a loop
*/
void	ft_usleep(long time);
/*
** Custom implementation of strstr function
*/
char	*ft_strstr(const char *haystack, const char *needle);
/*
** Calculates optimal thinking time to prevent
	starvation for odd number of philosophers
*/
void	think_time(t_philosopher *philo);
/*
** Thread-safe printing with timestamp and colors
*/
void	safe_print(t_philosopher *philo, char *action);
/*
** Checks if philosopher has died and handles death logic
*/
int		philosopher_died(t_philosopher *philo);
/*
** Main philosopher routine - coordinates eating, sleeping, thinking cycle
*/
void	*philosopher_routine(void *arg);
/*
** Monitor routine that watches for death and completion
*/
void	*monitor_routine(void *arg);
/*
** Creates and starts all philosopher threads
*/
int		create_philosopher_threads(t_table *table);
/*
** Creates and starts the monitor thread
*/
int		create_monitor_thread(t_table *table);
/*
** Signals all threads to start simulation
*/
int		signal_simulation_start(t_table *table);
/*
** Waits for all philosopher threads to complete
*/
int		wait_for_philosopher_threads(t_table *table);
/*
** Waits for monitor thread to complete
*/
int		wait_for_monitor_thread(t_table *table);
/*
** Waits for all threads to be ready before starting philosopher routine
*/
void	wait_for_simulation_start(t_philosopher *philo);
/*
** Checks if simulation has ended
*/
int		is_simulation_ended(t_philosopher *philo);
/*
** Prints death message with proper formatting and colors
*/
void	print_death_message(t_philosopher *philo);
/*
** Handles special case for single philosopher
*/
int		handle_single_philosopher(t_philosopher *philo);
/*
** Attempts to acquire first fork with safety checks
*/
int		acquire_first_fork(t_philosopher *philo);
/*
** Attempts to acquire second fork with safety checks
*/
int		acquire_second_fork(t_philosopher *philo);
/*
** Updates philosopher's meal data
*/
void	update_meal_data(t_philosopher *philo);
/*
** Initializes simulation control variables
*/
void	initialize_simulation_control(t_table *table);
/*
** Initializes all mutexes for the simulation
*/
int		initialize_mutexes(t_table *table);
/*
** Initializes all fork mutexes and assigns fork IDs
*/
int		initialize_fork_mutexes(t_table *table);
/*
** Initializes individual philosopher properties
*/
void	initialize_philosopher_properties(t_table *table, int philo_index);
/*
** Initializes all philosophers in the simulation
*/
int		initialize_philosophers(t_table *table);
/*
** Waits for all threads to be ready before starting monitoring
*/
void	wait_for_monitor_start(t_table *table);
/*
** Checks if simulation should end due to global flag
*/
int		should_end_simulation(t_table *table);
/*
** Checks all philosophers for death
*/
int		check_philosophers_death(t_table *table);
void	assign_forks(t_philosopher *philo, t_fork *forks,
			int philosopher_index);
long	get_time(void);
void	ft_usleep(long sleep_time_ms);
void	ft_usleep_interruptible(t_philosopher *philo, long sleep_time_ms);

#endif
