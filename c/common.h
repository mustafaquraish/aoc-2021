#pragma once

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

#define INFILE(fname) "input/"fname

typedef struct {
  u64 capacity;
  u64 size;
  u64 *data;
} Vector;

inline int Vec_push(Vector *vec, u64 value) {
  if (vec->size == vec->capacity) {
    if (vec->capacity == 0) 
      vec->capacity = 16;
    else 
      vec->capacity *= 2;
    vec->data = realloc(vec->data, vec->capacity * sizeof(u64));
  }
  vec->data[vec->size] = value;
  return vec->size++;
}

inline u64 Vec_pop(Vector *vec) {
  assert(vec->size > 0);
  return vec->data[--vec->size];
}

inline u64 Vec_top(Vector *vec) {
  assert(vec->size > 0);
  return vec->data[vec->size - 1];
}

inline u64 Vec_get(Vector *vec, u64 index) {
  assert(index < vec->size);
  return vec->data[index];
}

inline int Vec_geti(Vector *vec, u64 index) {
  assert(index < vec->size);
  return vec->data[index];
}

int cmp_u64_dec(const void *a, const void *b) {
  u64 *aa = (u64 *)a;
  u64 *bb = (u64 *)b;
  return (*aa < *bb) ? 1 : -1;
}

int cmp_u64_inc(const void *a, const void *b) {
  u64 *aa = (u64 *)a;
  u64 *bb = (u64 *)b;
  return (*aa < *bb) ? -1 : 1;
}

int sign(int x) { return x > 0 ? +1 : x == 0 ? 0 : -1; }
int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }
u64 maxu64(u64 a, u64 b) { return a > b ? a : b; }
u64 minu64(u64 a, u64 b) { return a < b ? a : b; }

void swap(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

char *fgetstrip(char *buf, int size, FILE *f) {
  char *ret = fgets(buf, size, f);
  if (ret && ret[strlen(ret) - 1] == '\n')
    ret[strlen(ret) - 1] = '\0';
  return ret;
}

#define GETLINE(buf, f) fgetstrip(buf, sizeof(buf), f)
