#include <assert.h>
#include <vadefs.h>
#include <thread>
#include "Engine.h"
#include "PlatformWin.h"
#include "Configuration.h"
#include "Logger.h"

#ifdef _WIN32
#include <windows.h>
#endif

//graphics modules
#include "../GDX9/gdx9.h"
#include "../GDX12/gdx12.h"

using namespace std;


#pragma warning( disable : 4297 ) 
// For exception import from plugins
_3RDE_API_C_ void throwException(const gString& location,
	const gString& file, int line, const gString& message)
{
	throw gEngineExceptionHandler(location, file, line, message);
}
#pragma warning( default : 4297 ) 
//C4297: throwException: ожидается, что функция не будет выдавать исключения, но она это делает

// ------------------------------------
//
//		*** class gPlugin ***
//
// ------------------------------------
template < class I, typename ... InterfaceCreationArgs >
gPlugin<I, InterfaceCreationArgs...>::gPlugin(
	const gString& libFileName,
	const gString& fnCreateName,
	const gString& fnDestroyName) :
	m_libFileName(libFileName),
	m_fnCreateName(fnCreateName),
	m_fnDestroyName(fnDestroyName),
	m_lpfnCreate(NULL),
	m_lpfnDestroy(NULL),
	m_dlHandle(NULL)
{

}

template < class I, typename ... InterfaceCreationArgs >
gPlugin<I, InterfaceCreationArgs...>::gPlugin(const gPlugin& other) :
	m_libFileName(other.m_libFileName),
	m_fnCreateName(other.m_fnCreateName),
	m_fnDestroyName(other.m_fnDestroyName),
	m_lpfnCreate(NULL),
	m_lpfnDestroy(NULL),
	m_dlHandle(NULL)
{
	finalize();
}

template < class I, typename ... InterfaceCreationArgs >
gPlugin<I, InterfaceCreationArgs...>::gPlugin() :
	m_libFileName(""),
	m_fnCreateName(""),
	m_fnDestroyName(""),
	m_lpfnCreate(NULL),
	m_lpfnDestroy(NULL),
	m_dlHandle(NULL)
{

};


template < class I, typename ... InterfaceCreationArgs >
gPlugin<I, InterfaceCreationArgs...>::~gPlugin()
{
	finalize();
}

template < class I, typename ... InterfaceCreationArgs >
bool gPlugin<I, InterfaceCreationArgs...>::gPlugin::initialize()
{
	if (m_dlHandle)
		return true;

#ifdef _WIN32
	m_dlHandle = LoadLibrary(m_libFileName.c_str());
	if (!m_dlHandle)
		return false;

	HMODULE handle = static_cast<HMODULE>(m_dlHandle);

	m_lpfnCreate = reinterpret_cast<fnCreate>(GetProcAddress(handle, m_fnCreateName.c_str()));
	m_lpfnDestroy = reinterpret_cast<fnDestroy>(GetProcAddress(handle, m_fnDestroyName.c_str()));

	typedef void(*fnThrowException)(void*);
	fnThrowException lpfnThrowException = reinterpret_cast<fnThrowException>(GetProcAddress(handle, "setThrowExceptionFunction"));
	if (lpfnThrowException)
		lpfnThrowException(&throwException);
#endif
	return true;
}

template < class I, typename ... InterfaceCreationArgs >
void gPlugin<I, InterfaceCreationArgs...>::setParameters(
	const gString& libFileName,
	const gString& fnCreateName,
	const gString& fnDestroyName)
{
	finalize();
	m_libFileName = libFileName;
	m_fnCreateName = fnCreateName;
	m_fnDestroyName = fnDestroyName;
}


template < class I, typename ... InterfaceCreationArgs >
std::shared_ptr<I> gPlugin<I, InterfaceCreationArgs...>
::createInterface(InterfaceCreationArgs ... args)
{
	std::shared_ptr<I> sp;

	if (!initialize())
		return sp;

	I* _obj = m_lpfnCreate(args...);
	sp = std::shared_ptr<I>(_obj, m_lpfnDestroy);
	return sp;
}

