#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

extern void temp_tests();

extern void vset_ops_tests();

extern void zero_perf_test();

extern void cache_test_header_body();
extern void cache_test_only_body();


int main(int argc, char *argv[]) {
  if (1) {
    char buf[2048];
    long i = vse8_test(buf+3);
    printf("i: %ld\n", i);

    i = vse8_test((char*)&zero_perf_test);
    printf("i: %ld\n", i);
  }

  if (1) {
    temp_tests();
  }

  if (1) {
    printf("\n\n=============== zero_perf_test ================\n");
    zero_perf_test();

    printf("\n\n=============== cache_test (header, body) ================\n");
    cache_test_header_body();
    cache_test_only_body();
  }

  if (1) {
    printf("\n\n=============== vset+ops ================\n");
    vset_ops_tests();
  }

  printf("\nDONE\n");
  return 0;
}
