# pour que ces etiquettes soient visibles des autres fichiers par le linker
.global sqrt
 
    
  /* This is important for an assembly programmer. This
* directive tells the assembler that don't optimize
* the order of the instructions as well as don't insert
* 'nop' instructions after jumps and branches.
*/
  
.set noreorder  
  
.data
    
.text

 /* t0: initial guess
    t1: division result of S/x0 
    t2: addition result of x0+(S/x0) 
    t3: division result of (x0+(S/x0))/2 
    t4: substraction result of x_n - x_(n-1) 
    t5: precision value*/
.ent sqrt
sqrt:
error: .float 0.5
    lw $t5, error($0)
    div $a0, 2 /* Initial guess = input/2 */
    mflo $t0 
   
loop:
    /* Raffinement */
    div $a0, $t0 /* S/x0 */
    mflo $t1 
    
    add $t2, $t0, $t1 /* x0+(S/x0) */
    
    div $t2, 2 /* (x0 + (S/x0))/2 */ 
    mflo $t3
    
    /* Calcul de la condition de convergence */
    sub $t4, $t0, $t3 /* x_n - x_(n-1) */
    move $t0, $t3 /* Assign new iteration value */
    bge $t4, $t5, loop /* Loop if x_n - x_(n-1) > E */
    
    move $v0, $t3
 
.end sqrt
            