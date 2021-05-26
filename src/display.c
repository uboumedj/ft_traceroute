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
	printf("traceroute to %s (%s), %d hops max, 60 byte packets\n", traceroute.user_requested_address, traceroute.address, traceroute.count);
}

/*
** function: display_sequence
** --------------------------
** called in the loop to display the current sequence's information after succesfully
** "pinging" the address. prints the sequence number, the packet's ttl and the calculated latency
*/

void	display_sequence(int received_bytes, t_reply reply, struct timeval start_timestamp, struct timeval end_timestamp)
{
	short		reply_ttl;
	double		time_elapsed;
	struct ip	*packet_content;

	packet_content = (struct ip *)reply.receive_buffer;
	reply_ttl = (short)packet_content->ip_ttl;
	time_elapsed = calculate_elapsed_time(start_timestamp, end_timestamp);
	if (!(traceroute.flags & F_FLAG))
	{
		if (ft_strcmp(traceroute.address, traceroute.user_requested_address))
		{
			printf("%lu bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2lf ms\n", received_bytes - sizeof(struct ip),
				traceroute.reversed_address, traceroute.address, traceroute.seq, reply_ttl, time_elapsed);
		}
		else
		{
			printf("%lu bytes from %s: icmp_seq=%d ttl=%d time=%.2lf ms\n", received_bytes - sizeof(struct ip),
				traceroute.address, traceroute.seq, reply_ttl, time_elapsed);
		}
	}
	else
	{
		ft_putchar('\b');
		fflush(stdout);
	}
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
	if (!(traceroute.flags & F_FLAG))
	{
		if (traceroute.flags & V_FLAG)
			printf("ft_traceroute: TTL set to %d seems insufficient.\n", traceroute.ttl);
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
	else
	{
		ft_putchar('\b');
		ft_putchar('E');
		fflush(stdout);
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