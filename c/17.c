#include "common.h"

double sqrt(double);
int abs(int x) { return x < 0 ? -x : x; }

int X1, X2, Y1, Y2;

void parse() {
  FILE *f = fopen(INFILE("17.txt"), "r");
  fscanf(f, "target area: x=%d..%d, y=%d..%d", &X1, &X2, &Y1, &Y2);
  fclose(f);
}

bool is_between(int a, int first, int second) {
  return (a >= first && a <= second) || (a >= second && a <= first);
}

bool check_vel(int vx, int vy) {
  int x = 0, y = 0;
  while (x <= X2 && y >= Y1) {
    x += vx;
    y += vy;
    vx -= sign(vx);
    vy -= 1;
    if (is_between(x, X1, X2) && is_between(y, Y1, Y2)) {
      return true;
    }
  }
  return false;
}

int main() {
  parse();
  int min_x = sqrt(2 * X1), max_x = X2+1;
  int max_y = abs(Y1) - 1,  min_y = -abs(Y1);
  
  int count = 0;
  for (int vx = min_x; vx <= max_x; vx++)
    for (int vy = min_y; vy <= max_y; vy++)
      if (check_vel(vx, vy))
        count++;
  printf("Part 1: %d\n", max_y*(max_y+1)/2);
  printf("Part 2: %d\n", count);
  return 0;
}