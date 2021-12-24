#include "common.h"

typedef struct Cube {
  i64 x1, x2, y1, y2, z1, z2;
  bool on;
} Cube;

#define CUBES_CAP 1024
Cube cubes[CUBES_CAP];
int cubes_len = 0;

void parse() {
  i64 x1, y1, z1, x2, y2, z2;
  char mode[5];
  FILE *f = fopen(INFILE("22.txt"), "r");
  while (fscanf(f, " %s x=%lld..%lld,y=%lld..%lld,z=%lld..%lld", 
                mode, &x1, &x2, &y1, &y2, &z1, &z2) != EOF) 
    cubes[cubes_len++] = (Cube){x1, x2, y1, y2, z1, z2, mode[1] == 'n'};
  fclose(f);
}

i64 Cube_area(Cube c) {
  return (c.x2 - c.x1 + 1) * (c.y2 - c.y1 + 1) * (c.z2 - c.z1 + 1);
}

void Cube_print(Cube c) {
  printf("(%lld,%lld,%lld) -> (%lld,%lld,%lld): %d\n", c.x1, c.y1, c.z1, c.x2, c.y2, c.z2, c.on);
}

bool Cube_overlaps(Cube a, Cube b) {
  return  !(a.x2 < b.x1 || a.x1 > b.x2 ||
            a.y2 < b.y1 || a.y1 > b.y2 ||
            a.z2 < b.z1 || a.z1 > b.z2);
}

Cube Cube_overlap(Cube a, Cube b) {
  return (Cube){
    .x1 = max(a.x1, b.x1),
    .y1 = max(a.y1, b.y1),
    .z1 = max(a.z1, b.z1),
    .x2 = min(a.x2, b.x2),
    .y2 = min(a.y2, b.y2),
    .z2 = min(a.z2, b.z2),
    .on = !b.on
  };
}

i64 solve(bool inbounds) {
  static Cube disjoint[CUBES_CAP * CUBES_CAP];
  int disjoint_len = 0;

  disjoint_len = 0;
  for (int i = 0; i < cubes_len; i++) {
    static const Cube bounds = {-50, 50, -50, 50, -50, 50, false};
    if (inbounds && !Cube_overlaps(cubes[i], bounds)) continue;

    int N = disjoint_len;
    for (int j = 0; j < N; j++)
      if (Cube_overlaps(cubes[i], disjoint[j]))
        disjoint[disjoint_len++] = Cube_overlap(cubes[i], disjoint[j]);
    if (cubes[i].on) 
      disjoint[disjoint_len++] = cubes[i];
  }
  i64 sum = 0;
  for (int i = 0; i < disjoint_len; i++)
    sum += Cube_area(disjoint[i]) * (disjoint[i].on ? 1 : -1);
  return sum;
}

int main() {
  parse();
  printf("Part 1: %lld\n", solve(true));
  printf("Part 2: %lld\n", solve(false));
  return 0;
}
