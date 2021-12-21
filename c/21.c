#include "common.h"

int o_A, o_B;

void parse() {
  FILE *f = fopen(INFILE("21.txt"), "r");
  int dummy;
  fscanf(f, " Player %d starting position: %d", &dummy, &o_A);
  fscanf(f, " Player %d starting position: %d", &dummy, &o_B);
  o_A--;
  o_B--;
  fclose(f);
}

void part1() {
  u64 a = o_A, b = o_B;
  u64 sa = 0, sb = 0;
  u64 C = 0;
  while (sa < 1000 && sb < 1000) {
    u64 dice = C%100 + (C+1)%100 + (C+2)%100 + 3;
    C += 3; 
    a = (a + dice) % 10;
    sa += a + 1;
    swapu64(&a, &b);
    swapu64(&sa, &sb); 
  }
  u64 res = minu64(sa, sb) * C;
  printf("Part 1: %llu\n", res);
}

i64 DP[10][10][25][25][2];

void num_wins(u64 a, u64 b, u64 sa, u64 sb, u64 *_wa, u64 *_wb) {
  if (DP[a][b][sa][sb][0] != -1) {
    *_wa = DP[a][b][sa][sb][0];
    *_wb = DP[a][b][sa][sb][1];
    return;
  }
  if (sa >= 21 || sb >= 21) {
    *_wa = sa > sb;
    *_wb = sb > sa;
    return;
  }

  u64 wa = 0, wb = 0;

  for (u64 i = 1; i <= 3; i++)
    for (u64 j = 1; j <= 3; j++)
      for (u64 k = 1; k <= 3; k++) {
        u64 ta = (a + i + j + k) % 10;
        u64 ra, rb;
        num_wins(b, ta, sb, sa + ta + 1, &rb, &ra);
        assert(wa + ra >= wa);
        assert(wb + rb >= wb);
        wa += ra;
        wb += rb;
      }

  DP[a][b][sa][sb][0] = wa;
  DP[a][b][sa][sb][1] = wb;
  *_wa = wa;
  *_wb = wb;
  return;
}

void part2() {
  memset(DP, -1, sizeof(DP));
  u64 wa, wb;
  num_wins(o_A, o_B, 0, 0, &wa, &wb);
  printf("Part 2: %llu\n", maxu64(wa, wb));
}

int main() {
  parse();
  part1();
  part2();
  return 0;
}