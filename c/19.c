#include "common.h"

#define MAX_POINTS 1024
#define MAX_SCANNERS 30

typedef struct {
  int x, y, z;
} Point;

typedef struct {
  Point points[MAX_POINTS];
  int size;
} PointSet;

PointSet accum = {0};
PointSet accumDistances[MAX_POINTS] = {0};
PointSet data[MAX_SCANNERS];
int num_scanners;

int Psum(Point a) { return a.x + a.y + a.z; }
int PlenSq(Point a) { return a.x*a.x + a.y*a.y + a.z*a.z; }
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
      if (buf[0] == '\0') break;
      int x, y, z;
      sscanf(buf, "%d,%d,%d", &x, &y, &z);
      PS_add(&data[num_scanners], (Point){x, y, z});
    }
    num_scanners++;
  }
  fclose(f);
}

void build_offsets(PointSet *result, PointSet *ps, Point p1) {
  for (int i = 0; i < ps->size; i++)
    result->points[i] = Psub(ps->points[i], p1);
  result->size = ps->size;
}

void build_distances(PointSet *result, PointSet *ps, Point p1) {
  for (int i = 0; i < ps->size; i++) {
    Point offset = {PlenSq(Psub(ps->points[i], p1)), 0, 0};
    result->points[i] = offset;
  }
  result->size = ps->size;
}

void update_accum_distances() {
  for (int i = 0; i < accum.size; i++)
    build_distances(&accumDistances[i], &accum, accum.points[i]);
}

bool enough_common_points(PointSet *ps, Point *a, Point *b) {
  PointSet off;
  for (int i = 0; i < ps->size; i++) {
    Point p2 = ps->points[i];
    build_distances(&off, ps, p2);
    for (int j = 0; j < accum.size; j++) {
      Point p1 = accum.points[j];
      if (PS_count_common(&off, &accumDistances[j]) >= 12) { 
        *a = p1, *b = p2;
        return true;
      }
    }
  }
  return false;
}

bool does_scanner_match(PointSet *ps, Point *r_offset, int *r_ori) {
  Point a, b;
  if (!enough_common_points(ps, &a, &b))
    return false;
  
  PointSet off1, off2;
  build_offsets(&off1, &accum, a);
  build_offsets(&off2, ps, b);

  PointSet oriented = {0};
  for (int ori = 0; ori < 24; ori++) {
    PS_orient(&oriented, &off2, ori);
    if (PS_count_common(&off1, &oriented) >= 12) {
      *r_offset = Psub(a, Porient(b, ori));
      *r_ori = ori;
      return true;
    }
  }
  assert(false && "Unreachable");
}

int main() {
  parse();
  accum = data[0];
  update_accum_distances();

  PointSet locations = {0};
  int num_done = 1, ori;
  int done[MAX_SCANNERS] = {true};

  for (int i = 0; num_done < num_scanners; i = (i+1) % num_scanners) {
    if (done[i]) continue;

    Point offset; 
    PointSet current = data[i];
    if (does_scanner_match(&current, &offset, &ori)) {
      PS_add(&locations, offset);
      for (int j = 0; j < current.size; j++) {
        Point p = current.points[j];
        PS_add(&accum, Padd(Porient(p, ori), offset));
      }
      update_accum_distances();
      done[i] = true;
      num_done++;
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
