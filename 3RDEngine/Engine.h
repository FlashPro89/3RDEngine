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

#define ETROW(msg) throw( gEngineExceptionHandler( __FUNCSIG__, __FILE__, __LINE__, (msg) ) )
#define ECHECK(e,msg) if(!e)throw( gEngineExceptionHandler( __FUNCSIG__, __FILE__, __LINE__, (msg) ) )

class g3RDEngine : public I3RDEngine
{
public:
	g3RDEngine();
	~g3RDEngine();

	eRENDERAPI getLatestSupportedGAPI();

	bool initialize( eRENDERAPI gapi );
	bool run();

	//objects getters
	SPPLATFORM Platform();
	SPRESOURCES	Resources();
	SPINPUT	Input();
	SPGRAPHICS Graphics();
	SPSOUNDS Sounds();
	SPPHYSICS Physics();
	SPSCENEGRAPH SceneGraph();
	SPUSERINTERFACE UserInterface();
	SPSCRIPTS Scripts();
	SPLOGGER Logger();

protected:
	bool finalize();

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
};

#endif

