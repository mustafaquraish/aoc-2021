#include "common.h"

int data[14][3];

void parse() {
  FILE *f = fopen(INFILE("24.txt"), "r");
  char buf[2048];

  int block = -1;
  int line = 0;
  while (GETLINE(buf, f)) {
    if (strncmp(buf, "inp", 3) == 0) {
      block++;
      line = 0;
    } else {
      line++;
      if (line == 4) {
        data[block][0] = buf[strlen(buf) - 1] == '1';
      } else if (line == 5) {
        assert(sscanf(buf, "add x %d", &data[block][1]) == 1);
      } else if (line == 15) {
        assert(sscanf(buf, "add y %d", &data[block][2]) == 1);
      }
    }
  }
  fclose(f);
}

bool solve(int idx, bool big, int z, char result[15]) {
  if (idx == 14 && z == 0)
    return true;
  if (data[idx][0]) {
    for (int i = 1; i < 10; i++) {
      int j = big ? 10 - i : i;
      int new_z = 26*z + data[idx][2] + j;
      if (solve(idx+1, big, new_z, result)) {
        result[idx] = j + '0';
        return true;
      }
    }
  } else {
    int digit = (z % 26) + data[idx][1];
    if (1 <= digit && digit <= 9 && solve(idx+1, big, z/26, result)) {
      result[idx] = digit + '0';
      return true;
    }
  }
  return false;
}

int main() {
  parse();
  char result1[15] = {0};
  char result2[15] = {0};
  solve(0, true, 0, result1);
  solve(0, false, 0, result2);
  printf("Part 1: %s\n", result1);
  printf("Part 2: %s\n", result2);
  return 0;
}