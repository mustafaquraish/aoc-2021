#include "common.h"

char binary_buffer[16384];
char *head;

void parse() {
  FILE *f = fopen(INFILE("16.txt"), "r");
  char buf[4096];

  GETLINE(buf, f);
  int N = strlen(buf);
  char *cur = binary_buffer;
  for (int i = 0; i < N; i++) {
    switch (buf[i]) {
      case '0': sprintf(cur, "0000"); break;
      case '1': sprintf(cur, "0001"); break;
      case '2': sprintf(cur, "0010"); break;
      case '3': sprintf(cur, "0011"); break;
      case '4': sprintf(cur, "0100"); break;
      case '5': sprintf(cur, "0101"); break;
      case '6': sprintf(cur, "0110"); break;
      case '7': sprintf(cur, "0111"); break;
      case '8': sprintf(cur, "1000"); break;
      case '9': sprintf(cur, "1001"); break;
      case 'A': sprintf(cur, "1010"); break;
      case 'B': sprintf(cur, "1011"); break;
      case 'C': sprintf(cur, "1100"); break;
      case 'D': sprintf(cur, "1101"); break;
      case 'E': sprintf(cur, "1110"); break;
      case 'F': sprintf(cur, "1111"); break;
    }
    cur += 4;
  }
  *cur = '\0';
  head = binary_buffer;
  fclose(f);
}

typedef struct packet {
  int version, type;
  u64 value;
  struct packet **subs;
  int num_subs;
} Packet;

int parse_int(int num_digits) {
  int res = 0;
  for (int i = 0; i < num_digits; i++)
    res = res * 2 + (head[i] - '0');
  head += num_digits;
  return res;
}

Packet *parse_packet() {
  Packet *packet = (Packet *)calloc(1, sizeof(Packet));
  packet->version = parse_int(3);
  packet->type = parse_int(3);
  if (packet->type == 4) {
    int loop = 1;
    while (loop) {
      loop = parse_int(1);
      packet->value = packet->value << 4 | parse_int(4);
    }
    return packet;
  }

  int mode = parse_int(1);
  if (mode == 0) {
    int sz = parse_int(15);
    while (sz > 0) {
      char *old = head;
      Packet *sub = parse_packet();
      packet->subs = realloc(packet->subs, (packet->num_subs + 1) * sizeof(Packet *));
      packet->subs[packet->num_subs++] = sub;
      sz -= (head - old);
    }
  } else {
    int num_iters = parse_int(11);
    for (int i = 0; i < num_iters; i++) {
      Packet *sub = parse_packet();
      packet->subs = realloc(packet->subs, (packet->num_subs + 1) * sizeof(Packet *));
      packet->subs[packet->num_subs++] = sub;
    }
  }
  return packet;
}

void dump_packet(Packet *packet, int depth) {
  printf("%*cPacket(%d, %d,", depth, ' ', packet->version, packet->type);
  if (packet->type == 4) {
    printf(" %llu)", packet->value);
    return;
  }
  printf("\n");
  for (int i = 0; i < packet->num_subs; i++) {
    dump_packet(packet->subs[i], depth+2); printf("\n");
  }
  printf("%*c)", depth, ' ');
}

u64 part1(Packet *packet) {
  u64 sum = packet->version;
  for (int i = 0; i < packet->num_subs; i++) {
    sum += part1(packet->subs[i]);
  }
  return sum;
}

u64 part2(Packet *packet) {
  if (packet->type == 4) return packet->value;
  if (packet->type == 0) {
    u64 sum = 0;
    for (int i = 0; i < packet->num_subs; i++)
      sum += part2(packet->subs[i]);
    return sum;
  }
  if (packet->type == 1) {
    u64 prod = 1;
    for (int i = 0; i < packet->num_subs; i++)
      prod *= part2(packet->subs[i]);
    return prod;
  }
  if (packet->type == 2) {
    u64 mn = INT64_MAX;
    for (int i = 0; i < packet->num_subs; i++)
      mn = minu64(mn, part2(packet->subs[i]));
    return mn;
  }
  if (packet->type == 3) {
    u64 mx = 0;
    for (int i = 0; i < packet->num_subs; i++)
      mx = maxu64(mx, part2(packet->subs[i]));
    return mx;
  }
  if (packet->type == 5) return part2(packet->subs[0]) > part2(packet->subs[1]);
  if (packet->type == 6) return part2(packet->subs[0]) < part2(packet->subs[1]);
  if (packet->type == 7) return part2(packet->subs[0]) == part2(packet->subs[1]);
  assert(false && "Unknown packet type");
}

int main() {
  parse();
  Packet *packet = parse_packet();
  // dump_packet(packet, 0); printf("\n");
  printf("Part 1: %llu\n", part1(packet));
  printf("Part 2: %llu\n", part2(packet));
  return 0;
}