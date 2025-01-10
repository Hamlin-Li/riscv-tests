    .text                     # Start text section
    .balign 4                 # align 4 byte instructions by 4 bytes
    # .global _start            # global
    .global cbo_zero # global
cbo_zero:
Loop1:
	cbo.zero 0(a0)
	addi a0, a0, 64
	addi a1, a1, -1
	bgt a1, x0, Loop1
ret


    .text                     # Start text section
    .balign 4                 # align 4 byte instructions by 4 bytes
    # .global _start            # global
    .global vector_zero # global
vector_zero:
	vsetivli        zero,8,e64,m2,ta,ma
	vmv.v.x v4, zero
Loop3:
	vse64.v v4, 0(a0)
        addi a0, a0, 64
        addi a1, a1, -1
        bgt a1, x0, Loop3
ret


    .text                     # Start text section
    .balign 4                 # align 4 byte instructions by 4 bytes
    # .global _start            # global
    .global plain_zero # global
plain_zero:
Loop2:
	sd x0, 0(a0)
	sd x0, 8(a0)
	sd x0, 16(a0)
	sd x0, 24(a0)
	sd x0, 32(a0)
	sd x0, 40(a0)
	sd x0, 48(a0)
	sd x0, 56(a0)
	addi a0, a0, 64
        addi a1, a1, -1
        bgt a1, x0, Loop2
ret
