#include "universe.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};
// The universe will be abstracted as a struct called Universe.

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *u = (Universe *) malloc(sizeof(Universe));
    u->rows = rows;
    u->cols = cols;
    u->toroidal = toroidal;
    if (u) {
        u->grid = (bool **) calloc(rows, sizeof(bool *));
        for (uint32_t r = 0; r < rows; r += 1) {
            u->grid[r] = (bool *) calloc(cols, sizeof(bool));
        }
    }
    return u;
}
// allocating a matrix of uint32_ts

void uv_delete(Universe *u) {
    for (uint32_t r = 0; r < u->rows; r += 1) {
        free(u->grid[r]);
    }
    free(u->grid);
    free(u);
}

uint32_t uv_rows(Universe *u) {
    return (u->rows);
}

uint32_t uv_cols(Universe *u) {
    return (u->cols);
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = true;
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = false;
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r < u->rows && c < u->cols) {
        return (u->grid[r][c]);
    } else {
        return false;
    }
}

bool uv_populate(Universe *u, FILE *infile) {
    uint32_t r;
    uint32_t c;
    while (fscanf(infile, "%d%d", &r, &c) != EOF) {
        if ((r < 0 || c < 0) || (r >= u->rows || c >= u->cols)) {
            fclose(infile);
            return false;
        } else {
            u->grid[r][c] = true;
        }
    }
    return true;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    int64_t r1 = r;
    int64_t c1 = c;
    int count = 0;
    int64_t i, j;
    int64_t h, g;
    for (i = r1 - 1; i <= r1 + 1; i = i + 1) {
        for (j = c1 - 1; j <= c1 + 1; j = j + 1) {
            if (i == r1 && j == c1) {
                continue;
            } else if ((u->toroidal) && ((i < 0 || j < 0) || (i >= u->rows || j >= u->cols))) {
                h = (i + (u->rows)) % (u->rows);
                g = (j + (u->cols)) % (u->cols);
                if (u->grid[h][g] == 1) {
                    count = count + 1;
                }
            } else if ((!u->toroidal) && ((i < 0 || j < 0 || i >= u->rows || j >= u->cols))) {
                continue;
            } else if (u->grid[i][j] == 1) {
                count = count + 1;
            }
        }
    }
    return count;
}

void uv_print(struct Universe *u, FILE *outfile) {
    for (uint32_t i = 0; i < u->rows; i++) {
        for (uint32_t j = 0; j < u->cols; j++) {
            if (u->grid[i][j]) {
                fputc('o', outfile);
            } else {
                fputc('.', outfile);
            }
        }
        fputc('\n', outfile);
    }
}
