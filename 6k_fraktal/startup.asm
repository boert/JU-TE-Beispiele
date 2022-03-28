;
;  TRUE/FALSE equates 
;
    TRUE  EQU 1
    FALSE EQU 0
 
; Konfiguration
    ; Datensegment (Nullwerte)
    CLRRRAM  EQU  FALSE
    CLRXRAM  EQU  FALSE
    ; Datensegment (Initialwerte)
    COPYRRAM EQU  TRUE 
    COPYXRAM EQU  TRUE 
  


    XDEF  _c_startup
    XREF  _main:ROM
    IF CLRRRAM
      XREF  _low_nearbss
      XREF  _len_nearbss
    ENDIF
    IF CLRXRAM
      XREF  _low_farbss
      XREF  _len_farbss
    ENDIF
    IF COPYRRAM
      XREF  _low_neardata
      XREF  _len_neardata
      XREF  _low_near_romdata
    ENDIF
    IF COPYXRAM
      XREF  _low_fardata
      XREF  _len_fardata
      XREF  _low_far_romdata
    ENDIF
  
    segment CODE
_c_startup:

    ; Tiny-Basic-Token
    ; to help start the program with '+' (RUN)
    DB $80, 1, 'M Fraktaldemo, Bert Lange, 02/2022', $0d
    DB $80, 2, 'C%E032', $0d
;   DB $80, 2, 'C%C010', $0d
    DB $80, 3, 'E', $0d
;
; Clear internal ram area (c_nearbss)
;
 IF CLRRRAM
      LD  R0,#_low_nearbss
      LD  R2,#_len_nearbss
      CP  R2,#0
      JR  Z,lab4
lab3:
      CLR @R0
      INC R0
      DJNZ  R2,lab3
lab4:
 ENDIF


;
; Clear external ram area (c_farbss)
;
 IF CLRXRAM
  LD  R2,#high(_low_farbss)
  LD  R3,#low(_low_farbss)
  LD  R0,#high(_len_farbss)
  LD  R1,#low(_len_farbss)

  LD  R4,R0
  OR  R4,R1
  JR  Z,lab6
  CLR R4
lab5:
  LDE @RR2,R4
  INCW  RR2
  DECW  RR0
  JR  NZ,lab5
lab6:
 ENDIF
;
; Copy ROM data into internal RAM
;
 IF COPYRRAM
  LD  R0,#high(_low_near_romdata)
  LD  R1,#low(_low_near_romdata)
  LD  R3,#_len_neardata
  LD  R4,#_low_neardata
  OR  R3,R3
        JR      Z,lab10
lab9:
  LDCI  @R4,@RR0
        DJNZ    R3,lab9
lab10:
 ENDIF

;
; Copy ROM data into external RAM
;
 IF COPYXRAM
  LD  R0,#high(_low_fardata)
  LD  R1,#low(_low_fardata)
  LD  R2,#high(_low_far_romdata)
  LD  R3,#low(_low_far_romdata)
  LD  R4,#high(_len_fardata)
  LD  R5,#low(_len_fardata)

  LD  R6,R4
  OR  R6,R5
  JR  Z,lab12

lab11:
  LDC R6,@RR2
  LDE @RR0,R6
  INCW  RR0
  INCW  RR2
  DECW  RR4
  JR  NZ,lab11

lab12:
 ENDIF


    CALL  _main
    RET
;__done:
;    JR  __done
  
    END _c_startup
