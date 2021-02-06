#include <windows.h>
#include "3RDEngineImpl.h"

using namespace std;

// ------------------------------------
//
//		*** class g3REngineImp ***
//
// ------------------------------------

g3RDEngineImpl::g3RDEngineImpl() : m_gapi(eRENDERAPI::RA_NOT_SUPPORT)
{

}

g3RDEngineImpl::~g3RDEngineImpl()
{
	MessageBox(0, "Engine Destructor from DLL", "Destructor", MB_SYSTEMMODAL);
}

eRENDERAPI g3RDEngineImpl::getLatestSupportedGAPI()
{
	return eRENDERAPI::RA_NOT_SUPPORT; // заглушка
}

bool g3RDEngineImpl::initialize(eRENDERAPI api)
{
	switch (api)
	{
	case eRENDERAPI::RA_DX9:
		return true;
	case eRENDERAPI::RA_DX12:
		return true;
	}
	return false;
}

bool g3RDEngineImpl::finalize()
{
	return true;
}

//objects getters
SPPLATFORM g3RDEngineImpl::Platform()
{
	return shared_ptr<IPlatform>(0);
}

SPRESOURCES	g3RDEngineImpl::Resources()
{
	return shared_ptr<IResources>(0);
}

SPINPUT	g3RDEngineImpl::Input()
{
	return shared_ptr<IInput>(0);
}

SPGRAPHICS g3RDEngineImpl::Graphics()
{
	return shared_ptr<IGraphics>(0);
}

SPSOUNDS g3RDEngineImpl::Sounds()
{
	return shared_ptr<ISounds>(0);
}

SPPHYSICS g3RDEngineImpl::Physics()
{
	return shared_ptr<IPhysics>(0);
}

SPSCENEGRAPH g3RDEngineImpl::SceneGraph()
{
	return shared_ptr<ISceneGraph>(0);
}

SPUSERINTERFACE g3RDEngineImpl::UserInterface()
{
	return shared_ptr<IUserInterface>(0);
}

SPLOGGER g3RDEngineImpl::Logger()
{
	return shared_ptr<ILogger>(0);
}