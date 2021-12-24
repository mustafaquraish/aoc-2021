#include "common.h"

typedef struct {
  int row, col;
  char val;
} Pod;

typedef struct {
  int N;
  Pod pods[20];
  int num_pods;
} Grid;

Grid grid;

int COLS[4] = {2, 4, 6, 8};
int COST[4] = {1, 10, 100, 1000};

void print_pod(Pod pod) {
  printf("%c @ (%d, %d)\n", pod.val, pod.row, pod.col);
}

bool in_room(Pod pod) {
  return pod.row > 0;
}

bool in_correct_room(Pod pod) {
  return pod.col == COLS[pod.val - 'A'];
}

void dump_grid(Grid *grid) {
  char buf[10][100] = {
    "+-----------+",
    "|...........|",
    "+-+.|.|.|.+-+",
    "  |.|.|.|.|  ",
    "  |.|.|.|.|  ",
    "  |.|.|.|.|  ",
    "  +-+-+-+-+  "
  };
  for (int i = 0; i < grid->num_pods; i++) {
    Pod pod = grid->pods[i];
    buf[pod.row+1][pod.col+1] = pod.val;
  }
  for (int i = 0; i < 7; i++) {
    if (grid->N == 2 && 4 <= i && i <= 5) continue;
    printf("%s\n", buf[i]);
  }
}

void parse() {
  FILE *f = fopen(INFILE("23.txt"), "r");
  char buf[2048];

  int row = 0;
  grid.num_pods = 0;
  while (GETLINE(buf, f)) {
    for (int col = 0; buf[col]; col++) {
      if ('A' <= buf[col] && buf[col] <= 'D') {
        grid.pods[grid.num_pods].row = row-1;
        grid.pods[grid.num_pods].col = col-1;
        grid.pods[grid.num_pods].val = buf[col];
        grid.num_pods++;
      }
    }
    row++;
  }
  grid.N = 2;
  fclose(f);
}

bool position_exists_in(Pod *pods, int num_pods, int row, int col) {
  for (int i = 0; i < num_pods; i++)
    if (pods[i].row == row && pods[i].col == col)
      return true;
  return false;
}

bool available_spots(Grid *grid, Pod pod, Pod results[10], int *res_count) {
  *res_count = 0;
  Pod visible[20], movable[20];
  int visible_count = 0;
  int movable_count = 0;

  for (int i = pod.col+1; i < 11; i++) {
    if (position_exists_in(grid->pods, grid->num_pods, 0, i))
      break;
    if (i != 2 && i != 4 && i != 6 && i != 8)
      movable[movable_count++] = (Pod){0, i, pod.val};
    visible[visible_count++] = (Pod){0, i, pod.val};
  }

  for (int i = pod.col-1; i >= 0; i--) {
    if (position_exists_in(grid->pods, grid->num_pods, 0, i))
      break;
    if (i != 2 && i != 4 && i != 6 && i != 8)
      movable[movable_count++] = (Pod){0, i, pod.val};
    visible[visible_count++] = (Pod){0, i, pod.val};
  }

  if (in_room(pod)) {
    if (in_correct_room(pod)) {
      if (pod.row == grid->N) return false;

      bool ok = true;
      for (int i = 0; i < grid->num_pods && ok; i++) {
        Pod cur = grid->pods[i];
        if (cur.col == pod.col && cur.row > pod.row) {
          ok = ok && cur.val == pod.val;
        }
      }

      // All elments below are the same, done!
      if (ok) return false;
    }

    // Trapped if there's something above me
    for (int i = 0; i < grid->num_pods; i++) {
      Pod cur = grid->pods[i];
      if (cur.col == pod.col && cur.row < pod.row) {
        return false;
      }
    }

    for (int i = 0; i < movable_count; i++) {
      results[*res_count] = movable[i];
      (*res_count)++;
    }
    return true;
  }

  // In hallway.
  int dest_col = COLS[pod.val - 'A'];

  // Can't access the column to the room, no moves.
  if (!position_exists_in(visible, visible_count, 0, dest_col)) {
    return false;
  }

  // Don't go back in if there's other chars
  bool ok = true;
  int count = 0;
  for (int i = 0; i < grid->num_pods && ok; i++) {
    Pod cur = grid->pods[i];
    if (cur.col == dest_col) {
      count++;
      ok = ok && cur.val == pod.val;
    }
  }
  if (!ok) return false;

  results[*res_count] = (Pod){grid->N - count, dest_col, pod.val};
  (*res_count)++;
  return true;
}

