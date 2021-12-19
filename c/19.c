#include "common.h"

typedef struct {
  int x, y, z;
} Point;

#define MAX_SCANNERS 30
#define MAX_POINTS 1024

typedef struct {
  Point points[MAX_POINTS];
  int size;
} PointSet;

PointSet data[MAX_SCANNERS];
int num_scanners;

PointSet accum = {0};
PointSet accumOffsets[MAX_POINTS] = {0};

int Psum(Point a) { return a.x + a.y + a.z; }
Point Psub(Point a, Point b) { return (Point){a.x - b.x, a.y - b.y, a.z - b.z}; }
Point Padd(Point a, Point b) { return (Point){a.x + b.x, a.y + b.y, a.z + b.z}; }
Point Pabs(Point a) { return (Point){abs(a.x), abs(a.y), abs(a.z)}; }
Point Porient(Point p, int ori) {
  int a = p.x, b = p.y, c = p.z;
  Point all_orientations[24] = {
    {+a,+b,+c}, {+b,+c,+a}, {+c,+a,+b}, {+c,+b,-a}, {+b,+a,-c}, {+a,+c,-b}, 
    {+a,-b,-c}, {+b,-c,-a}, {+c,-a,-b}, {+c,-b,+a}, {+b,-a,+c}, {+a,-c,+b}, 
    {-a,+b,-c}, {-b,+c,-a}, {-c,+a,-b}, {-c,+b,+a}, {-b,+a,+c}, {-a,+c,+b}, 
    {-a,-b,+c}, {-b,-c,+a}, {-c,-a,+b}, {-c,-b,-a}, {-b,-a,-c}, {-a,-c,-b} 
  };
  return all_orientations[ori];
}

bool PS_exists(PointSet *ps, Point p) {
  for (int i = 0; i < ps->size; i++) {
    Point cur = ps->points[i];
    if (cur.x == p.x && cur.y == p.y && cur.z == p.z)
      return true;
  }
  return false;
}

void PS_add(PointSet *ps, Point p) {
  if (PS_exists(ps, p)) return;
  assert(ps->size < MAX_POINTS);
  ps->points[ps->size++] = p;
}

void PS_dump(PointSet *ps) {
  printf("Pointset (%d points):\n", ps->size);
  for (int i = 0; i < ps->size; i++) {
    Point p = ps->points[i];
    printf("(%d,%d,%d)\n", p.x, p.y, p.z);
  }
  printf("\n");
}

int PS_count_common(PointSet *a, PointSet *b) {
  if (a->size == 0 || b->size == 0)
    return 0;

  int count = 0;
  for (int i = 0; i < a->size; i++) {
    if (PS_exists(b, a->points[i]))
      count++;
  }
  return count;
}

void PS_orient(PointSet *res, PointSet *ps, int ori) {
  *res = *ps;
  for (int i = 0; i < res->size; i++)
    res->points[i] = Porient(res->points[i], ori);
}

void parse() {
  FILE *f = fopen(INFILE("19.txt"), "r");
  char buf[2048];
  while (GETLINE(buf, f)) {
    while (GETLINE(buf, f)) {
      int N = strlen(buf);
      if (N == 0)
        break;
      int x, y, z;
      sscanf(buf, "%d,%d,%d", &x, &y, &z);
      PS_add(&data[num_scanners], (Point){x, y, z});
    }
    num_scanners++;
  }
  fclose(f);
}

void build_offsets(PointSet *result, PointSet *ps, Point p1) {
  for (int i = 0; i < ps->size; i++) {
    Point p2 = ps->points[i];
    Point offset = {p2.x - p1.x, p2.y - p1.y, p2.z - p1.z};
    result->points[i] = offset;
  }
  result->size = ps->size;
}

void update_accum_offsets() {
  for (int i = 0; i < accum.size; i++)
    build_offsets(&accumOffsets[i], &accum, accum.points[i]);
}

bool enough_common_points(PointSet *ps, Point *offset) {
  PointSet off;
  for (int i = 0; i < ps->size; i++) {
    Point p1 = ps->points[i];
    build_offsets(&off, ps, p1);
    for (int j = 0; j < accum.size; j++) {
      Point p2 = accum.points[j];
      if (PS_count_common(&off, &accumOffsets[j]) >= 12) { 
        *offset = Psub(p2, p1);
        return true;
      }
    }
  }
  return false;
}

bool does_scanner_match(PointSet *ps, Point *r_offset, int *r_ori) {
  PointSet oriented = {0};
  for (int ori = 0; ori < 24; ori++) {
    PS_orient(&oriented, ps, ori);
    if (enough_common_points(&oriented, r_offset)) {
      *r_ori = ori;
      return true;
    }
  }
  return false;
}

int Pcmp(const void *pa, const void *pb) {
  Point *a = (Point *)pa;
  Point *b = (Point *)pb;
  if (a->x != b->x) return a->x - b->x;
  if (a->y != b->y) return a->y - b->y;
  return a->z - b->z;
}

void remove_dups_accum() {
  qsort(accum.points, accum.size, sizeof(Point), Pcmp);
  int i = 0;
  for (int j = 1; j < accum.size; j++) {
    if (Pcmp(&accum.points[i], &accum.points[j]) != 0) {
      accum.points[++i] = accum.points[j];
    }
  }
  accum.size = i + 1;
}

int main() {
  parse();
  
  accum = data[0];
  update_accum_offsets();

  PointSet locations = {0};
  int num_done = 1, ori;
  int done[MAX_SCANNERS] = {true};

  for (int i = 0; num_done < num_scanners; i = (i+1) % num_scanners) {
    if (done[i]) continue;

    Point offset; 
    PointSet current = data[i];
    if (does_scanner_match(&current, &offset, &ori)) {
      PS_add(&locations, offset);
      done[i] = true;
      num_done++;
      for (int j = 0; j < current.size; j++) {
        Point p = current.points[j];
        accum.points[accum.size++] = Padd(Porient(p, ori), offset);
      }
      remove_dups_accum();
      update_accum_offsets();
    }
  }

  int max_dst = 0;
  for (int i = 0; i < locations.size; i++) {
    for (int j = 0; j < locations.size; j++) {
      int dst = Psum(Pabs(Psub(locations.points[i], locations.points[j])));
      max_dst = max(max_dst, dst);
    }
  }

  printf("Part 1: %d\n", accum.size);
  printf("Part 2: %d\n", max_dst);
  return 0;
}
