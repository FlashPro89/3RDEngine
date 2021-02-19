#pragma once

#ifndef _3RDE_LOGGER_H_
#define _3RDE_LOGGER_H_

#include "Interfaces.h"

class gLogger :
    public ISystem
{
public:
    gLogger();
    ~gLogger();

protected:
    SPFILE m_spLogger;
};

#endif

