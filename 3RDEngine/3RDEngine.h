#pragma once

#ifndef _3RDENGINE_H_
#define _3RDENGINE_H_

#include "../include/3RDE.h"
#include "../include/3RDError.h"
#include <memory>

class _3RDE_API_ IPlatform;
class _3RDE_API_ IResources;
class _3RDE_API_ IInput;
class _3RDE_API_ IGraphics;
class _3RDE_API_ ISounds;
class _3RDE_API_ IPhysics;
class _3RDE_API_ ISceneGraph;
class _3RDE_API_ IUserInterface;
class _3RDE_API_ ILogger;

typedef std::shared_ptr<IPlatform> SPPLATFORM;
typedef std::shared_ptr<IResources> SPRESOURCES;
typedef std::shared_ptr<IInput> SPINPUT;
typedef std::shared_ptr<IGraphics> SPGRAPHICS;
typedef std::shared_ptr<ISounds> SPSOUNDS;
typedef std::shared_ptr<IPhysics> SPPHYSICS;
typedef std::shared_ptr<ISceneGraph> SPSCENEGRAPH;
typedef std::shared_ptr<IUserInterface> SPUSERINTERFACE;
typedef std::shared_ptr<ILogger> SPLOGGER;

class gEngineErrorHandler : public gErrorHandler
{
public:
	gEngineErrorHandler( std::string location, std::string message, std::string file, std::string line);

protected:
	std::string m_location;
	std::string m_message;
	std::string m_file;
	std::string m_line;
};

class g3RDEngine : public I3RDEngine
{
public:
	g3RDEngine();
	~g3RDEngine();

	eRENDERAPI getLatestSupportedGAPI();

	bool initialize(eRENDERAPI gapi);
	void run();

	//objects getters
	SPPLATFORM Platform();
	SPRESOURCES	Resources();
	SPINPUT	Input();
	SPGRAPHICS Graphics();
	SPSOUNDS Sounds();
	SPPHYSICS Physics();
	SPSCENEGRAPH SceneGraph();
	SPUSERINTERFACE UserInterface();
	SPLOGGER Logger();

protected:
	bool finalize();

	eRENDERAPI m_gapi;
};

#endif

