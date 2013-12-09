#include "Hooking.h"

std::unordered_map<std::string, ClassHooker*> g_hookedClasses;

int FASTCALL cmHookLanding(void* r_ecx, void* r_edx)
{
	return 0;
}

void NAKED hookCode()
{
	ASM_START
		push ebp
		mov ebp, esp
		sub esp, 0x0C
		jmp code
		mov [ebp], 0x01020304
		mov [ebp-4], 0x05060708
		mov [ebp-8], 0x090A0B0C
		code:
		mov eax, [ebp]
		mov ebx, [ebp-4]
		mov edx, [ebp-8]
		call edx
		add esp, 0x0c
		pop ebp
		ret
		int 3 // end of function marker == 0xCC
		int 3
		int 3
		int 3
	ASM_END
}