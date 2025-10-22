all: NoWarpOS

NoWarpOS: NoWarpOS.c Makefile
	vc +aos68k -nostdlib -O1 -sc -fastcall -D__NOLIBBASE__ -lamiga -o $@ $<

clean:
	$(RM) NoWarpOS
