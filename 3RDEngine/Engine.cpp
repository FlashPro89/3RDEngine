#include <windows.h>
#include <assert.h>
#include "Engine.h"
#include "PlatformWin.h"
#include "Configuration.h"
#include "Logger.h"

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
	
	
	//ENGINE->getLogger()->logError(m_exceptionDescription);
	//ELOGERR(m_exceptionDescription);
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

	//auto spEngine = I3RDEngine::get();
	//spEngine.reset();
	//spEngine.reset(); // zero static shared_ptr
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

		// Create logger
		m_spLogger =
			std::make_shared<gLogger>(m_spPlatform, "Log.html");

		// Try to init systems ( first init config for all systems )
		ECHECK(m_spLogger->initialize(), "Failed logger initialization!");
		ECHECK(m_spConfiguration->initialize(), "Failed load configuration file!");
		ECHECK(m_spPlatform->initialize(), "Failed platform system initialization!");

		ELOGMSG("3RD Engine systems initialized successfull!");

		// Show window 
		m_spPlatform->getWindow()->showWindow(true);

		//m_spLogger->logMessage("Message");
		//m_spLogger->logWarning("Warning");
		//m_spLogger->logError("Error");
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
	m_spInput.reset();
	m_spGraphics.reset();
	m_spSounds.reset();
	m_spPhysics.reset();
	m_spSceneGraph.reset();
	m_spUserInterface.reset();
	m_spScripts.reset();
	m_spLogger.reset();
	m_spResources.reset();
	m_spConfiguration.reset();
	m_spPlatform.reset();

	return true;
}

//objects getters
SPPLATFORM g3RDEngine::getPlatform()
{
	return m_spPlatform;
}

SPRESOURCES	g3RDEngine::getResources()
{
	return m_spResources;
}

SPINPUT	g3RDEngine::getInput()
{
	return m_spInput;
}

SPGRAPHICS g3RDEngine::getGraphics()
{
	return m_spGraphics;
}

SPSOUNDS g3RDEngine::getSounds()
{
	return m_spSounds;
}

SPPHYSICS g3RDEngine::getPhysics()
{
	return m_spPhysics;
}

SPSCENEGRAPH g3RDEngine::getSceneGraph()
{
	return m_spSceneGraph;
}

SPUSERINTERFACE g3RDEngine::getUserInterface()
{
	return m_spUserInterface;
}

SPSCRIPTS g3RDEngine::getScripts()
{
	return m_spScripts;
}

SPLOGGER g3RDEngine::getLogger()
{
	return m_spLogger;
}

SPCONFIGURATION g3RDEngine::getConfiguration()
{
	return m_spConfiguration;
}
