#include "Hooking.h"

std::unordered_map<std::string, ClassHooker*> g_hookedClasses;

int FASTCALL cmHookLanding(ClassHooker *hooker, const char* funcName, void* thisptr)
{
	void* destAddr = hooker->getDestAddr(funcName);
	ASM_START
		mov ecx, thisptr
		push hooker
		call destAddr
	ASM_END
	return 0;
}

void NAKED hookCode()
{
	ASM_START
		push ebp
		mov ebp, esp
		push ecx
		mov ecx, 0xABABABAB
		mov edx, 0xBCBCBCBC
		mov eax, 0xCDCDCDCD
		call eax
		pop ebp
		ret
		int 3 // end of function marker == 0xCC
		int 3
		int 3
		int 3
	ASM_END
}