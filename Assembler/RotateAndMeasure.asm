ldr r0 180
ldr r1 3

loop:
ULTRASONIC_ROTATE r0
ULTRASONIC_MEASURE r3
sub r0 r1

UART_send 'r'
UART_send '0'
UART_send ':'
UART_send_int r0

UART_send 'r'
UART_send '3'
UART_send ':'
UART_send_int r3
skip_if r0
jump_to_add loop
end
