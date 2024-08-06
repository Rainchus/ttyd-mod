.global start_enemy_atk
.global end_enemy_atk

start_enemy_atk:
    stwu %sp, -48(%sp)
    stw %r0, 44(%sp)
    mflr %r0
    stw %r0, 52(%sp)
    stmw %r26, 8(%sp)
    mr %r31, %r29
    lwz %r30, 4(%r20)
    cmpwi %r30, 0xAB
    bgt+ loc_0x74
    lhz %r30, 10(%r23)
    cmpwi %r30, 0x0
    bne- loc_0x74
    cmpwi %r31, 0x0
    ble- loc_0x74

    lis %r30, Atk_Multiplier@ha
    lwz %r30, Atk_Multiplier@l(%r30)
    cmpwi %r30, 0x0
    ble+ loc_0x5C
    cmpwi %r30, 0x1388
    bgt- loc_0x5C
    mullw %r31, %r31, %r30
    addi %r31, %r31, 0x32
    li %r30, 0x64
    divw %r31, %r31, %r30

    loc_0x5C:
    cmpwi %r31, 0x1
    bge+ loc_0x68
    li %r31, 0x1

    loc_0x68:
    cmpwi %r31, 0x63
    ble+ loc_0x74
    li %r31, 0x63

    loc_0x74:
    mr %r0, %r31
    lmw %r26, 8(%sp)
    mr %r29, %r0
    addis %r3, %r30, 2
    lwz %r0, 52(%sp)
    mtlr %r0
    lwz %r0, 44(%sp)
    addi %sp, %sp, 0x30
    end_enemy_atk:
    b 0