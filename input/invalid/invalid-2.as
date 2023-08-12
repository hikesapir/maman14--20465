.entry LENGTH
.extern JONI


mcro num
18
endmcro

mov @r1 ,10
mov @r2 ,@r3
mov @r2 ,LENGTH
mov LENGTH ,@r3
mov LENGTH ,5
mov LENGTH ,JONI

num

; ignore it hahaha

cmp 5,10
cmp 5   ,@r4
cmp 5   ,LENGTH
cmp JONI   ,LENGTH
cmp JONI   ,12,
cmp JONI   ,@r5
cmp @r6   ,@r5
cmp @r6 ,JONI
cmp @r6 ,-39

mcro m2
    sub @r7, LENGTH
    bne STR
endmcro

label32   :  add @r1 ,10
add @r2 ,@r3
add @r2 ,LENGTH
add LENGTH ,@r3
add LENGTH ,5
add LENGTH ,JONI

sub @r1 ,10
sub @r2 ,@r3
sub @r2 ,LENGTH
sub LENGTH ,@r3
sub LENGTH ,+5
sub LENGTH ,JONI

sub ;did ignore?

m2

not JONI
not @r7

clr JONI
clr @r7

lea  @r2 ,LENGTH
lea  JONI ,LENGTH


inc JONI
inc @r7

dec JONI
dec @r7

STR: . string "abcdef"

jmp JONI
K: .data 22
jmp         @r7

bne JONI
bne @r7

red JONI
red @r7

prn: jsr JONI
jsr @r7

123not_valid: prn JONI
prn @r7
prn -56

LENGTH: .data 6,-9,15

rts
stop

K: .data 22
