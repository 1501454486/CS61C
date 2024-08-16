.globl factorial

.data
n: .word 8

.text
main:
    la t0, n
    lw a0, 0(t0)
    jal ra, factorial

    addi a1, a0, 0
    addi a0, x0, 1
    ecall # Print Result

    addi a1, x0, '\n'
    addi a0, x0, 11
    ecall # Print newline

    addi a0, x0, 10
    ecall # Exit

factorial:
    # YOUR CODE HERE
    # Handle base_case for 0!
    beq a0, x0, base_case   # if n == 0;
    
    addi t0, a0, -1      # int i = n-1;
loop:
    beq x0, t0, exit    # while(i)
    mul a0, a0, t0      # n *= i;
    addi t0, t0, -1     # i--;
    j loop
exit:
    jr ra
    
base_case:
    addi a0, a0, 1      # n = 1;
    jr ra               # return;