#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

extern void cbo_zero(long la[], int cnt);
extern void plain_zero(long la[], int cnt);
extern void vector_zero(long la[], int cnt);



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

void zero_perf_test() {
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

