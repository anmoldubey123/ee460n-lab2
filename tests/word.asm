        .ORIG x3000
        AND R0, R0, #0
        ADD R0, R0, #8
        LSHF R0, R0, #9
        LDW R1, R0, #0
        ADD R3, R3, #-1
        STW R3, R0, #2
        LDW R4, R0, #2
        STW R0, R0, #3
        LDW R5, R0, #3
        TRAP x25
        .END