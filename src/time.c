#include "../inc/ft_traceroute.h"

/*
** Functions directly handling time in the program
*/

/*
** function: wait_interval
** -----------------------
** used in the program's loop, if there's a -z flag, to ensure there's at least
** [traceroute.interval] seconds between each sent packet
*/

void			wait_interval(struct timeval start)
{
	struct timeval	current_time;
	struct timeval	goal_time;

	if (traceroute.interval)
	{
		current_time = start;
		goal_time.tv_sec = current_time.tv_sec + (long)traceroute.interval;
		goal_time.tv_usec = current_time.tv_usec + (long)((traceroute.interval - (long)traceroute.interval) * 1000000);
		while (timercmp(&current_time, &goal_time, <))
		{
			save_current_time(&current_time);
		}
	}
}

/*
** function: calculate_elapsed_time
** --------------------------------
** simply returns the time elapsed between the ICMP echo request being sent and
** the response being received
*/

double			calculate_elapsed_time(struct timeval start, struct timeval end)
{
	return (((double)((double)end.tv_sec - (double)start.tv_sec) * 1000) +
		(double)((double)end.tv_usec - (double)start.tv_usec) / 1000);
}

/*
** function: save_current_time
** ---------------------------
** saves the current time to the specified timeval structure. Basically saves a line of code
** in multiple locations :)
*/

void			save_current_time(struct timeval *destination)
{
	if (gettimeofday(destination, NULL) == -1)
		error_output_and_exit(TIMEOFDAY_ERROR);
}