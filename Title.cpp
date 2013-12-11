#include "Hooking.h"
#include <string>
#include "main.h"

//sbapi
class TitleHook : public ClassHooker
{
	public:
		TitleHook() : ClassHooker("Title")
		{
			setupHookMemberFunc("update", (void*)0x00419500);
		}
};

static TitleHook hooker;

struct TitleStruct
{
	//this contains member variables of the blablabnla
};

//plugin

//figuring out how to do the plugin side
class PluginTitle : public TitleStruct
{
	friend class PluginTitle_hookinit;
	void update(ClassHooker *ch)
	{
		ch->unhookMemberFunc("update");
		void* dest = (void*)0x00419500;
		ASM_START
			mov ecx, this
			call dest
		ASM_END
		ch->hookMemberFunc("update", m_selfFuncAddrs["update"]);
		return;
	}
	public:
		static std::unordered_map<std::string, void*> m_selfFuncAddrs;
};
std::unordered_map<std::string, void*> PluginTitle::m_selfFuncAddrs;

class PluginTitle_hookinit
{
	public:
		PluginTitle_hookinit()
		{
			PluginTitle::m_selfFuncAddrs["update"] = &(void*&)PluginTitle::update;
			g_hookedClasses["Title"]->hookMemberFunc("update", &(void*&)PluginTitle::update);
		}
};

static PluginTitle_hookinit init;