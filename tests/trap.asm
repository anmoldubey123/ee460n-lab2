        .ORIG x3000
        AND R0, R0, #0
        ADD R0, R0, #7
        TRAP x25
        ADD R0, R0, #9
        .END