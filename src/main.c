#include "philo.h"

static inline int	is_valid_digit(int nbr, int is_first_char)
{
	if (is_first_char && nbr == '+')
		return (1);
	return ((nbr >= '0' && nbr <= '9'));
}

/*
	- counter : = 1 sent
*/
static inline int invalid_as_numbers(int argc, char *argv[], int arg_word_counter)
{
	int	char_index;

	while (arg_word_counter < argc)
	{
		char_index = 0;
		while (argv[arg_word_counter][char_index])  // Check until null terminator
		{
			if (!(is_valid_digit(argv[arg_word_counter][char_index], (char_index == 0))))
			{
				print_error("Error: Invalid Input Not A Number\n"
							GREEN"Valid Input is: ./philo [PhilosophersNumber] [TimeToDie] [TimeToEat] [TimeToSleep] [OPT:EatingTimes]"RESET);
				return (EXIT_FAILURE);
			}
			char_index++;
		}
		arg_word_counter++;
	}
	return (EXIT_SUCCESS);
}

static inline int invalid_input_handler(int argc, char *argv[])
{
	int	counter;

	counter = 1;
	if (!(validate_input(argc)))
	{
		print_error_exit("Wrong Input!\n"
						GREEN"Valid Input is: ./philo [PhilosophersNumber] [TimeToDie] [TimeToEat] [TimeToSleep] [OPT:EatingTimes]"RESET);
		return (EXIT_FAILURE);
	}
	if (invalid_as_numbers(argc, argv , counter))
		return (EXIT_FAILURE);

	return (EXIT_SUCCESS);
}

int	main (int argc, char *argv[])
{
	if (invalid_input_handler(argc, argv))
		return (EXIT_FAILURE);
	start_program(argv);

	return (EXIT_SUCCESS);
}
