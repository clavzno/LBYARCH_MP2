section .text
    bits 64
    default rel
    global dot_product

dot_product:
    movsd xmm0, 0

.L1:
    test rcx, rcx
    jz .done
    movsd xmm1, [rdx]
    movsd xmm2, [r8]
    mulsd xmm1, xmm2
    addsd xmm0, xmm1
    add rdx, 8
    add r8, 8
    dec rcx
    jmp .L1

.done:
    movsd [r9], xmm0
    ret
