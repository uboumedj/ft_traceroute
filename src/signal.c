#include "../inc/ft_traceroute.h"

/*
** Signal handling
*/

/*
** function: set_signals
** ---------------------
** sets the required signal overrides for the program
*/

void	set_signals(void)
{
	signal(SIGINT, &handle_interrupt_signal);
}

/*
** function: handle_interrupt_signal
** ---------------------------------
** the override function for the SIGINT (CTRL-C) signal. Frees the memory before
** leaving
*/

void	handle_interrupt_signal(int signal)
{
	(void)signal;
	free_memory();
	exit(1);
}