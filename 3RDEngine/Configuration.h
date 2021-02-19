#pragma once

#ifndef _3RDE_CONFIGURATION_H_
#define _3RDE_CONFIGURATION_H_

#include "Interfaces.h"

class gConfiguration :
    public IConfiguration
{
public:
    gConfiguration( SPPLATFORM platform, const gString& config, bool useAsConfigBuffer );
    ~gConfiguration();

    bool initialize();

protected:
    gConfiguration() {}
    gConfiguration(const gConfiguration&) {}

    bool parseConfiguration();
    bool finalize();

    gString m_config;
    SPFILE m_file;
};

#endif

