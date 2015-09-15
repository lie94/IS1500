  # labwork.S
  # Written 2015 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1 2015-2016.
  # The macros PUSH and POP are in the public domain.

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm

  # You can add your own code here
  #
delay:
  PUSH $ra
  beq $t0, $0, whileLoop
  nop
whileLoop:
  PUSH $s0
  PUSH $s1
  slt $t0, $0, $a0
  addi $a0, $a0, -1
  #Executing the following loop should take 1 ms
  addi $s0, $0, 1     # t1 = i
  addi $s1, $0, 15000   #  Max loops
  jal forLoop
  nop
  POP $s1
  POP $s0
  POP $ra
  jr $ra
  nop 
forLoop:
  slt $t0, $s0, $s1
  addi $s0, $s0, 1
  bne $t0, $0, forLoop
  nop
  jr $ra
  nop
  # you can write your code for subroutine "hexasc" below this line
  #
hexasc: 
  andi  $v0, $a0, 15
  slti  $t0, $v0, 10
  beq   $t0, $0, bokst
  addi  $v0, $v0, 0x30
  jr $ra
  nop
bokst:  
  addi  $v0, $v0, 0x37
  jr $ra
  nop
time2string:
  PUSH $s1
  PUSH $s2
  add $s1, $a0, $0  # Temporary save of a0 in t4
  add $s2, $ra, $0  # Save ra:s value
  add $a0, $a1, $0 
  #Add first digit [s]
  jal hexasc
  nop
  sb  $v0, 4($s1)
  srl $a1, $a1, 4
  add $a0, $a1, $0
  #Add second digit [s]
  jal hexasc
  nop
  sb  $v0, 3($s1)  
  srl $a1, $a1, 4
  add $a0, $a1, $0
  #Add colon
  add $t0, $0, 0x3a 
  sb  $t0, 2($s1)
  #Add third  [m]
  jal hexasc
  nop
  sb  $v0, 1($s1)  
  srl $a1, $a1, 4
  add $a0, $a1, $0
  #Add fourth   [m]
  jal hexasc
  nop
  sb  $v0, 0($s1)  
  srl $a1, $a1, 4
  add $a0, $a1, $0
  #Add null space
  addi $t1, $0, 0x00
  sb $t1, 5($s1)
  #Set ra to it's previous value 
  add $ra, $s2, $0
  POP $s2
  POP $s1
  jr $ra
  nop

