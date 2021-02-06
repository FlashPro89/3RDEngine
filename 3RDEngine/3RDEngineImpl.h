#pragma once

#ifndef _3RENGINEIMPL_H_
#define _3RENGINEIMPL_H_

#include "../include/3RDEngine.h"
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

class g3RDEngineImpl : public I3RDEngine
{
public:
	g3RDEngineImpl();
	~g3RDEngineImpl();

	eRENDERAPI getLatestSupportedGAPI();

	bool initialize( eRENDERAPI gapi );
	bool finalize();

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
	eRENDERAPI m_gapi;
};

#endif

