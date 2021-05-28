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
	printf("traceroute to %s (%s), %d hops max, 60 byte packets\n", traceroute.user_requested_address,
		traceroute.address, traceroute.hops);
}

/*
** function: display_hop_start
** ---------------------------
** called in the loop to display the information about beginning of a hop (number, router name and IP)
*/

void	display_full_hop(int check, char *address_is_displayed, t_reply reply, struct timeval start, struct timeval end)
{
	if (check != ERROR_CODE)
	{
		if (!*address_is_displayed)
		{
			*address_is_displayed = 1;
			display_hop_address(reply);
		}
		traceroute.received_packets++;
		display_time(start, end);
	}
	else
	{
		printf(" *");
		traceroute.error_packets++;
	}
}

/*
** function: display_hop_address
** -----------------------------
** called in the loop to display the information about beginning of a hop (number, router name and IP)
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
		tmp_sockaddr.sin_addr = packet_content->ip_src;
		tmp_sockaddr.sin_family = AF_INET;
		tmp_sockaddr.sin_port = 0;
		if (getnameinfo((struct sockaddr *)&tmp_sockaddr, sizeof(struct sockaddr_in),
			hostname, sizeof(hostname), NULL, 0, NI_NAMEREQD) >= 0)
		{
			printf(" %s (%s)", hostname, ip);
		}
		else
		{
			printf(" %s (%s)", ip, ip);
		}
	}
}

/*
** function: display_time
** ----------------------
** called in the loop to display the information about beginning of a hop (number, router name and IP)
*/

void	display_time(struct timeval start, struct timeval end)
{
	double	elapsed_time;

	elapsed_time = calculate_elapsed_time(start, end);
	printf("  %.3lf ms", elapsed_time);
}

/*
** function: display_exceeded_sequence
** -----------------------------------
** displays information on a failed seq due to an exceeded TTL, i.e. the address where the packet
** stopped and its seq
*/

void	display_exceeded_sequence(t_reply reply)
{
	struct ip			*packet_content;
	char				ip[INET_ADDRSTRLEN];
	char				hostname[NI_MAXHOST];
	struct sockaddr_in	tmp_sockaddr;

	packet_content = (struct ip *)reply.receive_buffer;
	inet_ntop(AF_INET, &(packet_content->ip_src), ip, INET_ADDRSTRLEN);
	tmp_sockaddr.sin_addr = packet_content->ip_src;
	tmp_sockaddr.sin_family = AF_INET;
	tmp_sockaddr.sin_port = 0;
	if (getnameinfo((struct sockaddr *)&tmp_sockaddr, sizeof(struct sockaddr_in),
		hostname, sizeof(hostname), NULL, 0, NI_NAMEREQD) >= 0)
	{
		printf("From %s (%s): icmp_seq=%d Time to live exceeded\n", hostname, ip, traceroute.seq);
	}
	else
	{
		printf("From %s: icmp_seq=%d Time to live exceeded\n", ip, traceroute.seq);
	}
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