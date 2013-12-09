#pragma once
#include <map>
#include <string>
#include <unordered_map> //c++11 hash_map

#ifdef _WIN32
#define ASM_START _asm {
#define ASM_END }
#define EXPORT _declspec(dllexport)
#define FASTCALL __fastcall
#define NAKED _declspec(naked)
#endif

#define HOOKCLASS(classname) namespace SBApiHooks \
	{ \
		class classname \
		{ \
			
#define ENDHOOKCLASS };}

//addr is temporary, will be replaced with symbol/signature later
#define HOOKMEMBERFUNCTION(funcName, addr) void hook_funcName();

class ClassHooker;
extern std::unordered_map<std::string, ClassHooker*> g_hookedClasses;

int FASTCALL cmHookLanding(void* r_ecx, void* r_edx);

void hookCode();

class ClassHooker
{
	const char* m_classname;
	std::unordered_map<std::string, void*> m_hookableFunctions;
	protected:
		std::unordered_map<std::string, void*> m_destAddress;
		static ClassHooker* singleton;

		ClassHooker(const char* classname) : m_classname(classname)
		{
		}
		
		//addr is temporary, until scanning engine is in place
		void setupHookMemberFunc(const char* funcName, void* addr)
		{
			m_hookableFunctions[funcName] = addr;
		}
	public:

		void hookMemberFunc(void* addr)
		{
			ASM_START
				mov eax, 0
				mov ebx, 1
			ASM_END
			cmHookLanding(0, 0);
		}
};

