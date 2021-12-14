#include "common.h"

int main() {
  u64 x1 = 0, y1 = 0;
  u64 x2 = 0, y2 = 0, aim = 0;

  FILE *f = fopen(INFILE("02.txt"), "r");
  char buf[2048];

  while (fgets(buf, 2048, f)) {
    int val;
    if (sscanf(buf, "forward %d", &val) == 1) {
      x1 += val, x2 += val;
      y2 += aim * val;
    } else if (sscanf(buf, "down %d", &val) == 1) {
      y1 += val;
      aim += val;
    } else if (sscanf(buf, "up %d", &val) == 1) {
      y1 -= val;
      aim -= val;
    }
  }

  printf("Part 1: %llu\n", x1 * y1);
  printf("Part 2: %llu\n", x2 * y2);
  fclose(f);
  return 0;
}