; Generalnie ten assembler jest strasznie głupie ale działą
; Warunek konieczny, to ustawienie skoku do konkretnie określinej
; komurki pamięci
jump_to_add 256

; Nie można wywoływać funkcji które są niżej. Jeszcze nie wiem jak ale to uzysjam
addition:
ldr r0 15
ldr r1 5
add r0 r1
ret

. 256
main:
UART_send 'r'
call addition
end
stop