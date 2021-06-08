# pour que ces etiquettes soient visibles des autres fichiers par le linker
.global sqrt_babylon
 
    
  /* This is important for an assembly programmer. This
* directive tells the assembler that don't optimize
* the order of the instructions as well as don't insert
* 'nop' instructions after jumps and branches.
*/
  
.set noreorder  
  
.data
    
.text


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
    sll $t0, $t6, $t5 /* shift left MSB comparison value*/
    add $t5, $t5, 1 /* p++ */
    blt $t0, $a0, find_msb /* Loop until MSB greater than value*/
    nop
    
    /* MSB position found */ 
    sub $t5, $t5, 1 /* Remove 1 from p position counter */
    div $t0, $t5, 2 /* q=p/2 */
    
    addi $t6, $t6, 1 /* dummy value=2 */
    sll $t0, $t6, $t0 /* x0=2^q */
    
iterate:
    /* Iteration until value is found */
    div $t1, $a0, $t0 /* S/x0 */
    
    add $t2, $t0, $t1 /* x0+(S/x0) */
    
    div $t3, $t2, 2 /* (x0 + (S/x0))/2 */ 
    
    /* Calcul de la condition de convergence */
    sub $t4, $t0, $t3 /* x_n - x_(n-1) */
    move $t0, $t3 /* Assign new iteration value */
    
    bge $t4, $a1, iterate /* Loop if x_n - x_(n-1) > E */
    nop
    
    move $v0, $t3
 
.end sqrt_babylon
            