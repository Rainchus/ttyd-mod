.global start_enemy_hp
.global end_enemy_hp

start_enemy_hp:
    lwz %r5, 0 (%r26) #load actor init struct pointer
    mr %r28, %r3 #r28 is free to use, so we push r3 here for later

    #If the actor id is not an enemy, exit
    lha %r6, 2 (%r5) #load last 2 bytes of word for actor ID
    cmpwi %r6, 171 #if greater than kUnitBonetail, don't poke hp
    bgt+ exit

    #If the actor spawns with zero hp or less, dont poke and exit
    lha %r6, 8 (%r5) #load actor's hp
    cmpwi %r6, 0
    ble- exit

    modifyActorsHp:
    lis %r4, HP_Multiplier@ha
    lwz %r4, HP_Multiplier@l(%r4)

    mullw %r6, %r6, %r4
    li %r4, 100
    divw %r6, %r6, %r4
    cmpwi %r6, 0
    bgt+ validHPVal
    li %r6, 1 #set hp to 1 if it was 0 or lower
    validHPVal:
    sth %r6, 0x0108 (%r30) #set actor's current hp
    sth %r6, 0x010A (%r30) #set actor's max hp
    sth %r6, 0x010C (%r30) #set actor's ? hp (definitely hp related)

    lbz %r6, 0x00C4 (%r5) #load base defense
    lis %r7, Def_Extra@ha
    lbz %r7, Def_Extra@l(%r7)
    add %r6, %r6, %r7

    #set new def
    stb %r6, 0x0174 (%r5)
    stb %r6, 0x0175 (%r5)
    stb %r6, 0x0176 (%r5)
    stb %r6, 0x0177 (%r5)
    stb %r6, 0x0178 (%r5)

    #bl TestOSReport
    exit:

    mr %r3, %r28 #pop r3

    end_enemy_hp:
    b 0

