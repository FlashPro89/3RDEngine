#include <Windows.h>
#include "../include/3RDE.h"
#include "../include/3RDMath.h"
#include <math.h>

//test
#include <d3dx9.h>
#pragma comment( lib, "d3dx9.lib" )

#ifdef _DEBUG
#pragma comment ( lib, "../lib/3RDEngine_d.lib" )
#else
#pragma comment ( lib, "../lib/3RDEngine.lib" )
#endif

#include<assert.h>

const float G_EPSILON = 6E-6f;

void compareMatrixes(const D3DXMATRIX& dxM, const gMatrix4& gM)
{
	float delta_11 = fabsf(fabsf(dxM._11) - fabsf(gM._11));
	float delta_12 = fabsf(fabsf(dxM._12) - fabsf(gM._12));
	float delta_13 = fabsf(fabsf(dxM._13) - fabsf(gM._13));
	float delta_14 = fabsf(fabsf(dxM._14) - fabsf(gM._14));

	float delta_21 = fabsf(fabsf(dxM._21) - fabsf(gM._21));
	float delta_22 = fabsf(fabsf(dxM._22) - fabsf(gM._22));
	float delta_23 = fabsf(fabsf(dxM._23) - fabsf(gM._23));
	float delta_24 = fabsf(fabsf(dxM._24) - fabsf(gM._24));

	float delta_31 = fabsf(fabsf(dxM._31) - fabsf(gM._31));
	float delta_32 = fabsf(fabsf(dxM._32) - fabsf(gM._32));
	float delta_33 = fabsf(fabsf(dxM._33) - fabsf(gM._33));
	float delta_34 = fabsf(fabsf(dxM._34) - fabsf(gM._34));

	float delta_41 = fabsf(fabsf(dxM._41) - fabsf(gM._41));
	float delta_42 = fabsf(fabsf(dxM._42) - fabsf(gM._42));
	float delta_43 = fabsf(fabsf(dxM._43) - fabsf(gM._43));
	float delta_44 = fabsf(fabsf(dxM._44) - fabsf(gM._44));

	if ( (delta_11 > G_EPSILON) || (delta_12 > G_EPSILON) || (delta_13 > G_EPSILON) || (delta_14 > G_EPSILON) ||
		 (delta_21 > G_EPSILON) || (delta_22 > G_EPSILON) || (delta_23 > G_EPSILON) || (delta_24 > G_EPSILON) ||
		 (delta_31 > G_EPSILON) || (delta_32 > G_EPSILON) || (delta_33 > G_EPSILON) || (delta_34 > G_EPSILON) ||
		 (delta_41 > G_EPSILON) || (delta_42 > G_EPSILON) || (delta_43 > G_EPSILON) || (delta_44 > G_EPSILON) )

		throw( "Matrixes Compare Failed!" );
}

void compareVectors4(const D3DXVECTOR4& dxV, const gVector4& gV)
{
	float deltaA = (fabsf(fabsf(dxV.x) - fabsf(gV.x)));
	float deltaB = (fabsf(fabsf(dxV.y) - fabsf(gV.y)));
	float deltaC = (fabsf(fabsf(dxV.z) - fabsf(gV.z)));
	float deltaD = (fabsf(fabsf(dxV.w) - fabsf(gV.w)));

	if( deltaA > G_EPSILON ||
		deltaB > G_EPSILON || 
		deltaC > G_EPSILON || 
		deltaD > G_EPSILON )
	throw("Vector4 Compare Failed!");
}

void compareVectors3(const D3DXVECTOR3& dxV, const gVector3& gV)
{
	float deltaA = (fabsf(fabsf(dxV.x) - fabsf(gV.x)));
	float deltaB = (fabsf(fabsf(dxV.y) - fabsf(gV.y)));
	float deltaC = (fabsf(fabsf(dxV.z) - fabsf(gV.z)));

	if (deltaA > G_EPSILON ||
		deltaB > G_EPSILON ||
		deltaC > G_EPSILON )

		throw("Vector3 Compare Failed!");
}

void compareVectors2(const D3DXVECTOR2& dxV, const gVector2& gV)
{
	float deltaA = (fabsf(fabsf(dxV.x) - fabsf(gV.x)));
	float deltaB = (fabsf(fabsf(dxV.y) - fabsf(gV.y)));

	if (deltaA > G_EPSILON ||
		deltaB > G_EPSILON  )

		throw("Vector2 Compare Failed!");
}

