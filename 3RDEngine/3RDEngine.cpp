#include <windows.h>
#include "3RDEngine.h"

using namespace std;

// ------------------------------------
//
//		*** class g3REngine ***
//
// ------------------------------------

g3RDEngine::g3RDEngine() : m_gapi(eRENDERAPI::RA_NOT_SUPPORT)
{

}

g3RDEngine::~g3RDEngine()
{
	finalize();
	MessageBox(0, "Engine Destructor from DLL", "Destructor", MB_SYSTEMMODAL);
}

eRENDERAPI g3RDEngine::getLatestSupportedGAPI()
{
	return eRENDERAPI::RA_NOT_SUPPORT; // заглушка
}

bool g3RDEngine::initialize(eRENDERAPI api)
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

void g3RDEngine::run()
{
	return;
}

bool g3RDEngine::finalize()
{
	return true;
}

//objects getters
SPPLATFORM g3RDEngine::Platform()
{
	return shared_ptr<IPlatform>(0);
}

SPRESOURCES	g3RDEngine::Resources()
{
	return shared_ptr<IResources>(0);
}

SPINPUT	g3RDEngine::Input()
{
	return shared_ptr<IInput>(0);
}

SPGRAPHICS g3RDEngine::Graphics()
{
	return shared_ptr<IGraphics>(0);
}

SPSOUNDS g3RDEngine::Sounds()
{
	return shared_ptr<ISounds>(0);
}

SPPHYSICS g3RDEngine::Physics()
{
	return shared_ptr<IPhysics>(0);
}

SPSCENEGRAPH g3RDEngine::SceneGraph()
{
	return shared_ptr<ISceneGraph>(0);
}

SPUSERINTERFACE g3RDEngine::UserInterface()
{
	return shared_ptr<IUserInterface>(0);
}

SPLOGGER g3RDEngine::Logger()
{
	return shared_ptr<ILogger>(0);
}