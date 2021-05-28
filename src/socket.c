#include "../inc/ft_traceroute.h"

/*
** Functions that handle socket creation and manipulation
*/

/*
** function: create_socket
** -----------------------
** opens a new socket and saves its fd to [traceroute.socket_fd]
*/

void	create_socket(void)
{
	int				socket_fd;
	struct timeval	timeout;

	timeout.tv_sec = 0;
	timeout.tv_usec = 500000;
	if ((socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
		error_output_and_exit(SOCKET_ERROR);
	if ((setsockopt(socket_fd, IPPROTO_IP, IP_TTL, &(traceroute.ttl), sizeof(traceroute.ttl))) == -1)
		error_output_and_exit(SETSOCKOPT_ERROR);
	if ((setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (const void *)&timeout, sizeof(timeout))) == -1)
		error_output_and_exit(SETSOCKOPT_ERROR);
	traceroute.socket_fd = socket_fd;
}