#include "../inc/ft_traceroute.h"

/*
** Functions related to the ping loop
*/

/*
** function: initialise_packet
** --------------------------
** initialises the different parts of the ICMP echo request packet to be sent to the
** destination address, performing byte swaps on necessary parts for endianness compliance
*/

void			initialise_packet(struct s_packet *packet, struct timeval current_time)
{
	ft_bzero(packet, sizeof(t_packet));
	packet->icmp_header.icmp_type = ICMP_ECHO;
	packet->icmp_header.icmp_code = 0;
	packet->icmp_header.icmp_seq = BSWAP16(traceroute.seq);
	packet->icmp_header.icmp_id = BSWAP16(traceroute.process_id);
	ft_memcpy(&packet->icmp_header.icmp_dun, &(current_time.tv_sec), sizeof(current_time.tv_sec));
	packet->icmp_header.icmp_cksum = checksum(packet, sizeof(*packet));
}

/*
** function: checksum
** ------------------
** performs a checksum of the unsigned short at the address and returns the result. Used
** during the packet creation, doing a 16-bit ones's complement of the one's complement sum
** of the ICMP message 
*/

unsigned short	checksum(void *address, int len)
{
	unsigned short	*buff;
	unsigned long	sum;

	buff = (unsigned short *)address;
	sum = 0;
	while (len > 1)
	{
		sum += *buff;
		buff++;
		len -= sizeof(unsigned short);
	}
	if (len)
		sum += *(unsigned char *)buff;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	return ((unsigned short)~sum);
}

/*
** function: send_packet
** ---------------------
** sends packets to the destination address and handles various errors encountered
** by the sendto function
*/

char			send_packet(t_packet *packet)
{
	ssize_t sent_bytes;

	sent_bytes = sendto(traceroute.socket_fd, packet, sizeof(*packet), 0,
		(struct sockaddr*)&traceroute.sockaddr, sizeof(traceroute.sockaddr));
	if (sent_bytes <= 0)
	{
		if (errno == ENETUNREACH)
			error_output(NO_CONNEXION_ERROR);
		else
			error_output(SENDTO_ERROR);
		return (ERROR_CODE);
	}
	return (SUCCESS_CODE);
}

/*
** function: receive_reply
** -----------------------
** stores the response inside the reply structure, and handles the various errors
** encountered by the recvfrom function
*/

char			receive_reply(t_reply *reply)
{
	ft_bzero(reply, sizeof(t_reply));
	reply->received_bytes = recvfrom(traceroute.socket_fd, reply->receive_buffer, sizeof(reply->receive_buffer), 0, NULL, NULL);
	if (reply->received_bytes > 0)
	{
		return (check_reply(reply));
	}
	else
	{
		if ((errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR))
		{
			if (traceroute.flags & V_FLAG)
				error_output(TIMEOUT_ERROR);
		}
		else
			error_output(RECV_ERROR);
		return (ERROR_CODE);
	}
	return (SUCCESS_CODE);
}

/*
** function: check_reply
** ---------------------
** reads the content of the response packet to verify that it is what was expected
** i.e. the right protocol, ID and seq number
*/

char			check_reply(t_reply *reply)
{
	struct ip	*packet_content;
	struct icmp *tmp_icmp_part;

	packet_content = (struct ip *)reply->receive_buffer;
	if (packet_content->ip_p != IPPROTO_ICMP)
	{
		return (ERROR_CODE);
	}
	reply->icmp = (struct icmp *)(reply->receive_buffer + (packet_content->ip_hl << 2));
	if (reply->icmp->icmp_type == 11 && reply->icmp->icmp_code == 0)
	{
		tmp_icmp_part = (struct icmp *)(reply->icmp + 1);
		if (BSWAP16(tmp_icmp_part->icmp_id) != traceroute.process_id || BSWAP16(tmp_icmp_part->icmp_seq) != traceroute.seq)
		{
			return (receive_reply(reply));
		}
		return (TTL_EXCEEDED_CODE);
	}
	else if (BSWAP16(reply->icmp->icmp_id) != traceroute.process_id || BSWAP16(reply->icmp->icmp_seq) != traceroute.seq)
	{
		return (receive_reply(reply));
	}
	return (SUCCESS_CODE);
}

/*
** function: main_loop
** -------------------
** main loop function that iterates on each "TTL" until it reaches the desired address or
** it goes above the number of hops specified by the user
*/

void			main_loop(void)
{
	save_current_time(&traceroute.starting_time);
	while(!traceroute.reached && traceroute.hops > 0)
	{
		loop_single_hop();
		traceroute.ttl++;
		if ((setsockopt(traceroute.socket_fd, IPPROTO_IP, IP_TTL, &(traceroute.ttl), sizeof(traceroute.ttl))) == -1)
			error_output_and_exit(SETSOCKOPT_ERROR);
		traceroute.hops--;
	}
}

/*
** function: loop_single_hop
** -------------------------
** secondary loop, called by the main_loop at each TTL. Sends [count] packets
** to the address reached when the current TTL is exceeded
*/

void			loop_single_hop(void)
{
	int					count;
	struct timeval		current_start_timestamp;
	struct timeval		current_ending_timestamp;
	t_packet			packet;
	t_reply				reply;
	char				check;
	char				address_is_displayed;

	count = traceroute.count;
	address_is_displayed = 0;
	printf("%2u ", traceroute.ttl);
	while (count > 0)
	{
		save_current_time(&current_start_timestamp);
		initialise_packet(&packet, current_start_timestamp);
		check = send_packet(&packet);
		if (check == SUCCESS_CODE)
		{
			check = receive_reply(&reply);
			save_current_time(&current_ending_timestamp);
			if (check == SUCCESS_CODE)
				traceroute.reached = 1;
			display_query(check, &address_is_displayed, reply, current_start_timestamp, current_ending_timestamp);
		}
		count--;
		traceroute.seq++;
		if (traceroute.interval)
			wait_interval(current_start_timestamp);
	}
	printf("\n");
}