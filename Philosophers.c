#include "Philosophers.h"

int	start_program(char *argv[])
{
	t_table	table;

	table_initialization(&table, argv);
	start_philos_dinner_sim(&table);
	clean_table(&table);

	reutrn (EXIT_SUCCESS);
}
