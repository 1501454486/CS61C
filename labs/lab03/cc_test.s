.globl simple_fn naive_pow inc_arr

.data
failure_message: .asciiz "Test failed for some reason.\n"
success_message: .asciiz "Sanity checks passed! Make sure there are no CC violations.\n"
array:
    .word 1 2 3 4 5
exp_inc_array_result:
    .word 2 3 4 5 6

.text
main:
    # We test our program by loading a bunch of random values
    # into a few saved registers - if any of these are modified
    # after these functions return, then we know calling
    # convention was broken by one of these functions

    li s0, 2623
    li s1, 2910

    # ... skipping middle registers so the file isn't too long
    # If we wanted to be rigorous, we would add checks for
    # s2-s20 as well
    li s11, 134

    # Now, we call some functions
    # simple_fn: should return 1

    jal simple_fn # Shorthand for "jal ra, simple_fn"
    li t0, 1
    bne a0, t0, failure

    # naive_pow: should return 2 ** 7 = 128
    li a0, 2
    li a1, 7
    jal naive_pow
    li t0, 128
    bne a0, t0, failure

    # inc_arr: increments "array" in place
    la a0, array
    li a1, 5
    jal inc_arr

    jal check_arr # Verifies inc_arr and jumps to "failure" on failure

    # Check the values in the saved registers for sanity
    li t0, 2623
    li t1, 2910
    li t2, 134
    bne s0, t0, failure
    bne s1, t1, failure
    bne s11, t2, failure

    # If none of those branches were hit, print a message and exit normally

    li a0, 4
    la a1, success_message
    ecall
    li a0, 10
    ecall

# Just a simple function. Returns 1.
#
# FIXME Fix the reported error in this function (you can delete lines
# if necessary, as long as the function still returns 1 in a0).
simple_fn:
    li a0, 1
    ret

# Computes a0 to the power of a1.
# This is analogous to the following C pseudocode:
#
# uint32_t naive_pow(uint32_t a0, uint32_t a1) {
#     uint32_t s0 = 1;
#     while (a1 != 0) {
#         s0 *= a0;
#         a1 -= 1;
#     }
#     return s0;
# }
#
# FIXME There's a CC error with this function!
# The big all-caps comments should give you a hint about what's
# missing. Another hint: what does the "s" in "s0" stand for?

naive_pow:
    # BEGIN PROLOGUE
    addi sp, sp, -52
    sw ra, 0(sp)
    sw s0, 4(sp)            # store s0;
    sw s1, 8(sp)            # store s1;
    sw s2, 12(sp)            # store s2;
    sw s3, 16(sp)            # store s3;
    sw s4, 20(sp)            # store s4;
    sw s5, 24(sp)            # store s5;
    sw s6, 28(sp)            # store s6;
    sw s7, 32(sp)            # store s7;
    sw s8, 36(sp)            # store s8;
    sw s9, 40(sp)            # store s9;
    sw s10, 44(sp)            # store s10;
    sw s11, 48(sp)            # store s11;
    # END PROLOGUE

    li s0, 1
naive_pow_loop:
    beq a1, zero, naive_pow_end
    mul s0, s0, a0
    addi a1, a1, -1
    j naive_pow_loop
naive_pow_end:
    mv a0, s0
    # BEGIN EPILOGUE
    lw s11, 48(sp)          # load s11;
    lw s10, 44(sp)          # load s10;
    lw s9, 40(sp)           # load s9;
    lw s8, 36(sp)           # load s8;
    lw s7, 32(sp)           # load s7;
    lw s6, 28(sp)           # load s6;
    lw s5, 24(sp)           # load s5;
    lw s4, 20(sp)           # load s4;
    lw s3, 16(sp)           # load s3;
    lw s2, 12(sp)            # load s2;
    lw s1, 8(sp)            # load s1;
    lw s0, 4(sp)            # load s0;
    lw ra, 0(sp)
    addi sp, sp, 52         # sp += 52;
    # END EPILOGUE
    ret

