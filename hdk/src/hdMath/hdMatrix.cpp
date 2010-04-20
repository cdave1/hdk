/*
 *  Matrix.cpp
 *  hdGameEngine
 *
 *  Created by david on 16/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdMath/hdMatrix.h"
#include "hdCore.h"
#ifdef __APPLE__
#include <TargetConditionals.h>

// iPhone Specific
#if (TARGET_OS_IPHONE == 1) && (TARGET_IPHONE_SIMULATOR == 0)
#warning "iPhone"
#ifdef _ARM_ARCH_7
#include "hdMath/neon/neon_matrix_impl.h"
#else
#include "hdMath/vfp/matrix_impl.h"
#endif

#endif
#endif

static bool vfpu = false;

void InitMatrixSettings()
{
#if (TARGET_OS_IPHONE == 1) && (TARGET_IPHONE_SIMULATOR == 0)
	vfpu = (strcmp(SystemSettings_SystemArchName(), "armv6") == 0);
#else
	vfpu = false;
#endif
}



void MatrixIdentity(hdMatrix &mOut)
{
	mOut.f[__11] = 1;	mOut.f[__12] = 0;	mOut.f[__13] = 0;	mOut.f[__14] = 0;
	mOut.f[__21] = 0;	mOut.f[__22] = 1;	mOut.f[__23] = 0;	mOut.f[__24] = 0;
	mOut.f[__31] = 0;	mOut.f[__32] = 0;	mOut.f[__33] = 1;	mOut.f[__34] = 0;
	mOut.f[__41] = 0;	mOut.f[__42] = 0;	mOut.f[__43] = 0;	mOut.f[__44] = 1;
}


void hdMatrixMultiply(hdMatrix			&mOut,
					const hdMatrix	&mA,
					const hdMatrix	&mB)
{

#if (TARGET_OS_IPHONE == 1) && (TARGET_IPHONE_SIMULATOR == 0)
#if defined(_ARM_ARCH_7) && defined(__ARM_NEON__)
	NEON_Matrix4Mul(mA.f,
					mB.f,
					mOut.f);
#else
	if (vfpu)
	{
		Matrix4Mul_Test(mA.f,
						mB.f,
						mOut.f);
	}
	else
	{
		hdMatrix ret;
		
		ret.f[__11] = mA.f[__11]*mB.f[__11] + mA.f[__12]*mB.f[__21] + mA.f[__13]*mB.f[__31] + mA.f[__14]*mB.f[__41];	
		ret.f[__12] = mA.f[__11]*mB.f[__12] + mA.f[__12]*mB.f[__22] + mA.f[__13]*mB.f[__32] + mA.f[__14]*mB.f[__42];	
		ret.f[__13] = mA.f[__11]*mB.f[__13] + mA.f[__12]*mB.f[__23] + mA.f[__13]*mB.f[__33] + mA.f[__14]*mB.f[__43];	
		ret.f[__14] = mA.f[__11]*mB.f[__14] + mA.f[__12]*mB.f[__24] + mA.f[__13]*mB.f[__34] + mA.f[__14]*mB.f[__44];
		
		ret.f[__21] = mA.f[__21]*mB.f[__11] + mA.f[__22]*mB.f[__21] + mA.f[__23]*mB.f[__31] + mA.f[__24]*mB.f[__41];	
		ret.f[__22] = mA.f[__21]*mB.f[__12] + mA.f[__22]*mB.f[__22] + mA.f[__23]*mB.f[__32] + mA.f[__24]*mB.f[__42];	
		ret.f[__23] = mA.f[__21]*mB.f[__13] + mA.f[__22]*mB.f[__23] + mA.f[__23]*mB.f[__33] + mA.f[__24]*mB.f[__43];	
		ret.f[__24] = mA.f[__21]*mB.f[__14] + mA.f[__22]*mB.f[__24] + mA.f[__23]*mB.f[__34] + mA.f[__24]*mB.f[__44];
		
		ret.f[__31] = mA.f[__31]*mB.f[__11] + mA.f[__32]*mB.f[__21] + mA.f[__33]*mB.f[__31] + mA.f[__34]*mB.f[__41];	
		ret.f[__32] = mA.f[__31]*mB.f[__12] + mA.f[__32]*mB.f[__22] + mA.f[__33]*mB.f[__32] + mA.f[__34]*mB.f[__42];	
		ret.f[__33] = mA.f[__31]*mB.f[__13] + mA.f[__32]*mB.f[__23] + mA.f[__33]*mB.f[__33] + mA.f[__34]*mB.f[__43];	
		ret.f[__34] = mA.f[__31]*mB.f[__14] + mA.f[__32]*mB.f[__24] + mA.f[__33]*mB.f[__34] + mA.f[__34]*mB.f[__44];
		
		ret.f[__41] = mA.f[__41]*mB.f[__11] + mA.f[__42]*mB.f[__21] + mA.f[__43]*mB.f[__31] + mA.f[__44]*mB.f[__41];
		ret.f[__42] = mA.f[__41]*mB.f[__12] + mA.f[__42]*mB.f[__22] + mA.f[__43]*mB.f[__32] + mA.f[__44]*mB.f[__42];
		ret.f[__43] = mA.f[__41]*mB.f[__13] + mA.f[__42]*mB.f[__23] + mA.f[__43]*mB.f[__33] + mA.f[__44]*mB.f[__43];
		ret.f[__44] = mA.f[__41]*mB.f[__14] + mA.f[__42]*mB.f[__24] + mA.f[__43]*mB.f[__34] + mA.f[__44]*mB.f[__44];
		
		mOut = ret;
	}
#endif	
	//Matrix4Mul(mA.f,
	//		   mB.f,
	//		   mOut.f);
	
#else
	hdMatrix ret;
	
	ret.f[__11] = mA.f[__11]*mB.f[__11] + mA.f[__12]*mB.f[__21] + mA.f[__13]*mB.f[__31] + mA.f[__14]*mB.f[__41];	
	ret.f[__12] = mA.f[__11]*mB.f[__12] + mA.f[__12]*mB.f[__22] + mA.f[__13]*mB.f[__32] + mA.f[__14]*mB.f[__42];	
	ret.f[__13] = mA.f[__11]*mB.f[__13] + mA.f[__12]*mB.f[__23] + mA.f[__13]*mB.f[__33] + mA.f[__14]*mB.f[__43];	
	ret.f[__14] = mA.f[__11]*mB.f[__14] + mA.f[__12]*mB.f[__24] + mA.f[__13]*mB.f[__34] + mA.f[__14]*mB.f[__44];
	
	ret.f[__21] = mA.f[__21]*mB.f[__11] + mA.f[__22]*mB.f[__21] + mA.f[__23]*mB.f[__31] + mA.f[__24]*mB.f[__41];	
	ret.f[__22] = mA.f[__21]*mB.f[__12] + mA.f[__22]*mB.f[__22] + mA.f[__23]*mB.f[__32] + mA.f[__24]*mB.f[__42];	
	ret.f[__23] = mA.f[__21]*mB.f[__13] + mA.f[__22]*mB.f[__23] + mA.f[__23]*mB.f[__33] + mA.f[__24]*mB.f[__43];	
	ret.f[__24] = mA.f[__21]*mB.f[__14] + mA.f[__22]*mB.f[__24] + mA.f[__23]*mB.f[__34] + mA.f[__24]*mB.f[__44];
	
	ret.f[__31] = mA.f[__31]*mB.f[__11] + mA.f[__32]*mB.f[__21] + mA.f[__33]*mB.f[__31] + mA.f[__34]*mB.f[__41];	
	ret.f[__32] = mA.f[__31]*mB.f[__12] + mA.f[__32]*mB.f[__22] + mA.f[__33]*mB.f[__32] + mA.f[__34]*mB.f[__42];	
	ret.f[__33] = mA.f[__31]*mB.f[__13] + mA.f[__32]*mB.f[__23] + mA.f[__33]*mB.f[__33] + mA.f[__34]*mB.f[__43];	
	ret.f[__34] = mA.f[__31]*mB.f[__14] + mA.f[__32]*mB.f[__24] + mA.f[__33]*mB.f[__34] + mA.f[__34]*mB.f[__44];
	
	ret.f[__41] = mA.f[__41]*mB.f[__11] + mA.f[__42]*mB.f[__21] + mA.f[__43]*mB.f[__31] + mA.f[__44]*mB.f[__41];
	ret.f[__42] = mA.f[__41]*mB.f[__12] + mA.f[__42]*mB.f[__22] + mA.f[__43]*mB.f[__32] + mA.f[__44]*mB.f[__42];
	ret.f[__43] = mA.f[__41]*mB.f[__13] + mA.f[__42]*mB.f[__23] + mA.f[__43]*mB.f[__33] + mA.f[__44]*mB.f[__43];
	ret.f[__44] = mA.f[__41]*mB.f[__14] + mA.f[__42]*mB.f[__24] + mA.f[__43]*mB.f[__34] + mA.f[__44]*mB.f[__44];
	
	mOut = ret;
#endif
}


void PrintMatrix(hdMatrix &matrix)
{
#if 0
	hdPrintf("\t %f %f %f %f\n", matrix.f[__11], matrix.f[__12], matrix.f[__13], matrix.f[__14]);
	hdPrintf("\t %f %f %f %f\n", matrix.f[__21], matrix.f[__22], matrix.f[__23], matrix.f[__24]);
	hdPrintf("\t %f %f %f %f\n", matrix.f[__31], matrix.f[__32], matrix.f[__33], matrix.f[__34]);
	hdPrintf("\t %f %f %f %f\n", matrix.f[__41], matrix.f[__42], matrix.f[__43], matrix.f[__44]);
#else
	printf("4x4 Matrix:\n");
	printf("\t %f %f %f %f\n", matrix.f[__11], matrix.f[__12], matrix.f[__13], matrix.f[__14]);
	printf("\t %f %f %f %f\n", matrix.f[__21], matrix.f[__22], matrix.f[__23], matrix.f[__24]);
	printf("\t %f %f %f %f\n", matrix.f[__31], matrix.f[__32], matrix.f[__33], matrix.f[__34]);
	printf("\t %f %f %f %f\n", matrix.f[__41], matrix.f[__42], matrix.f[__43], matrix.f[__44]);
#endif
}


void MatrixTranslation(
					   hdMatrix	&mOut,
					   const float	fX,
					   const float	fY,
					   const float	fZ)
{
	mOut.f[__11] = 1;	mOut.f[__12] = 0;	mOut.f[__13] = 0;	mOut.f[__14] = 0;
	mOut.f[__21] = 0;	mOut.f[__22] = 1;	mOut.f[__23] = 0;	mOut.f[__24] = 0;
	mOut.f[__31] = 0;	mOut.f[__32] = 0;	mOut.f[__33] = 1;	mOut.f[__34] = 0;
	mOut.f[__41] = fX;	mOut.f[__42] = fY;	mOut.f[__43] = fZ;	mOut.f[__44] = 1;
}


void MatrixRotationX(hdMatrix	&mOut,
					 const float fAngle)
{
	float fsin = hdSinf(fAngle);
	float fcos = hdCosf(fAngle);
	
	mOut.f[__11] = 1;	mOut.f[__12] = 0;	mOut.f[__13] = 0;	mOut.f[__14] = 0;
	mOut.f[__21] = 0;	mOut.f[__22] = fcos; mOut.f[__23] = fsin; mOut.f[__24] = 0;
	mOut.f[__31] = 0;	mOut.f[__32] = -fsin;mOut.f[__33] = fcos;	mOut.f[__34] = 0;
	mOut.f[__41] = 0;	mOut.f[__42] = 0;	mOut.f[__43] = 0;	mOut.f[__44] = 1;
}


void MatrixRotationY(hdMatrix	&mOut,
					 const float fAngle)
{
	float fsin = hdSinf(fAngle);
	float fcos = hdCosf(fAngle);
	
	mOut.f[__11] = fcos;	mOut.f[__12] = 0;	mOut.f[__13] = -fsin;mOut.f[__14] = 0;
	mOut.f[__21] = 0;	mOut.f[__22] = 1;	mOut.f[__23] = 0;	mOut.f[__24] = 0;
	mOut.f[__31] = fsin;	mOut.f[__32] = 0;	mOut.f[__33] = fcos;	mOut.f[__34] = 0;
	mOut.f[__41] = 0;	mOut.f[__42] = 0;	mOut.f[__43] = 0;	mOut.f[__44] = 1;
}


void MatrixRotationZ(hdMatrix	&mOut,
					 const float fAngle)
{
	float fsin = hdSinf(fAngle);
	float fcos = hdCosf(fAngle);
	
	mOut.f[__11] = fcos;	mOut.f[__12] = fsin;	mOut.f[__13] = 0;	mOut.f[__14] = 0;
	mOut.f[__21] = -fsin;mOut.f[__22] = fcos;	mOut.f[__23] = 0;	mOut.f[__24] = 0;
	mOut.f[__31] = 0;	mOut.f[__32] = 0;	mOut.f[__33] = 1;	mOut.f[__34] = 0;
	mOut.f[__41] = 0;	mOut.f[__42] = 0;	mOut.f[__43] = 0;	mOut.f[__44] = 1;
}


void MatrixRotationXYZOrigin(hdMatrix &mOut, const hdVec3 &rotation)
{
	hdMatrix rot, rx, ry, rz;
	MatrixIdentity(rot);
	MatrixIdentity(rx);
	MatrixIdentity(ry);
	MatrixIdentity(rz);
	
	if (rotation.x != 0.0) 
	{
		MatrixRotationX(rx, rotation.x);
	}
	if (rotation.y != 0.0)
	{ 
		MatrixRotationY(ry, rotation.y);
	}
	if (rotation.z != 0.0)
	{
		MatrixRotationZ(rz, rotation.z);
	}
	
	hdMatrixMultiply(rot, rx, ry);
	hdMatrixMultiply(rot, rot, rz);
	
	mOut = rot;
}


// Matrix to rotate a vector around arbitrary angles around an arbitrary axis
void MatrixRotationXYZAxis(hdMatrix &mOut, const float fAngle, const hdVec3& axis)
{
	MatrixIdentity(mOut); 
	
	float fsin = hdSinf(fAngle);
	float fcos = hdCosf(fAngle);
		
	mOut.f[__11] = (axis.x * axis.x) * (1 - fcos) + fcos;
	mOut.f[__12] = (axis.x * axis.y) * (1 - fcos) + (axis.z * fsin);
	mOut.f[__13] = (axis.x * axis.z) * (1 - fcos) - (axis.y * fsin);
	
	mOut.f[__21] = (axis.y * axis.x) * (1 - fcos) - (axis.z * fsin);
	mOut.f[__22] = (axis.y * axis.y) * (1 - fcos) + fcos;
	mOut.f[__23] = (axis.y * axis.z) * (1 - fcos) + (axis.x * fsin);
	
	mOut.f[__31] = (axis.z * axis.x) * (1 - fcos) + (axis.y * fsin);
	mOut.f[__32] = (axis.z * axis.y) * (1 - fcos) - (axis.x * fsin);
	mOut.f[__33] = (axis.z * axis.z) * (1 - fcos) + fcos;
}



void MatrixVec3Multiply(hdVec3		&vOut,
						const hdVec3	&vIn,
						const hdMatrix	&mIn)
{

#if (TARGET_OS_IPHONE == 1) && (TARGET_IPHONE_SIMULATOR == 0)
#if defined(_ARM_ARCH_7) && defined(__ARM_NEON__)
	float res[4];
	
	NEON_Matrix4Vector3Mul(mIn.f, &vIn.x, res);
	vOut.Set(res[0], res[1], res[2]);
#else
	
	if (vfpu)
	{
		Matrix4Vector3Mul(mIn.f, &vIn.x, &vOut.x);
	}
	else
	{
		hdVec3 res;
		
		// row vector * matrix

		res.x = vIn.x * mIn.f[__11] + vIn.y * mIn.f[__12] + vIn.z * mIn.f[__13];
		res.y = vIn.x * mIn.f[__21] + vIn.y * mIn.f[__22] + vIn.z * mIn.f[__23];
		res.z = vIn.x * mIn.f[__31] + vIn.y * mIn.f[__32] + vIn.z * mIn.f[__33];
		
		vOut = res;
	}
#endif


#else
	hdVec3 res;

	
	// row vector * matrix
	res.x = vIn.x * mIn.f[__11] + vIn.y * mIn.f[__21] + vIn.z * mIn.f[__31];
	res.y = vIn.x * mIn.f[__12] + vIn.y * mIn.f[__22] + vIn.z * mIn.f[__32];
	res.z = vIn.x * mIn.f[__13] + vIn.y * mIn.f[__23] + vIn.z * mIn.f[__33];

	vOut = res;
#endif
}


// HACK HACK HACK
// Treat vIn as a 4d vector with (x, y, z, 1)
void MatrixVec4Multiply(hdVec3 &vOut, const hdVec3 &vIn, const hdMatrix &mIn)
{
	hdVec3 res;
	
	res.x = vIn.x * mIn.f[__11] + vIn.y * mIn.f[__21] + vIn.z * mIn.f[__31] + 1 * mIn.f[__41];
	res.y = vIn.x * mIn.f[__12] + vIn.y * mIn.f[__22] + vIn.z * mIn.f[__32] + 1 * mIn.f[__42];
	res.z = vIn.x * mIn.f[__13] + vIn.y * mIn.f[__23] + vIn.z * mIn.f[__33] + 1 * mIn.f[__43];
	//res.w=vIn.x * mIn.f[__14] + vIn.y * mIn.f[__24] + vIn.z * mIn.f[__34] + 1 * mIn.f[__44];
	
	vOut = res;	
	
}



void MatrixLookAtRH(hdMatrix &mOut, const hdVec3 &vEye, const hdVec3 &vAt, const hdVec3 &vUp)
{
	hdVec3 f, vUpActual, s, u;
	hdMatrix	t;
	
	f.x = vAt.x - vEye.x;
	f.y = vAt.y - vEye.y;
	f.z = vAt.z - vEye.z;
	
	//MatrixVec3Normalize(f, f);
	//MatrixVec3Normalize(vUpActual, vUp);
	//MatrixVec3CrossProduct(s, f, vUpActual);
	//MatrixVec3CrossProduct(u, s, f);
	
	f = hdNormalize3(f);
	vUpActual = hdNormalize3(vUp);
	s = hdCross(f, vUpActual);
	u = hdCross(s, f);
	
	
	mOut.f[ 0] = s.x;
	mOut.f[ 1] = u.x;
	mOut.f[ 2] = -f.x;
	mOut.f[ 3] = 0;
	
	mOut.f[ 4] = s.y;
	mOut.f[ 5] = u.y;
	mOut.f[ 6] = -f.y;
	mOut.f[ 7] = 0;
	
	mOut.f[ 8] = s.z;
	mOut.f[ 9] = u.z;
	mOut.f[10] = -f.z;
	mOut.f[11] = 0;
	
	mOut.f[12] = 0;
	mOut.f[13] = 0;
	mOut.f[14] = 0;
	mOut.f[15] = 1;
	
	MatrixTranslation(t, -vEye.x, -vEye.y, -vEye.z);
	hdMatrixMultiply(mOut, t, mOut);
}


void MatrixPerspectiveFovRH(
							hdMatrix	&mOut,
							const float	fFOVy,
							const float	fAspect,
							const float	fNear,
							const float	fFar,
							const bool  bRotate)
{
	float f, n, fRealAspect;
	
	if (bRotate)
		fRealAspect = 1.0f / fAspect;
	else
		fRealAspect = fAspect;
	
	// cotangent(a) == 1.0f / tan(a);
	f = 1.0f / (float)tan(fFOVy * 0.5f);
	n = 1.0f / (fNear - fFar);
	
	mOut.f[ 0] = f / fRealAspect;
	mOut.f[ 1] = 0;
	mOut.f[ 2] = 0;
	mOut.f[ 3] = 0;
	
	mOut.f[ 4] = 0;
	mOut.f[ 5] = f;
	mOut.f[ 6] = 0;
	mOut.f[ 7] = 0;
	
	mOut.f[ 8] = 0;
	mOut.f[ 9] = 0;
	mOut.f[10] = (fFar + fNear) * n;
	mOut.f[11] = -1;
	
	mOut.f[12] = 0;
	mOut.f[13] = 0;
	mOut.f[14] = (2 * fFar * fNear) * n;
	mOut.f[15] = 0;
	
	if (bRotate)
	{
		hdMatrix mRotation, mTemp = mOut;
		MatrixRotationZ(mRotation, -90.0f*hd_pi/180.0f);
		hdMatrixMultiply(mOut, mTemp, mRotation);
	}
}



void MatrixInverse(
				   hdMatrix			&mOut,
				   const hdMatrix	&mIn)
{
	hdMatrix	mDummyMatrix;
	double		det_1;
	double		pos, neg, temp;
	
    /* Calculate the determinant of submatrix A and determine if the
	 the matrix is singular as limited by the double precision
	 floating-point data representation. */
    pos = neg = 0.0;
    temp =  mIn.f[ 0] * mIn.f[ 5] * mIn.f[10];
    if (temp >= 0.0) pos += temp; else neg += temp;
    temp =  mIn.f[ 4] * mIn.f[ 9] * mIn.f[ 2];
    if (temp >= 0.0) pos += temp; else neg += temp;
    temp =  mIn.f[ 8] * mIn.f[ 1] * mIn.f[ 6];
    if (temp >= 0.0) pos += temp; else neg += temp;
    temp = -mIn.f[ 8] * mIn.f[ 5] * mIn.f[ 2];
    if (temp >= 0.0) pos += temp; else neg += temp;
    temp = -mIn.f[ 4] * mIn.f[ 1] * mIn.f[10];
    if (temp >= 0.0) pos += temp; else neg += temp;
    temp = -mIn.f[ 0] * mIn.f[ 9] * mIn.f[ 6];
    if (temp >= 0.0) pos += temp; else neg += temp;
    det_1 = pos + neg;
	
    /* Is the submatrix A singular? */
    if ((det_1 == 0.0) || (hdAbs(det_1 / (pos - neg)) < 1.0e-15))
	{
        /* Matrix M has no inverse */
        printf("Matrix has no inverse : singular matrix\n");
        return;
    }
    else
	{
        /* Calculate inverse(A) = adj(A) / det(A) */
        det_1 = 1.0 / det_1;
        mDummyMatrix.f[ 0] =   ( mIn.f[ 5] * mIn.f[10] - mIn.f[ 9] * mIn.f[ 6] ) * (float)det_1;
        mDummyMatrix.f[ 1] = - ( mIn.f[ 1] * mIn.f[10] - mIn.f[ 9] * mIn.f[ 2] ) * (float)det_1;
        mDummyMatrix.f[ 2] =   ( mIn.f[ 1] * mIn.f[ 6] - mIn.f[ 5] * mIn.f[ 2] ) * (float)det_1;
        mDummyMatrix.f[ 4] = - ( mIn.f[ 4] * mIn.f[10] - mIn.f[ 8] * mIn.f[ 6] ) * (float)det_1;
        mDummyMatrix.f[ 5] =   ( mIn.f[ 0] * mIn.f[10] - mIn.f[ 8] * mIn.f[ 2] ) * (float)det_1;
        mDummyMatrix.f[ 6] = - ( mIn.f[ 0] * mIn.f[ 6] - mIn.f[ 4] * mIn.f[ 2] ) * (float)det_1;
        mDummyMatrix.f[ 8] =   ( mIn.f[ 4] * mIn.f[ 9] - mIn.f[ 8] * mIn.f[ 5] ) * (float)det_1;
        mDummyMatrix.f[ 9] = - ( mIn.f[ 0] * mIn.f[ 9] - mIn.f[ 8] * mIn.f[ 1] ) * (float)det_1;
        mDummyMatrix.f[10] =   ( mIn.f[ 0] * mIn.f[ 5] - mIn.f[ 4] * mIn.f[ 1] ) * (float)det_1;
		
        /* Calculate -C * inverse(A) */
        mDummyMatrix.f[12] = - ( mIn.f[12] * mDummyMatrix.f[ 0] + mIn.f[13] * mDummyMatrix.f[ 4] + mIn.f[14] * mDummyMatrix.f[ 8] );
        mDummyMatrix.f[13] = - ( mIn.f[12] * mDummyMatrix.f[ 1] + mIn.f[13] * mDummyMatrix.f[ 5] + mIn.f[14] * mDummyMatrix.f[ 9] );
        mDummyMatrix.f[14] = - ( mIn.f[12] * mDummyMatrix.f[ 2] + mIn.f[13] * mDummyMatrix.f[ 6] + mIn.f[14] * mDummyMatrix.f[10] );
		
        /* Fill in last row */
        mDummyMatrix.f[ 3] = 0.0f;
		mDummyMatrix.f[ 7] = 0.0f;
		mDummyMatrix.f[11] = 0.0f;
        mDummyMatrix.f[15] = 1.0f;
	}
	
   	/* Copy contents of dummy matrix in pfMatrix */
	mOut = mDummyMatrix;
}
