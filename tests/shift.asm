        .ORIG x3000
        ADD R1, R1, #5
        LSHF R2, R1, #3
        ADD R3, R3, #-1
        RSHFL R4, R3, #4
        RSHFA R5, R3, #4
        RSHFA R6, R1, #1
        LSHF R7, R3, #4
        RSHFA R0, R3, #0
        TRAP x25
        .END