#pragma once

#ifndef _3RDE_SCRIPTS_
#define _3RDE_SCRIPTS_

#include <map>
#include "Interfaces.h"
#include "../lua/lua.hpp"

typedef lua_State* PLUASTATE;
class gVirtualMachine;
typedef std::shared_ptr<gVirtualMachine> SPVIRTUALMACHINE;

class gVirtualMachine
{
public:
	gVirtualMachine();
	~gVirtualMachine();

	PLUASTATE get();

protected:
	PLUASTATE m_pLuaState;

	gVirtualMachine( const gVirtualMachine& vm) {};
};

class gScripts : public IScripts
{
public:
	gScripts();
	~gScripts();

	bool initialize();

	SPVIRTUALMACHINE createVirtualMachine( gString  name );

protected:
	bool finalize();
	std::map<gString, SPVIRTUALMACHINE> m_machinesMap;
};

#endif

