#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

# define _GNU_SOURCE
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <netinet/ip_icmp.h>
# include <errno.h>
# include <math.h>

/*
** OPTION FLAG DEFINES
*/

# define BAD_FLAG			0b10000000
# define V_FLAG				0b00000001
# define H_FLAG				0b00000010
# define T_FLAG				0b00000100
# define M_FLAG				0b00001000
# define Q_FLAG				0b00010000

/*
** ERROR MESSAGE DEFINES
*/

# define USAGE				"Usage: ft_traceroute [-h help] [-m max hops] destination"
# define BAD_FLAG_ERROR		"ft_traceroute: invalid option -- '%c'\n"
# define BAD_TTL_ERROR		"ft_traceroute: can't set time to live: invalid argument"
# define BAD_COUNT_ERROR	"ft_traceroute: max hops cannot be more than 255"
# define BAD_QUERIES_ERROR	"ft_traceroute: number of probes per hop has to be between 1 and 10"
# define PERMISSION_ERROR	"ft_traceroute: socket: operation not permitted"
# define UNKNOWN_ADDR_ERROR	"ft_traceroute: %s: failure in name resolution\n"
# define SOCKET_ERROR		"ft_traceroute: socket: operation not permitted"
# define SETSOCKOPT_ERROR	"ft_traceroute: setsockopt: error while setting socket options"
# define TIMEOFDAY_ERROR	"ft_traceroute: gettimeofday: error while trying to access current time"
# define SENDTO_ERROR		"ft_traceroute: sendto: error while trying to send packet"
# define NO_CONNEXION_ERROR	"ft_traceroute: sendto: the network is unreachable"
# define RECV_ERROR			"ft_traceroute: recvmsg: error while trying to access ICMP reply"
# define REPLY_ERROR		"ft_traceroute: reply is not what was expected (not ICMP)"
# define TIMEOUT_ERROR		"ft_traceroute: recvmsg: request timed out"

/*
** FUNCTION RETURN DEFINES
*/

# define TTL_EXCEEDED_CODE	2
# define ERROR_CODE			1
# define SUCCESS_CODE		0

/*
** BYTE SWAP MACROS (ENDIANNESS)
*/

# define BSWAP16(x)			((__uint16_t) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))
# define BSWAP32(x)			((((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >> 8) | (((x) & 0x0000ff00u) << 8) | (((x) & 0x000000ffu) << 24))

typedef struct				s_packet
{
	struct icmp				icmp_header;
	char					data_buffer[36];
}							t_packet;

typedef struct				s_reply
{
	int						received_bytes;
	struct icmp				*icmp;
	char					receive_buffer[84];
}							t_reply;

typedef struct				s_traceroute
{
	int						flags;
	double					interval;
	char					*user_requested_address;
	char					*address;
	char					*reversed_address;
	char					reached;
	int						ttl;
	char					hops;
	int						count;
	pid_t					process_id;
	int						socket_fd;
	int						seq;
	struct sockaddr_in		sockaddr;
	struct timeval			starting_time;
	struct timeval			ending_time;
}							t_traceroute;

/*
** GLOBAL VARIABLE
*/

t_traceroute				traceroute;

/*
** PROGRAM INITIALISING
*/

void						initialise_parameters(char **argv);
int							parse_flags(char **argv);
void						get_ttl(char **argv, int index);
void						get_hops(char **argv, int index);
void						get_queries(char **argv, int index);
void						handle_flood(int flags);
void						store_flags(int *flags, char **argv, int i);
void						get_address(char **dest, char **argv);
char						check_previous_argument(char **argv, int i);
void						interpret_address(char *input);
void						reverse_dns(void);
void						create_socket(void);

/*
** LOOP
*/

void						main_loop(void);
void						loop_single_hop(void);
void						initialise_packet(struct s_packet *packet, struct timeval current_time);
char						send_packet(t_packet *packet);
char						receive_reply(t_reply *reply);
char						check_reply(t_reply *replys);
void						wait_interval(struct timeval start);

/*
** HELPING FUNCTIONS
*/

unsigned short				checksum(void *address, int len);
void						save_current_time(struct timeval *destination);
void						free_memory(void);
double						calculate_elapsed_time(struct timeval start, struct timeval end);
void						set_signals(void);
void						handle_interrupt_signal(int signal);
int							ft_strcmp(const char *s1, const char *s2);
void						*ft_memset(void *ptr, int value, size_t len);
void						ft_bzero(void *s, size_t n);
void						*ft_memcpy(void *dest, const void *src, size_t len);
char						*ft_strdup(const char *s);
size_t						ft_strlen(const char *str);

/*
** DISPLAY FUNCTIONS
*/

void						display_header(void);
void						display_query(int check, char *address_is_displayed, t_reply reply, struct timeval start, struct timeval end);
void						display_hop_address(t_reply reply);
void						display_time(struct timeval start, struct timeval end);
void						error_output(char *message);
void						error_output_and_exit(char *message);

#endif
