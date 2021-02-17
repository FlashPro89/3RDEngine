#pragma once

#ifndef _3RDE_SCRIPTS_
#define _3RDE_SCRIPTS_

#include "Interfaces.h"
#include "../lua/lua.hpp"

typedef std::shared_ptr<lua_State> SPLUASTATE;

class gScripts : public IScripts
{
public:
	gScripts();
	~gScripts();

	bool initialize();

protected:
	
	bool finalize();

	SPLUASTATE m_spState; //test
};

#endif