template < class I, typename ... InterfaceCreationArgs >
void gPlugin<I, InterfaceCreationArgs...>
::finalize()
{
#ifdef _WIN32
	if (m_dlHandle)
	{
		ECHECK(FreeLibrary((HMODULE)m_dlHandle), gString("Failed to unload ") + m_libFileName);
		FreeLibrary((HMODULE)m_dlHandle);;
		m_dlHandle = NULL;
	}
#endif
	m_lpfnCreate = NULL;
	m_lpfnDestroy = NULL;
}

// ------------------------------------
//
//		*** class gEngineErrorHandler ***
//
// ------------------------------------

gEngineExceptionHandler::gEngineExceptionHandler(const gString& location,
	const gString& file, int line, const gString& message)
{
	m_exceptionDescription = "Exception in \"" + location + "\"\n" + "File: " + file + "\nLine: " + std::to_string(line);
	ELOGERR(message + gString(" ( ") + gString(m_exceptionDescription) + gString(" )"));

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

g3RDEngine::g3RDEngine() : 
	m_gapi(eRENDERAPI::RA_NOT_SUPPORT)
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

		// Create logger
		m_spLogger =
			std::make_shared<gLogger>(m_spPlatform, "Log.html");

		// Plug-In Graphics Lib
#ifdef _WIN32
#if 0
	#ifdef _DEBUG
		m_graphicsPlugin.setParameters("GDX9_d.dll", "createGraphicsDX9", "destroyGraphicsDX9");
	#else
		m_graphicsPlugin.setParameters("GDX9.dll", "createGraphicsDX9", "destroyGraphicsDX9");
	#endif
#else
	#ifdef _DEBUG
		m_graphicsPlugin.setParameters("GDX12_d.dll", "createGraphicsDX12", "destroyGraphicsDX12");
	#else
		m_graphicsPlugin.setParameters("GDX12.dll", "createGraphicsDX12", "destroyGraphicsDX12");
	#endif
#endif
#endif
		m_spGraphics = m_graphicsPlugin.createInterface(m_spPlatform, m_spConfiguration);
		ECHECK(m_spGraphics, "Failed load graphics plugin");

		// Try to init systems ( first init config for all systems )
		ECHECK(m_spLogger->initialize(), "Failed logger initialization!");
		ECHECK(m_spConfiguration->initialize(), "Failed load configuration file!");
		ECHECK(m_spPlatform->initialize(), "Failed platform system initialization!");
		ECHECK(m_spGraphics->initialize(), "Failed graphics system initialization!");
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
#ifdef _WIN32
		MessageBox( 0, e.getExceptionDescription(), "3RDE Game Engine", MB_ICONERROR );
#endif
		finalize();
	}
	return true;
}

int g3RDEngine::run()
{
	if (m_spPlatform.get() == 0) 
		return false;

	bool renderEnded;
	bool run;

	runRenderingThread( &renderEnded, &run );

	bool runResult = m_spPlatform->runMainLoop( &renderEnded, &run );

	finalize(); // ???
	return runResult ? 0 : -1;
}

bool g3RDEngine::finalize()
{
	m_spConfiguration.reset();
	m_spInput.reset();
	m_spGraphics.reset();
	m_spSounds.reset();
	m_spPhysics.reset();
	m_spSceneGraph.reset();
	m_spUserInterface.reset();
	m_spScripts.reset();
	m_spResources.reset();
	m_spPlatform.reset();
	m_spLogger.reset();
	
	return true;
}

void g3RDEngine::runRenderingThread( bool* renderEnded, bool* renderRunned )
{
	// run rendering loop
	//bool run = false;
	//bool renderEnded = false;
	*renderRunned = false;
	*renderEnded = false;

	auto renderQueue = m_spGraphics->getRenderQueue();

	auto renderingLoop
	{
		[=]()
		{
			while (*renderRunned != false)
			{
				renderQueue->execute();
				//Sleep(1);
			}
			*renderEnded = true;
		}
	};
	*renderRunned = true;
	std::thread renderThread(renderingLoop);

	renderThread.detach();
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
