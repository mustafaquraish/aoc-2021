#include "common.h"

char lines[128][1024];
int line_count = 0;

typedef struct Node {
  u64 value;
  bool leaf;
  struct Node *left, *right;
} Node;

Node *new_leaf(u64 value) {
  Node *node = calloc(1, sizeof(Node));
  node->leaf = true;
  node->value = value;
  return node;
}

Node *new_internal(Node *left, Node *right) {
  Node *node = calloc(1, sizeof(Node));
  node->leaf = false;
  node->left = left;
  node->right = right;
  return node;
}

void print_bst(Node *root) {
  if (root->leaf)
    printf("%llu", root->value);
  else {
    printf("[");
    print_bst(root->left);
    printf(",");
    print_bst(root->right);
    printf("]");
  }
}

Node *create_bst_helper(char **txt) {
  if (isdigit(**txt))
    return new_leaf(strtol(*txt, txt, 10));
  (*txt)++; // skip the '['
  Node *left = create_bst_helper(txt);
  (*txt)++; // skip the ','
  Node *right = create_bst_helper(txt);
  (*txt)++; // skip the ']'
  return new_internal(left, right);
}

Node *create_line_bst(int line_index) {
  char *tmp = lines[line_index];
  return create_bst_helper(&tmp);
}

Node *join_bst(Node *left, Node *right) {
  if (left == NULL) return right;
  if (right == NULL) return left;
  return new_internal(left, right);
}

void free_bst(Node *root) {
  if (root) free_bst(root->left);
  if (root) free_bst(root->right);
  if (root) free(root);
}

void parse() {
  FILE *f = fopen(INFILE("18.txt"), "r");
  while (GETLINE(lines[line_count], f)) line_count++;
  fclose(f);
}

u64 bst_mag(Node *root) {
  if (root->leaf) return root->value;
  return bst_mag(root->left)*3 + bst_mag(root->right)*2;
}

bool should_explode(Node *root, int depth) {
  return (depth >= 4 && root->left->leaf && root->right->leaf);
}

bool bst_explode(Node *root, int *a, int *b, int depth) {
  if (root->leaf) return false;
  if (should_explode(root, depth)) {
    *a = root->left->value;
    *b = root->right->value;
    return true;
  }

  if (bst_explode(root->left, a, b, depth+1)) {
    if (*b < 0) return true;
    if (*a >= 0) {
      free_bst(root->left);
      root->left = new_leaf(0);
    }
    Node *cur = root->right;
    while (!cur->leaf) cur = cur->left;
    cur->value += *b;
    *b = -1;
    return true;
  }

  if (bst_explode(root->right, a, b, depth+1)) {
    if (*a < 0) return true;
    if (*b >= 0) {
      free_bst(root->right);
      root->right = new_leaf(0);
    }
    Node *cur = root->left;
    while (!cur->leaf) cur = cur->right;
    cur->value += *a;
    *a = -1;
    return true;
  }
  return false;
}

bool bst_split(Node *root, int depth) {
  if (!root->leaf) {
    if (bst_split(root->left, depth+1)) return true;
    if (bst_split(root->right, depth+1)) return true;
    return false;
  }
  if (root->value < 10) return false;
  root->leaf = false;
  root->left = new_leaf(root->value / 2);
  root->right = new_leaf((root->value + 1) / 2);
  return true;
}

void simplify(Node *root) {
  for (;;) {
    int a = -1, b = -1;
    if (bst_explode(root, &a, &b, 0)) continue;
    if (bst_split(root, 0)) continue;
    break;
  }
}

void part1() {
  Node *root = NULL;
  for (int i = 0; i < line_count; i++) {
    root = join_bst(root, create_line_bst(i));
    simplify(root);
  }
  printf("Part 1: %llu\n", bst_mag(root));
  free_bst(root);
}

void part2() {
  int result = 0;
  for (int i = 0; i < line_count; i++) {
    for (int j = 0; j < line_count; j++) {
      if (i == j) continue;
      Node *i_plus_j = join_bst(create_line_bst(i), create_line_bst(j));
      Node *j_plus_i = join_bst(create_line_bst(j), create_line_bst(i));
      simplify(i_plus_j), simplify(j_plus_i);
      result = max(result, max(bst_mag(i_plus_j), bst_mag(j_plus_i)));
      free_bst(i_plus_j);
      free_bst(j_plus_i);
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