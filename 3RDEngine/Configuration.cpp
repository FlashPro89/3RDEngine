#include "Configuration.h"
#include "Engine.h"

// ------------------------------------
//
//		*** class gConfiguration ***
//
// ------------------------------------

gConfiguration::gConfiguration( SPPLATFORM platform, 
    const gString& config, bool useAsConfigBuffer ) : m_config("")
{
    if (useAsConfigBuffer)
        m_config = config;
    else
    {
        m_file = platform->openFile(config, false, true);
    }
}

gConfiguration::~gConfiguration() 
{
    finalize();
}

bool gConfiguration::initialize()
{
    if (m_config == "") //need to load from file
    {
        if(!m_file->isOpened())
            return false;

        size_t fsize = m_file->getFileSize();
        m_config.resize(fsize + 1);
        if ( fsize != m_file->read(&m_config[0], fsize))
            return false;
    }
    return parseConfiguration();
}

bool gConfiguration::parseConfiguration()
{
    //parse and close file
    m_file.reset();
    return true;
}

bool gConfiguration::finalize()
{
    return false;
}
    
