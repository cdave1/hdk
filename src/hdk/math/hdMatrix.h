/*
 * Copyright (c) 2014 Hackdirt Ltd.
 * Author: David Petrie (david@davidpetrie.com)
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software. Permission is granted to anyone to use this software for
 * any purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim
 * that you wrote the original software. If you use this software in a product, an
 * acknowledgment in the product documentation would be appreciated but is not
 * required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

/*
 Partially borrowed from the Oolong header file, Matrix.h
 */

#ifndef _HD_MATRIX_
#define _HD_MATRIX_

#include <hdk/math/hdMathCommon.h>

/* Column major order */
#define __11 0
#define __21 1
#define __31 2
#define __41 3
#define __12 4
#define __22 5
#define __32 6
#define __42 7
#define __13 8
#define __23 9
#define __33 10
#define __43 11
#define __14 12
#define __24 13
#define __34 14
#define __44 15

/*
 * Matrix contains floats
 */
class hdMatrix
{
public:
    float* operator [] ( const int Row )
    {
        return &f[Row<<2];
    }
    float f[16];
};


void InitMatrixSettings();

/*
 * Reset matrix to identity
 * outputs the identity matrix
 */
void MatrixIdentity(hdMatrix &mOut);

/*
 * Multiply mA by mB and assign the result to mOut
 * (mOut = p1 * p2). A copy of the result matrix is done in
 * the function because mOut can be a parameter mA or mB.
 */
void hdMatrixMultiply(hdMatrix   &mOut,
                      const hdMatrix &mA,
                      const hdMatrix &mB);

void PrintMatrix(hdMatrix &matrix);

/*
 * Build a translation matrix mOut using fX, fY and fZ.
 */
void MatrixTranslation(hdMatrix &mOut,
                       const float fX,
                       const float fY,
                       const float fZ);

/*
 * Build a scale matrix mOut using fX, fY and fZ.
 */
void MatrixScaling(hdMatrix &mOut,
                   const float fX,
                   const float fY,
                   const float fZ);

/*
 * Create an X rotation matrix mOut.
 */
void MatrixRotationX(hdMatrix &mOut,
                     const float fAngle);
/*
 * Create an Y rotation matrix mOut.
 */
void MatrixRotationY(hdMatrix &mOut,
                     const float fAngle);

/*
 * Create an Z rotation matrix mOut.
 */
void MatrixRotationZ(hdMatrix &mOut,
                     const float fAngle);

void MatrixRotationXYZOrigin(hdMatrix &mOut, const hdVec3 &rotation);

/*
 * Matrix to rotate a vector an angle around an arbitrary axis
 */
void MatrixRotationXYZAxis(hdMatrix &mOut, const float fAngle, const hdVec3& axis);

void MatrixVec3Multiply(hdVec3  &vOut,
                        const hdVec3 &vIn,
                        const hdMatrix &mIn);

void MatrixVec4Multiply(hdVec3 &vOut, const hdVec3 &vIn, const hdMatrix &mIn);

void MatrixLookAtRH(hdMatrix &mOut, const hdVec3 &vEye, const hdVec3 &vAt, const hdVec3 &vUp);

void MatrixPerspectiveFovRH (hdMatrix  &mOut,
                             const float fFOVy,
                             const float fAspect,
                             const float fNear,
                             const float fFar,
                             const bool  bRotate);

#endif
