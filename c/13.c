#include "common.h"

Vector PX = {0};
Vector PY = {0};

bool point_exists(int x, int y) {
  for (int i = 0; i < PX.size; i++)
    if (PX.data[i] == x && PY.data[i] == y)
      return true;
  return false;
}

void dump() {
  int mn_x = INT32_MAX, mx_x = INT32_MIN;
  int mn_y = INT32_MAX, mx_y = INT32_MIN;
  for (int i = 0; i < PX.size; i++) {
    mn_x = min(mn_x, PX.data[i]);
    mx_x = max(mx_x, PX.data[i]);
    mn_y = min(mn_y, PY.data[i]);
    mx_y = max(mx_y, PY.data[i]);
  }
  for (int y = mn_y; y <= mx_y; y++) {
    for (int x = mn_x; x <= mx_x; x++) {
      if (point_exists(x, y)) printf("█");
      else printf(" ");
    }
    printf("\n");
  }
}

void fold_vertical(int t) {
  for (int i = 0; i < PY.size; i++)
    if (PY.data[i] > t) {
      int ny = t+t-PY.data[i];
      if (!point_exists(PX.data[i], ny)) {
        PY.data[i] = ny;
        continue;
      }
      PX.data[i] = PX.data[--PX.size];
      PY.data[i] = PY.data[--PY.size];
      --i;
    }
}

void fold_horizontal(int t) {
  for (int i = 0; i < PX.size; i++)
    if (PX.data[i] > t) {
      int nx = t+t-PX.data[i];
      if (!point_exists(nx, PY.data[i])) {
        PX.data[i] = nx;
        continue;
      }
      PX.data[i] = PX.data[--PX.size];
      PY.data[i] = PY.data[--PY.size];
      --i;
    }
}

int main() {
  FILE *f = fopen(INFILE("13.txt"), "r");
  char buf[2048], axis;
  int a, b, t;

  while (GETLINE(buf, f)) {
    if (sscanf(buf, "%d,%d", &a, &b) == 2)
      Vec_push(&PX, a), Vec_push(&PY, b);
    else
      break;
  }

  bool first = true;
  while (GETLINE(buf, f)) {
    assert(sscanf(buf, "fold along %c=%d", &axis, &t) == 2);
    if (axis == 'x') 
      fold_horizontal(t);
    else
      fold_vertical(t);
    if (first) {
      first = false;
      printf("Part 1: %llu\n", PX.size);
    }
  }

  fclose(f); 
  printf("Part 2:\n");
  dump();

  return 0;
}