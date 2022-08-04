#include <limits.h>
#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

#include "tetris.h"
#include "ttrs_action.h"
#include "ttrs_matrix.h"
#include "ttrs_mino.h"
#include "ttrs_print.h"

t_keyhook_func g_keyhooks[UCHAR_MAX] = {};

#define MINO_DOWN(pos) pos.row + 1, pos.col
#define MINO_LEFT(pos) pos.row, pos.col - 1
#define MINO_RIGHT(pos) pos.row, pos.col + 1
#define MINO_POS(pos) pos.row, pos.col

void assign_keyhook_funcp()
{
	g_keyhooks[MINO_DOWN_KEY] = try_drop;
	g_keyhooks[LEFT_KEY] = try_left;
	g_keyhooks[RIGHT_KEY] = try_right;
	g_keyhooks[SPIN_KEY] = try_spin;
	g_keyhooks[SPACE_KEY] = hard_drop;
}

t_status try_drop(t_tetris *tetris, t_mino *mino)
{
	(void)tetris;
	if (can_place_on_matrix(tetris->matrix, &mino->mino_type, MINO_DOWN(mino->pos))) {
		mino->pos.row += 1;
		tetris->is_moved = true;
	} else {
		return TETRIS_LOCK_DOWN;
	}
	return TETRIS_PLAY;
}

t_status try_left(t_tetris *tetris, t_mino *mino)
{
	(void)tetris;
	if (can_place_on_matrix(tetris->matrix, &mino->mino_type, MINO_LEFT(mino->pos))) {
		mino->pos.col -= 1;
		tetris->is_moved = true;
	}
	return TETRIS_PLAY;
}

t_status try_right(t_tetris *tetris, t_mino *mino)
{
	(void)tetris;
	if (can_place_on_matrix(tetris->matrix, &mino->mino_type, MINO_RIGHT(mino->pos))) {
		mino->pos.col += 1;
		tetris->is_moved = true;
	}
	return TETRIS_PLAY;
}

t_status try_spin(t_tetris *tetris, t_mino *mino)
{
	t_mino spined = *mino;
	spin_right(&spined.mino_type);
	if (can_place_on_matrix(tetris->matrix, &spined.mino_type, MINO_POS(spined.pos))) {
		*mino = spined;
		tetris->is_moved = true;
	}
	return TETRIS_PLAY;
}

t_status hard_drop(t_tetris *tetris, t_mino *mino)
{
	while (can_place_on_matrix(tetris->matrix, &mino->mino_type, MINO_DOWN(mino->pos))) {
		mino->pos.row += 1;
	}
	tetris->is_moved = true;
	return TETRIS_LOCK_DOWN;
}

t_status create_new_mino(t_matrix matrix, t_mino *mino)
{
	*mino = generate_random_mino();
	if (!can_place_on_matrix(matrix, &mino->mino_type, MINO_POS(mino->pos))) {
		return TETRIS_GAME_OVER;
	}
	return TETRIS_PLAY;
}

t_status handle_key_input(t_tetris *tetris, t_mino *mino)
{
	int c = getch();
	if (c == ERR) {
		return TETRIS_PLAY;
	}
	refresh_screen(tetris, mino);
	if (g_keyhooks[c]) {
		return g_keyhooks[c](tetris, mino);
	}
	return TETRIS_PLAY;
}
