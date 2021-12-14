#include "common.h"

Vector values = {0};

void parse() {
  FILE *f = fopen(INFILE("01.txt"), "r");
  int val;
  while (fscanf(f, "%d ", &val) == 1)
    Vec_push(&values, val);
  fclose(f);
}

void part1() {
  int count = 0;
  for (int i = 1; i < values.size; i++) {
    int a = Vec_get(&values, i - 1);
    int b = Vec_get(&values, i);
    count += (a < b);
  }
  printf("Part 1: %d\n", count);
}

void part2() {
  int count = 0;
  for (int i = 3; i < values.size; i++) {
    int a = Vec_get(&values, i - 3);
    int b = Vec_get(&values, i - 2);
    int c = Vec_get(&values, i - 1);
    int d = Vec_get(&values, i);
    count += (a + b + c < b + c + d);
  }
  printf("Part 1: %d\n", count);
}

int main() {
  parse();
  part1();
  part2();
  return 0;
}