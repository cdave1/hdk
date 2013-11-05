/*
 *  b2Opt.h
 *  TotemGame
 *
 *  Created by david on 12/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 * Box2D ARM VFP/NEON optimisations.
 *
 */

#ifndef B2_OPTIMIZED_FUNCTIONS_H 
#define B2_OPTIMIZED_FUNCTIONS_H


#define B2EXPORT inline


/*
 * 2x2 matrix * 2x1 vector = 2x1 vector
 */
//B2EXPORT void opt_b2Vec2Mul(const float * A, const float * v, float *dst);

/*
 * 2x2 matrix * 2x2 matrix = 2x2 matrix
 */
//B2EXPORT void opt_b2MatMul(const float * A, const float * B, float *dst);

/*
 * 2x2 matrix * 2x2 matrix = 2x2 matrix
 */
//B2EXPORT void opt_b2MatMulT(const float * A, const float * B, float *dst);

/*
 * 3x3 matrix * 3x1 vector = 3x1 vector
 */
//B2EXPORT void opt_b2Vec3Mul(const float * A, const float * v, float *dst);




#include "hdMath/hdMatrix.h"

// iPhone Specific
#if TARGET_OS_IPHONE

#ifdef _ARM_ARCH_7
#include "hdMath/neon/neon_matrix_impl.h"
#else
#include "hdMath/vfp/common_macros.h"
#include "hdMath/vfp/vfp_clobbers.h"
#endif

#endif

/*
 * 2x2 matrix * 2x1 vector = 2x1 vector
 */
B2EXPORT void opt_b2Vec2Mul(const float* A, const float* v, float *dst)
{
#if defined(_ARM_ARCH_7) && defined(__ARM_NEON__)
	
	asm volatile
	(
	 "vldmia %1, {q8}	\n\t"		// load A into quad word bank (four contiguous floats)
	 "vldmia %2, {d2}		\n\t"	// load v into double work bank on q1 (two contiguous floats)
	 "vmul.f32 d0, d16, d2[0]\n\t"  // multiply first two floats in q8 by first vector float
	 "vmla.f32 d0, d17, d2[1]\n\t"  // add result of previous to result of second two * seconf vector float
	 "vstmia %0, {d0}"				// put in result.
	 
	 : // no output
	 : "r" (dst), "r" (A), "r" (v) 	// input - note *value* of pointer doesn't change
	 : "memory", "d0", "d2", "q8" 
	 );
	
#elif 0 // defined(_ARM_ARCH_6) && !defined(__ARM_NEON__)
	
	asm volatile 
	(
	 VFP_SWITCH_TO_ARM
	 
	 VFP_VECTOR_LENGTH(1)
	 
	 // Load vector to scalar bank.
	 "fldmias  %1, {s0-s1}    \n\t"
	 
	 // Load the whole matrix.
	 "fldmias  %0, {s16-s19}     \n\t"     
	 
	 // First column times matrix.
	 "fmuls s8, s16, s0        \n\t" // s8 = s16 * s0, s9 = s17 * s1
	 "fmacs s8, s18, s1       \n\t"  // s8 += (s18 * s0), s9 += (s19 * s1)
	 
	 // Save vector.
	 "fstmias  %2!, {s8-s9}  \n\t" 
	 
	 VFP_VECTOR_LENGTH_ZERO
	 VFP_SWITCH_TO_THUMB
	 : 
	 : "r" (A), "r" (v), "r" (dst)
	 : "r0", "cc", "memory", VFP_CLOBBER_S0_S1, VFP_CLOBBER_S8_S9, VFP_CLOBBER_S16_S19 
	 );
	
	
#else
	//#error "b2 optimisations not available for this architecture."
	dst[0] = (A[0] * v[0]) + (A[2] * v[1]);
	dst[1] = (A[1] * v[0]) + (A[3] * v[1]);
#endif
}


