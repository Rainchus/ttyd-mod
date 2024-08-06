.global start_enemy_hp
.global end_enemy_hp

.global start_enemy_hp_2
.global end_enemy_hp_2

start_enemy_hp:
    stwu %sp, -16(%sp)
    stw %r0, 20(%sp)
    stmw %r30, 8(%sp)
    mr %r30, %r3
    mr %r31, %r4
    lwz %r6, 0(%r5)
    cmplwi %r6, 171
    lha %r6, 8(%r5)
    bgt- loc_0x48

    mr %r3, %r6

    lis %r4, HP_Multiplier@ha
    lwz %r4, HP_Multiplier@l(%r4)

    mullw %r3, %r3, %r4
    li %r4, 100
    divw %r3, %r3, %r4

    sth %r3, 10(%r5)
    mr %r6, %r3

    loc_0x48:
    mr %r5, %r6
    mr %r3, %r30
    mr %r4, %r31
    lmw %r30, 8(%sp)
    lwz %r0, 20(%sp)
    addi %sp, %sp, 0x10
    end_enemy_hp:
    b 0


start_enemy_hp_2:
    lwz %r6, 0(%r5)
    cmplwi %r6, 171
    bgt- loc_0x14
    lha %r5, 10(%r5)
    b loc_0x18

    loc_0x14:
    lha %r5, 8(%r5)
    loc_0x18:
    end_enemy_hp_2:
    b 0
