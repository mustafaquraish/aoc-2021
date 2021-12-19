#include "common.h"

bool is_open(char c, u64 *kind) {
  switch (c) {
    case '(': *kind = 1; return true;
    case ')': *kind = 1; return false;
    case '[': *kind = 2; return true;
    case ']': *kind = 2; return false;
    case '{': *kind = 3; return true;
    case '}': *kind = 3; return false;
    case '<': *kind = 4; return true;
    case '>': *kind = 4; return false;
  }
  assert(false && "unreachable");
}

int score(char c) {
  switch (c) {
    case ')': return 3;
    case ']': return 57;
    case '}': return 1197;
    case '>': return 25137;
  }
  assert(false && "unreachable");
}

void solve(char *buf, u64 *res, Vector *counts) {
  Vector stack = {0};
  int N = strlen(buf);

  for (int i = 0; i < N; i++) {
    u64 kind;
    if (is_open(buf[i], &kind)) {
      Vec_push(&stack, kind);
    } else if (Vec_top(&stack) == kind) {
      Vec_pop(&stack);
    } else {
      (*res) += score(buf[i]);
      return;
    }
  }

  u64 tmp = 0;
  for (int i = stack.size-1; i >= 0; i--)
    tmp = tmp * 5 + stack.data[i];
  Vec_push(counts, tmp);
  free(stack.data);
}

int main() {
  FILE *f = fopen(INFILE("10.txt"), "r");
  char buf[2048];

  u64 res1 = 0;
  Vector counts2 = {0};
  while (GETLINE(buf, f))
    solve(buf, &res1, &counts2);
  fclose(f);

  printf("Part 1: %llu\n", res1);

  qsort(counts2.data, counts2.size, sizeof(u64), cmp_u64_inc);
  printf("Part 2: %llu\n", counts2.data[counts2.size / 2]);
  return 0;
}