# JU-TE-Beispiele
Ein kleines Beispielprogramm in C für den JU+TE Computer
mit dem C-Compiler Z8CC von Zilog.

```
z8cc.exe  -model=S "main.c"

Z8/Z8Plus ANSI C Compiler, Version 4.05
Copyright (C) 1999-2000 ZiLOG, Inc. All Rights Reserved

main.c
z8asm.exe startup.asm  

Z8/Z8Plus Macro Assembler, Version 3.02
Copyright (C) 1991-2000 ZiLOG, Inc. All Rights Reserved


startup.asm
Lines Assembled: 17
z8link.exe  -color @main.lnk

Z8 Linker/Locator, Version 4.02
Copyright (C) 1999-2000 ZiLOG, Inc. All Rights Reserved



0 Errors
0 Warnings
```
