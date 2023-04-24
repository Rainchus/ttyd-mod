.global startGSWF
.global endGSWF

startGSWF:
stwu %sp, -0x18 (%sp)
mflr %r0
stw %r0, 0x10 (%sp)
stw %r31, 0x14 (%sp)
mr %r31, %r3

bl set_GSWF_Hook

srawi %r4, %r31,5

lwz %r0, 0x10 (%sp)
lwz %r31, 0x14 (%sp)
mtlr %r0
addi %sp, %sp, 0x18
endGSWF:
b 0