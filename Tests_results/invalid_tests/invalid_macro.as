; Macro issues


mcro macro2
clr r1
mcroend

mcro after char
    print
mcroend

mcro macro1
    mov r1, r2
    add r3, r4
mcroend chars


macro2
mcro macrox
.string "macro"
mcroend
