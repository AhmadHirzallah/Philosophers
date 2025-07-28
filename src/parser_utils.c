#include "philo.h"

void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		print_error("Error: Memory allocation failed");
		return (NULL);
	}
	return (ptr);
}
