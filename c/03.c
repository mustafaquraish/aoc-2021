#include "common.h"

#define MAX_VALS 1024
char values[MAX_VALS][64];
int num_values = 0;

bool removed[MAX_VALS];
int num_removed = 0;

void reset_removed() {
  memset(removed, 0, sizeof(removed));
  num_removed = 0;
}

void parse() {
  FILE *f = fopen(INFILE("03.txt"), "r");
  char buf[2048];

  num_values = 0;
  while (fgets(buf, 2048, f)) {
    if (buf[strlen(buf) - 1] == '\n')
      buf[strlen(buf) - 1] = '\0';
    strcpy(values[num_values], buf);
    num_values++;
  }
  reset_removed();
  fclose(f);
}

char common_bit(int col, char ifsame) {
  int on_count = 0, total_count = 0;
  for (int i = 0; i < num_values; i++) {
    if (removed[i])
      continue;
    total_count++;
    on_count += (values[i][col] == '1');
  }

  int off_count = total_count - on_count;
  if (on_count == off_count)
    return ifsame;
  else
    return on_count > off_count ? '1' : '0';
}

char other(char c) { return c == '0' ? '1' : '0'; }

void remove_all_with(int col, char bit) {
  for (int i = 0; i < num_values; i++) {
    if (removed[i])
      continue;
    if (values[i][col] == bit) {
      removed[i] = true;
      num_removed++;
    }
  }
}

int find_unremoved() {
  for (int i = 0; i < num_values; i++) {
    if (!removed[i])
      return i;
  }
  assert(false && "unreachable");
}

void part1() {
  int N = strlen(values[0]);
  int a = 0, b = 0;
  for (int cur_bit = 0; cur_bit < N; cur_bit++) {
    a = a * 2 + (common_bit(cur_bit, '0') == '1');
    b = b * 2 + (common_bit(cur_bit, '1') == '0');
  }
  printf("Part 1: %d\n", a * b);
}

void part2() {
  for (int cur_bit = 0; num_values - num_removed > 1; cur_bit++) {
    char bit = common_bit(cur_bit, '1');
    remove_all_with(cur_bit, other(bit));
  }
  int a = strtol(values[find_unremoved()], NULL, 2);
  ;

  reset_removed();
  for (int cur_bit = 0; num_values - num_removed > 1; cur_bit++) {
    char bit = other(common_bit(cur_bit, '1'));
    remove_all_with(cur_bit, other(bit));
  }
  int b = strtol(values[find_unremoved()], NULL, 2);
  ;
  printf("Part 2: %d\n", a * b);
}

int main() {
  parse();
  part1();
  part2();
  return 0;
}