#include "common.h"

#define MAX_GRID_SIZE 200
int grid[MAX_GRID_SIZE][MAX_GRID_SIZE][2] = {};
int SX = 0, SY = 0;

void dump_grid() {
  for (int i = 0; i < SY; i++) {
    for (int j = 0; j < SX; j++)
      printf("%c", (char)grid[i][j][0]);
    printf("\n");
  }
}

void parse() {
  FILE *f = fopen(INFILE("25.txt"), "r");
  char buf[2048];
  while (GETLINE(buf, f)) {
    for (SX = 0; buf[SX]; SX++)
      grid[SY][SX][0] = buf[SX];
    SY++;
  }
  fclose(f);
}

bool step(int count) {
  bool changed = false;

  int step_num = 2 * count;
  for (int i = 0; i < SY; ++i) {
    for (int j = 0; j < SX; ++j) {
      if (grid[i][j][0] != '>' || grid[i][j][1] == step_num)
        continue;
      int ni = i;
      int nj = (j + 1) % SX;
      if (grid[ni][nj][0] == '.' && grid[ni][nj][1] != step_num) {
        grid[i][j][0] = '.';
        grid[i][j][1] = step_num;
        grid[ni][nj][0] = '>';
        grid[ni][nj][1] = step_num;
        changed = true;
      }
    }
  }
  step_num = 2 * count + 1;
  for (int i = 0; i < SY; ++i) {
    for (int j = 0; j < SX; ++j) {
      if (grid[i][j][0] != 'v' || grid[i][j][1] == step_num)
        continue;
      int ni = (i + 1) % SY;
      int nj = j;
      if (grid[ni][nj][0] == '.' && grid[ni][nj][1] != step_num) {
        grid[i][j][0] = '.';
        grid[i][j][1] = step_num;
        grid[ni][nj][0] = 'v';
        grid[ni][nj][1] = step_num;
        changed = true;
      }
    }
  }
  return changed;
}

int main() {
  parse();
  int count;
  // for (int i = 0; i < 10; i++) {
  //   printf("Iteration %d\n", i);
  //   dump_grid();
  //   step(i+1);
  // }
  for (count = 1; step(count); count++)
    ;
  printf("Part 1: %d\n", count);
  return 0;
}