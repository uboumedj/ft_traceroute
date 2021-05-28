#include "../inc/ft_traceroute.h"

/*
** Functions handling the display of the program's various outputs
*/

/*
** function: display_header
** ------------------------
** displays the ping function's header with the destination address' info
*/

void	display_header(void)
{
	printf("traceroute to %s (%s), %d hops max, %lu byte packets\n", traceroute.user_requested_address,
		traceroute.address, traceroute.hops, sizeof(t_packet) + 20);
}

/*
** function: display_query
** -----------------------
** displays the current query's information or '*' if the query encountered and error (usually a timeout)
*/

void	display_query(int check, char *address_is_displayed, t_reply reply, struct timeval start, struct timeval end)
{
	if (check != ERROR_CODE)
	{
		if (!*address_is_displayed)
		{
			*address_is_displayed = 1;
			display_hop_address(reply);
		}
		display_time(start, end);
	}
	else
		printf(" *");
	fflush(stdout);
}

/*
** function: display_hop_address
** -----------------------------
** called in the loop to display the current hop's router name and IP when it is found
*/

void	display_hop_address(t_reply reply)
{
	struct ip			*packet_content;
	char				ip[INET_ADDRSTRLEN];
	char				hostname[NI_MAXHOST];
	struct sockaddr_in	tmp_sockaddr;

	packet_content = (struct ip *)reply.receive_buffer;
	inet_ntop(AF_INET, &(packet_content->ip_src), ip, INET_ADDRSTRLEN);
	if (ft_strcmp(ip, "0.0.0.0"))
	{
		if (traceroute.flags & N_FLAG)
			printf(" %s", ip);
		else
		{
			tmp_sockaddr.sin_addr = packet_content->ip_src;
			tmp_sockaddr.sin_family = AF_INET;
			tmp_sockaddr.sin_port = 0;
			if (getnameinfo((struct sockaddr *)&tmp_sockaddr, sizeof(struct sockaddr_in),
				hostname, sizeof(hostname), NULL, 0, NI_NAMEREQD) >= 0)
			{
				printf(" %s (%s)", hostname, ip);
			}
			else
				printf(" %s (%s)", ip, ip);
		}
	}
}

/*
** function: display_time
** ----------------------
** displays the current query's round trip time
*/

void	display_time(struct timeval start, struct timeval end)
{
	double	elapsed_time;

	elapsed_time = calculate_elapsed_time(start, end);
	printf("  %.3lf ms", elapsed_time);
}

/*
** function: display_total_time
** ----------------------------
** displays the program's total time if verbose mode is activated
*/

void	display_total_time(void)
{
	double	elapsed_time;

	elapsed_time = calculate_elapsed_time(traceroute.starting_time, traceroute.ending_time);
	printf("Program finished tracing the route to %s in %.0lf ms\n", traceroute.address, elapsed_time);
}

/*
** function: error_output
** ----------------------
** prints the given message on the standard error output
*/

void	error_output(char *message)
{
	fprintf(stderr, "%s\n", message);
}

/*
** function: error_output_and_exit
** -------------------------------
** prints the given message on the standard error output, then frees all freeable
** memory and exits with an error code (1)
*/

void	error_output_and_exit(char *message)
{
	fprintf(stderr, "%s\n", message);
	free_memory();
	exit(1);
}