#pragma region dijkstra
#if 0

typedef struct {
  Grid grid;
  int pri;
} QElem;

#define MAX_QUEUE_SIZE 500000
QElem Q[MAX_QUEUE_SIZE];
int Qsize = 0;

void push(Grid *grid, int priority) {
  // printf("Pushing %d: %d\n", priority, Qsize);
  assert(Qsize < MAX_QUEUE_SIZE);
  Q[Qsize] = (QElem){*grid, priority};
  int cur = Qsize++;
  int PARENT = ((cur + 1) >> 1) - 1;
  while (cur > 0 && Q[cur].pri < Q[PARENT].pri) {
    QElem tmp = Q[cur];
    Q[cur] = Q[PARENT];
    Q[PARENT] = tmp;
    cur = PARENT;
    PARENT = ((cur + 1) >> 1) - 1;
  }
}

void pop(Grid *grid, int *p) {
  *grid = Q[0].grid;
  *p = Q[0].pri;
  Q[0] = Q[--Qsize];
  int cur, min = 0;
  do {
    cur = min;
    int RCHILD = (cur + 1) << 1;
    int LCHILD = RCHILD - 1;
    if (LCHILD < Qsize && Q[LCHILD].pri < Q[min].pri)
      min = LCHILD;
    if (RCHILD < Qsize && Q[RCHILD].pri < Q[min].pri)
      min = RCHILD;
    QElem tmp = Q[cur];
    Q[cur] = Q[min];
    Q[min] = tmp;
  } while (cur != min);
  return;
}

/**
 * TODO: Fix this, it doesn't work :(
 */
i64 dijkstras(Grid *grid) {
  Qsize = 0;
  push(grid, 0);
  while (Qsize > 0) {
    int pri;
    Grid cur;
    pop(&cur, &pri);

    bool done = true;
    for (int i = 0; i < cur.num_pods; i++)
      done = done && in_correct_room(cur.pods[i]);
    if (done) return pri;

    Pod results[10];
    int res_count;
    bool final;
    for (int i = 0; i < cur.num_pods; i++) {
      Pod pod = cur.pods[i];
      if (available_spots(&cur, pod, results, &res_count, &final)) {
        for (int j = 0; j < res_count; j++) {
          Grid new_grid = cur;
          new_grid.pods[i] = results[j];

          int distance = abs(pod.row - results[j].row) + abs(pod.col - results[j].col);
          int step_cost = distance * COST[pod.val - 'A'];
          
          push(&new_grid, pri + step_cost);
        }
      }
    }
  }
  assert(false && "Shouldn't reach here");
}
#endif
#pragma endregion dijkstra

#define HT_CAP 300000
#define BUCKET_CAP 100
Grid CACHE_G[HT_CAP][BUCKET_CAP];
i64  CACHE_C[HT_CAP][BUCKET_CAP];
u64 cache_counts[HT_CAP];

bool Grid_eq(Grid *a, Grid *b) {
  if (a->num_pods != b->num_pods) return false;
  for (int i = 0; i < a->num_pods; i++) {
    if (a->pods[i].val != b->pods[i].val) return false;
    if (a->pods[i].row != b->pods[i].row) return false;
    if (a->pods[i].col != b->pods[i].col) return false;
  }
  return true;
}

u64 Grid_hash(Grid *grid) {
  u64 hash = 0xdeafbeefc001beef;
  for (int i = 0; i < grid->num_pods; i++) {
    hash = hash * 239 + grid->pods[i].val;
    hash = hash ^ (hash >> 32);
    hash = hash * 239 + grid->pods[i].row;
    hash = hash ^ (hash << 14);
    hash = hash * 239 + grid->pods[i].col;
  }
  return hash;
}

