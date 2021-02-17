#pragma once

#ifndef _3RDE_INTERFACES_
#define _3RDE_INTERFACES_

#include <memory>

class ISystem
{
public:
	virtual ~ISystem() {};

	virtual bool initialize() = 0;

protected:
	ISystem() {};
	virtual bool finalize() = 0;
};

class IErrorHandler
{
public:
	virtual ~IErrorHandler() {};

	virtual const char* getErrorLocation() const = 0;
	virtual const char* getErrorMessage() const = 0;
	virtual const char* getFileName() const = 0;
	virtual const char* getFileLine() const = 0;

protected:
	IErrorHandler() {};
};

class IPlatform : public ISystem
{
public:
	virtual ~IPlatform() {};
protected:
	IPlatform() {};
};

class IResources : public ISystem
{
public:
	virtual ~IResources() {};
protected:
	IResources() {};
};

class IInput : public ISystem
{
public:
	virtual ~IInput() {};
protected:
	IInput() {};
};

class IGraphics : public ISystem
{
public:
	virtual ~IGraphics() {};
protected:
	IGraphics() {};
};

class ISounds : public ISystem
{
public:
	virtual ~ISounds() {};
protected:
	ISounds() {};
};

class IPhysics : public ISystem
{
public:
	virtual ~IPhysics() {};
protected:
	IPhysics() {};
};

class ISceneGraph : public ISystem
{
public:
	virtual ~ISceneGraph() {};
protected:
	ISceneGraph() {};
};

class IUserInterface : public ISystem
{
public:
	virtual ~IUserInterface() {};
protected:
	IUserInterface() {};
};

class IScripts : public ISystem
{
public:
	virtual ~IScripts() {};
protected:
	IScripts() {};
};

class ILogger : public ISystem
{
public:
	virtual ~ILogger() {};
protected:
	ILogger() {};
};

typedef std::shared_ptr<IPlatform> SPPLATFORM;
typedef std::shared_ptr<IResources> SPRESOURCES;
typedef std::shared_ptr<IInput> SPINPUT;
typedef std::shared_ptr<IGraphics> SPGRAPHICS;
typedef std::shared_ptr<ISounds> SPSOUNDS;
typedef std::shared_ptr<IPhysics> SPPHYSICS;
typedef std::shared_ptr<ISceneGraph> SPSCENEGRAPH;
typedef std::shared_ptr<IUserInterface> SPUSERINTERFACE;
typedef std::shared_ptr<IScripts> SPSCRIPTS;
typedef std::shared_ptr<ILogger> SPLOGGER;


#endif
