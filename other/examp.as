MAIN:   add r3, LIST
LOOP:   prn #48

mcro amc
        cmp K, #-6
        bne &END
mcroend

mcro bmc
        lea STR, r6
        inc r6
        mov r3, K
        sub r1, r4
        bne END
mcroend

mcro cmc
        jmp &LOOP
        dec K
mcroend

        amc
        bmc

        prn #56
        dec K
        jmp &LOOP

END:    stop

STR:    .string "abcd"
LIST:   .data 6, -9
        .data -100
K:      .data 31

TEMP:   .data 0, 3, 7, -2
BUF:    .data 100, 200, -50
SOME:   .string "test_string"