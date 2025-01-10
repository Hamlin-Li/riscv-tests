#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

extern long vle32_test(long);
extern long test_temp(long);
extern long test_temp_2(long);
extern long test_temp_x(long);
extern long test_temp_2_x(long);

extern long vclmul_test(void);

void vclmul() {
  long x = 0;
  if (0) {
    x = vle32_test(100);
  } else {
    x = vclmul_test();
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
