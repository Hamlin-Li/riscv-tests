    .text                     # Start text section
    .balign 4                 # align 4 byte instructions by 4 bytes
    # .global _start            # global
    .global vclmul_test # global
vclmul_test:
   li      a6,4
   vsetvli t0,a6,e64,m2,tu,mu
   vclmul.vv v2, v4, v6
   vclmulh.vv v2, v4, v6
   mv a0, a6
ret
