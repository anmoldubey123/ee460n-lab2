        .ORIG x3000
        AND R0, R0, #0
        ADD R0, R0, #8
        LSHF R0, R0, #9
        ADD R1, R1, #-1
        STB R1, R0, #0
        LDB R2, R0, #0
        LDW R3, R0, #0
        ADD R4, R4, #10
        STB R4, R0, #1
        LDW R5, R0, #0
        LDB R6, R0, #1
        TRAP x25
        .END