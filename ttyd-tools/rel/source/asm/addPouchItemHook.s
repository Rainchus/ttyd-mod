.global startAddPouch
.global endAddPouch

startAddPouch:
stwu %sp, -0x18 (%sp)
mflr %r0
stw %r0, 0x10 (%sp)
stw %r31, 0x14 (%sp)
mr %r31, %r3
bl key_item_func_add

lwz %r0, 0x10 (%sp)
lwz %r31, 0x14 (%sp)
mtlr %r0
addi %sp, %sp, 0x18

stwu %sp, -0x20 (%sp)
endAddPouch:
b 0