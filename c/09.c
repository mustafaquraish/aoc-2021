#include "common.h"

#define GRID_SIZE 128
int grid[GRID_SIZE * GRID_SIZE];
bool visited[GRID_SIZE * GRID_SIZE] = {0};
int sx = 0;
int sy = 0;

Vector lowest = {0};

#define IDX(x, y) ((x) + (y) * GRID_SIZE)

void parse() {
  FILE *f = fopen(INFILE("09.txt"), "r");
  char buf[2048];

  for (int i = 0; i < GRID_SIZE*GRID_SIZE; i++)
    grid[i] = 9;

  while (GETLINE(buf, f)) {
    sx = max(sx, strlen(buf));
    for (int x = 0; buf[x]; x++)
      grid[IDX(x+1,sy+1)] = buf[x] - '0';
    sy++;
  }

  fclose(f);
}

void dump() {
  for (int j = 1; j <= sy; j++) {
    for (int i = 1; i <= sx; i++)
      printf("%d", grid[IDX(i,j)]);
    printf("\n");
  }

  printf("Lowest:\n");
  for (u64 i = 0; i < lowest.size; i++)
    printf(" (%2llu, %2llu)\n", lowest.data[i] % GRID_SIZE, lowest.data[i] / GRID_SIZE);
}

void find_lowest() {
  for (int j = 1; j <= sy; j++)
    for (int i = 1; i <= sx; i++) {
      int val = grid[IDX(i,j)];
      if (grid[IDX(i+1,j)] > val && grid[IDX(i-1,j)] > val &&
          grid[IDX(i,j+1)] > val && grid[IDX(i,j-1)] > val)
        Vec_push(&lowest, i + j * GRID_SIZE);
    }
}

static int dirs[4] = {-1, 1, GRID_SIZE, -GRID_SIZE};

int dfs(int loc) {
  Vector stack = {0};
  Vec_push(&stack, loc);
  int count = 0;
  while (stack.size > 0) {
    int cur = Vec_pop(&stack);
    if (visited[cur])
      continue;
    visited[cur] = true;
    count++;
    for (int i = 0; i < 4; i++) {
      int n = cur + dirs[i];
      if (grid[n] > grid[cur] && grid[n] != 9)
        Vec_push(&stack, n);
    }
  }
  free(stack.data);
  return count;
}

void part1() {
  u64 res = 0;
  for (u64 i = 0; i < lowest.size; i++)
    res += grid[lowest.data[i]] + 1;
  printf("Part 1: %llu\n", res);
}

void part2() {
  Vector sizes = {0, 0, NULL};
  for (u64 i = 0; i < lowest.size; i++) {
    int size = dfs(lowest.data[i]);
    Vec_push(&sizes, size);
  }
  qsort(sizes.data, sizes.size, sizeof(u64), cmp_u64_dec);
  u64 result = sizes.data[0] * sizes.data[1] * sizes.data[2];
  printf("Part 2: %llu\n", result);
}

int main() {
  parse();
  find_lowest();
  part1();
  part2();
  return 0;
}