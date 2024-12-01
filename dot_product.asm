section .text
    bits 64
    default rel
    global dot_product

dot_product:
    ; n@rcx, vec1@rdx, vec2@r8, vec3@r9
    ; float return value: parameters are xmm0 xmm1 xmm2 xmm3
    push rsi ; good practice according to ppt
    push rbp 
    xorps xmm0, xmm0 ; performs a bitwise XOR operation on packed single precision floating-point values

L1:
    test rcx, rcx ; clear rcx
    jz done
    movsd xmm1, [rdx] ; move scalar double-precision floating-point value from memory location pointed to by rdx to xmm1 https://www.felixcloutier.com/x86/movsd   
    movsd xmm2, [r8] ; move vec2 content to xmm2
    mulsd xmm1, xmm2 ; multiply the low double precision floating-point value https://www.felixcloutier.com/x86/mulsd
    addsd xmm0, xmm1 ; add scalar double precision floating-point values https://www.felixcloutier.com/x86/addsd
    add rdx, 8 ; move to next i in vec1(rdx) aka next value in the vector
    add r8, 8  ; move to next i in vec2(r8)
    dec rcx ; decrement size n(rcx)
    jmp L1

done:
    movsd [r9], xmm0 ; move the result to r9
    pop rbp ; good practice according to ppt
    pop rsi
    ret
