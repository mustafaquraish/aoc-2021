#include "common.h"

#define BOARD_SIZE 1024
int board_1[BOARD_SIZE][BOARD_SIZE] = {0};
int board_2[BOARD_SIZE][BOARD_SIZE] = {0};

int board_result(int board[BOARD_SIZE][BOARD_SIZE]) {
  int count = 0;
  for (int i = 0; i < BOARD_SIZE; i++)
    for (int j = 0; j < BOARD_SIZE; j++)
      if (board[i][j] > 1)
        count++;
  return count;
}

void dump_board(int mx) {
  for (int i = 0; i < mx; i++) {
    for (int j = 0; j < mx; j++) {
      printf("%d", board_1[i][j]);
    }
    printf("\n");
  }
}

void solve() {
  FILE *f = fopen(INFILE("05.txt"), "r");
  char buf[2048];
  int x1, y1, x2, y2;

  while (fgets(buf, 2048, f)) {
    sscanf(buf, "%d,%d -> %d,%d", &x1, &y1, &x2, &y2);
    int dx = x2 - x1, dy = y2 - y1;
    for (int i = 0; i <= max(abs(dx), abs(dy)); i++) {
      int x = x1 + sign(dx) * i;
      int y = y1 + sign(dy) * i;
      if (x1 == x2 || y1 == y2)
        board_1[y][x] += 1;
      board_2[y][x] += 1;
    }
  }
  printf("Part 1: %d\n", board_result(board_1));
  printf("Part 2: %d\n", board_result(board_2));
  fclose(f);
}

int main() {
  solve();
  return 0;
}