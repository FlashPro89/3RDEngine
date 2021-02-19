#pragma once

#ifndef _3RDE_H_
#define _3RDE_H_

#include <memory>

#ifdef _3RDENGINE_EXPORTS
#define _3RDE_API_ __declspec(dllexport)
#define _3RDE_API_C_ extern "C" __declspec(dllexport)
#else
#define _3RDE_API_ __declspec(dllimport)
#define _3RDE_API_C_ extern "C" __declspec(dllimport)
#endif

enum class eRENDERAPI
{
	RA_NOT_SUPPORT = -1,
	RA_DX9 = 0,
	RA_DX12 = 1,
	RA_VULKAN = 2 // ??
};

class _3RDE_API_ I3RDEngine;

typedef std::shared_ptr<I3RDEngine> SP3RDENGINE;
typedef std::weak_ptr<I3RDEngine> WP3RDENGINE;

class _3RDE_API_ I3RDEngine
{
public:
	I3RDEngine() {}
	virtual ~I3RDEngine() {};

	static SP3RDENGINE get(); // return shared pointer of engine single instance 

	virtual void setApplicationName( const char* applicationName ) = 0;
	virtual bool initialize( const char* config, bool useAsConfigBuffer = false ) = 0;
	virtual bool run() = 0; // run main game loop, initialize first

protected:
	virtual bool finalize() = 0;
};

_3RDE_API_ SP3RDENGINE Get3RDEngine();

#endif
