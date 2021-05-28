#include "../inc/ft_traceroute.h"

/*
** Initial flags (options) handling
*/

/*
** function: parse_flags
** --------------------
** parses the program's arguments, stopping every time the first character of an argument
** is '-' (an option). It then handles that argument with store_flags
*/

int		parse_flags(char **argv)
{
	int i = 1;
	int flags = 0;

	while (argv[i])
	{
		if (argv[i][0] == '-')
			store_flags(&flags, argv, i);
		i++;
	}
	return (flags);
}

/*
** function: store_flags
** --------------------
** parses an argument, treating every character as a program option. If it recognises
** the option, activates the corresponding flag. If it doesn't, the program ends :(
*/

void	store_flags(int *flags, char **argv, int index)
{
	int i = 1;

	while (argv[index][i])
	{
		switch (argv[index][i])
		{
			case 'v':
				*flags |= V_FLAG;
				break;
			case 'h':
				*flags |= H_FLAG;
				break;
			case 'm':
				*flags |= M_FLAG;
				get_hops(argv, index);
				break;
			case 't':
				*flags |= T_FLAG;
				get_ttl(argv, index);
				break;
			case 'q':
				*flags |= Q_FLAG;
				get_queries(argv, index);
				break;
			default:
				*flags |= BAD_FLAG;
				fprintf(stderr, BAD_FLAG_ERROR, argv[index][i]);
				return;
		}
		i++;
	}
}

/*
** function: get_ttl
** -----------------
** parses the next argument after a -t option to get the TTL that the user inputs, and
** checks for errors
*/

void	get_ttl(char **argv, int index)
{
	int		ttl;

	if ((argv[index][1] == 't' && argv[index][2] == 0)
		&& (argv[index + 1] != NULL))
	{
		ttl = atoi(argv[index + 1]);
		if (ttl <= 255 && ttl > 0)
		{
			traceroute.ttl = ttl;
			return;
		}
	}
	error_output_and_exit(BAD_TTL_ERROR);
}

/*
** function: get_hops
** ------------------
** parses the next argument after a -m option to get the max hops that the user inputs,
** checking for errors along the way
*/

void	get_hops(char **argv, int index)
{
	long	count;

	if ((argv[index][1] == 'm' && argv[index][2] == 0)
		&& (argv[index + 1] != NULL))
	{
		count = atol(argv[index + 1]);
		if (count > 0 && count <= 255)
		{
			traceroute.hops = (int)count;
			return;
		}
	}
	error_output_and_exit(BAD_COUNT_ERROR);
}

/*
** function: get_queries
** ---------------------
** parses the next argument after a -q option to get the number of queries that the user inputs,
** checking for errors along the way
*/

void	get_queries(char **argv, int index)
{
	double	interval;

	if ((argv[index][1] == 'i' && argv[index][2] == 0)
		&& (argv[index + 1] != NULL))
	{
		interval = atof(argv[index + 1]);
		if (interval > 0 && interval <= 10)
		{
			traceroute.interval = interval;
			return;
		}
	}
	error_output_and_exit(BAD_QUERIES_ERROR);
}