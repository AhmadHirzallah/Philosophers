/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atol.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahirzall <ahirzall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:51:55 by ahirzall          #+#    #+#             */
/*   Updated: 2025/07/29 01:17:02 by ahirzall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Checks if character is a whitespace character
*/
static int	is_whitespace(char character)
{
	return (character == ' ' || (character >= 9 && character <= 13));
}

/*
** Skips whitespace characters and returns true if string continues
*/
static int	skip_leading_whitespace(const char *str, int *index)
{
	while (is_whitespace(str[*index]))
		(*index)++;
	return (str[*index] != '\0');
}

/*
** Handles sign character and updates sign value and index
*/
static void	handle_sign_character(const char *str, int *index, long *sign)
{
	if (str[*index] == '-')
	{
		*sign = -1;
		(*index)++;
	}
	else if (str[*index] == '+')
	{
		(*index)++;
	}
}

/*
** Converts numeric characters to long integer
*/
static int	convert_digits_to_number(const char *str, int *index, long *result)
{
	*result = 0;
	if (str[*index] < '0' || str[*index] > '9')
		return (0);
	while (str[*index] >= '0' && str[*index] <= '9')
	{
		*result = *result * 10 + (str[*index] - '0');
		(*index)++;
	}
	return (1);
}

/*
** Converts string to long integer (custom implementation of atol)
*/
long	ft_atol(const char *str)
{
	int		char_index;
	long	result;
	long	sign;

	sign = 1;
	char_index = 0;
	if (!skip_leading_whitespace(str, &char_index))
		return (0);
	if (str[char_index] == '+' || str[char_index] == '-')
		handle_sign_character(str, &char_index, &sign);
	if (!convert_digits_to_number(str, &char_index, &result))
		return (0);
	return (result * sign);
}
