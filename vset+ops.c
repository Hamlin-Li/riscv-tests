#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


extern void vset_e8_m8();
extern void vset_e16_m8();
extern void vset_e32_m8();
extern void vset_e64_m8();

extern char vops_char(char c);
extern short vops_short(short s);
extern int vops_int(int i);
extern long vops_long(long l);

long vset_test_static(long l, const int outer_loop, const int inner_loop) {

  for (int j = 0; j < outer_loop; j++) {
    vset_e8_m8();
    for (int i = 0; i < inner_loop; i++) {
      l = vops_char((char)l);
    }
    vset_e16_m8();
    for (int i = 0; i < inner_loop; i++) {
      l = vops_short((short)l);
    }
    vset_e32_m8();
    for (int i = 0; i < inner_loop; i++) {
      l = vops_int((int)l);
    }
    vset_e64_m8();
    for (int i = 0; i < inner_loop; i++) {
      l = vops_long((long)l);
    }
  }
}

long vset_test_static_condition(long l, const int outer_loop, const int inner_loop) {

  for (int j = 0; j < outer_loop; j++) {
    vset_e8_m8();
    for (int i = 0; i < inner_loop; i++) {
      if ((i%2) == (j%2)) { vset_e8_m8(); }
      l = vops_char((char)l);
    }
    vset_e16_m8();
    for (int i = 0; i < inner_loop; i++) {
      if ((i%2) == (j%2)) { vset_e16_m8(); }
      l = vops_short((short)l);
    }
    vset_e32_m8();
    for (int i = 0; i < inner_loop; i++) {
      if ((i%2) == (j%2)) { vset_e32_m8(); }
      l = vops_int((int)l);
    }
    vset_e64_m8();
    for (int i = 0; i < inner_loop; i++) {
      if ((i%2) == (j%2)) { vset_e64_m8(); }
      l = vops_long((long)l);
    }
  }
}

long vset_test_static_redundant(long l, const int outer_loop, const int inner_loop) {

  for (int j = 0; j < outer_loop; j++) {
    for (int i = 0; i < inner_loop; i++) {
      vset_e8_m8();
      l = vops_char((char)l);
    }
    for (int i = 0; i < inner_loop; i++) {
      vset_e16_m8();
      l = vops_short((short)l);
    }
    for (int i = 0; i < inner_loop; i++) {
      vset_e32_m8();
      l = vops_int((int)l);
    }
    for (int i = 0; i < inner_loop; i++) {
      vset_e64_m8();
      l = vops_long((long)l);
    }
  }
}

long vset_test_dynamic(long l, const int outer_loop, const int inner_loop) {

  for (int j = 0; j < outer_loop; j++) {
    for (int i = 0; i < inner_loop; i++) {
      vset_e8_m8();
      l = vops_char((char)l);
      vset_e16_m8();
      l = vops_short((short)l);
      vset_e32_m8();
      l = vops_int((int)l);
      vset_e64_m8();
      l = vops_long((long)l);
    }
  }
}

void vset_ops_test(const int outer_loop) {
  long l = 1;
  const int inner_loop = 8;
#define COUNT 4
  long(*vset_tests[COUNT])(long, const int, const int) = {&vset_test_static, &vset_test_static_condition, &vset_test_static_redundant, &vset_test_dynamic};
  char* names[COUNT] = {"vset_test_static", "vset_test_static_condition", "vset_test_static_redundant", "vset_test_dynamic"};
  for (int i = 0; i < COUNT; i++) {
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    l = (*vset_tests[i])(l, outer_loop, inner_loop);

    gettimeofday(&stop, NULL);
    printf("%s takes %lu us\n", names[i], (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
  }
  printf("result: %ld\n", l);
}

void vset_ops_tests() {
  long l = 1;
  // loop: 100000
  //  vset_test_static takes 126241 us
  //  vset_test_static_condition takes 137494 us
  //  vset_test_static_redundant takes 139417 us
  //  vset_test_dynamic takes 137705 us
  // loop: 1000000
  //  vset_test_static takes 1262085 us
  //  vset_test_static_condition takes 1374732 us
  //  vset_test_static_redundant takes 1393921 us
  //  vset_test_dynamic takes 1376919 us

  int out_loops[3] = {100000, 1000000, 10000000};
  for (int i = 0; i < sizeof(out_loops)/sizeof(int); i++) {
    vset_ops_test(out_loops[i]);
  }
}


