#pragma once
#ifdef _WIN32

#include "../3RDEngine/Interfaces.h"

#ifdef GDX9_EXPORTS
#define GDX9_API __declspec(dllexport)
#define GDX9_API_C extern "C" __declspec(dllexport)
#else
#define GDX9_API __declspec(dllimport)
#define GDX9_API_C extern "C" __declspec(dllimport)
#endif

GDX9_API_C IGraphics* createGraphicsDX9(SPPLATFORM platform, SPCONFIGURATION configuration);
GDX9_API_C void destroyGraphicsDX9(IGraphics*);
GDX9_API_C void setThrowExceptionFunction(void* fn);


#endif
