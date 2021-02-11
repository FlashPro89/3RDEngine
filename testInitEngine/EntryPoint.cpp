#include <Windows.h>
#include "../include/3RDEngine.h"
#include "../include/3RDMath.h"

//test
#include <d3dx9.h>
#pragma comment( lib, "d3dx9.lib" )

#ifdef _DEBUG
#pragma comment ( lib, "../lib/3RDEngine_d.lib" )
#else
#pragma comment ( lib, "../lib/3RDEngine.lib" )
#endif

void mathlib_test()
{
	gMatrix4 m(true);
	float d = m.determinant();

	gVector2 v2;
	gVector3 v3;
	gVector4 v4;

	//test null lenght
	float l2 = v2.lenght();
	float l3 = v3.lenght();
	float l4 = v4.lenght();

	v2.normalize();
	v3.normalize();
	v4.normalize();

	l2 = v2.lenght();
	l3 = v3.lenght();
	l4 = v4.lenght();

	v2.set(1.f); v3.set(1.f); v4.set(1.f);

	// ��������� ����� ��������
	l2 = v2.lenght();
	l3 = v3.lenght();
	l4 = v4.lenght();

	v2.normalize();
	v3.normalize();
	v4.normalize();

	l2 = v2.lenght();
	l3 = v3.lenght();
	l4 = v4.lenght();

	//test plane from point & norm
	D3DXPLANE plane;
	D3DXVECTOR3 planeP(10.f, 30.f, 5.f), planeN(-1.f, 2.f, -0.5f);
	//D3DXVec3Normalize(&planeN, &planeN);
	D3DXPlaneFromPointNormal(&plane, &planeP, &planeN);

	gPlane p;
	gVector3 pP(10.f, 30.f, 5.f), pN(-1.f, 2.f, -0.5f);
	//pN.normalize();
	p.fromPointNormalizedNormal(pP, pN);

	//test plane from 3 points
	D3DXVECTOR3 p0(10.f, 30.f, 5.f), p1(40.f, 20.f, -5.f), p2(5.f, 6.f, 90.f);
	D3DXPlaneFromPoints(&plane, &p0, &p1, &p2);

	gVector3 gp0(10.f, 30.f, 5.f), gp1(40.f, 20.f, -5.f), gp2(5.f, 6.f, 90.f);
	p.fromPoints(gp0, gp1, gp2);

	//plane normalize test
	p.fromEquationConstants(10.f, 20.f, 30.f, 1.f);
	plane = { 10.f, 20.f, 30.f, 1.f };
	p.normalize();
	D3DXPlaneNormalize(&plane, &plane);


	// plane test project to plane

	p.fromPointNormal(gVector3(10.f, 0.f, 0.f), gVector3(1.f, 0.f, 0.f));
	p.normalize();
	gVector3 projected = p.projectPointToPlane(gVector3(0.f, 0.f, 0.f));

	float projPointDist = p.getDistanceTo(projected);
	float distPointToPlane = projected.distanceTo(gVector3(0.f, 0.f, 0.f));

	//quat test
	{
		D3DXQUATERNION quat;
		gQuaternion q;
		gVector3 angles;
		q.identity();

		D3DXQuaternionRotationYawPitchRoll(&quat, 0.f, -gPI_4_f, 0.f);
		q.fromPitchYawRoll(-gPI_4_f, 0.f, 0.f);
		angles = q.getPitchYawRoll();

		D3DXQuaternionRotationYawPitchRoll(&quat, -gPI_4_f, 0.f, 0.f);
		q.fromPitchYawRoll(0.f, -gPI_4_f, 0.f);
		angles = q.getPitchYawRoll();

		D3DXQuaternionRotationYawPitchRoll(&quat, 0.f, 0.f, -gPI_4_f);
		q.fromPitchYawRoll(0.f, 0.f, -gPI_4_f);
		angles = q.getPitchYawRoll();

		D3DXQuaternionRotationYawPitchRoll(&quat, -gPI_8_f, -gPI_16_f, -gPI_4_f);
		q.fromPitchYawRoll(-gPI_16_f, -gPI_8_f, -gPI_4_f);
		angles = q.getPitchYawRoll();

		q.identity();
	}
	
	{
		//test matrixes
		D3DXMATRIX dxMrX, dxMrY, dxMrZ, invertedX, invertedY, invertedZ;
		float det;
		//rot

		D3DXMatrixRotationX(&dxMrX, gPI_16_f);
		D3DXMatrixRotationY(&dxMrY, gPI_16_f);
		D3DXMatrixRotationZ(&dxMrZ, gPI_16_f);

		D3DXMatrixInverse(&invertedX, &det, &dxMrX);
		D3DXMatrixInverse(&invertedY, &det, &dxMrY);
		D3DXMatrixInverse(&invertedZ, &det, &dxMrZ);

		gMatrix4 gMX, gMY, gMZ, gInvX, gInvY, gInvZ;

		gMX.setRotationX(gPI_16_f);
		gMY.setRotationY(gPI_16_f);
		gMZ.setRotationZ(gPI_16_f);

		gInvX = gMX;
		gInvY = gMY;
		gInvZ = gMZ;

		gInvX.inverse();
		gInvY.inverse();
		gInvZ.inverse();

		//transl
		D3DXMatrixTranslation(&dxMrX, 100.f, 0.f, 0.f);
		D3DXMatrixTranslation(&dxMrY, 0.f, 100.f, 0.f);
		D3DXMatrixTranslation(&dxMrZ, 0.f, 0.f, 100.f);

		D3DXMatrixInverse(&invertedX, &det, &dxMrX);
		D3DXMatrixInverse(&invertedY, &det, &dxMrY);
		D3DXMatrixInverse(&invertedZ, &det, &dxMrZ);

		gMX.setTranslation(100.f, 0.f, 0.f);
		gMY.setTranslation(0.f, 100.f, 0.f);
		gMZ.setTranslation(0.f, 0.f, 100.f);

		gInvX = gMX;
		gInvY = gMY;
		gInvZ = gMZ;

		gInvX.inverse();
		gInvY.inverse();
		gInvZ.inverse();
	}

	//test mat concat
	{
		gMatrix4 matTr, matRot, matRes, matRes2;
		matTr.setTranslation(100.f, 0.f, 100.f);
		matRot.setRotationX( gPI_8_f );
		matRes = matRot * matTr;

		matRes2 = matRot;
		matRes2 *= matTr;
		int a = 2;
	}

}

void testASM()
{
	float* pResult = (float*)_aligned_malloc(16 * sizeof(float), 16);

	delete[] pResult;
}

int WINAPI WinMain
(
	HINSTANCE hInstance,	// ��������� �� ������� ���������
	HINSTANCE hPrevInstance,	// ��������� �� ���������� ���������� ���������
	LPSTR lpCmdLine,		// ��������� ������
	int nCmdShow		// ��� ����������� ���� ���������
)
{
	SP3RDENGINE engine = Create3RDEngine();

	mathlib_test();
	testASM();

	engine->initialize(eRENDERAPI::RA_DX12);
	engine->finalize();
	return 0;
}