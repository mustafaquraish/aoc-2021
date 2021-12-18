#include "common.h"

typedef struct {
  int value;
  int depth;
} Value;

typedef struct {
  Value vals[128];
  int size;
} ValueArray;

ValueArray data[128];
ValueArray current;
int line_count = 0;

void dump(ValueArray *a) {
  for (int i = 0; i < a->size; i++)
    printf("%d ", a->vals[i].value);
  printf("\n");
}

void parse() {
  FILE *f = fopen(INFILE("18.txt"), "r");
  char buf[2048];
  while (GETLINE(buf, f)) {
    char *tmp = buf;
    int count = 0;
    int depth = 0;
    while (*tmp) {
      if (isdigit(*tmp)) {
        data[line_count].vals[count].value = strtol(tmp, &tmp, 10);
        data[line_count].vals[count].depth = depth;
        count++;
      } else {
        if (*tmp == '[') depth++;
        if (*tmp == ']') depth--;
        tmp++;
      }
    }
    data[line_count].size = count;
    line_count++;
  }
  fclose(f);
}

void shift_left(ValueArray *a, int index) {
  assert(index > 0);
  for (int i = index; i < a->size; i++)
    a->vals[i-1] = a->vals[i];
  a->size--;
}

void shift_right(ValueArray *a, int index) {
  assert(index < a->size);
  for (int i = a->size; i > index; i--)
    a->vals[i] = a->vals[i-1];
  a->size++;
}

bool do_split(ValueArray *a) {
  for (int i = 0; i < a->size; i++) {
    if (a->vals[i].value >= 10) {
      Value v = a->vals[i];
      shift_right(a, i);
      a->vals[i+0] = (Value) {(v.value+0)/2, v.depth+1};
      a->vals[i+1] = (Value) {(v.value+1)/2, v.depth+1};
      return true;
    }
  }
  return false;
}

bool do_explode(ValueArray *a) {
  for (int i = 0; i < a->size-1; i++) {
    if (a->vals[i].depth == a->vals[i+1].depth && a->vals[i].depth > 4) {
      if (i > 0)         a->vals[i-1].value += a->vals[i].value;
      if (i < a->size-2) a->vals[i+2].value += a->vals[i+1].value;
      shift_left(a, i+2);
      a->vals[i].value = 0;
      a->vals[i].depth--;
      return true;
    }
  }
  return false;
}

ValueArray *concat(ValueArray *accum, ValueArray *other) {
  for (int i = 0; i < accum->size; i++) {
    accum->vals[i].depth++;
  }
  for (int i = 0; i < other->size; i++) {
    accum->vals[accum->size] = other->vals[i];
    accum->vals[accum->size].depth++;
    accum->size++;
  }
  for (;;) {
    if (do_explode(accum)) continue;
    if (do_split(accum)) continue;
    break;
  }
  return accum;
}

int mag(ValueArray *a) {
  ValueArray tmp = *a;
  while (tmp.size > 1)
    for (int i = 0; i < a->size-1; i++)
      if (tmp.vals[i].depth == tmp.vals[i+1].depth) {
        tmp.vals[i].value = tmp.vals[i].value * 3 + tmp.vals[i+1].value * 2;
        tmp.vals[i].depth--;
        shift_left(&tmp, i+2);
        break;
      }
  return tmp.vals[0].value;
}

void part1() {
  current = data[0];
  for (int i = 1; i < line_count; i++)
    concat(&current, &data[i]);
  printf("Part 1: %d\n", mag(&current));
}

void part2() {
  int result = 0;
  for (int i = 0; i < line_count; i++) {
    for (int j = 0; j < line_count; j++) {
      if (i == j) continue;
      current = data[i];
      result = max(result, mag(concat(&current, &data[j])));

      current = data[j];
      result = max(result, mag(concat(&current, &data[i])));
    }
  }
  printf("Part 2: %d\n", result);
}

int main() {
  parse();
  part1();
  part2();
  return 0;
}