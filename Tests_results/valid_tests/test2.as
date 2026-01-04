; Test 2 - External labels used in cmp and prn

.extern EXTMACRO
.extern OUTSIDE
.entry ENTRYLABEL
.entry LOOPA

MAIN:   lea ENTRYLABEL, r1
        add r1, r2
        sub r2, r3
        cmp EXTMACRO, #4
        bne LOOPA
        jsr FUNCA
        stop

FUNCA: mov r2, r2
        inc r3
        dec r1
        clr r4
        jmp OUTSIDE
        rts

LOOPA: prn #3
        add r1, r1
        cmp r1, EXTMACRO
        jmp FUNCA
        stop

ENTRYLABEL: .data 10, -20, 30
TEXT1: .string "Hello ASM"