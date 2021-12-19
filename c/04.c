#include "common.h"

#define BOARD_SIZE 5
#define MAX_BOARDS 512
int boards[MAX_BOARDS][BOARD_SIZE][BOARD_SIZE] = {0};
int num_boards = 0;

Vector values = {0};

void parse() {
  FILE *f = fopen(INFILE("04.txt"), "r");
  char buf[2048], *line, *tmp;

  values.size = 0;
  fgets(buf, 2048, f);
  for (line = buf; (tmp = strsep(&line, ","));) {
    Vec_push(&values, atoi(tmp));
  }

  num_boards = 0;
  while (fgets(buf, 2048, f)) {
    for (int row = 0; row < BOARD_SIZE; row++) {
      fgets(buf, 2048, f);
      line = buf;
      for (int col = 0; col < BOARD_SIZE; col++)
        boards[num_boards][row][col] = strtol(line, &line, 10);
    }
    num_boards++;
  }

  fclose(f);
}

void dump_boards() {
  for (int i = 0; i < num_boards; i++) {
    for (int row = 0; row < BOARD_SIZE; row++) {
      for (int col = 0; col < BOARD_SIZE; col++) {
        printf("%2d ", boards[i][row][col]);
      }
      printf("\n");
    }
    printf("\n\n");
  }
}

int board_sum(int index) {
  int sum = 0;
  for (int row = 0; row < BOARD_SIZE; row++)
    for (int col = 0; col < BOARD_SIZE; col++)
      if (boards[index][row][col] > 0)
        sum += boards[index][row][col];
  return sum;
}

void mark(int index, int value) {
  for (int row = 0; row < BOARD_SIZE; row++)
    for (int col = 0; col < BOARD_SIZE; col++)
      if (boards[index][row][col] == value)
        boards[index][row][col] = -1;
}

bool did_win(int index) {
  for (int i = 0, j = 0; i < BOARD_SIZE; i++) {
    for (j = 0; j < BOARD_SIZE && boards[index][i][j] < 0; j++)
      ;
    if (j >= BOARD_SIZE)
      return true;
    for (j = 0; j < BOARD_SIZE && boards[index][j][i] < 0; j++)
      ;
    if (j >= BOARD_SIZE)
      return true;
  }
  return false;
}

void solve() {
  int num_won = 0;
  for (u64 n = 0; n < values.size; n++) {
    int value = values.data[n];
    for (int i = 0; i < num_boards; i++) {
      if (did_win(i))
        continue;
      mark(i, value);
      if (!did_win(i))
        continue;
      num_won++;
      if (num_won == 1) {
        printf("Part 1: %d\n", board_sum(i) * value);
      } else if (num_won == num_boards) {
        printf("Part 2: %d\n", board_sum(i) * value);
        return;
      }
    }
  }
}

int main() {
  parse();
  solve();
  return 0;
}