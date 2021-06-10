# pour que ces etiquettes soient visibles des autres fichiers par le linker
.global sqrt_babylon
.global magnitude
  
.set noreorder  
  
.data
    
.text

/*************************************************************************/
  
 /* a0: value to find the square root of
    a1: precision
    t0: MSB comparison value and initial guess
    t1: division result of S/x0 
    t2: addition result of x0+(S/x0) 
    t3: division result of (x0+(S/x0))/2 
    t4: substraction result of x_n - x_(n-1) 
    t5: position counter 
    t6: dummy value for bit shifting */
.ent sqrt_babylon
sqrt_babylon:
    li $t5, 0
    li $t6, 1
    
    /* Initial guess calculation */
find_msb:
    sll $t0, $t6, $t5	    # shift left MSB comparison value
    add $t5, $t5, 1	    # p++ 
    beq $t5, 31, msb_found  # escape loop if counter reaches 31 (bit 30 is MSB)
    nop
    blt $t0, $a0, find_msb  # Loop until MSB greater than value
    nop
    
msb_found:    
    /* MSB position found */ 
    sub $t5, $t5, 1 # Remove excess count from position counter
    div $t0, $t5, 2 # q=p/2 
    
    addi $t6, $t6, 1 # dummy value=2 
    sll $t0, $t6, $t0 # x0=2^q 
    
iterate:
    # Iteration until value is found 
    div $t1, $a0, $t0 # S/x0 
    
    add $t2, $t0, $t1 # x0+(S/x0) 
    
    div $t3, $t2, 2 # (x0 + (S/x0))/2 
    
    # Calcul de la condition de convergence 
    sub $t4, $t0, $t3 # x_n - x_(n-1) */
    move $t0, $t3 # Assign new iteration value 
    
    bge $t4, $a1, iterate # Loop if x_n - x_(n-1) > E 
    nop
    
    move $v0, $t3 # return value
    
    jr $ra
    nop
    
.end sqrt_babylon
    
/************************************************************************/
            
 /* a0: 12-bit ax
    a1: 12-bit ay
    a2: 12-bit az */
.ent magnitude
magnitude:
    addi $sp, $sp, -4 # reserve word on stack
    sw $ra, 0($sp)    # save return address
    
    mul $a0, $a0, $a0 # ax^2
    mul $a1, $a1, $a1 # ay^2
    mul $a2, $a2, $a2 # az^2
    
    add $a0, $a0, $a1 # ax^2 + ay^2 + az^2
    add $a0, $a0, $a2
    
    li $a1, 1	      # precision = 1
     
    jal sqrt_babylon  # compute square root
    nop
    
    lw $ra, 0($sp)    # restore return address
    addi $sp, $sp, 4  # release stack memory
    
    jr $ra
    nop
    
.end magnitude
    