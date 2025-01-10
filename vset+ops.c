#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


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

void vset_ops_tests() {
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


