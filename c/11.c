#include "common.h"

#define GRID_SIZE 10
int orig[GRID_SIZE][GRID_SIZE];
int grid[GRID_SIZE][GRID_SIZE];

void parse() {
  FILE *f = fopen(INFILE("11.txt"), "r");
  char buf[2048];

  int row = 0;
  while (GETLINE(buf, f)) {
    for (int col = 0; col < GRID_SIZE; col++) {
      orig[row][col] = buf[col] - '0';
    }
    row++;
  }

  fclose(f);
}

void dump() {
  for (int row = 0; row < GRID_SIZE; row++) {
    for (int col = 0; col < GRID_SIZE; col++)
      printf("%d", grid[row][col]);
    printf("\n");
  }
}

int step() {
  for (int row = 0; row < GRID_SIZE; row++)
    for (int col = 0; col < GRID_SIZE; col++)
      grid[row][col] += 1;
  int flashed = 0;
  bool changed = true;
  while (changed) {
    changed = false;
    for (int row = 0; row < GRID_SIZE; row++)
      for (int col = 0; col < GRID_SIZE; col++) {
        if (grid[row][col] <= 9) continue;
        grid[row][col] = 0;
        changed = true;
        flashed++;
        for (int r = row - 1; r <= row + 1; r++)
          for (int c = col - 1; c <= col + 1; c++)
            if (0 <= r && r < GRID_SIZE && 
                0 <= c && c < GRID_SIZE &&
                grid[r][c] > 0) {
              grid[r][c]++;
            }
      }
  }
  return flashed;
}

void part1() {
  memcpy(grid, orig, sizeof(grid));
  u64 result = 0;
  for (int i = 0; i < 100; i++) {
    result += step();
  }
  printf("Part 1: %llu\n", result);
}

void part2() {
  memcpy(grid, orig, sizeof(grid));
  int i;
  for (i = 1; step() != GRID_SIZE * GRID_SIZE; i++)
    ;
  printf("Part 2: %i\n", i);
}


int main() {
  parse();
  part1();
  part2();
  return 0;
}