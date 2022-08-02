#include "tetris.h"
#include "ttrs_field.h"
#include "ttrs_keyhook.h"
#include "ttrs_mino.h"
#include "ttrs_print.h"
#include "ttrs_texture.h"
#include "ttrs_time.h"

#include <limits.h>
#include <ncurses.h>
#include <stdio.h>
#include <time.h>

#include "ttrs_print.h"

#define SCORE_UNIT 100

#define DOWN_KEY 's'
#define LEFT_KEY 'a'
#define RIGHT_KEY 'd'
#define ROTATE_KEY 'w'

#define GAME_OVER "\nGame over!"

t_keyhook_func g_keyhooks[UCHAR_MAX] = {};

static void end_tetris(const t_tetris *tetris)
{
	print_field(tetris->field_ptr, printf);
	puts(GAME_OVER);
	print_score(tetris->score);
}

static void assign_keyhook_funcp()
{
	g_keyhooks[DOWN_KEY] = try_move_down;
	g_keyhooks[LEFT_KEY] = try_move_left;
	g_keyhooks[RIGHT_KEY] = try_move_right;
	g_keyhooks[ROTATE_KEY] = try_move_rotate;
}

static t_tetris create_tetris()
{
	t_tetris tetris = {
		.field_ptr = {},
		.score = 0,
		.is_alive = true,
	};
	tetris.time.interval = INIT_INTERVAL_TIME,
	clock_gettime(CLOCK_MONOTONIC, &tetris.time.prev_fall_time);
	tetris.time.decrease_time = INIT_DECREASE_TIME;
	return tetris;
}

static void start_tetris(t_tetris *tetris)
{
	t_mino mino = generate_random_mino();

	while (tetris->is_alive) {
		handle_key_input(tetris, &mino);
		if (is_time_to_fall(&tetris->time)) {
			bool is_reached_ground = try_move_down(tetris, &mino) == false;
			if (is_reached_ground) {
				place_mino_on_field(tetris->field_ptr, &mino);
				int num_of_erased = erase_filled_lines(tetris->field_ptr);
				update_fall_speed(&tetris->time, num_of_erased);
				mino = generate_random_mino();
				tetris->is_alive = can_place_in_field(tetris->field_ptr, &mino.mino_type, mino.pos);
				tetris->score += SCORE_UNIT * num_of_erased;
			}
			update_screen(tetris, &mino);
			clock_gettime(CLOCK_MONOTONIC, &tetris->time.prev_fall_time);
		}
	}
}

static void init_tetris()
{
	srand(time(NULL));
	assign_keyhook_funcp();
}

static void run_tetris(t_tetris *tetris)
{
	initscr();
	timeout(1);
	start_tetris(tetris);
	endwin();
}

int main()
{
	t_tetris tetris = create_tetris();
	init_tetris();
	run_tetris(&tetris);
	end_tetris(&tetris);
}