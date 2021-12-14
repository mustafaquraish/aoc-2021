#include "common.h"

Vector values = {0};
int vmin = INT32_MAX;
int vmax = INT32_MIN;

char buf[8192];

void parse() {
  FILE *f = fopen(INFILE("07.txt"), "r");
  fgets(buf, 8192, f);

  char *line = buf, *tmp;
  while ((tmp = strsep(&line, ","))) {
    int val = atoi(tmp);
    Vec_push(&values, atoi(tmp));
    vmin = min(vmin, val);
    vmax = max(vmax, val);
  }

  fclose(f);
}

u64 solve(bool part1) {
  u64 best = INT64_MAX;
  for (int i = vmin; i <= vmax; i++) {
    u64 sum = 0;
    for (int j = 0; j < values.size; j++) {
      u64 val = values.data[j];
      u64 d = val > i ? val - i : i - val;
      sum += part1 ? d : d * (d + 1) / 2;
    }
    if (sum < best)
      best = sum;
  }
  return best;
}

int main() {
  parse();
  printf("Part 1: %llu\n", solve(true));
  printf("Part 2: %llu\n", solve(false));
  return 0;
}