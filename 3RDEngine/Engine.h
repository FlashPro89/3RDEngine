#pragma once

#ifndef _3RDENGINE_H_
#define _3RDENGINE_H_

#include "../include/3RDE.h"
#include "Interfaces.h"
#include <string>

class gEngineExceptionHandler : public IExceptionHandler
{
public:
	gEngineExceptionHandler( const gString& location, const gString& file, int line, const gString& message = "" );
	~gEngineExceptionHandler();

	const char* getExceptionDescription() const;

protected:
	gString m_exceptionDescription;
};

class g3RDEngine : public I3RDEngine
{
public:
	g3RDEngine();
	~g3RDEngine();

	// need move to GraphicsSystem
	eRENDERAPI getLatestSupportedGAPI();

	void setApplicationName(const char* applicationName);
	bool initialize( const char* config, bool useAsConfigBuffer );
	bool run();

	//objects getters
	SPPLATFORM getPlatform();
	SPRESOURCES	getResources();
	SPINPUT	getInput();
	SPGRAPHICS getGraphics();
	SPSOUNDS getSounds();
	SPPHYSICS getPhysics();
	SPSCENEGRAPH getSceneGraph();
	SPUSERINTERFACE getUserInterface();
	SPSCRIPTS getScripts();
	SPLOGGER getLogger();
	SPCONFIGURATION getConfiguration();

protected:
	bool finalize();

	gString m_applicationName;
	eRENDERAPI m_gapi;

	SPPLATFORM m_spPlatform;
	SPRESOURCES	m_spResources;
	SPINPUT	m_spInput;
	SPGRAPHICS m_spGraphics;
	SPSOUNDS m_spSounds;
	SPPHYSICS m_spPhysics;
	SPSCENEGRAPH m_spSceneGraph;
	SPUSERINTERFACE m_spUserInterface;
	SPSCRIPTS m_spScripts;
	SPLOGGER m_spLogger;
	SPCONFIGURATION m_spConfiguration;
};

#define ENGINE (static_cast<g3RDEngine*>(I3RDEngine::get().get()))
#define EPLATFORM (static_cast<g3RDEngine*>(I3RDEngine::get().get()))->getPlatform()
#define ERESOURCES (static_cast<g3RDEngine*>(I3RDEngine::get().get()))->getResources()
#define EINPUT (static_cast<g3RDEngine*>(I3RDEngine::get().get()))->getInput()
#define EGRAPHICS (static_cast<g3RDEngine*>(I3RDEngine::get().get()))->getGraphics()
#define ESOUNDS (static_cast<g3RDEngine*>(I3RDEngine::get().get()))->getSounds()
#define EPHYSICS (static_cast<g3RDEngine*>(I3RDEngine::get().get()))->getPhysics()
#define ESCENEGRAPH (static_cast<g3RDEngine*>(I3RDEngine::get().get()))->getSceneGraph()
#define EUI (static_cast<g3RDEngine*>(I3RDEngine::get().get()))->getUserInterface()
#define ESCRIPTS (static_cast<g3RDEngine*>(I3RDEngine::get().get()))->getScripts()
#define ELOGGER (static_cast<g3RDEngine*>(I3RDEngine::get().get()))->getLogger()
#define ECONFIG (static_cast<g3RDEngine*>(I3RDEngine::get().get()))->getConfiguration()

#define ETROW(msg) throw( gEngineExceptionHandler( __FUNCSIG__, __FILE__, __LINE__, (msg) ) )
#define ECHECK(e,msg) if(!e)throw( gEngineExceptionHandler( __FUNCSIG__, __FILE__, __LINE__, (msg) ) )
#define ELOGMSG(...) if( I3RDEngine::get().use_count()!=0)ELOGGER->logMessage(__VA_ARGS__)
#define ELOGWRN(...) if( I3RDEngine::get().use_count()!=0)ELOGGER->logWarning(__VA_ARGS__)
#define ELOGERR(...) if( I3RDEngine::get().use_count()!=0)ELOGGER->logError(__VA_ARGS__)

#endif

