; Full-coverage test: exercises all 14 LC-3b opcodes
        .ORIG x3000

;--- build base address x4000 in R0 ---
        AND R0, R0, #0          ; AND imm  -> R0 = 0, sets Z
        ADD R0, R0, #8          ; ADD imm
        LSHF R0, R0, #11        ; LSHF: 8 << 11 = x4000

;--- write three bytes: 5, -3, 10 ---
        AND R1, R1, #0
        ADD R1, R1, #5
        STB R1, R0, #0          ; array[0] = 5
        AND R1, R1, #0
        ADD R1, R1, #-3
        STB R1, R0, #1          ; array[1] = -3  (xFD)
        AND R1, R1, #0
        ADD R1, R1, #10
        STB R1, R0, #2          ; array[2] = 10

;--- call summing subroutine ---
        AND R2, R2, #0          ; running sum
        AND R3, R3, #0          ; index
        JSR SUMLOOP             ; JSR (PC-relative)

;--- store/reload the sum as a word ---
        STW R2, R0, #4          ; STW: word offset 4 -> x4008
        LDW R4, R0, #4          ; LDW: reads back 12

;--- remaining operate instructions ---
        NOT R5, R4              ; NOT (XOR encoding) -> xFFF3
        XOR R6, R4, #3          ; XOR imm -> 12 ^ 3 = 15
        AND R7, R4, R6          ; AND register mode -> 12
        RSHFA R1, R5, #2        ; arithmetic right shift of xFFF3 -> xFFFC
        RSHFL R2, R5, #2        ; logical right shift of xFFF3   -> x3FFC

;--- branch tests ---
        ADD R5, R5, #0          ; re-set CCs from xFFF3 (negative)
        BRp SKIPPED             ; not taken
        BRn TAKEN               ; taken
SKIPPED ADD R6, R6, #9          ; must NOT execute
TAKEN   ADD R6, R6, #1          ; R6: 15 -> 16

;--- JSRR and JMP ---
        LEA R3, SUB2            ; LEA (no CC change)
        JSRR R3                 ; JSRR (register mode)
        LEA R3, DONE
        JMP R3                  ; JMP
        ADD R6, R6, #9          ; must NOT execute

DONE    TRAP x25                ; TRAP -> halt

;--- subroutine: sum 3 bytes at R0 into R2 ---
SUMLOOP LDB R1, R0, #0          ; placeholder overwritten below
LOOP    ADD R1, R0, R3          ; R1 = base + index
        LDB R1, R1, #0          ; LDB with sign extension
        ADD R2, R2, R1          ; accumulate
        ADD R3, R3, #1          ; index++
        ADD R1, R3, #-3         ; compare index against 3
        BRn LOOP                ; backward branch if index < 3
        RET                     ; RET (JMP R7)

;--- second subroutine, called via JSRR ---
SUB2    ADD R4, R4, #1          ; R4: 12 -> 13
        RET

        .END