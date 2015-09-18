.section .sram.text, "ax"
.align 8

.global rk3368_ddr_memSet
.global rk3368_ddr_memCpy
.global rk3368_ddr_memCpy_mask

//int memSet(uint32 *dstAddr, uint32 count, uint32 value);

rk3368_ddr_memSet:
        //PUSH     {r3,r4,r5,r6,r7,lr}
        PUSH	  {r3-r6,lr}
        MOV R3, R2
        MOV R4, R2
        MOV R5, R2
        MOV R6, R2
memset4:
        CMP R1,#4
        BCC memset1
        STMIA R0!,{R3-R6}
        SUB R1,R1,#4
        CMP R1,#0
        BNE memset4
memset1:
        CMP R1,#0
        BEQ memsetret
        //STR R3,[R0,#0]
        //ADD R0,R0,#4
        STMIA R0!,{R3}
        SUB R1,R1,#1
        B  memset1
memsetret:
        POP     {r3-r6,pc}

//int rk3368_ddr_memCpy(uint32 *dstAddr, uint32 *srcAddr, uint32 count);
rk3368_ddr_memCpy:
        PUSH	  {r3-r6,lr}
memCmp4:
        CMP  R2,#4
        BCC  memCmp1
        LDMIA R1!,{R3-R6}
        STMIA R0!,{R3-R6}
        SUB   R2,R2,#4
        B   memCmp4
memCmp1:
        CMP  R2,#0
        BEQ  memCmpret
        LDMIA R1!,{R3}
        STMIA R0!,{R3}
        SUB   R2,R2,#1
        B   memCmp1
memCmpret:
        POP     {r3-r6,pc}

rk3368_ddr_memCpy_mask:
        PUSH      {r3-r7,lr}
        LDR R7,=0xffff0000
memCmpM4:
        CMP  R2,#4
        BCC  memCmpM1
        LDMIA R1!,{R3-R6}
        ORR R3,R3,R7
        ORR R4,R4,R7
        ORR R5,R5,R7
        ORR R6,R6,R7
        STMIA R0!,{R3-R6}
        SUB   R2,R2,#4
        B   memCmpM4
memCmpM1:
        CMP  R2,#0
        BEQ  memCmpMret
        LDMIA R1!,{R3}
        ORR R3,R3,R7
        STMIA R0!,{R3}
        SUB   R2,R2,#1
        B   memCmpM1
memCmpMret:
        POP     {r3-r7,pc}


