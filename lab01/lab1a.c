/*
 * lab1a.c
 *
 *  Created on:
 *      Author:
 */

/* include helper functions for game */
#include "lifegame.h"

/* add whatever other includes here */

/* number of generations to evolve the world */
#define NUM_GENERATIONS 50

/* functions to implement */

/* this function should set the state of all
   the cells in the next generation and call
   finalize_evolution() to update the current
   state of the world to the next generation */
void next_generation(void);

/* this function should return the state of the cell
   at (x,y) in the next generation, according to the
   rules of Conway's Game of Life (see handout) */
int get_next_state(int x, int y);

/* this function should calculate the number of alive
   neighbors of the cell at (x,y) */
int num_neighbors(int x, int y);

int main(void)
{
	int n;

    //initialize_world();
    initialize_world_from_file("sship.txt");

	for (n = 0; n < NUM_GENERATIONS; n++)
		next_generation();

    //output_world();
    save_world_to_file("sshiptest.txt");

	return 0;
}

void next_generation(void) {
	/* TODO: for every cell, set the state in the next
	   generation according to the Game of Life rules

	   Hint: use get_next_state(x,y) */
    int w = get_world_width();
    int h = get_world_height();
    int x, y;
    for (x = 0; x < w; x++) {
        for (y = 0; y < h; y++) {
            set_cell_state(x, y, get_next_state(x, y));
        }
    }

	finalize_evolution(); /* called at end to finalize */
}

int get_next_state(int x, int y) {
	/* TODO: for the specified cell, compute the state in
	   the next generation using the rules

	   Use num_neighbors(x,y) to compute the number of live
	   neighbors */
    int nn = num_neighbors(x, y);
    int cs = get_cell_state(x, y);
    if (cs == ALIVE) {
        if (nn < 2 || nn > 3)
            return DEAD;
    } else {
        if (nn == 3)
            return ALIVE;
    }
    return cs;
}

int num_neighbors(int x, int y) {
	/* TODO: for the specified cell, return the number of
	   neighbors that are ALIVE

	   Use get_cell_state(x,y) */
    int h = get_world_height(), w = get_world_width();
    int xlo = x - 1 < 0 ? 0 : x - 1;
    int xhi = x + 1 > w - 1 ? w - 1 : x + 1;
    int ylo = y - 1 < 0 ? 0 : y - 1;
    int yhi = y + 1 > h - 1 ? h - 1 : y + 1;
    int xt, yt;
    int nn = 0;
    for (xt = xlo; xt <= xhi; xt++) {
        for (yt = ylo; yt <= yhi; yt++) {
            if (get_cell_state(xt, yt) == ALIVE) nn++;
        }
    }

    if (get_cell_state(x, y) == ALIVE) nn--;

    return nn;
}