void compareQuats(const D3DXQUATERNION& dxQ, const gQuaternion& gQ)
{
	float deltaA = (fabsf(fabsf(dxQ.x) - fabsf(gQ.x)));
	float deltaB = (fabsf(fabsf(dxQ.y) - fabsf(gQ.y)));
	float deltaC = (fabsf(fabsf(dxQ.z) - fabsf(gQ.z)));
	float deltaD = (fabsf(fabsf(dxQ.w) - fabsf(gQ.w)));

	if ( ( deltaA > G_EPSILON ) ||
		( deltaB > G_EPSILON ) ||
		( deltaC > G_EPSILON ) ||
		( deltaD > G_EPSILON ) 	)

		throw("Quternions Compare Failed!");
}

void comparePlanes(const D3DXPLANE& dxP, const gPlane& gP)
{
	float deltaA = (fabsf(fabsf(dxP.a) - fabsf(gP.a)));
	float deltaB = (fabsf(fabsf(dxP.b) - fabsf(gP.b)));
	float deltaC = (fabsf(fabsf(dxP.c) - fabsf(gP.c)));
	float deltaD = (fabsf(fabsf(dxP.d) - fabsf(gP.d)));

	if( deltaA > G_EPSILON  ||
		deltaB > G_EPSILON  ||
		deltaC > G_EPSILON	||
		deltaD > G_EPSILON  )
		throw("Planes Compare Failed!");
}

