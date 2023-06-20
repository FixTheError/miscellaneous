
.data

p1: .asciiz "\nEnter the first number (0-50)"

p2: .asciiz "\nEnter the second number(0-50)"

err0: .asciiz "\nYour number is less than 0"

err50: .asciiz "\nYour input is greater than 50"

.text

.globl main
main:

	li $s4,0
	li $s5,50
	li $s3,0


pr1:
	li $v0,4

	la $a0,p1

	syscall

	li $v0,5

	syscall

	move $s2,$v0

	blt $s2,$s4,errl0

	bgt $s2,$s5,errg50


pr2:
	li $v0,4

	la $a0,p2

	syscall

	li $v0,5

	syscall

	move $s1,$v0

	blt $s1,$s4,errl0

	bgt $s1,$s5,errg50

	beq $s2,$s4,zero

	beq $s1,$s4,zero


loop:	
	add $s0,$s0,$s2
	addi $s3,$s3,1

	blt $s3,$s1,loop


print:
	li $v0,1

	move $a0,$s0
	syscall

	jr $31


errl0:
	li $v0,4

	la $a0,err0

	syscall

	blt $s2,$s4,pr1

	blt $s1,$s4,pr2


errg50:
	li $v0,4

	la $a0,err50

	syscall

	bgt $s2,$s5,pr1

	bgt $s1,$s5,pr2


zero:
	li $s0,0

	j print
