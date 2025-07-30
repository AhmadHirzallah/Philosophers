/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:51:57 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/30 07:26:42 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_valid_character(int character, int is_first_position)
{
	if (is_first_position && character == '+')
		return (1);
	return (character >= '0' && character <= '9');
}

static int	is_invalid_numeric_arguments(int argc, char *argv[])
{
	int	arg_index;
	int	char_index;

	arg_index = 1;
	while (arg_index < argc)
	{
		char_index = 0;
		while (argv[arg_index][char_index])
		{
			if (!(is_valid_character(argv[arg_index][char_index],
				(char_index == 0))))
			{
				print_error("Error: Invalid Input Not A Number\n"
					COLOR_SUCCESS"Valid Input is: ./philo [PhilosophersNumber] "
					"[TimeToDie] [TimeToEat] [TimeToSleep] [OPT:EatingTimes]"
					RESET);
				return (EXIT_FAILURE);
			}
			char_index++;
		}
		arg_index++;
	}
	return (EXIT_SUCCESS);
}

static int	is_invalid_program_input(int argc, char *argv[])
{
	if (!(validate_input(argc)))
	{
		print_error_exit("Wrong Input!\n"
			COLOR_SUCCESS"Valid Input is: ./philo [PhilosophersNumber] "
			"[TimeToDie] [TimeToEat] [TimeToSleep] [OPT:EatingTimes]"RESET);
		return (EXIT_FAILURE);
	}
	if (is_invalid_numeric_arguments(argc, argv))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	main(int argc, char *argv[])
{
	if (is_invalid_program_input(argc, argv))
		return (EXIT_FAILURE);
	start_program(argv);
	return (EXIT_SUCCESS);
}