void mathlib_test()
{
	try
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

		// единична€ длина векторов
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
		D3DXVECTOR3 planeP(10.7f, 30.3f, -71.f), planeN(-1.f, 2.f, -0.5f);
		D3DXVec3Normalize(&planeN, &planeN);
		D3DXPlaneFromPointNormal(&plane, &planeP, &planeN);

		gPlane p;
		gVector3 pP(10.7f, 30.3f, -71.f), pN(-1.f, 2.f, -0.5f);
		//pN.normalize();
		p.fromPointNormal(pP, pN);

		comparePlanes(plane, p);

		//test plane from 3 points
		D3DXVECTOR3 p0(10.f, 30.f, 5.f), p1(40.f, 20.f, -5.f), p2(5.f, 6.f, 90.f);
		D3DXPlaneFromPoints(&plane, &p0, &p1, &p2);

		gVector3 gp0(10.f, 30.f, 5.f), gp1(40.f, 20.f, -5.f), gp2(5.f, 6.f, 90.f);
		p.fromPoints(gp0, gp1, gp2);

		comparePlanes(plane, p);

		//plane normalize test
		p.fromEquationConstants(10.f, 20.f, 30.f, 1.f);
		plane = { 10.f, 20.f, 30.f, 1.f };
		p.normalize();
		D3DXPlaneNormalize(&plane, &plane);

		comparePlanes(plane, p);

		// plane test project to plane

		p.fromPointNormal(gVector3(10.f, 0.f, 0.f), gVector3(1.f, 0.f, 0.f));
		p.normalize();
		gVector3 projected = p.projectPointToPlane(gVector3(0.f, 0.f, 0.f));

		float projPointDist = p.getDistanceTo(projected);
		float distPointToPlane = projected.distanceTo(gVector3(0.f, 0.f, 0.f));

		//quat test
		{
			D3DXQUATERNION quat, quat2, quat3, quatr;
			gQuaternion q,q2,q3, qr;
			gVector3 angles;
			q.identity();

			D3DXQuaternionRotationYawPitchRoll(&quat, 0.f, -gPI_4_f, 0.f);
			q.fromXYZAngles(-gPI_4_f, 0.f, 0.f);
			angles = q.getXYZAngles();
			compareQuats(quat, q);

			D3DXQuaternionRotationYawPitchRoll(&quat, -gPI_4_f, 0.f, 0.f);
			q.fromXYZAngles(0.f, -gPI_4_f, 0.f);
			angles = q.getXYZAngles();
			compareQuats(quat, q);

			D3DXQuaternionRotationYawPitchRoll(&quat, 0.f, 0.f, -gPI_4_f);
			q.fromXYZAngles(0.f, 0.f, -gPI_4_f);
			angles = q.getXYZAngles();
			compareQuats(quat, q);

			D3DXQuaternionRotationYawPitchRoll(&quat, -10 *gPI_8_f, -10 * gPI_16_f, -10 * gPI_4_f);
			q.fromXYZAngles(-10 * gPI_16_f, -10 * gPI_8_f, -10 * gPI_4_f);
			angles = q.getXYZAngles();
			compareQuats(quat, q);

			D3DXQuaternionRotationYawPitchRoll(&quat, 10 * gPI_8_f, 10 * gPI_16_f, 10 * gPI_4_f);
			q.fromXYZAngles(10 * gPI_16_f, 10 *gPI_8_f, 10 * gPI_4_f);
			angles = q.getXYZAngles();
			compareQuats(quat, q);

			//test mul & inverse quats
			D3DXQuaternionRotationYawPitchRoll( &quat, 0, gPI_8_f, 0 );
			D3DXQuaternionRotationYawPitchRoll( &quat2, gPI_2_f, 0.f, 0);
			D3DXQuaternionRotationYawPitchRoll(&quat3, 0, 0.f, gPI_4_f);
			//D3DXVECTOR3 axis2(0.f, 1.f, 0.f), axis1(1.f, 0.f, 0.f), axis3( 0.f, 0.f, 1.f );
			//D3DXQuaternionRotationAxis(&quat, &axis1, gPI_4_f);
			//D3DXQuaternionRotationAxis(&quat2, &axis2, gPI_8_f);
			//D3DXQuaternionRotationAxis(&quat3, &axis3, gPI_2_f);

			q.fromXYZAngles( gPI_8_f, 0.f, 0.f );
			q2.fromXYZAngles( 0.f, gPI_2_f, 0.f );
			q3.fromXYZAngles(0.f, 0.f, gPI_4_f);
			//q.fromAxisAngle(gVector3(1.f, 0.f, 0.f), gPI_4_f);
			q2.fromAxisAngle(gVector3(0.f, 1.f, 0.f), gPI_2_f);
			//q3.fromAxisAngle(gVector3(0.f, 0.f, 1.f), gPI_2_f);


			D3DXQuaternionMultiply(&quatr, &quat, &quat2);
			D3DXQuaternionMultiply(&quatr, &quatr, &quat3);
			qr = q3 * q2 * q;

			compareQuats(quat, q);
			compareQuats(quat2, q2);
			compareQuats(quat3, q3);
			compareQuats(quatr, qr); // quats mul test

			qr = q3 *= q2;
			qr *= q;
			//compareQuats(quatr, q);
			compareQuats(quatr, qr); 

			//quat inverse test
			qr.inverse();
			D3DXQuaternionInverse(&quat, &quatr);
			compareQuats(quat, qr); 

			//test vec transform by quat
			gVector3 v1, v2, v3;
			v1 = gVector3(1.f, 0.f, 0.f);
			v2 = gVector3(0.f, 0.f, 1.f);
			v3 = gVector3(-1.f, 0.f, 0.f);

			gVector3 t1 = q2.rotateVec(v1);
			gVector3 t2 = q2.rotateVec(v2);
			gVector3 t3 = q2.rotateVec(v3);

			D3DXMATRIX mRot;
			D3DXMatrixRotationY(&mRot, gPI_2_f );

			D3DXVECTOR3 dxv1(1.f, 0.f, 0.f);
			D3DXVECTOR3 dxv2(0.f, 0.f, 1.f);
			D3DXVECTOR3 dxv3(-1.f, 0.f, 0.f);
			
			D3DXVec3TransformCoord( &dxv1, &dxv1, &mRot );
			D3DXVec3TransformCoord( &dxv2, &dxv2, &mRot );
			D3DXVec3TransformCoord( &dxv3, &dxv3, &mRot );

			compareVectors3(dxv1, t1);
			compareVectors3(dxv2, t2);
			compareVectors3(dxv3, t3);

			int a = 3;
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

			//test rotations
			compareMatrixes(dxMrX, gMX);
			compareMatrixes(dxMrY, gMY);
			compareMatrixes(dxMrZ, gMZ);

			//test invert matrixes
			compareMatrixes(invertedX, gInvX);
			compareMatrixes(invertedY, gInvY);
			compareMatrixes(invertedZ, gInvZ);

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

			//test translations
			compareMatrixes(dxMrX, gMX);
			compareMatrixes(dxMrY, gMY);
			compareMatrixes(dxMrZ, gMZ);

			//test invert matrixes
			compareMatrixes(invertedX, gInvX);
			compareMatrixes(invertedY, gInvY);
			compareMatrixes(invertedZ, gInvZ);

			//test vect3 vec4 transform
			D3DXVECTOR3 vec3(10.f, 20.f, 30.f);
			D3DXVECTOR4 vec4(10.f, 20.f, 30.f,2.f), vec3r, vec4r;

			D3DXVec3Transform(&vec3r, &vec3, &dxMrX);
			D3DXVec4Transform(&vec4r, &vec4, &dxMrX);

			gVector3 gvec3(10.f, 20.f, 30.f);
			gVector4 gvec4(10.f, 20.f, 30.f, 2.f);

			gMX.transformVec(gvec3);
			gMX.transformVec(gvec4);

			compareVectors3( D3DXVECTOR3( vec3r.x, vec3r.y, vec3r.z ) , gvec3 );
			compareVectors4(vec4r, gvec4);

			int g = 654;
		}

		//test mat concat
		{	
			D3DXMATRIX dxMRot, dxMTrans, dxMRes;
			D3DXMatrixTranslation( &dxMTrans, 100.f, 0.f, 100.f);
			D3DXMatrixRotationX(&dxMRot, gPI_8_f);
			D3DXMatrixMultiply(&dxMRes, &dxMRot, &dxMTrans);

			gMatrix4 matTr, matRot, matRes, matRes2;
			matTr.setTranslation(100.f, 0.f, 100.f);
			matRot.setRotationX(gPI_8_f);
			matRes = matRot * matTr;

			matRes2 = matRot;
			matRes2 *= matTr;

			compareMatrixes( dxMRes, matRes );
			compareMatrixes( dxMRes, matRes2 );

			int a = 2;
		}

		//test proj & view matrixes
		{
			D3DXMATRIX dxView;
			D3DXVECTOR3 vEye(0.f, 0.f, -10.f);
			D3DXVECTOR3 vAt(0.f, 0.f, 0.f);
			D3DXVECTOR3 vUp(0.f, 1.f, 0.f);

			D3DXMatrixLookAtRH(&dxView, &vEye, &vAt, &vUp);
			gMatrix4 gView;
			gView.setLookAtRH(gVector3(0.f, 0.f, -10.f), gVector3(0.f, 0.f, 0.f));

			D3DXMATRIX dxPerspProj;
			D3DXMatrixPerspectiveFovRH(&dxPerspProj, 1.33334f, 1.f, 0.f, 1.f);
			gMatrix4 gPerspProj;
			gPerspProj.setPerspetiveRHFOV(1.33334f, 1.f, 0.f, 1.f);

			D3DXMATRIX dxOrthoProj;
			D3DXMatrixOrthoRH(&dxOrthoProj, 800.f, 600.f, 0.f, 1.f);
			gMatrix4 gOrthoProj;
			gOrthoProj.setOrthoRH(800.f, 600.f, 0.f, 1.f);

			compareMatrixes(dxView, gView);
			compareMatrixes(dxPerspProj, gPerspProj);
			compareMatrixes(dxOrthoProj, gOrthoProj);
		
		}
	}
	catch (const char* msg)
	{
		MessageBox(0, msg, "MathLib test failed!", MB_APPLMODAL);
	}
}

void testASM()
{
	//float* pResult = (float*)_aligned_malloc(16 * sizeof(float), 16);

	//_freea(pResult);
}

int WINAPI WinMain
(
	HINSTANCE hInstance,	// указатель на текущий экземпл€р
	HINSTANCE hPrevInstance,	// указатель на предыдущйи запушенный экземпл€р
	LPSTR lpCmdLine,		// командна€ строка
	int nCmdShow		// тип отображени€ окна программы
)
{

	SP3RDENGINE engine = Create3RDEngine();

	

	mathlib_test();
	testASM();

	if (engine->initialize(eRENDERAPI::RA_DX12))
		engine->run();
	

	return 0;
}