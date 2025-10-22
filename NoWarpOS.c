#include <proto/exec.h>
#include <proto/dos.h>

#include <exec/exec.h>
#include <dos/dos.h>

#include <string.h>

extern LONG LVOOpenLibrary;

const char Version[] = "$VER: NoWarpOS 1.0 (22.10.2025) by Patrik Axelsson";

static struct Library *NoWarpOSOpenLibrary(
		__reg("a6") struct ExecBase *SysBase,
		__reg("a1") const char *libName,
		__reg("d0") ULONG version); 

static struct Library *(* OriginalOpenLibrary)(
		__reg("a6") struct ExecBase *SysBase,
		__reg("a1") const char *libName,
		__reg("d0") ULONG version);


LONG NoWarpOS() {
	struct ExecBase *SysBase = *(struct ExecBase **) 4;
	struct DosLibrary *DOSBase = (void *) OpenLibrary("dos.library", 36);
	if (NULL == DOSBase) {
		return RETURN_FAIL;
	}

	OriginalOpenLibrary = (void *) SetFunction((void *) SysBase, &LVOOpenLibrary, (void *) &NoWarpOSOpenLibrary);
	PutStr("OpenLibrary() patched to deny powerpc.library opens, break to restore!\n");
	
	Wait(SIGBREAKF_CTRL_C);
	SetFunction((void *) SysBase, &LVOOpenLibrary, (void *) OriginalOpenLibrary);
	
	CloseLibrary((void *) DOSBase);
	return RETURN_OK;
}

static struct Library *NoWarpOSOpenLibrary(
		__reg("a6") struct ExecBase *SysBase,
		__reg("a1") const char *libName,
		__reg("d0") ULONG version) {
	if (0 == strcmp("powerpc.library", libName)) {
		return NULL;
	}
	OriginalOpenLibrary(SysBase, libName, version);
}
