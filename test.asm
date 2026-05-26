!to "test.prg", cbm
* = $1000
        LDA #$00
        STA $00
        LDA #$01
        STA $01
        LDA #$0A
        STA $03

loop
        LDA $00
        CLC
        ADC $01
        STA $02

        LDA $01
        STA $00

        LDA $02
        STA $01

        DEC $03
        BNE loop

halt
        NOP
        JMP halt