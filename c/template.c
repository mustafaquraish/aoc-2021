#include "common.h"

void parse() {
  FILE *f = fopen(INFILE("00.txt"), "r");
  char buf[2048];

  while (GETLINE(buf, f)) {
    char *line = buf;
  }

  fclose(f);
}

void part1() {
  int result = 0;
  printf("Part 1: %d\n", result);
}

void part2() {
  int result = 0;
  printf("Part 1: %d\n", result);
}

int main() {
  parse();
  part1();
  part2();
  return 0;
}