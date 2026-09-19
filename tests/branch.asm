        .ORIG x3000
        AND R0, R0, #0
        ADD R0, R0, #3
LOOP    ADD R0, R0, #-1
        BRp LOOP
        LEA R1, TARGET
        AND R2, R2, #0
        BRz SKIP
        ADD R2, R2, #9
SKIP    ADD R3, R3, #7
        BRn NOTAKEN
        ADD R4, R4, #5
NOTAKEN ADD R5, R5, #1
TARGET  TRAP x25
        .END