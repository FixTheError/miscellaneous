#Ryan Whitt, project 1, program 1, last 3: 615#
	.data

prmt1: .asciiz "\nPlease enter your current driving speed (1-200 MPH)"

prmt2: .asciiz "\nPlease enter the speed limit of your current road (15-70 MPH)"

msg1: .asciiz "\n$120 fine"

msg2: .asciiz "\n$140 fine"

msg3: .asciiz "\nClass B misdemeanor carrying up to 6 months in jail and a max of $1500 in fines"

msg4: .asciiz "\nClass A misdemeanor carrying up to 1 year in jail and a max of $2500 in fines"

msg5: .asciiz "\nYou are driving at a safe speed, but not safely since you're distracted by the computer."

	.text

	.globl main

main:

p1:	li $v0, 4

	la $a0, prmt1

	syscall

	li $v0, 5

	syscall

	move $s0, $v0
	blt $s0, 1, p1

	bgt $s0, 200, p1



p2:	li $v0, 4

	la $a0, prmt2

	syscall

	li $v0, 5

	syscall

	move $s1, $v0
	blt $s1, 15, p2

	bgt $s1, 70, p2

	sub $s2, $s0, $s1
	ble $s2, 0, m5

	ble $s2, 20, m1

	ble $s2, 25, m2

	ble $s2, 34, m3

	bgt $s2, 34, m4


m1:	li $v0, 4

	la $a0, msg1

	syscall

	j end


m2:	li $v0, 4

	la $a0, msg2

	syscall

	j end


m3:	li $v0, 4

	la $a0, msg3

	syscall

	j end


m4:	li $v0, 4

	la $a0, msg4

	syscall

	j end


m5:	li $v0, 4

	la $a0, msg5
	syscall

	j end


end:	jr $31





