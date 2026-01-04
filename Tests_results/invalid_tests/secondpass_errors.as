.entry MISSINGLABEL
.extern EXTLABEL
.extern USEDEXTERN

MAIN:    mov r3, r4
         add r3, EXTLABEL
         jsr MISSINGFUNC
         jmp NONEXISTENT
         .entry NOTDEFINED

LABEL1:  .data 5, 10
         cmp #5, LABEL1
         prn UNDECLARED

         add r1, USEDEXTERN
         sub r2, EXTLABEL
         lea EXTLABEL, r3

NOTDEFINED: .string "hello"

; Using .entry for extern — should be an error
.entry EXTLABEL

; Duplicate .entry for the same label
.entry NOTDEFINED