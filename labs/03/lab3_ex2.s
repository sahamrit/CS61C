main: addu $t0, $zero, $s0
addu $t1, $zero, $s1
addu $t2, $t1, $t0
addu $t3, $t2, $t1
addu $t4, $t3, $t2
addu $t5, $t4, $t3
addu $t6, $t5, $t4
addu $t7, $t6, $t5
addu $a0,$0,$t7
li $v0,1 
syscall # syscall to print value stored in register $t0. See HELP section of MARS simulator for all these types of instructions
addi $v0,$0,10
syscall