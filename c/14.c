#include "common.h"

#define SIZE 26

u64 start[SIZE][SIZE] = {0};
u64 counts[SIZE][SIZE] = {0};
int mapping[SIZE][SIZE] = {0};
int first = 0;

void parse() {
  FILE *f = fopen(INFILE("14.txt"), "r");
  char buf[2048];
  
  memset(start, 0, sizeof(start));

  GETLINE(buf, f);
  int N = strlen(buf);
  for (int i = 0; i < N-1; i++) {
    int a = buf[i] - 'A';
    int b = buf[i+1] - 'A';
    start[a][b]++;
  }
  
  first = buf[0] - 'A';
  while (GETLINE(buf, f)) {
    char a, b, c;
    if (sscanf(buf, "%c%c -> %c", &a, &b, &c) == 3)
      mapping[a-'A'][b-'A'] = c-'A';
  }

  fclose(f);
}

void dump_counts() {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (counts[i][j] != 0l) {
        printf("%c%c: %llu, ", i+'A', j+'A', counts[i][j]);
      }
    }
  }
  printf("\n");
}

u64 get_proof() {
  u64 char_counts[SIZE] = {0};
  char_counts[first] = 1;
  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
      if (counts[i][j] > 0)
        char_counts[j] += counts[i][j];
  u64 mn = INT64_MAX;
  u64 mx = 0;
  for (int i = 0; i < SIZE; i++) {
    u64 c = char_counts[i];
    if (c == 0) continue;
    if (c < mn) mn = c;
    if (c > mx) mx = c;
  }
  return mx - mn;
}

u64 solve(int iters) {
  memcpy(counts, start, sizeof(start));
  for (int i = 0; i < iters; i++) {
    u64 tmp[SIZE][SIZE] = {0};
    for (int a = 0; a < SIZE; a++) {
      for (int b = 0; b < SIZE; b++) {
        if (counts[a][b] <= 0) continue;
        int c = mapping[a][b];
        tmp[a][c] += counts[a][b];
        tmp[c][b] += counts[a][b];
      }
    }
    memcpy(counts, tmp, sizeof(counts));
  }
  return get_proof();
}

int main() {
  parse();
  printf("Part 1: %llu\n", solve(10));
  printf("Part 2: %llu\n", solve(40));
  return 0;
}