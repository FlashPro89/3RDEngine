#pragma once

#ifndef _3RDENGINE_H_
#define _3RDENGINE_H_

#include "../include/3RDE.h"
#include "Interfaces.h"
#include <string>

class gEngineErrorHandler : public IErrorHandler
{
public:
	gEngineErrorHandler( const char* location, const char* file, int line, const char* message = 0 );
	~gEngineErrorHandler();

	const char* getErrorLocation() const;
	const char* getErrorMessage() const;
	const char* getFileName() const;
	const char* getFileLine() const;

protected:
	std::string m_location;
	std::string m_message;
	std::string m_file;
	std::string m_line;
};

#define EEXCEPT ( throw(gEngineErrorHandler(__FUNCSIG__, __FILE__, __LINE__)); )
#define EEXCEPT_MSG (msg) ( throw(gEngineErrorHandler(__FUNCSIG__, __FILE__, __LINE__, msg)); )

class g3RDEngine : public I3RDEngine
{
public:
	g3RDEngine();
	~g3RDEngine();

	eRENDERAPI getLatestSupportedGAPI();

	bool initialize(eRENDERAPI gapi);
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

