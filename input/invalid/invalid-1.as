.entry LENGTH
.extern W
MA IN: mov @r3 ,LENGTHT
LOOP : jmp L1
prn -5.3
bne W
sub @r1, @r4
.extern W
bne L3
L1: inc K
.entry LOOP
jmp W
END: stop
STR: .string “abcdef”
LENGTH: .data 6,-9,15
K: .data 22
.extern L3