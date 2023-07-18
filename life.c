#include "universe.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DELAY  50000
#define OPTION "tsn:i:o:"

int main(int argc, char **argv) {
    uint32_t rows, cols;
    int generations = 0;
    FILE *input = stdin;
    FILE *output = stdout;
    bool ncurses = true;
    bool toroidal = false;
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTION)) != -1) {
        switch (opt) {
        case 't': toroidal = true; break;
        case 's': ncurses = false; break;
        case 'n': generations = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'i': input = fopen(optarg, "r"); break; // fopen
        case 'o': output = fopen(optarg, "w"); break;
        }
    }
    if (generations < 0) {
        printf("invalid generation\n");
        return 1;
    }

    if (input == NULL) {
        printf("file is invalid");
        return 1;
    }
    if (fscanf(input, "%d%d", &rows, &cols) != 2) {
        printf("Invalid format\n");
        return 1;
    }

    Universe *A = uv_create(rows, cols, toroidal);
    Universe *B = uv_create(rows, cols, toroidal);
    //uv_populate(A, input);
    if (!uv_populate(A, input)) {
        printf("point is invalid\n");
        return 1;
    }
    fclose(input);
    initscr();
    curs_set(FALSE);
    while (generations) {

        // ncurses.
        if (ncurses) {
            clear();
            for (uint32_t i = 0; i < rows; i++) {
                for (uint32_t j = 0; j < cols; j++) {
                    if (uv_get_cell(A, i, j)) {
                        printw("o");
                    } else {
                        printw(".");
                    }
                }
                printw("\n");
            }
            refresh();
            usleep(DELAY);
        }
        //end ncurses

        //Perform one generation.
        for (uint32_t r = 0; r < rows; r++) {
            for (uint32_t c = 0; c < cols; c++) {
                int count = uv_census(A, r, c);
                bool state = uv_get_cell(A, r, c);
                if (state) { //Any live cell with two or three live neighbors survives.
                    if ((count == 2) || (count == 3)) {
                        uv_live_cell(B, r, c);
                    } else {
                        uv_dead_cell(B, r, c);
                    }
                } else { // Any dead cell with exactly three live neighbors becomes a live cell.
                    if (count == 3) {
                        uv_live_cell(B, r, c);
                    } else {
                        uv_dead_cell(B, r, c);
                    }
                }
            }
        }

        // swap pointer a->b, b->a
        Universe *temp = A;
        A = B;
        B = temp;

        // do the next generation
        generations = generations - 1;
    }
    endwin();
    uv_print(A, output);
    fclose(output);
    uv_delete(A);
    uv_delete(B);
    return 0;
}
