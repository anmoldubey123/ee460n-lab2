        .ORIG x3000
        AND R0, R0, #0
        ADD R0, R0, #5
        JSR SUB1
        ADD R1, R1, #1
        LEA R2, SUB2
        JSRR R2
        ADD R3, R3, #1
        LEA R4, DONE
        JMP R4
        ADD R5, R5, #9
DONE    ADD R6, R6, #2
        TRAP x25
SUB1    ADD R0, R0, #1
        RET
SUB2    ADD R0, R0, #2
        RET
        .END