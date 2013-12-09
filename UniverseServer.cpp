#include "Hooking.h"
#include <string>

//sbapi
class UniverseServerHook : public ClassHooker
{
	public:
		UniverseServerHook() : ClassHooker("UniverseServer")
		{
			setupHookMemberFunc("clientNick", (void*)0x004F1760);
		}
};

static UniverseServerHook hooker;

struct UniverseServerStruct
{
	//this contains member variables of the blablabnla
};

//plugin

//figuring out how to do the plugin side
class PluginUniverseServer : public UniverseServerStruct
{
	std::string clientNick(unsigned int clientId)
	{
		return "penis";
	}
};

