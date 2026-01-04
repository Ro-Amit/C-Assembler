;firstpass errors

mcro macro123
    add r4,r6
mcroend

mov r1, r2
add r2, r3
add r3, #6
macro123: add #4,r2
stop

.what problem
.NONO problem

DATA1: .data 4, 5, 6
STR1:  .string "oops
K:     .data 8, , 9

INVALID!LABEL: add r1, r2
dupLabel: .data 7
dupLabel: .string "repeat"

WRONGINST r3, r4
add r1, r2, r3
stop
.extern
.extern EXTLABEL
.entry
.entry dupLabel
mov r4, K
rts
.extern EXTLABEL
