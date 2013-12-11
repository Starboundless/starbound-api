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
struct PluginTitle : public TitleStruct
{
	void update(ClassHooker *ch)
	{
		ch->unhookMemberFunc("update");
		void* funcAddr;
		CALL_FUNC(update, funcAddr);
		ch->hookMemberFunc("update", m_selfFuncAddrs["update"]);
		return;
	}
	HOOKDEST_DECL(PluginTitle)
};
HOOKDEST_DEF(PluginTitle)

HOOKTABLE_START(PluginTitle)
	SETUPHOOKEDFUNC(Title, update)
HOOKTABLE_END(PluginTitle)

/*template<typename T> struct PluginTitle_hookinit { 
	PluginTitle_hookinit() 
	{
		T::m_selfFuncAddrs[ "update" ] = &(void*&)T::update; 
		g_hookedClasses[ "Title" ]->hookMemberFunc( "update" , &(void*&)T::update);
	}
}; 
static PluginTitle_hookinit<PluginTitle> PluginTitle_init;*/