!to "test.prg", cbm
* = $1000
lda #$00
sta $42

start
inc $42
ldx $42
inx
jmp start