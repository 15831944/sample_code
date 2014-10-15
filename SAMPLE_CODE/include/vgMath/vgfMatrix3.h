/*
-----------------------------------------------------------------------------
  VRES(Virtual Reality GIS Engine System)    
  -------------------------------------------------------------
  License details ... waiting to add...
  -------------------------------------------------------------
  Copyright  (C) 2007 , leven - All Rights Reserved
-----------------------------------------------------------------------------
*/

#ifndef __VGFMATRIX3_H__
#define __VGFMATRIX3_H__

#include <vgMath/vgfPrerequisites.h>
#include <vgMath/vgfVector3.h>


namespace vgMath {


	/**
	@date 	2007/02/20  17:43	
	@author  leven

	@brief 	3*3�����ࡣ��������ϵ

	����:
		Column	 ��
		Row		 ��
		Diagonal  �Խ���
		determinant   ����ʽ��ֵ
		decomposition �ֽ�
		orthonormal	 ��׼����
		orthogonal		����

	@see    
	*/
	// NOTE.  The (x,y,z) coordinate system is assumed to be right-handed.
	// Coordinate axis rotation matrices are of the form
	//   RX =    1       0       0
	//           0     cos(t) -sin(t)
	//           0     sin(t)  cos(t)
	// where t > 0 indicates a counterclockwise rotation in the yz-plane
	//   RY =  cos(t)    0     sin(t)
	//           0       1       0
	//        -sin(t)    0     cos(t)
	// where t > 0 indicates a counterclockwise rotation in the zx-plane
	//   RZ =  cos(t) -sin(t)    0
	//         sin(t)  cos(t)    0
	//           0       0       1
	// where t > 0 indicates a counterclockwise rotation in the xy-plane.
	
	class VgfExport Matrix3
	{

	public:
		union
		{
			struct
			{  Real m[3][3];  };
			Real v[9];
		};

		// for faster access
		friend class Matrix4;

	public:

		/** Default constructor.
		@note
		It does <b>NOT</b> initialize the matrix for efficiency.
		*/
		inline Matrix3 () { makeZero(); };
		inline explicit Matrix3 (const Real arr[3][3]);
		inline Matrix3 (const Matrix3& rkMatrix);

		Matrix3 (Real fEntry00, Real fEntry01, Real fEntry02,
			Real fEntry10, Real fEntry11, Real fEntry12,
			Real fEntry20, Real fEntry21, Real fEntry22);

		// member access, allows use of construct mat[r][c]
		inline Real* operator[] (size_t iRow) const;

		//inline operator Real* ()
		//{
		//	return (Real*)m[0];
		//}

		///ע��size_t����0��ͷ.
		Vector3 getColumn (size_t iCol) const;
		Vector3 getRow (size_t iCol) const;
		void setColumn(size_t iCol, const Vector3& vec);
		void setRow(size_t iCol, const Vector3& vec);

		///ָ������������������һ��Matrix3,ע�⣺ÿ������Matrix3����Ϊһ�С�
		void fromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);



		// assignment and comparison
		inline Matrix3& operator= (const Matrix3& rkMatrix);

		///�ȽϺ���.�ӵ�һ������ʼ,ֱ������һ������ȵķ���.
		// comparison
		inline bool operator == (const Matrix3& rkMatrix) const;
		inline bool operator != (const Matrix3& rkMatrix) const;
		inline bool operator<  (const Matrix3& rkM) const;
		inline bool operator<= (const Matrix3& rkM) const;
		inline bool operator>  (const Matrix3& rkM) const;
		inline bool operator>= (const Matrix3& rkM) const;


		// arithmetic operations
		Matrix3 operator+ (const Matrix3& rkMatrix) const;
		Matrix3 operator- (const Matrix3& rkMatrix) const;
		///�������,ע��Ϊ���.
		Matrix3 operator* (const Matrix3& rkM) const;
		Matrix3 operator- () const;

		// arithmetic updates
		Matrix3& operator+= (const Matrix3& rkM);
		Matrix3& operator-= (const Matrix3& rkM);
		Matrix3& operator*= (Real fScalar);
		Matrix3& operator/= (Real fScalar);

		///�ҳ�����
		// matrix * vector [3x3 * 3x1 = 3x1]
		Vector3 operator* (const Vector3& rkVector) const;

