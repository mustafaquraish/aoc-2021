#include "common.h"

u64 counts[11] = {0};

void parse() {
  FILE *f = fopen(INFILE("06.txt"), "r");
  char buf[2048];
  fgets(buf, 2048, f);

  memset(counts, 0, sizeof(counts));
  char *line = buf, *tmp;
  while ((tmp = strsep(&line, ",")))
    counts[atoi(tmp)]++;

  fclose(f);
}

u64 solve(int iters) {
  parse();
  for (int i = 0; i < iters; i++) {
    counts[7] += counts[0];
    counts[9] += counts[0];
    memcpy(counts, counts + 1, 10 * sizeof(counts[0]));
  }
  u64 result = 0;
  for (int i = 0; i < 12; i++)
    result += counts[i];
  return result;
}

int main() {
  printf("Part 1: %llu\n", solve(80));
  printf("Part 2: %llu\n", solve(256));
  return 0;
}