// // ie b2Vec2(v.x * A.col1.x + v.y * A.col1.y, v.x * A.col2.x + v.y * A.col2.y)
/*
 B2EXPORT void opt_b2Vec2MulT(const float *A, const float *B, float *dst)
 {
 #ifdef _ARM_ARCH_7
 
 #elif defined(_ARM_ARCH_6)
 asm volatile (VFP_SWITCH_TO_ARM
 
 VFP_VECTOR_LENGTH(2)
 
 // Load vector to scalar bank.
 "fldmias  %1, {s0-s1}    \n\t"
 
 // Load the whole matrix.
 "fldmias  %0, {s16-s19}     \n\t"     
 
 // First column times matrix.
 "fmuls s8, s0, s16       \n\t" // s8 = s0 * s16, s9 = s17 * s1
 "fmacs s8, s1, s18       \n\t" // s8 += (s18 * s0), s9 += (s19 * s1)
 
 // Save vector.
 "fstmias  %2!, {s8-s9}  \n\t" 
 
 VFP_VECTOR_LENGTH_ZERO
 VFP_SWITCH_TO_THUMB
 : 
 : "r" (A), "r" (v), "r" (dst)
 : "r0", "cc", "memory", VFP_CLOBBER_S0_S1, VFP_CLOBBER_S8_S9, VFP_CLOBBER_S16_S19 
 );
 #else
 //#error "b2 optimisations not available for this architecture."
 dst[0] = (A[0] * v[0]) + (A[1] * v[1]);
 dst[1] = (A[2] * v[0]) + (A[3] * v[1]);
 #endif
 }*/



// A * B
B2EXPORT void opt_b2MatMul(const float* A, const float* B, float *dst)
{
	//#ifdef _ARM_ARCH_7
	
#if defined(_ARM_ARCH_6) && !defined(__ARM_NEON__)
	asm volatile (VFP_SWITCH_TO_ARM
				  VFP_VECTOR_LENGTH(1)
				  
				  // load A in s0 to s3
				  "fldmias %1!, {s0-s3} \n\t"
				  
				  // load B in s16 to s19
				  "fldmias %2, {s16-s19} \n\t"
				  
				  /* 
				   ret.f[0] = A[0]*B[0] + A[1]*B[2]
				   ret.f[1] = A[0]*B[1] + A[1]*B[3]
				   
				   ret.f[2] = A[2]*B[0] + A[3]*B[2]
				   ret.f[3] = A[2]*B[1] + A[3]*B[3]
				   */				  
				  
				  "fmuls s8, s16, s0 \n\t" // s8 = s16 * s0, s9 = s17 * s0
				  "fmuls s10, s16, s2 \n\t" // s10 = s16 * s2, s11 = s16 * s3
				  "fmacs s8, s18, s1 \n\t" // s8 += s18 * s1, s9 += s19 * s1
				  "fmacs s10, s18, s3 \n\t" // s10 += s18 * s3, s11 += s19 * s3
				  
				  "fstmias %0!, {s8-s11} \n\t"
				  
				  VFP_VECTOR_LENGTH_ZERO
				  VFP_SWITCH_TO_THUMB
				  : "=r" (dst), "=r" (B)
				  : "r" (A), "0" (dst), "1" (B)
				  : "r0", "cc", "memory", VFP_CLOBBER_S0_S3, VFP_CLOBBER_S8_S11, VFP_CLOBBER_S16_S19
				  );
#else
	hdMatrix mA, mB, mOut; 
	
	MatrixIdentity(mA);
	MatrixIdentity(mB);
	
	mA.f[__11] = A[0];	mA.f[__12] = A[1];
	mA.f[__21] = A[2];	mA.f[__22] = A[3];
	
	mB.f[__11] = B[0];	mB.f[__12] = B[1];
	mB.f[__21] = B[2];	mB.f[__22] = B[3];
	
	hdMatrixMultiply(mOut, mA, mB);
	
	dst[0] = mOut.f[__11]; dst[2] = mOut.f[__21];
	dst[1] = mOut.f[__12]; dst[3] = mOut.f[__22];
#endif
}





