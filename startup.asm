    XDEF  _c_startup
    XREF  _main:ROM
  
    segment CODE
_c_startup:

    ; Tiny-Basic-Token
    ; to help start the program with '+' (RUN)
    DB $80, 1, 'C%E010', $0d
    DB $80, 2, 'E', $0d
    BLKB 3, 0

    CALL  _main
    RET
  
    END _c_startup
