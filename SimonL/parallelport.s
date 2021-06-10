# pour que ces etiquettes soient visibles des autres fichiers par le linker
.global parallelPortSend
  
.set noreorder  
  
.data
    
.text

 /* a0: 4-bit data, right-aligned
    t0: shifted input value
    t1: PORTC data buffer for operations
    t2: PORTG data buffer for operations
    t3: register for shifting intermediate parity calculations
    t4: parity value
    */
.ent parallelPortSend
parallelPortSend:
    /* Input sanitizing */
    andi $t0, $a0, 0x0F	    # clear upper nibble from input
    
    /* Calculate parity */
    srl $t3, $t0, 2	    # right-shift by half input width (2 bits)
    xor $t4, $t0, $t3	    # xor with shifted value
    srl $t3, $t4, 1	    # right-shift by half previous result width (1 bit)
    xor $t4, $t4, $t3	    # xor again
    andi $t4, $t4, 1	    # clear all bits but LSB
    
    /* Changing bus value */
    lw $t1, PORTC($0)	    # load PORTC current value
    sll $t0, 1		    # align input data (since output is PORTC[1..4]
    andi $t1, $t1, 0xFFE1   # clear bit 1..4
    or $t1, $t1, $t0	    # set bits according to input
    sw $t1, LATC($0)	    # set new PORTC value
    nop
    
    /* Output strobe and parity bit */
    lw $t2, PORTG($0)	    # load PORTG current value
    ori $t2, $t2, 0x40	    # set strobe high (PORTG6)
    sll $t4, 7		    # align parity bit with destination (PORTG7)
    andi $t2, $t2, 0xFF7F   # clear parity bit in register
    or	$t2, $t2, $t4	    # set parity bit according to calculated value in register
    sw $t2, LATG($0)	    # set new PORTG value
    nop
    
    /* Wait for ACK */
    ackloop:
    /*lw $t2, PORTG($0)	    # load PORTG current value*/
    lw $t2, PORTB($0)	    # load PORTB for right BTN
    andi $t2, $t2, 0x0100   # clear every bit except ACK (PORTG8)
    beqz $t2, ackloop	    # test if ACK bit is set, loop if it isn't
    nop
    
    /* Reset strobe */
    lw $t2, PORTG($0)	    # load PORTG current value
    andi $t2, $t2, 0xFFBF   # clear strobe bit in register
    sw $t2, LATG($0)	    # set new PORTG value
    nop
    
    jr $ra
    nop
.end parallelPortSend
 

