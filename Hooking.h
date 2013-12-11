#pragma once
#include <map>
#include <string>
#include <unordered_map> //c++11 hash_map
#include <stack>
#include <unordered_set>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

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

int FASTCALL cmHookLanding(ClassHooker *hooker, const char* funcName, void* thisptr);

void hookCode();

class ClassHooker
{
	const char* m_classname;
	std::unordered_map<std::string, void*> m_hookableFunctions;
	std::unordered_map<std::string, std::stack<void*>> m_destAddrs;
	std::unordered_set<std::string> m_hookedFunctions;
	std::unordered_map<std::string, char*> m_originalBytes;
	protected:
		std::unordered_map<std::string, void*> m_destAddress;
		static ClassHooker* singleton;

		ClassHooker(const char* classname) : m_classname(classname)
		{
			g_hookedClasses[classname] = this;
		}
		
		//addr is temporary, until scanning engine is in place
		void setupHookMemberFunc(const char* funcName, void* addr)
		{
			m_hookableFunctions[funcName] = addr;
		}

		void doHook(const char* funcName)
		{
			DWORD temp;
			VirtualProtect(&hookCode, 23, PAGE_EXECUTE_READ, &temp);
			char* tCode = new char[23]; //tailored code
			memcpy(tCode, &hookCode, 23);
			const unsigned char mrk1[4] = {0xAB, 0xAB, 0xAB, 0xAB};
			const unsigned char mrk2[4] = {0xBC, 0xBC, 0xBC, 0xBC};
			const unsigned char mrk3[4] = {0xCD, 0xCD, 0xCD, 0xCD};
			for(unsigned int i = 0; i < 23; ++i)
			{
				unsigned char c = tCode[i];
				if(c == mrk1[0])
				{
					if(memcmp(tCode+i, mrk1, 4) == 0)
					{
						*(int*)(tCode+i) = (int)this;
					}
				}
				else if(c == mrk2[0])
				{
					if(memcmp(tCode+i, mrk2, 4) == 0)
					{
						*(int*)(tCode+i) = (int)funcName;
					}
				}
				else if(c == mrk3[0])
				{
					if(memcmp(tCode+i, mrk3, 4) == 0)
					{
						*(int*)(tCode+i) = (int)&cmHookLanding;
					}
				}
			}
			VirtualProtect(m_hookableFunctions[funcName], 23, PAGE_EXECUTE_READWRITE, &temp);
			char* orig = new char[23];
			memcpy(orig, m_hookableFunctions[funcName], 23);
			m_originalBytes[funcName] = orig;
			memcpy(m_hookableFunctions[funcName], tCode, 23);
			delete [] tCode;
		}

		void doUnhook(const char* funcName)
		{
			memcpy(m_hookableFunctions[funcName], m_originalBytes[funcName], 23);
			delete [] m_originalBytes[funcName];
			m_originalBytes[funcName] = NULL;
		}
	public:

		void* getDestAddr(const char* funcName)
		{
			if(m_destAddrs[funcName].empty())
			{
				return m_hookableFunctions[funcName];
			}
			return m_destAddrs[funcName].top();
		}

		void hookMemberFunc(const char* funcName, void* newAddr)
		{
			if(m_hookableFunctions.find(funcName) == m_hookableFunctions.end())
			{
				return;
			}
			if(m_hookedFunctions.find(funcName) == m_hookedFunctions.end())
			{
				doHook(funcName);
			}
			m_destAddrs[funcName].push(newAddr);
		}
		void unhookMemberFunc(const char* funcName)
		{
			if(!m_destAddrs[funcName].empty())
			{
				m_destAddrs[funcName].pop();
				if(m_destAddrs[funcName].empty())
				{
					doUnhook(funcName);
				}
			}
		}
};

