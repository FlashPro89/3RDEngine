#pragma once
#ifndef _GDX12_H_
#endif _GDX12_H_

#ifdef _WIN32

#include "../3RDEngine/Interfaces.h"

#ifdef GDX12_EXPORTS
#define GDX12_API __declspec(dllexport)
#define GDX12_API_C extern "C" __declspec(dllexport)
#else
#define GDX12_API __declspec(dllimport)
#define GDX12_API_C extern "C" __declspec(dllimport)
#endif

GDX12_API_C IGraphics* createGraphicsDX12(SPPLATFORM platform, SPCONFIGURATION configuration);
GDX12_API_C void destroyGraphicsDX12(IGraphics*);
GDX12_API_C void setThrowExceptionFunction(void* fn);


#endif

