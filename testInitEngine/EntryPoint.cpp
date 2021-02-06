#include <Windows.h>
#include "../include/3RDEngine.h"
#include "../include/3RDMath.h"

#ifdef _DEBUG
#pragma comment ( lib, "../lib/3RDEngine_d.lib" )
#else
#pragma comment ( lib, "../lib/3RDEngine.lib" )
#endif


int WINAPI WinMain
(
	HINSTANCE hInstance,	// указатель на текущий экземпл€р
	HINSTANCE hPrevInstance,	// указатель на предыдущйи запушенный экземпл€р
	LPSTR lpCmdLine,		// командна€ строка
	int nCmdShow		// тип отображени€ окна программы
)
{
	SP3RDENGINE engine = Create3RDEngine();

	gMatrix4 m;
	m.identity();
	float d = m.determinant();

	gVector3 right( 1.f, 0.f, 0.f);
	gVector3 up(0.f, 1.f, 0.f);

	gVector3 cross = right ^ up;
	gVector3 tmp = right;
	tmp ^= up;

	engine->initialize(eRENDERAPI::RA_DX12);
	engine->finalize();
	return 0;
}