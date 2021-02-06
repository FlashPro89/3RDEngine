#include "../include/3RDEngine.h"
#include "3RDEngineImpl.h"

using namespace std;

_3RDE_API_ SP3RDENGINE Create3RDEngine()
{	
	SP3RDENGINE spEngine = shared_ptr<g3RDEngineImpl>(new g3RDEngineImpl);

	auto use = spEngine.use_count();
	auto unicue = spEngine.unique();

	return spEngine;
}