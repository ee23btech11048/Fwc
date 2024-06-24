.include "/data/data/com.termux/files/home/m328Pdef.inc"

.EQU SEGMENT_PORT = PORTD
.EQU DIGIT_PORT = PORTB
.EQU SEGMENT_DDR = DDRD
.EQU DIGIT_DDR = DDRB

start:
    ldi r24, 0b11111100
    out SEGMENT_DDR, r24
    ldi r24, 0b00000001
    out DIGIT_DDR, r24

    ldi r17, 1
    ldi r18, 2
    ldi r19, 10
    ldi r20, 100

    sub r18, r17

    mov r21, r20
    ldi r22, 0
    rcall divide
    mov r20, r22

    mul r18, r20
    mov r23, r0

    cpi r23, 10
    breq display_10
    cpi r23, 40
    breq display_40
    cpi r23, 50
    breq display_50
    cpi r23, 60
    breq display_60
    rjmp start

display_10:
    ldi r16, 0b11100100
    out SEGMENT_PORT, r16
    ldi r16, 0b00000001
    out DIGIT_PORT, r16
    rjmp start

display_40:
    ldi r16, 0b01100110
    out SEGMENT_PORT, r16
    ldi r16, 0b00000000
    out DIGIT_PORT, r16
    rjmp start

display_50:
    ldi r16, 0b01101101
    out SEGMENT_PORT, r16
    ldi r16, 0b00000001
    out DIGIT_PORT, r16
    rjmp start

display_60:
    ldi r16, 0b00001000
    out SEGMENT_PORT, r16
    ldi r16, 0b00000000
    out DIGIT_PORT, r16
    rjmp start

divide:
    clr r22
div_loop:
    cp r21, r19
    brlo div_done
    sub r21, r19
    inc r22
    rjmp div_loop
div_done:
    ret