		// vector * matrix [1x3 * 3x3 = 1x3]
		VgfExport friend Vector3 operator* (const Vector3& rkVector,
			const Matrix3& rkMatrix);

		// matrix * scalar
		Matrix3 operator* (Real fScalar) const;
		Matrix3 operator/ (Real fScalar) const;

		// scalar * matrix
		VgfExport friend Matrix3 operator* (Real fScalar, const Matrix3& rkMatrix);

		// utilities
		///�����ضԽ��߷�ת.
		Matrix3 transpose () const;				// M^T

		///��ת������������,���Ч��.
		Matrix3 transposeTimes (const Matrix3& rkM) const;  // this^T * M

		///���Բ�����ת��.
		Matrix3 timesTranspose (const Matrix3& rkM) const;  // this * M^T

		///��������
		Matrix3 inverse () const;

		///����ʽ��ֵ
		Real determinant () const;

		///��3�����󴴽�,����ʽ:M = L*S*R.
		///����L ,R Ϊ��������,SΪ�ԽǾ���.
		// Singular value decomposition, M = L*S*R, where L and R are orthogonal
		// and S is a diagonal matrix whose diagonal entries are nonnegative.
		void singularValueComposition (const Matrix3& rkL,
			const Vector3& rkS, const Matrix3& rkR);
		inline void singularValueComposition (const Matrix3& rkL, const Matrix3& rkS,
			const Matrix3& rkR);

		///����ֽ�(����ֽ�),����ʽ:M = L*S*R.
		///����L ,R Ϊ��������,SΪ�ԽǾ���.
		void singularValueDecomposition (Matrix3& rkL, Vector3& rkS,
			Matrix3& rkR) const;
		void singularValueDecomposition (Matrix3& rkL, Matrix3& rkS,
			Matrix3& rkR) const;


		/**
			����ķ-ʩ������������.
			input:
				M = [m0|m1|m2]
			output:
				Q = [q0|q1|q2]
				����q0,q1,q2Ϊ���������ĵ�λ����.
		
			 Gram-Schmidt orthonormalization (applied to columns of rotation matrix)
			 Algorithm uses Gram-Schmidt orthogonalization.  If 'this' matrix is
			 M = [m0|m1|m2], then orthonormal output matrix is Q = [q0|q1|q2],
			
			   q0 = m0/|m0|
			   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
			   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
			
			 where |V| indicates length of vector V and A*B indicates dot
			 product of vectors A and B.
		 */
		void orthonormalize ();

		///�ֽ����Ϊ��������Q,�Խ��߾���D,�������Ǿ���U
		///����Ĺ�ϵΪ:M = Q*D*U.
		///δ֪��;.
		// factor M = Q*D*U with orthogonal Q, diagonal D, upper triangular U
		void QDUDecomposition (Matrix3& rkQ, Matrix3& rkD,
			Matrix3& rkU) const;
		// orthogonal Q, diagonal D, upper triangular U stored as (u01,u02,u12)
		void QDUDecomposition (Matrix3& rkQ, Vector3& rkD,
			Vector3& rkU) const;


		/**
		 ����������õ���ת�����ת��,ע����������������.
		 matrix must be orthonormal.

		 Let (x,y,z) be the unit-length axis and let A be an angle of rotation.
		 The rotation matrix is R = I + sin(A)*P + (1-cos(A))*P^2 where
		 I is the identity and
		
		       +-        -+
		   P = |  0 -z +y |
		       | +z  0 -x |
		       | -y +x  0 |
		       +-        -+
		
		 If A > 0, R represents a counterclockwise rotation about the axis in
		 the sense of looking from the tip of the axis vector towards the
		 origin.  Some algebra will show that
		
		   cos(A) = (trace(R)-1)/2  and  R - R^t = 2*sin(A)*P
		
		 In the event that A = pi, R-R^t = 0 which prevents us from extracting
		 the axis through P.  Instead note that R = I+2*P^2 when A = pi, so
		 P^2 = (R-I)/2.  The diagonal entries of P^2 are x^2-1, y^2-1, and
		 z^2-1.  We can solve these for axis (x,y,z).  Because the angle is pi,
		 it does not matter which sign you choose on the square roots.
		*/
		void toAxisAngle (Vector3& rkAxis, Radian& rfAngle) const;
		inline void toAxisAngle (Vector3& rkAxis, Real& rfReal) const;
		inline void toAxisAngle (Vector3& rkAxis, Degree& rfAngle) const;

