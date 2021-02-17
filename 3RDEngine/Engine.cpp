#include <windows.h>
#include "Engine.h"

using namespace std;
// ------------------------------------
//
//		*** class gEngineErrorHandler ***
//
// ------------------------------------
gEngineErrorHandler::gEngineErrorHandler(const char* location, 
	const char* file, int line, const char* message)
{
	m_location = location;
	m_message = message;
	m_file = file;
	m_line = std::to_string(line);
}

gEngineErrorHandler::~gEngineErrorHandler()
{

}

const char* gEngineErrorHandler::getErrorLocation() const
{
	return m_location.c_str();
}

const char* gEngineErrorHandler::getErrorMessage() const
{
	return m_message.c_str();
}
const char* gEngineErrorHandler::getFileName() const
{
	return m_file.c_str();
}
const char* gEngineErrorHandler::getFileLine() const
{
	return  m_line.c_str();
}

// ------------------------------------
//
//		*** class g3RDEngine ***
//
// ------------------------------------

g3RDEngine::g3RDEngine() : m_gapi(eRENDERAPI::RA_NOT_SUPPORT)
{

}

g3RDEngine::~g3RDEngine()
{
	finalize();
	MessageBox(0, "Engine Destructor from DLL", "Destructor", 
		MB_SERVICE_NOTIFICATION | MB_ICONERROR );
}

eRENDERAPI g3RDEngine::getLatestSupportedGAPI()
{
	return eRENDERAPI::RA_NOT_SUPPORT; // заглушка
}

bool g3RDEngine::initialize(eRENDERAPI api)
{
	try
	{
		throw(gEngineErrorHandler(__FUNCSIG__, __FILE__, __LINE__));
		switch (api)
		{
		case eRENDERAPI::RA_DX9:
			return true;
		case eRENDERAPI::RA_DX12:
			return true;
		}
	}
	catch (const IErrorHandler& e)
	{
		e.getErrorLocation();
		finalize();
	}
	return false;
}

bool g3RDEngine::run()
{
	return true;
}

bool g3RDEngine::finalize()
{
	return true;
}

//objects getters
SPPLATFORM g3RDEngine::Platform()
{
	return m_spPlatform;
}

SPRESOURCES	g3RDEngine::Resources()
{
	return m_spResources;
}

SPINPUT	g3RDEngine::Input()
{
	return m_spInput;
}

SPGRAPHICS g3RDEngine::Graphics()
{
	return m_spGraphics;
}

SPSOUNDS g3RDEngine::Sounds()
{
	return m_spSounds;
}

SPPHYSICS g3RDEngine::Physics()
{
	return m_spPhysics;
}

SPSCENEGRAPH g3RDEngine::SceneGraph()
{
	return m_spSceneGraph;
}

SPUSERINTERFACE g3RDEngine::UserInterface()
{
	return m_spUserInterface;
}

SPSCRIPTS g3RDEngine::Scripts()
{
	return m_spScripts;
}

SPLOGGER g3RDEngine::Logger()
{
	return m_spLogger;
}