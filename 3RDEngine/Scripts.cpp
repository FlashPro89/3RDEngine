#include "Scripts.h"
#include "../lua/lualib.h"
#include "../lua/lauxlib.h"

// ------------------------------------
//
//		*** class gVirtualMachine ***
//
// ------------------------------------

gVirtualMachine::gVirtualMachine()
{
	//m_pLuaState = luaL_newstate();
	//luaL_openlibs(m_pLuaState);
}

gVirtualMachine::~gVirtualMachine()
{
	//if (m_pLuaState)
	//	lua_close(m_pLuaState);
}

PLUASTATE gVirtualMachine::get()
{
	return m_pLuaState;
}

// ------------------------------------
//
//		*** class gScripts ***
//
// ------------------------------------

gScripts::gScripts()
{

}

gScripts::~gScripts() 
{
	finalize();
}

bool gScripts::initialize()
{
	return true;
}

SPVIRTUALMACHINE gScripts::createVirtualMachine( gString  name )
{
	SPVIRTUALMACHINE vm;

	auto it = m_machinesMap.find(name);
	if (it == m_machinesMap.end())
	{
		vm = std::make_shared<gVirtualMachine>();
		m_machinesMap[name] = vm;
	}
	return vm;
}

bool gScripts::finalize()
{
	m_machinesMap.clear();
	return true;
}

/*
#include <functional>

class CWindow
{
public:
	CWindow() {}
	~CWindow() {}

	void onClick( float x, float y )
	{
		MessageBox(0, "Window was clicked!", "Warning", MB_OK);
	};
};

typedef std::function<void(float, float)> WNDONCLICKCALLBACK;

class CEventManager
{
public:
	CEventManager() {}
	~CEventManager() {}

	void setWindow(CWindow& win)
	{
		currentWindow = &win;
		using std::placeholders::_1; using std::placeholders::_2;
		callback = std::bind(&CWindow::onClick, win, _1, _2);
	}

	void processEvents()
	{
		callback(100.f, 200.f);
	}

protected:
	CWindow* currentWindow;
	WNDONCLICKCALLBACK callback;
};

int main()
{
	CWindow w;
	using std::placeholders::_1; using std::placeholders::_2;
	std::function<void(float,float)> wCallback_OnClick = std::bind( &CWindow::onClick, &w, _1, _2 );

	wCallback_OnClick(10.f, 20.f);

	CEventManager mgr;
	mgr.setWindow(w);
	mgr.processEvents();
*/