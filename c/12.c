#include "common.h"

#define MAT_MAX 20
int mat[MAT_MAX][MAT_MAX] = {0};
char nodes[MAT_MAX][1024];
int visited[MAT_MAX] = {0};
int nodes_size = 0, start_index, end_index;

int insert_and_get_index(char *name) {
  for (int i = 0; i < nodes_size; i++) {
    if (strcmp(nodes[i], name) == 0)
      return i;
  }
  if (strcmp(name, "start") == 0) start_index = nodes_size;
  if (strcmp(name, "end") == 0) end_index = nodes_size;
  strcpy(nodes[nodes_size], name);
  return nodes_size++;
}

void parse() {
  FILE *f = fopen(INFILE("12.txt"), "r");
  char buf[100];

  while (fgets(buf, 100, f)) {
    char *line = buf;
    int i = insert_and_get_index(strsep(&line, "-"));
    int j = insert_and_get_index(strsep(&line, "\n"));
    mat[i][j] = 1;
    mat[j][i] = 1;
  }
}

void DFS(int cur, int *count1, int *count2, int twice) {
  if (cur == end_index) {
    if (count1) (*count1)++;
    if (count2) (*count2)++;
    return;
  }

  for (int n = 0; n < nodes_size; n++) {
    if (!mat[cur][n]) continue;
    if (!visited[n]) {
      visited[n] = islower(nodes[n][0]);
      DFS(n, count1, count2, twice);
      visited[n] = 0;
    } else if (!twice && n != start_index) {
      DFS(n, NULL, count2, 1);
    }
  }
}

int main() {
  parse();
  int count1 = 0, count2 = 0;
  visited[start_index] = 1;
  DFS(start_index, &count1, &count2, 0);

  printf("Part 1: %d\n", count1);
  printf("Part 2: %d\n", count2);
  return 0;
}