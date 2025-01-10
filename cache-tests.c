#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


extern void cbo_zero(long la[], int cnt);


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


