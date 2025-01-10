    .text                     # Start text section
    .balign 4                 # align 4 byte instructions by 4 bytes
    # .global _start            # global
    .global test_temp # global
test_temp:
    # ctz(Rd, Rs);
    ctz a0, a0
    # andi(tmp1, Rd, step - 1);
    andi t0, a0, 7
    # sub(Rd, Rd, tmp1);
    sub a0, a0, t0
ret

    .text                     # Start text section
    .balign 4                 # align 4 byte instructions by 4 bytes
    # .global _start            # global
    .global test_temp_x # global
test_temp_x:
    # ctz(Rd, Rs);
    ctz a0, a0
    # andi(tmp1, Rd, step - 1);
    andi a0, a0, -7
    # sub(Rd, Rd, tmp1);
    # sub a0, a0, t0
ret


    .text                     # Start text section
    .balign 4                 # align 4 byte instructions by 4 bytes
    # .global _start            # global
    .global test_temp_2 # global
test_temp_2:
    # ctz(Rd, Rs);
    ctz a0, a0
    # andi(tmp1, Rd, step - 1);
    andi t0, a0, 15
    # sub(Rd, Rd, tmp1);
    sub a0, a0, t0
ret

    .text                     # Start text section
    .balign 4                 # align 4 byte instructions by 4 bytes
    # .global _start            # global
    .global test_temp_2_x # global
test_temp_2_x:
    # ctz(Rd, Rs);
    ctz a0, a0
    # andi(tmp1, Rd, step - 1);
    andi a0, a0, -15
    # sub(Rd, Rd, tmp1);
    # sub a0, a0, t0
ret
