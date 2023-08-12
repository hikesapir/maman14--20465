.entry LENGTH
.extern W
.extern JONI
.entry KING
MAIN: mov @r3 ,LENGTH
red: add 0 JONI
mcro m2
    sub @r7, LENGTH
    bne STR
endmcro
LOOP: jmp L1
prn -5
bne W
NOT JONI
cmp JONI,       ,QUEEN
red  @r9
add 0 ,JONI
sub @r1, @r4
mcro m1 
    sub @r1, @r4
    bne END
endmcro
.entry QUEEN
m2
bne L3
L1: inc K
.entry LOOP
lea 1r @,HELLO
jmp W
m1
clr QUEEN
END: stop
STR: .string "abcdef"
LENGTH: .data 6,-9,15
QUEEN .string 'sapir'
K: .data 22
.extern L3