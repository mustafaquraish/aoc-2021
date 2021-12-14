#include "common.h"

void parse_line(char *line, int entries[10], int outs[4]) {
  for (int i = 0; i < 10; i++) {
    char *word = strsep(&line, " ");
    for (char *c = word; *c; c++)
      entries[i] |= 1 << (*c - 'a');
  }
  line += 2;
  for (int i = 0; i < 4; i++) {
    char *word = strsep(&line, " ");
    for (char *c = word; *c; c++)
      outs[i] |= 1 << (*c - 'a');
  }
}

char num_bits(char x) {
  char count = 0;
  for (; x; x >>= 1)
    count += x & 1;
  return count;
}

char find_where(int entries[10], char len, char cmp, char match_bits, char na,
                char nb) {
  for (int i = 0; i < 10; i++) {
    if (num_bits(entries[i]) == len &&
        num_bits(entries[i] & cmp) == match_bits && entries[i] != na &&
        entries[i] != nb) {
      return entries[i];
    }
  }
  return 0;
}

void solve_digits(int entries[10], int outs[4], int solns[4]) {
  char D[10];
  D[1] = find_where(entries, 2, 0, 0, 0, 0);
  D[4] = find_where(entries, 4, 0, 0, 0, 0);
  D[7] = find_where(entries, 3, 0, 0, 0, 0);
  D[8] = find_where(entries, 7, 0, 0, 0, 0);
  D[3] = find_where(entries, 5, D[7], 3, 0, 0);
  D[9] = find_where(entries, 6, D[3], 5, 0, 0);
  D[6] = find_where(entries, 6, D[1], 1, 0, 0);
  D[5] = find_where(entries, 5, D[6], 5, 0, 0);
  D[0] = find_where(entries, 6, 0, 0, D[6], D[9]);
  D[2] = find_where(entries, 5, 0, 0, D[5], D[3]);

  for (int j = 0; j < 4; ++j)
    for (int i = 0; i < 10; ++i)
      if (outs[j] == D[i]) {
        solns[j] = i;
        break;
      }
}

void part1(int outs[4], int *res) {
  for (int i = 0; i < 4; i++) {
    char num = num_bits(outs[i]);
    *res += (num == 2 || num == 3 || num == 4 || num == 7);
  }
}

void part2(int entries[10], int outs[4], int *res) {
  long val;
  int solns[4];
  solve_digits(entries, outs, solns);
  for (int i = val = 0; i < 4; ++i)
    val = val * 10 + solns[i];
  *res += val;
}

int main() {
  FILE *f = fopen(INFILE("08.txt"), "r");
  char buffer[256];

  int res1 = 0, res2 = 0;
  while (fgets(buffer, 256, f)) {
    int entries[10] = {0}, outs[4] = {0};
    parse_line(buffer, entries, outs);
    part1(outs, &res1);
    part2(entries, outs, &res2);
  }

  printf("Part 1: %d\n", res1);
  printf("Part 2: %d\n", res2);
}