# Increments the elements of an array in-place.
# a0 holds the address of the start of the array, and a1 holds
# the number of elements it contains.
#
# This function calls the "helper_fn" function, which takes in an
# address as argument and increments the 32-bit value stored there.
inc_arr:
    # BEGIN PROLOGUE
    #
    # FIXME What other registers need to be saved?
    #

    addi sp, sp, -52
    sw ra, 0(sp)
    sw s0, 4(sp)            # store s0;
    sw s1, 8(sp)            # store s1;
    sw s2, 12(sp)            # store s2;
    sw s3, 16(sp)            # store s3;
    sw s4, 20(sp)            # store s4;
    sw s5, 24(sp)            # store s5;
    sw s6, 28(sp)            # store s6;
    sw s7, 32(sp)            # store s7;
    sw s8, 36(sp)            # store s8;
    sw s9, 40(sp)            # store s9;
    sw s10, 44(sp)            # store s10;
    sw s11, 48(sp)            # store s11;
    # END PROLOGUE

    mv s0, a0 # Copy start of array to saved register
    mv s1, a1 # Copy length of array to saved register
    li t0, 0 # Initialize counter to 0

inc_arr_loop:
    beq t0, s1, inc_arr_end
    slli t1, t0, 2 # Convert array index to byte offset
    add a0, s0, t1 # Add offset to start of array

    # Prepare to call helper_fn
    #
    # FIXME Add code to preserve the value in t0 before we call helper_fn
    # Hint: What does the "t" in "t0" stand for?
    # Also ask yourself this: why don't we need to preserve t1?
    #
    
    addi sp, sp, -4         # sp -= 4;
    sw t0, 0(sp)            # store t0 to stack;

    jal helper_fn
    # Finished call for helper_fn

    lw t0, 0(sp)            # load t0;
    addi sp, sp, 4

    addi t0, t0, 1 # Increment counter
    j inc_arr_loop
inc_arr_end:
    # BEGIN EPILOGUE
    lw s11, 48(sp)          # load s11;
    lw s10, 44(sp)          # load s10;
    lw s9, 40(sp)           # load s9;
    lw s8, 36(sp)           # load s8;
    lw s7, 32(sp)           # load s7;
    lw s6, 28(sp)           # load s6;
    lw s5, 24(sp)           # load s5;
    lw s4, 20(sp)           # load s4;
    lw s3, 16(sp)           # load s3;
    lw s2, 12(sp)            # load s2;
    lw s1, 8(sp)            # load s1;
    lw s0, 4(sp)            # load s0;
    lw ra, 0(sp)
    addi sp, sp, 52         # sp += 48;
    # END EPILOGUE
    ret

# This helper function adds 1 to the value at the memory address in a0.
# It doesn't return anything.
# C pseudocode for what it does: "*a0 = *a0 + 1"
#
# FIXME This function also violates calling convention, but it might not
# be reported by the Venus CC checker (try and figure out why).
# You should fix the bug anyway by filling in the prologue and epilogue
# as appropriate.

helper_fn:
    # BEGIN PROLOGUE
    addi sp, sp, -4
    sw s0, 0(sp)            # store s0 to stack;
    # END PROLOGUE

    lw t1, 0(a0)
    addi s0, t1, 1
    sw s0, 0(a0)
    
    # BEGIN EPILOGUE
    lw s0, 0(sp)            # load s0;
    addi sp, sp, 4
    # END EPILOGUE
    ret

# YOU CAN IGNORE EVERYTHING BELOW THIS COMMENT

# Checks the result of inc_arr, which should contain 2 3 4 5 6 after
# one call.
# You can safely ignore this function; it has no errors.
check_arr:
    la t0, exp_inc_array_result
    la t1, array
    addi t2, t1, 20 # Last element is 5*4 bytes off
check_arr_loop:
    beq t1, t2, check_arr_end
    lw t3, 0(t0)
    lw t4, 0(t1)
    bne t3, t4, failure
    addi t0, t0, 4
    addi t1, t1, 4
    j check_arr_loop
check_arr_end:
    ret
    

# This isn't really a function - it just prints a message, then
# terminates the program on failure. Think of it like an exception.
failure:
    li a0, 4 # String print ecall
    la a1, failure_message
    ecall
    li a0, 10 # Exit ecall
    ecall
    
