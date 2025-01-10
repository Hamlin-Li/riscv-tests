    .text                     # Start text section
    .balign 4                 # align 4 byte instructions by 4 bytes
    # .global _start            # global
    .global vse8_test # global
vse8_test:
	vsetvli a1,a5,e8,m8,tu,mu
	vse8.v  v0,(a0)
	mv a0, a1
ret
