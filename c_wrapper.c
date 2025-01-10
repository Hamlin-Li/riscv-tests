#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

extern long vle32_test(long);
extern long vse8_test(char*);
extern long vclmul_test(void);
extern void cbo_zero(long la[], int cnt);
extern void plain_zero(long la[], int cnt);
extern void vector_zero(long la[], int cnt);
extern long test_temp(long);
extern long test_temp_2(long);
extern long test_temp_x(long);
extern long test_temp_2_x(long);

long vse8_test(char* buf);

/*
void vclmul() {
  long x = 0;
  if (0) {
    x = vle32_test(100);
  } else {
    x = vclmul_test();
  }
}

void shuffle(long a[], int cnt) {
  for (int i = 0; i < cnt; i++) {
    a[i] = 10000 - i;
  }
}

void c_zero(long la[], int cnt) {
  for (int i = 0; i < cnt*8; i++) {
    la[i] = 0;
  }
}

void perf_zero(void (*funcPtr)(long[], int), int aligned, char* msg) {
  long la[2048];
  shuffle(la, 2048);
  long* lb = &la[8];
  lb = (long*)((long)lb & -64L);
  struct timeval stop, start;
  gettimeofday(&start, NULL);
  long* data = aligned ? lb : la;
  for (int i = 0; i < 1000000; i++) {
    (*funcPtr)(data, 1024/8);
  }
  gettimeofday(&stop, NULL);
  printf("%s takes %lu us\n", msg, (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
  if (0) {
    printf("\tla: %p, lb: %p, diff: %lx\n", la, lb, (lb-la));
  }
}

void sanity_zero(void (*funcPtr)(long[], int), int aligned, char* msg) {
  long la[2048];
  shuffle(la, 2048);
  long* lb = &la[8];
  lb = (long*)((long)lb & -64L);
  long* data = aligned ? lb : la;
  printf("\n==== sanity: %s ==== \n", msg);
  printf("%ld\n", data[0]); printf("%ld\n", data[512]); printf("%ld\n", data[1020]); printf("%ld\n", data[1023]);
  (*funcPtr)(data, 1024/8);
  printf("%ld\n", data[0]); printf("%ld\n", data[512]); printf("%ld\n", data[1020]); printf("%ld\n", data[1023]);
}

void perf_test() {
  if (1) {
    printf("==== aligned zeroing ==== \n");
    perf_zero(&cbo_zero, 1, "cbo_zero"); 
    perf_zero(&plain_zero, 1, "plain_zero");
    perf_zero(&c_zero, 1, "c_zero");
    perf_zero(&vector_zero, 1, "vector_zero");

    printf("\n==== unaligned zeroing ==== \n");
    perf_zero(&cbo_zero, 0, "cbo_zero");
    perf_zero(&plain_zero, 0, "plain_zero");
    perf_zero(&c_zero, 0, "c_zero");
    perf_zero(&vector_zero, 0, "vector_zero");
  }

  if (1) {
    printf("\n==== unaligned zeroing ==== \n");
    sanity_zero(&cbo_zero, 1, "cbo_zero");
    sanity_zero(&plain_zero, 1, "plain_zero");
    sanity_zero(&c_zero, 1, "c_zero");
    sanity_zero(&vector_zero, 1, "vector_zero");

    printf("\n==== unaligned zeroing ==== \n");
    sanity_zero(&cbo_zero, 0, "cbo_zero");
    sanity_zero(&plain_zero, 0, "plain_zero");
    sanity_zero(&c_zero, 0, "c_zero");
    sanity_zero(&vector_zero, 0, "vector_zero");
  }
}

void temp_tests() {
  long arr[8] = {0x1, 0x100, 0x10000, 0x1000000, 0x100000000L, 0x10000000000L, 0x10000000000L, 0x100000000000L};
  for (int i = 0; i < 8; i++) {
    printf("i: %d, %lx\n", i, arr[i]);
    printf("    test_temp,   %ld\n", test_temp(arr[i]));
    printf("    test_temp_2, %ld\n", test_temp_2(arr[i]));
    printf("    test_temp_x,   %ld\n", test_temp_x(arr[i]));
    printf("    test_temp_2_x, %ld\n", test_temp_2_x(arr[i]));
  }
}
*/
void cache_test_sequential() {
  int size = 1024*1024*128;
  const int header_size = 8;
  int step = 1;
  const int step_limit = 1000;
  void* mem = malloc(size*sizeof(long) + step_limit*sizeof(long)*2);
  long* la = (long*) mem;

  struct timeval stop, start;
  gettimeofday(&start, NULL);

  for (int i = 1; la < (long*)mem + size; i++) {
    if (la > (long*)mem + size) {
      printf("error  ..., %lx, %lx", (long)la, (long)(mem + i*8));
      exit(0);
    }

    *la = i;
    la++;
    memset(la, 0, (i%step_limit)*sizeof(long));
    la += i%step_limit;
  }

  gettimeofday(&stop, NULL);
  printf("%s takes %lu us\n", "cache_test_sequential", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

  free(mem);
}

void cache_test_sequential_cbo_zero() {
  int size = 1024*1024*128;
  const int header_size = 8;
  int step = 1;
  const int step_limit = 1000;
  void* mem = malloc(size*sizeof(long) + step_limit*sizeof(long)*2);
  long* la = (long*) mem;

  struct timeval stop, start;
  gettimeofday(&start, NULL);

  for (int i = 1; la < (long*)mem + size; i++) {
    if (la > (long*)mem + size) {
      printf("error  ..., %lx, %lx", (long)la, (long)(mem + i*8));
      exit(0);
    }

    *la = i;
    la++;
//    memset(la, 0, (i%step_limit)*sizeof(long));
    cbo_zero(((long)(la+63)) & -64l, (i%step_limit+7)/8);
    la += i%step_limit;
  }

  gettimeofday(&stop, NULL);
  printf("%s takes %lu us\n", "cache_test_sequential_cbo_zero", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

  free(mem);
}

void cache_test_reverse() {
  int size = 1024*1024*128;
  const int header_size = 8;
  int step = 1;
  const int step_limit = 1000;
  void* mem = malloc(size*sizeof(long) + step_limit*sizeof(long)*2);
  long* la = (long*) mem;
  long* prefetch_la = la + 16;
  
  struct timeval stop, start;
  gettimeofday(&start, NULL);
  
  for (int i = 1; la < (long*)mem + size; i++) {
    if (la > prefetch_la) {
      printf("la: %lx, prefetch_la: %lx\n", (long)la, (long)prefetch_la);
    }
    if (la > (long*)mem + size) {
      printf("error  ..., %lx, %lx", (long)la, (long)(mem + i*8));
      exit(0);
    }
    if (prefetch_la > (long*)mem + size) {
      printf("error 2 ..., %lx, %lx", (long)prefetch_la, (long)((long*)mem + size));
      exit(0);
    }

    memset(prefetch_la++, 0, (i%step_limit)*sizeof(long));
    prefetch_la += i%step_limit;
    *la = i;
    la++;
    la += i%step_limit;
  }

  gettimeofday(&stop, NULL);
  printf("%s takes %lu us\n", "cache_test_reverse", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

  free(mem);
}

void cache_test_reverse_cbo_zero() {
  int size = 1024*1024*128;
  const int header_size = 8;
  int step = 1;
  const int step_limit = 1000;
  void* mem = malloc(size*sizeof(long) + step_limit*sizeof(long)*2);
  long* la = (long*) mem;
  long* prefetch_la = la + 16;

  struct timeval stop, start;
  gettimeofday(&start, NULL);

  for (int i = 1; la < (long*)mem + size; i++) {
    if (la > prefetch_la) {
      printf("la: %lx, prefetch_la: %lx\n", (long)la, (long)prefetch_la);
    }
    if (la > (long*)mem + size) {
      printf("error  ..., %lx, %lx", (long)la, (long)(mem + i*8));
      exit(0);
    }
    if (prefetch_la > (long*)mem + size) {
      printf("error 2 ..., %lx, %lx", (long)prefetch_la, (long)((long*)mem + size));
      exit(0);
    }

//    memset(prefetch_la++, 0, (i%step_limit)*sizeof(long));
    cbo_zero(((long)(prefetch_la+63)) & -64l, (i%step_limit+7)/8);    prefetch_la++;
    prefetch_la += i%step_limit;
    *la = i;
    la++;
    la += i%step_limit;
  }

  gettimeofday(&stop, NULL);
  printf("%s takes %lu us\n", "cache_test_reverse_cbo_zero", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

  free(mem);
}

/*
cache_test_sequential takes 1039307 us
cache_test_sequential_cbo_zero takes 1068394 us
cache_test_reverse takes 1061907 us
cache_test_reverse_cbo_zero takes 1064935 us
*/
void cache_test_header_body() {
  for (int i = 0; i < 2; i++) {
    printf("\nloop %d\n", i);
    cache_test_sequential();
    cache_test_sequential_cbo_zero();
    cache_test_reverse();
    cache_test_reverse_cbo_zero();
  }
}

void cache_test_only_body_memset(int step_factor) {
  int size = 1024*1024*128;
  int step = 64*step_factor;
  void* mem = malloc(size*sizeof(long) + step*2*sizeof(long));
  long* la = (long*) mem;
  long* lb = (long*)&la[8];
  lb = (long*)((long)lb & -64L);

  struct timeval stop, start;
  gettimeofday(&start, NULL);

  for (int i = 0; i < size/step; i++) {
    memset(lb, 0, step*8);
    lb += step;
  }

  gettimeofday(&stop, NULL);
  printf("%s(%d) takes %lu us\n", "cache_test_only_body_memset", step_factor, (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

  free(mem);
}

void cache_test_only_body_cbo_zero(int step_factor) {
  int size = 1024*1024*128;
  int step = 64*step_factor;
  void* mem = malloc(size*sizeof(long) + step*2*sizeof(long));
  long* la = (long*) mem;
  long* lb = (long*)&la[8];
  lb = (long*)((long)lb & -64L);

  struct timeval stop, start;
  gettimeofday(&start, NULL);

  for (int i = 0; i < size/step; i++) {
    cbo_zero(lb, step/8);
    lb += step;
  }

  gettimeofday(&stop, NULL);
  printf("%s(%d) takes %lu us\n", "cache_test_only_body_cbo_zero", step_factor, (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

  free(mem);
}

void cache_test_only_body() {
  for (int i = 0; i < 2; i++) {
    printf("\nloop %d\n", i);
    for (int j = 1; j <= 128; j<<=1) {
      cache_test_only_body_memset(j);
      cache_test_only_body_cbo_zero(j);
    }
  }
}

extern void vset_e8_m8();
extern void vset_e16_m8();
extern void vset_e32_m8();
extern void vset_e64_m8();

extern char vaddx_char(char c);
extern short vaddx_short(short s);
extern int vaddx_int(int i);
extern long vaddx_long(long l);

long vset_test_static(long l, const int outer_loop, const int inner_loop) {

  for (int j = 0; j < outer_loop; j++) {
    vset_e8_m8();
    for (int i = 0; i < inner_loop; i++) {
      l = vaddx_char((char)l);
    }
    vset_e16_m8();
    for (int i = 0; i < inner_loop; i++) {
      l = vaddx_short((short)l);
    }
    vset_e32_m8();
    for (int i = 0; i < inner_loop; i++) {
      l = vaddx_int((int)l);
    }
    vset_e64_m8();
    for (int i = 0; i < inner_loop; i++) {
      l = vaddx_long((long)l);
    }
  }
}

long vset_test_static_redundant(long l, const int outer_loop, const int inner_loop) {

  for (int j = 0; j < outer_loop; j++) {
    for (int i = 0; i < inner_loop; i++) {
      vset_e8_m8();
      l = vaddx_char((char)l);
    }
    for (int i = 0; i < inner_loop; i++) {
      vset_e16_m8();
      l = vaddx_short((short)l);
    }
    for (int i = 0; i < inner_loop; i++) {
      vset_e32_m8();
      l = vaddx_int((int)l);
    }
    for (int i = 0; i < inner_loop; i++) {
      vset_e64_m8();
      l = vaddx_long((long)l);
    }
  }
}

long vset_test_dynamic(long l, const int outer_loop, const int inner_loop) {

  for (int j = 0; j < outer_loop; j++) {
    for (int i = 0; i < inner_loop; i++) {
      vset_e8_m8();
      l = vaddx_char((char)l);
      vset_e16_m8();
      l = vaddx_short((short)l);
      vset_e32_m8();
      l = vaddx_int((int)l);
      vset_e64_m8();
      l = vaddx_long((long)l);
    }
  }
}

void vset_tests() {
  long l = 1;
  // vset_test_static takes 126266 us
  // vset_test_static_redundant takes 139398 us
  // vset_test_dynamic takes 137694 us
  const int outer_loop = 100000;

  // vset_test_static takes 1267081 us
  // vset_test_static_redundant takes 1399254 us
  // vset_test_dynamic takes 1382531 us
  //  const int outer_loop = 1000000;

  const int inner_loop = 8;
  long(*vset_tests[3])(long, const int, const int) = {&vset_test_static, &vset_test_static_redundant, &vset_test_dynamic};
  char* names[3] = {"vset_test_static", "vset_test_static_redundant", "vset_test_dynamic"};
  for (int i = 0; i < 3; i++) {
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    l = (*vset_tests[i])(l, outer_loop, inner_loop);

    gettimeofday(&stop, NULL);
    printf("%s takes %lu us\n", names[i], (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
  }
  printf("result: %ld\n", l);
}


int main(int argc, char *argv[]) {
  // perf_test();
  char buf[2048];
  // long i = vse8_test(buf+3);
  // long i = vse8_test((char*)&perf_test);
  // printf("i: %ld\n", i);

  // temp_tests();

  // cache_test_header_body();
  // cache_test_only_body();

  vset_tests();

  printf("PASSED\n");
  return 0;
}
