  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0,2
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop
delay:
	PUSH $ra
	PUSH $s0
	PUSH $s1
whileLoop:	
	slt $t0, $0, $a0
	beq $t0, $0, end
	nop
	addi $a0, $a0, -1
	#Executing the following loop should take 1 ms
	addi $s0, $0, 1   	# t1 = i
	addi $s1, $0, 4000 	#  Max loops
	j forLoop
	nop
end: 
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
	j whileLoop
	nop
  # you can write your code for subroutine "hexasc" below this line
  #
hexasc: 
	andi 	$v0, $a0, 15
	slti 	$t0, $v0, 10
	beq 	$t0, $0, bokst
	addi 	$v0, $v0, 0x30
	jr $ra
	nop
bokst:	
	addi 	$v0, $v0, 0x37
	jr $ra
	nop
time2string:
	PUSH $s1
	PUSH $s2
	add $s1, $a0, $0 	# Temporary save of a0 in t4
	add $s2, $ra, $0	# Save ra:s value
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
	#Add third 	[m]
	jal hexasc
	nop
	sb  $v0, 1($s1)  
	srl $a1, $a1, 4
	add $a0, $a1, $0
	#Add fourth 	[m]
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
