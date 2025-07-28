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

// Define color codes
# define RESET "\033[0m"
# define BOLD "\033[1m"
# define UNDERLINE "\033[4m"

// Foreground colors
# define BLACK "\033[30m"
# define RED "\033[31m"
# define GREEN "\033[92m"
# define YELLOW "\033[93m"
# define BLUE "\033[94m"
# define MAGENTA "\033[95m"
# define CYAN "\033[96m"
# define WHITE "\033[37m"

// Background colors
# define BGBLACK "\033[40m"
# define BGRED "\033[41m"
# define BGGREEN "\033[42m"
# define BGYELLOW "\033[43m"
# define BGBLUE "\033[44m"
# define BGMAGENTA "\033[45m"
# define BGCYAN "\033[46m"
# define BGWHITE "\033[47m"
# define BGBRIGHTGREEN "\033[102m"

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
	long		last_meal_passed_time;
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
	t_fork			*forks;
	t_philosopher	*philosophers_arr;
}					t_table;


int	validate_input(int argc);
void	print_error(const char *error);
void	print_error_exit(const char *error);
int	start_program(char *argv[]);
long	ft_atol(const char *str);
int		table_initialization(t_table *table, char **argv);
void	*safe_malloc(size_t size);
int	print_error_with_return(const char *error);
int	safe_mutex_handle(pthread_mutex_t *mutex, t_operation_code opcode);
int	safe_thread_handle(pthread_t *thread, void *(*foo)(void *), void *data, t_operation_code opcode);
int		start_philos_dinner_sim(t_table *table);
void	clean_table(t_table *table);


#endif