// A^T * B
B2EXPORT void opt_b2MatMulT(const float* A, const float* B, float *dst)
{
	//#ifdef _ARM_ARCH_7
	
#if defined(_ARM_ARCH_6) && !defined(__ARM_NEON__)
	asm volatile (VFP_SWITCH_TO_ARM
				  VFP_VECTOR_LENGTH(1)
				  
				  // load A in s0 to s3
				  "fldmias %1!, {s0-s3} \n\t"
				  
				  // load B in s16 to s19
				  "fldmias %2, {s16-s19} \n\t"
				  
				  /* 
				   ret.f[0] = A[0]*B[0] + A[1]*B[2]
				   ret.f[1] = A[0]*B[1] + A[1]*B[3]
				   
				   ret.f[2] = A[2]*B[0] + A[3]*B[2]
				   ret.f[3] = A[2]*B[1] + A[3]*B[3]
				   */				  
				  
				  "fmuls s8, s16, s0 \n\t" // s8 = s16 * s0, s9 = s17 * s0
				  "fmuls s10, s16, s2 \n\t" // s10 = s16 * s2, s11 = s16 * s3
				  "fmacs s8, s18, s1 \n\t" // s8 += s18 * s1, s9 += s19 * s1
				  "fmacs s10, s18, s3 \n\t" // s10 += s18 * s3, s11 += s19 * s3
				  
				  "fstmias %0!, {s8-s11} \n\t"
				  
				  VFP_VECTOR_LENGTH_ZERO
				  VFP_SWITCH_TO_THUMB
				  : "=r" (dst), "=r" (B)
				  : "r" (A), "0" (dst), "1" (B)
				  : "r0", "cc", "memory", VFP_CLOBBER_S0_S3, VFP_CLOBBER_S8_S11, VFP_CLOBBER_S16_S19
				  );
#else
	//#error "b2 optimisations not available for this architecture."
	hdMatrix mA, mB, mOut;
	
	MatrixIdentity(mA);
	MatrixIdentity(mB);
	
	mA.f[__11] = A[0];	mA.f[__12] = A[1];
	mA.f[__21] = A[2];	mA.f[__22] = A[3];
	
	mB.f[__11] = B[0];	mB.f[__12] = B[1];
	mB.f[__21] = B[2];	mB.f[__22] = B[3];
	
	hdMatrixMultiply(mOut, mA, mB);
	
	dst[0] = mOut.f[__11]; dst[2] = mOut.f[__21];
	dst[1] = mOut.f[__12]; dst[3] = mOut.f[__22];
#endif
	
	
}


/// Multiply a matrix times a vector.
B2EXPORT void opt_b2Vec3Mul(const float* A, const float* v, float *dst)
{
	/*
	 #ifdef _ARM_ARCH_7
	 float res[4];
	 
	 NEON_Matrix4Vector3Mul(A, v, res);
	 
	 
	 dst[0] = res[0]; dst[1] = res[1]; dst[2] = res[2];
	 #elif defined(_ARM_ARCH_6) && !defined(__ARM_NEON__)
	 */
#if defined(_ARM_ARCH_6) && !defined(__ARM_NEON__)
	//Matrix4Vector3Mul(A, v, dst);
	
	// return b2Vec3(vOut.x, vOut.y, vOut.z);
	asm volatile (VFP_SWITCH_TO_ARM
				  
				  VFP_VECTOR_LENGTH(2)
				  
				  // Load vector to scalar bank.
				  "fldmias  %1, {s0-s2}    \n\t"
				  
				  // Load the whole matrix.
				  "fldmias  %0, {s16-s25}     \n\t"     
				  
				  // First column times matrix.
				  "fmuls s8, s16, s0        \n\t" // s8 = s16 * s0, s9 = s17 * s1
				  "fmacs s8, s19, s1       \n\t"  // s8 += (s18 * s0), s9 += (s19 * s1)
				  "fmacs s8, s22, s2       \n\t"
				  
				  // Save vector.
				  "fstmias  %2!, {s8-s10}  \n\t" 
				  
				  VFP_VECTOR_LENGTH_ZERO
				  VFP_SWITCH_TO_THUMB
				  : 
				  : "r" (A), "r" (v), "r" (dst)
				  : "r0", "cc", "memory", VFP_CLOBBER_S0_S2, VFP_CLOBBER_S8_S10, VFP_CLOBBER_S16_S25 
				  );
#else
	
	/*
	 
	 */ 
	//#error "b2 optimisations not available for this architecture."
	hdMatrix m;
	hdVec3 vec(v[0], v[1], v[2]), vOut;
	
	MatrixIdentity(m);
	
	//	m.f[__11] = A.col1.x;	m.f[__12] = A.col1.y;	m.f[__13] = A.col1.z;
	//	m.f[__21] = A.col2.x;	m.f[__22] = A.col2.y;	m.f[__23] = A.col2.z;
	//	m.f[__31] = A.col3.x;	m.f[__32] = A.col3.y;	m.f[__33] = A.col3.z;
	
	
	m.f[__11] = A[0];	m.f[__12] = A[3];	m.f[__13] = A[6];
	m.f[__21] = A[1];	m.f[__22] = A[4];	m.f[__23] = A[7];
	m.f[__31] = A[2];	m.f[__32] = A[5];	m.f[__33] = A[8];
	
	
	
	MatrixVec3Multiply(vOut, vec, m);
	
	dst[0] = vOut.x; dst[1] = vOut.y; dst[2] = vOut.z;
#endif	
}

#endif