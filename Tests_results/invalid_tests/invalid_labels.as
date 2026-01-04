; Invalid label names
3Label:     mov r1, r2
bad@label:  add r3, r1
ThisLabelNameIsWayTooLong123abcde: sub r1, r2
mov:        prn r1
Label One:  stop
REPEAT:     .data 5
REPEAT:     .string "again"
.extern REPEAT
.entry REPEAT
:Invalid    mov r2, r3
goodLabel:  .data 7
Another:    .string "ok"
bad label:  rts
inv#char:   cmp r1, r2
repeat:     dec r3
.entry not_defined