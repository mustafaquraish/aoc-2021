#include "common.h"

typedef struct Cube {
  i64 x1, x2;
  i64 y1, y2;
  i64 z1, z2;
} Cube;

#define CUBES_CAP 1024
Cube cubes[CUBES_CAP];
bool modes[CUBES_CAP];
int cubes_len = 0;

void parse() {
  i64 x1, y1, z1, x2, y2, z2;
  char mode[5];
  FILE *f = fopen(INFILE("22.txt"), "r");
  while (fscanf(f, " %s x=%lld..%lld,y=%lld..%lld,z=%lld..%lld", 
                mode, &x1, &x2, &y1, &y2, &z1, &z2) != EOF) 
  {
    cubes[cubes_len] = (Cube){x1, x2, y1, y2, z1, z2};
    modes[cubes_len] = mode[1] == 'n';
    cubes_len++;
  }
  fclose(f);
}

i64 Cube_area(Cube c) {
  return (c.x2 - c.x1 + 1) * (c.y2 - c.y1 + 1) * (c.z2 - c.z1 + 1);
}

void Cube_print(Cube c) {
  printf("(%lld,%lld,%lld) -> (%lld,%lld,%lld)\n", c.x1, c.y1, c.z1, c.x2, c.y2, c.z2);
}

bool Cube_overlaps(Cube a, Cube b) {
  return  !(a.x2 < b.x1 || a.x1 > b.x2 ||
            a.y2 < b.y1 || a.y1 > b.y2 ||
            a.z2 < b.z1 || a.z1 > b.z2);
}

void Cube_subtract(Cube a, Cube b, Cube *res, int *idx) {
  if (!Cube_overlaps(a, b)) {
    res[(*idx)++] = a;
    return;
  }
  int max_x1 = max(a.x1, b.x1);
  int min_x2 = min(a.x2, b.x2);
  int max_y1 = max(a.y1, b.y1);
  int min_y2 = min(a.y2, b.y2);

  if (b.x1 > a.x1) res[(*idx)++] = (Cube){ a.x1, b.x1 - 1, a.y1, a.y2, a.z1, a.z2 };
  if (b.y1 > a.y1) res[(*idx)++] = (Cube){ max_x1, min_x2, a.y1, b.y1 - 1, a.z1, a.z2 };
  if (b.z1 > a.z1) res[(*idx)++] = (Cube){ max_x1, min_x2, max_y1, min_y2, a.z1, b.z1 - 1 };

  if (b.x2 < a.x2) res[(*idx)++] = (Cube){ b.x2 + 1, a.x2, a.y1, a.y2, a.z1, a.z2 };
  if (b.y2 < a.y2) res[(*idx)++] = (Cube){ max_x1, min_x2, b.y2 + 1, a.y2, a.z1, a.z2 };
  if (b.z2 < a.z2) res[(*idx)++] = (Cube){ max_x1, min_x2, max_y1, min_y2, b.z2 + 1, a.z2 };
}

i64 solve(bool inbounds) {
  static Cube disjoint[CUBES_CAP * CUBES_CAP];
  static Cube buffer[CUBES_CAP * CUBES_CAP];
  int disjoint_len = 0;
  int buffer_len = 0;

  disjoint_len = 0;
  for (int i = 0; i < cubes_len; i++) {
    static const Cube bounds = {-50, 50, -50, 50, -50, 50};
    if (inbounds && !Cube_overlaps(cubes[i], bounds)) continue;

    buffer_len = 0;
    for (int j = 0; j < disjoint_len; j++)
      Cube_subtract(disjoint[j], cubes[i], buffer, &buffer_len);
    if (modes[i]) buffer[buffer_len++] = cubes[i];
    
    memcpy(disjoint, buffer, buffer_len * sizeof(Cube));
    disjoint_len = buffer_len;
  }
  i64 sum = 0;
  for (int i = 0; i < disjoint_len; i++)
    sum += Cube_area(disjoint[i]);
  return sum;
}

int main() {
  parse();
  printf("Part 1: %lld\n", solve(true));
  printf("Part 2: %lld\n", solve(false));
  return 0;
}