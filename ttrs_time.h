#ifndef TTRS_TIME_H
#define TTRS_TIME_H

#include <stdbool.h>
#include <sys/time.h>
#include <time.h>

#define INIT_DECREASE_TIME 1000000
#define INIT_INTERVAL_TIME 400000000
#define DECREASE_UNIT_TIME 1000
#define INTERVAL_TIME_MIN 1000
#define SEC_TO_NSEC(sec) (sec * 1000000000)

typedef long t_nsec;
typedef long t_unix_time_nsec;

typedef struct
{
	t_nsec interval;
	t_unix_time_nsec next_drop_time;
	t_nsec decrease_time;
} t_tetris_time;

struct timespec get_current_time();

bool is_time_to_drop(t_tetris_time *time);
void update_drop_speed(t_tetris_time *time, int count);
void update_next_drop_time(t_tetris_time *time);
long get_current_nsec();
long get_current_usec();

#endif
