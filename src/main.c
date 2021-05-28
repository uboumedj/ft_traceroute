#include "../inc/ft_traceroute.h"

/*
** function: initialise_parameters
** -------------------------------
** sets up the whole [traceroute] data structure for the program, parsing the arguments
** and storing them in their rightful places
*/

void	initialise_parameters(char **argv)
{
	traceroute.ttl = 1;
	traceroute.hops = 30;
	traceroute.interval = 1;
	traceroute.count = 3;
	traceroute.flags = parse_flags(argv);
	if (traceroute.flags & H_FLAG || traceroute.flags & BAD_FLAG)
	{
		error_output(USAGE);
		exit(1);
	}
	else
	{
		traceroute.process_id = getpid();
		traceroute.seq = 1;
		traceroute.sent_packets = 0;
		traceroute.received_packets = 0;
		traceroute.error_packets = 0;
		traceroute.socket_fd = -1;
		traceroute.reached = 0;
		get_address(&(traceroute.user_requested_address), argv);
		interpret_address(traceroute.user_requested_address);
	}
}

/*
** function: free_memory
** ---------------------
** frees allocated memory, as the name says it
*/

void	free_memory(void)
{
	if (traceroute.address)
		free(traceroute.address);
	if (traceroute.reversed_address)
		free(traceroute.reversed_address);
}

/*
** function: main
** --------------
** MAIN FUNCTION !!
*/

int		main(int argc, char **argv)
{
	if (argc == 1)
		error_output(USAGE);
	else
	{
		initialise_parameters(argv);
		if (traceroute.address)
			create_socket();
		if (traceroute.socket_fd != -1)
		{
			set_signals();
			display_header();
			main_loop();
		}
		free_memory();
	}
	return (0);
}