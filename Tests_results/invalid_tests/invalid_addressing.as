; Invalid addressing modes

mov &LABEL, r3
cmp r1, &r3
lea #5, r3
clr #7
inc &LABEL
red #12
prn &VALUE
bne #9
jsr #label
jmp r3
jsr r4
add &LABEL, &TARGET
cmp #4, &r5