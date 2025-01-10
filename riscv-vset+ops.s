    .text                     # Start text section
    .balign 4                 # align 4 byte instructions by 4 bytes
    .global vset_e8_m8
vset_e8_m8:
    vsetivli x0, 8, e8, m8
ret

    .text                     # Start text section
    .balign 4                 # align 4 byte instructions by 4 bytes
    .global vset_e16_m8
vset_e16_m8:
    vsetivli x0, 8, e16, m8
ret

    .text                     # Start text section
    .balign 4                 # align 4 byte instructions by 4 bytes
    .global vset_e32_m8
vset_e32_m8:
    vsetivli x0, 8, e32, m8
ret

    .text                     # Start text section
    .balign 4                 # align 4 byte instructions by 4 bytes
    .global vset_e64_m8
vset_e64_m8:
    vsetivli x0, 8, e64, m8
ret

    .text                     # Start text section
    .balign 4                 # align 4 byte instructions by 4 bytes
    .global vaddx_char
vaddx_char:
    vid.v v16
    vadd.vx v8, v16, a0
    vredsum.vs v16, v8, v16
    vmv.x.s a0, v16
ret

    .text                     # Start text section
    .balign 4                 # align 4 byte instructions by 4 bytes
    .global vaddx_short
vaddx_short:
    vid.v v16
    vadd.vx v8, v16, a0
    vredsum.vs v16, v8, v16
    vmv.x.s a0, v16
ret

    .text                     # Start text section
    .balign 4                 # align 4 byte instructions by 4 bytes;
    .global vaddx_int
vaddx_int:
    vid.v v16
    vadd.vx v8, v16, a0
    vredsum.vs v16, v8, v16
    vmv.x.s a0, v16
ret

    .text                     # Start text section
    .balign 4                 # align 4 byte instructions by 4 bytes
    .global vaddx_long
vaddx_long:
    vid.v v16
    vadd.vx v8, v16, a0
    vredsum.vs v16, v8, v16
    vmv.x.s a0, v16
ret


