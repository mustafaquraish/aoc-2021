#include "common.h"

#define GRID_SIZE 512
int grid1[GRID_SIZE][GRID_SIZE];
int grid2[GRID_SIZE][GRID_SIZE];
int sx = 0, sy = 0;

void parse() {
  FILE *f = fopen(INFILE("15.txt"), "r");
  char buf[2048];

  while (GETLINE(buf, f)) {
    sx = max(sx, strlen(buf));
    for (int col = 0; buf[col]; col++) {
      grid1[sy][col] = buf[col] - '0';
    }
    sy++;
  }
  fclose(f);

  for (int y = 0; y < sy; y++)
    for (int x = 0; x < sx; x++)
      for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
          grid2[sy * j + y][sx * i + x] = (grid1[y][x] + i + j - 1) % 9 + 1;
}

void dump_grid(int data[GRID_SIZE][GRID_SIZE], int nx, int ny) {
  for (int row = 0; row < ny; row++) {
    for (int col = 0; col < nx; col++)
      printf("%d", data[row][col]);
    printf("\n");
  }
}

typedef struct {
  int x, y, pri;
} QElem;
QElem Q[GRID_SIZE * GRID_SIZE];
int Qsize = 0;

void push(int x, int y, int priority) {
  Q[Qsize] = (QElem){x, y, priority};
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

void pop(int *x, int *y, int *p) {
  *x = Q[0].x;
  *y = Q[0].y;
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


int dijkstras(int graph[GRID_SIZE][GRID_SIZE], int ex, int ey) {
  Qsize = 0;
  int dist[GRID_SIZE][GRID_SIZE];
  for (int row = 0; row < GRID_SIZE; row++)
    for (int col = 0; col < GRID_SIZE; col++)
      dist[row][col] = INT32_MAX;
  bool visited[GRID_SIZE][GRID_SIZE] = {{0}};
  push(0, 0, 0);

  int cx, cy, cdist;
  while (Qsize > 0) {
    pop(&cx, &cy, &cdist);
    visited[cy][cx] = true;
    if (cx == ex && cy == ey)
      return cdist;

    static int dirs[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    for (int i = 0; i < 4; i++) {
      int nx = cx + dirs[i][0];
      int ny = cy + dirs[i][1];
      if (nx < 0 || nx > ex || ny < 0 || ny > ey || visited[ny][nx])
        continue;
      if (dist[ny][nx] > cdist + graph[ny][nx]) {
        dist[ny][nx] = cdist + graph[ny][nx];
        push(nx, ny, dist[ny][nx]);
      }
    }
  }
  assert(false && "No path found");
}

int main() {
  parse();
  printf("Part 1: %d\n", dijkstras(grid1, sx - 1, sy - 1));
  printf("Part 2: %d\n", dijkstras(grid2, sx * 5 - 1, sy * 5 - 1));
  return 0;
}