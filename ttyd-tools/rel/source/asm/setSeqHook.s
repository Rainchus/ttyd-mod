.global startSeqSet
.global endSeqSet

startSeqSet:
stw %r5, 0x185C (%r13)

stwu %sp, -0x20 (%sp)
mflr %r0
stw %r0, 0x10 (%sp)
stw %r29, 0x14 (%sp)
stw %r30, 0x18 (%sp)
stw %r31, 0x1C (%sp)

bl set_seq_hook

lwz %r0, 0x10 (%sp)
lwz %r29, 0x14 (%sp)
lwz %r30, 0x18 (%sp)
lwz %r31, 0x1C (%sp)
mtlr %r0
addi %sp, %sp, 0x20


endSeqSet:
b 0