		/**
			������ת��������ת�ĽǶȽ�����ת����.
			����:
				mat.FromAxisAngle(Vector3(0,1,0) ,Degree(45).valueRadians());
				cout << mat;
				Vector3 v(1,0,0);
				cout << mat * v << endl;
			���Կ���v����������y������˳ʱ����ת��45��.
			ע��rkAxisҪnormalized.
		*/
		void fromAxisAngle (const Vector3& rkAxis, const Real& fReals);
		void fromAxisAngle (const Vector3& rkAxis, const Radian& fRadians);
		void fromAxisAngle (const Vector3& rkAxis, const Degree& fDegrees);



		/**
			�ֽ����ŷ����:yaw*pitch*roll,���ǵ��������Ʒֱ��ת���ǡ������ǡ��¶���.

			ע��:
			1.�Ӿ���ֽ⵽ŷ���ǳ�����ϵ��˳��,����,����x����תr1,����y����תr2�ͽ����෴��
			˳�����ɵľ��󲢲�һ��.Ҳ���ǹ�ϵ��˳�������.
				�����һϵ�к�������������һ��:ToEulerAnglesXYZ -ָ���Ƶ���ת������ΪXYZ,������
			�Դ�����.
			2.���ɵ�Ϊ����ŷ����,��ΧΪ[-pi,pi],   [-pi/2,pi/2],   [-pi,pi](�������Ӧ)
			3.����Ϊfalseʱ,wAngle is 0 and vAngle is either pi/2 or
			-pi/2.(���Ӣ��ע��)
			
			���EulerAngles.pdf

				Euler Angle Formulas
				David Eberly
				Geometric Tools, Inc.
				http://www.geometrictools.com
				c
				Copyright 1998-2006. All Rights Reserved.
				Created: December 1, 1999

		 Extract Euler angles from rotation matrices.  The return value is
		 'true' iff the factorization is unique relative to certain angle
		 ranges.  That is, if (U,V,W) is some permutation(�û�) of (X,Y,Z), the angle
		 ranges for the outputs from ToEulerAnglesUVW(uAngle,vAngle,wAngle) are
		 uAngle in [-pi,pi], vAngle in [-pi/2,pi/2], and wAngle in [-pi,pi].  If
		 the function returns 'false', wAngle is 0 and vAngle is either pi/2 or
		 -pi/2.

		 A common problem is to factor a rotation matrix as a product of rotations about the coordinate axes. The
		 form of the factorization depends on the needs of the application and what ordering is specified.

		 The matrix must be orthonormal.  The decomposition is yaw*pitch*roll
		 where yaw is rotation about the Up vector, pitch is rotation about the
		 Right axis, and roll is rotation about the Direction axis.

		*/
		bool toEulerAnglesXYZ (Radian& rfYAngle, Radian& rfPAngle,Radian& rfRAngle) const;
		bool toEulerAnglesXZY (Radian& rfYAngle, Radian& rfPAngle,Radian& rfRAngle) const;
		bool toEulerAnglesYXZ (Radian& rfYAngle, Radian& rfPAngle,Radian& rfRAngle) const;
		bool toEulerAnglesYZX (Radian& rfYAngle, Radian& rfPAngle,Radian& rfRAngle) const;
		bool toEulerAnglesZXY (Radian& rfYAngle, Radian& rfPAngle,Radian& rfRAngle) const;
		bool toEulerAnglesZYX (Radian& rfYAngle, Radian& rfPAngle,Radian& rfRAngle) const;
		inline bool toEulerAnglesXYZ (float& rfYAngle, float& rfPAngle,float& rfRAngle) const;
		inline bool toEulerAnglesXZY (float& rfYAngle, float& rfPAngle,float& rfRAngle) const;
		inline bool toEulerAnglesYXZ (float& rfYAngle, float& rfPAngle,float& rfRAngle) const;
		inline bool toEulerAnglesYZX (float& rfYAngle, float& rfPAngle,float& rfRAngle) const;
		inline bool toEulerAnglesZXY (float& rfYAngle, float& rfPAngle,float& rfRAngle) const;
		inline bool toEulerAnglesZYX (float& rfYAngle, float& rfPAngle,float& rfRAngle) const;
		inline bool toEulerAnglesXYZ (Degree& rfYAngle, Degree& rfPAngle,Degree& rfRAngle) const;
		inline bool toEulerAnglesXZY (Degree& rfYAngle, Degree& rfPAngle,Degree& rfRAngle) const;
		inline bool toEulerAnglesYXZ (Degree& rfYAngle, Degree& rfPAngle,Degree& rfRAngle) const;
		inline bool toEulerAnglesYZX (Degree& rfYAngle, Degree& rfPAngle,Degree& rfRAngle) const;
		inline bool toEulerAnglesZXY (Degree& rfYAngle, Degree& rfPAngle,Degree& rfRAngle) const;
		inline bool toEulerAnglesZYX (Degree& rfYAngle, Degree& rfPAngle,Degree& rfRAngle) const;


