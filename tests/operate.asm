        .ORIG x3000
        ADD R1, R1, #5
        ADD R2, R1, #-6
        ADD R3, R1, R2
        AND R4, R2, #0
        AND R5, R2, R1
        XOR R6, R1, #3
        NOT R7, R1
        ADD R0, R2, R2
        TRAP x25
        .END