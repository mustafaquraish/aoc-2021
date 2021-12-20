#include "common.h"

#define MAX_GRID_SIZE 512
int grid[MAX_GRID_SIZE][MAX_GRID_SIZE];
int buffer[MAX_GRID_SIZE][MAX_GRID_SIZE];
int grid_size;
int mapping[1024] = {0};
int default_val = 0;

void dump_grid() {
  for (int i = 0; i < grid_size; i++) {
    for (int j = 0; j < grid_size; j++)
      printf("%c ", grid[i][j] ? '#' : '.');
    printf("\n");
  }
}

void parse() {
  FILE *f = fopen(INFILE("20.txt"), "r");
  char buf[2048];

  GETLINE(buf, f);
  int i = 0;
  for (char *line = buf; *line; line++)
    mapping[i++] = (*line) == '#';

  GETLINE(buf, f); // Skip newline

  while (GETLINE(buf, f)) {
    int i = 0;
    for (char *line = buf; *line; line++)
      grid[grid_size][i++] = (*line) == '#';
    grid_size++;
  }

  fclose(f);
}

int pixel(int i, int j) {
  if (i < 0 || i >= grid_size || j < 0 || j >= grid_size)
    return default_val;
  return grid[i][j];
}

int mapping_index(int i, int j) {
  static const int NBS[9][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    { 0, -1}, { 0, 0}, { 0, 1},
    { 1, -1}, { 1, 0}, { 1, 1}
  };
  int k = 0;
  for (int n = 0; n < 9; n++)
    k = k << 1 | pixel(i + NBS[n][0], j + NBS[n][1]);
  return k;
}

void enhance() {
  for (int i = -1; i <= grid_size; i++)
    for (int j = -1; j <= grid_size; j++)
      buffer[i+1][j+1] = mapping[mapping_index(i, j)];
  grid_size += 2;
  memcpy(grid, buffer, sizeof(grid));

  // Careful! in this case the whole infinite grid outside changes values
  // every iteration. Assumes the pair is always (0,1) or (1,0) only.
  if (mapping[0] == 1) // (and mapping[-1] == 0)
    default_val = !default_val;
}

int sum_on() {
  int sum = 0;
  for (int i = 0; i < grid_size; i++)
    for (int j = 0; j < grid_size; j++)
      sum += grid[i][j];
  return sum;
}

int main() {
  parse();

  int i = 0;
  for (; i < 2; i++) enhance();
  printf("Part 1: %d\n",sum_on());

  for (; i < 50; i++) enhance();
  printf("Part 2: %d\n",sum_on());

  return 0;
}