		///��ŷ���Ǵ�������,ע��Ҳ�ǹ�ϵ����ת˳��.
		void fromEulerAnglesXYZ (const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
		void fromEulerAnglesXZY (const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
		void fromEulerAnglesYXZ (const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
		void fromEulerAnglesYZX (const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
		void fromEulerAnglesZXY (const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
		void fromEulerAnglesZYX (const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
		inline void fromEulerAnglesXYZ (const float& fYAngle, const float& fPAngle, const float& fRAngle);
		inline void fromEulerAnglesXZY (const float& fYAngle, const float& fPAngle, const float& fRAngle);
		inline void fromEulerAnglesYXZ (const float& fYAngle, const float& fPAngle, const float& fRAngle);
		inline void fromEulerAnglesYZX (const float& fYAngle, const float& fPAngle, const float& fRAngle);
		inline void fromEulerAnglesZXY (const float& fYAngle, const float& fPAngle, const float& fRAngle);
		inline void fromEulerAnglesZYX (const float& fYAngle, const float& fPAngle, const float& fRAngle);
		inline void fromEulerAnglesXYZ (const Degree& fYAngle, const Degree& fPAngle, const Degree& fRAngle);
		inline void fromEulerAnglesXZY (const Degree& fYAngle, const Degree& fPAngle, const Degree& fRAngle);
		inline void fromEulerAnglesYXZ (const Degree& fYAngle, const Degree& fPAngle, const Degree& fRAngle);
		inline void fromEulerAnglesYZX (const Degree& fYAngle, const Degree& fPAngle, const Degree& fRAngle);
		inline void fromEulerAnglesZXY (const Degree& fYAngle, const Degree& fPAngle, const Degree& fRAngle);
		inline void fromEulerAnglesZYX (const Degree& fYAngle, const Degree& fPAngle, const Degree& fRAngle);


		/**
			������ֵ����������,ע������������ԳƵ�����.
			rkRot = [u0|u1|u2] ,��������������Ϊ��������.
			rkDiag = diag(d0,d1,d2) ,�Խ����ϵ�3��ֵ�ֱ�Ϊ����ֵ,��rkRot������������Ӧ,
			���������С��ϵ:d0 <= d1 <= d2
		 The matrix must be symmetric.  Factor M = R * D * R^T where
		 R = [u0|u1|u2] is a rotation matrix with columns u0, u1, and u2 and
		 D = diag(d0,d1,d2) is a diagonal matrix whose diagonal entries are d0,
		 d1, and d2.  The eigenvector u[i] corresponds to eigenvector d[i].
		 The eigenvalues are ordered as d0 <= d1 <= d2.
		 */
		void eigenDecomposition (Matrix3& rkRot, Matrix3& rkDiag) const;
		void eigenDecomposition (Real afEigenvalue[3], Vector3 akEigenvector[3]) const;




		///�о���Ӧ����������㷽��,��δȷ��,��ʱ����.
		Real SpectralNorm () const;
		///static��Ա,��Ӧ��Math���д���,����ȥ��,��tenorProdect()����(Сд).
		///Ϊ�˲��Ե��걸��,�˴�����.
		static void TensorProduct (const Vector3& rkU, const Vector3& rkV,
			Matrix3& rkProduct);
		///������������һ���ĺ���,����ȥ��.Ϊ�˲��Ե��걸��,�˴�����.
		///ע��,����һ�����ϵİ汾,�����������������������С��ϵ:d0 <= d1 <= d2
		// eigensolver, matrix must be symmetric
		void EigenSolveSymmetric (Real afEigenvalue[3],
			Vector3 akEigenvector[3]) const;


	protected:

		// support for singular value decomposition
		//static const Real ms_fSvdEpsilon;				
		//static const unsigned int ms_iSvdMaxIterations;
		static void Bidiagonalize (Matrix3& kA, Matrix3& kL,
			Matrix3& kR);
		static void GolubKahanStep (Matrix3& kA, Matrix3& kL,
			Matrix3& kR);

		///����������ֵ����������.
		// ------Support for eigendecomposition start--------.  
		// The Tridiagonalize function applies
		// a Householder transformation to the matrix.  If that transformation
		// is the identity (the matrix is already tridiagonal), then the return
		// value is 'false'.  Otherwise, the transformation is a reflection and
		// the return value is 'true'.  The QLAlgorithm returns 'true' iff the
		// QL iteration scheme converged.
		bool tridiagonalize (Real afDiag[3], Real afSubd[2]);
		// This is an implementation of the symmetric QR algorithm from the book
		// "Matrix Computations" by Gene H. Golub and Charles F. Van Loan, second
		// edition.  The algorithm is 8.2.3.  The implementation has a slight
		// variation to actually make it a QL algorithm, and it traps the case
		// when either of the subdiagonal terms s0 or s1 is zero and reduces the
		// 2-by-2 subblock directly.
		bool QLAlgorithm (Real afDiag[3], Real afSubDiag[3]);
		// ------Support for eigendecomposition end--------.  


		// support for spectral norm
		static Real MaxCubicRoot (Real afCoeff[3]);

		///���ڱȽ�,����operator == ��.
		// support for comparisons
		inline int compareArrays (const Matrix3& rkM) const;


	public:


		inline bool  isSymmetric() const ;
		inline Matrix3& makeZero();
		inline Matrix3& makeIdentity();
		inline Matrix3& makeTranspose();
		inline Matrix3& makeInverse();
		inline Matrix3& makeAdjoint();

		///�������Ǿ��󴴽��Գƾ���
		inline Matrix3& makeSymmetricFromUpperTriangularMatrix();

		///�������Ǿ��󴴽��Գƾ���
		inline Matrix3& makeSymmetricFromLowerTriangularMatrix();

		

		///ͨ���в������в�����þ����е�ĳ��ֵ.
		///ע��row��col����Ϊ0,��0��0��,��ΧΪ[0-2]
		inline Real getValueByPosition(const uint& row , const uint& col );
		inline void setValueByPosition(const uint& row , const uint& col , const Real& value);

		///���öԽ����ϵ�������,������Ϊ0.
		inline Matrix3& makeDiagonal(Real fM00, Real fM11, Real fM22);

		///�����Ĳ��
		Matrix3 crossProduct( const Matrix3& rkm ) const;
		/**
			����ÿ����Ӧ�����,��:
				Matrix3 {       1 ,       0 ,       0 ,
								0 ,       1 ,       0 ,
								0 ,       0 ,       1 }

				Matrix3 { 0.365258 , 0.769486 , -0.523906 ,
						0.564374 , -0.630612 , -0.532739 ,
						-0.740316 , -0.101092 , -0.664614 }

				result:
				--------------------------------------
				Matrix3 { 0.365258 ,       0 ,      -0 ,
								0 , -0.630612 ,      -0 ,
								-0 ,      -0 , -0.664614 }

		*/

		Matrix3 tensorProduct(const Matrix3& rkM ) const;

		///��������
		Matrix3 adjoint () const;

		///�������,�������
		inline friend StdOStream& operator <<
			( StdOStream& o, const Matrix3& v );

		String getDetails() const
		{
			StdStringStream o;
			o << (*this);
			return o.str();
		}

	public:
		static const Real EPSILON;
		static const Matrix3 ZERO;
		static const Matrix3 IDENTITY;
	};

	#include <vgMath/vgfMatrix3Impl.inl>

	
}

#endif // end of __VGFMATRIX3_H__