u64 cache_hits = 0;
u64 cache_misses = 0;
u64 cache_false_pos = 0;
u64 cache_size = 0;

i64 check_cache(Grid *grid) {
  u64 hash = Grid_hash(grid) % HT_CAP;
  for (u64 i = 0; i < cache_counts[hash]; i++) {
    if (Grid_eq(&CACHE_G[hash][i], grid)) {
      cache_hits++;
      return CACHE_C[hash][i];
    }
    cache_false_pos++;
  }
  cache_misses++;
  return -1;
}

void put_cache(Grid *grid, i64 cost) {
  u64 hash = Grid_hash(grid) % HT_CAP;
  assert(cache_counts[hash] < BUCKET_CAP);
  CACHE_G[hash][cache_counts[hash]] = *grid;
  CACHE_C[hash][cache_counts[hash]] = cost;
  cache_counts[hash]++;
  cache_size++;
}

void reset_cache() {
  for (int i = 0; i < HT_CAP; i++) cache_counts[i] = 0;
  cache_size = 0;
  cache_misses = 0;
  cache_hits = 0;
  cache_false_pos = 0;
}

void print_cache() {
  printf("----------------------------------\n");
  printf("Cache size: %llu\n", cache_size);
  printf("Cache hits: %llu\n", cache_hits);
  printf("Cache misses: %llu\n", cache_misses);
  printf("Cache comparisons: %llu\n", cache_false_pos);

  u64 mx = 0;
  for (int i = 0; i < HT_CAP; i++)
    if (cache_counts[i] > mx) 
      mx = cache_counts[i];
  printf("Max bucket size: %llu\n", mx);
}

i64 solve(Grid *grid) {
  i64 cost = check_cache(grid);
  if (cost != -1) return cost;

  bool done = true;
  for (int i = 0; i < grid->num_pods; i++)
    done = done && in_correct_room(grid->pods[i]);
  if (done) {
    return 0;
  }

  i64 lowest = INT32_MAX;
  for (int i = 0; i < grid->num_pods; i++) {
    Pod moves[10];
    int move_count;
    if (available_spots(grid, grid->pods[i], moves, &move_count)) {
      for (int j = 0; j < move_count; j++) {
        Pod backup = grid->pods[i]; 

        int distance = abs(moves[j].row - backup.row) + abs(moves[j].col - backup.col);
        int step_cost = distance * COST[backup.val - 'A'];

        grid->pods[i] = moves[j];
        int res = solve(grid);
        grid->pods[i] = backup;

        if (res != INT32_MAX && step_cost + res < lowest) {
          lowest = step_cost + res;
        }
      }
    }
  }
  put_cache(grid, lowest);
  return lowest;
}

void part1() {
  reset_cache();
  int res = solve(&grid);
  printf("Part 1: %d\n", res);
}

void part2() {
  reset_cache();
  Grid new_grid = grid;
  new_grid.N = 4;
  for (int i = 0; i < new_grid.num_pods; i++)
    if (new_grid.pods[i].row == 2)
      new_grid.pods[i].row = 4;
  
  new_grid.pods[new_grid.num_pods++] = (Pod){2, 2, 'D'};
  new_grid.pods[new_grid.num_pods++] = (Pod){2, 4, 'C'};
  new_grid.pods[new_grid.num_pods++] = (Pod){2, 6, 'B'};
  new_grid.pods[new_grid.num_pods++] = (Pod){2, 8, 'A'};

  new_grid.pods[new_grid.num_pods++] = (Pod){3, 2, 'D'};
  new_grid.pods[new_grid.num_pods++] = (Pod){3, 4, 'B'};
  new_grid.pods[new_grid.num_pods++] = (Pod){3, 6, 'A'};
  new_grid.pods[new_grid.num_pods++] = (Pod){3, 8, 'C'};

  int result = solve(&new_grid);
  printf("Part 2: %d\n", result);
}

int main() {
  parse();
  part1();
  part2();
  return 0;
}