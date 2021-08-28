PREFIX=WINEARCH=win32 WINEPREFIX=~/.wine32 wine ~/.wine32/drive_c/Z8/bin/

CC=$(PREFIX)z8cc.exe 
LINK=$(PREFIX)z8link.exe 
ASM=$(PREFIX)z8asm.exe


all: main.ihx

main.obj: main.c Makefile
	 $(CC) -model=S "main.c"

main.ihx: main.obj startup.obj
	$(LINK) -color @main.lnk


startup.obj: startup.asm
	$(ASM) startup.asm  



main.bin: main.ihx
	srec_cat main.ihx -intel -o main.bin -bin 


dump: main.ihx
	objdump -s main.ihx


clean:
	rm -f main.ihx
	rm -f main.bin
	rm -f *.lst
	rm -f *.obj
	rm -f main.map
	rm -f main.lod
	rm -f main.src
