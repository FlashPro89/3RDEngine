#include <windows.h>
#include <assert.h>
#include "Engine.h"
#include "PlatformWin.h"
#include "Configuration.h"

using namespace std;
// ------------------------------------
//
//		*** class gEngineErrorHandler ***
//
// ------------------------------------
gEngineExceptionHandler::gEngineExceptionHandler(const gString& location,
	const gString& file, int line, const gString& message)
{
	m_exceptionDescription = "Exception in \"" + location + "\"\n" + "File: " + file + "\nLine: " + std::to_string(line);
	if (message != "")
		m_exceptionDescription += "\nMessage: " + message;
}

gEngineExceptionHandler::~gEngineExceptionHandler()
{

}

const char* gEngineExceptionHandler::getExceptionDescription() const
{
	return m_exceptionDescription.c_str();
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

void g3RDEngine::setApplicationName( const char* applicationName )
{
	m_applicationName = applicationName;
}

bool g3RDEngine::initialize(const char* config, bool useAsConfigBuffer)
{
	try
	{
		// At the beginning create systems, and then initialize it
		// Create platform system
#ifdef _WIN32
		m_spPlatform = std::make_shared<gPlatformWin>();
#else
		static_assert(false, "Only Windows platform supported now!");
#endif
	
		// Create configuration
		m_spConfiguration = 
			std::make_shared<gConfiguration>(m_spPlatform,config, useAsConfigBuffer);

		// Try to init systems ( first init config for all systems )
		ECHECK(m_spConfiguration->initialize(), "Failed load configuration file!");
		ECHECK(m_spPlatform->initialize(), "Failed platform system initialization!");

		// Show window 
		m_spPlatform->getWindow()->showWindow(true);
	}
	catch (const gEngineExceptionHandler& e)
	{
		// need platform independent function!
		MessageBox( 0, e.getExceptionDescription(), "3RDE Game Engine", MB_ICONERROR );
		finalize();
	}
	return true;
}

bool g3RDEngine::run()
{
	if (m_spPlatform.get() == 0) 
		return false;

	return( m_spPlatform->runMainLoop() );
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