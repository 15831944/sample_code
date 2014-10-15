


#ifndef __VGKVEC3_H__
#define __VGKVEC3_H__

#include <vgKernel/vgkForward.h>
#include <sstream>

namespace vgKernel {

	class  VGK_EXPORT Vec3
	{
	public:

		union
		{
			struct
			{  float x , y , z;  };
			float v[3];
		};

	public:

		inline Vec3()
			: x( 0.0 ), y( 0.0 ), z( 0.0 )
		{
		}

		inline Vec3( const float fX, const float fY, const float fZ )
			: x( fX ), y( fY ), z( fZ )
		{
		}

		inline explicit Vec3( const float vec[3] )
			: x( vec[0] ),
			y( vec[1] ),
			z( vec[2] )
		{
		}

		inline explicit Vec3( const int vec[3] )
		{
			x = (float)vec[0];
			y = (float)vec[1];
			z = (float)vec[2];
		}

		inline explicit Vec3( float* const r )
			: x( r[0] ), y( r[1] ), z( r[2] )
		{
		}

		inline explicit Vec3( const float scaler )
			: x( scaler )
			, y( scaler )
			, z( scaler )
		{
		}

		inline Vec3( const Vec3& rkVector )
			: x( rkVector.x ), y( rkVector.y ), z( rkVector.z )
		{
		}

		inline float operator [] ( const size_t i ) const;
		inline float& operator [] ( const size_t i );


		///operator����������д
		inline Vec3& operator = ( const Vec3& rkVector );
		inline Vec3& operator = ( const float fScaler );
		inline bool operator == ( const Vec3& rkVector ) const;
		inline bool operator != ( const Vec3& rkVector ) const;
		inline Vec3 operator + ( const Vec3& rkVector ) const;
		inline Vec3 operator - ( const Vec3& rkVector ) const;
		inline Vec3 operator * ( const float fScalar ) const;
		inline Vec3 operator / ( const float fScalar ) const;
		inline Vec3 operator * ( const Vec3& rkVector) const;
		inline Vec3 operator / ( const Vec3& rkVector) const;

		///Ӧ��ע������:��������Ϊ��ʱ.
		inline const Vec3& operator + () const;
		inline Vec3 operator - () const;

		///��������Ϊ�������ʱ,ע��Ϊ��Ԫ����.
		inline friend Vec3 operator * ( const float fScalar, const Vec3& rkVector );
		inline friend Vec3 operator / ( const float fScalar, const Vec3& rkVector );
		inline friend Vec3 operator + (const Vec3& lhs, const float rhs);
		inline friend Vec3 operator + (const float lhs, const Vec3& rhs);
		inline friend Vec3 operator - (const Vec3& lhs, const float rhs);
		inline friend Vec3 operator - (const float lhs, const Vec3& rhs);

		///����߼���Ӧ��
		// arithmetic updates
		inline Vec3& operator += ( const Vec3& rkVector );
		inline Vec3& operator += ( const float fScalar );
		inline Vec3& operator -= ( const Vec3& rkVector );
		inline Vec3& operator -= ( const float fScalar );
		inline Vec3& operator *= ( const float fScalar );
		inline Vec3& operator *= ( const Vec3& rkVector );
		inline Vec3& operator /= ( const float fScalar );
		inline Vec3& operator /= ( const Vec3& rkVector );

		///���Ⱥ���
		inline float length () const;

		///ʱ���õ����ȵ�ƽ��,����Ƚϵ�ʱ��
		inline float squaredLength () const;


		///���뺯��,����2��������ľ���
		inline float distance(const Vec3& rhs) const;
		inline float squaredDistance(const Vec3& rhs) const;


		///����2���ĵ��.
		inline float dotProduct(const Vec3& vec) const;
		inline float absDotProduct(const Vec3& vec) const;

		///��׼������,ע��:			
		///1. ��ʱ���Զž�0���������.
		///2. ���з���ֵ,����ֵΪ��ǰ�����ĳ���,ע�ⲻ��vector3,��Ҫ����vector3 ,��ʹ��normalisedRef().
		inline float normalise();

		///���ؾ�����λ������������.
		inline Vec3& normalisedRef();

		///������
		inline Vec3 crossProduct( const Vec3& rkVector ) const;

		///������������͵�һ��.
		inline Vec3 midPoint( const Vec3& vec = ZERO ) const;

		///�ȽϺ���
		inline bool operator < ( const Vec3& rhs ) const;
		inline bool operator > ( const Vec3& rhs ) const;

		///ȡ���������Ƚ϶��õ�����Сֵ,���Ҹ��豾��
		inline Vec3 makeFloor( const Vec3& cmp );

		///ȡ���������Ƚ϶��õ������ֵ,���豾��,��������ֵ.
		inline Vec3 makeCeil( const Vec3& cmp );
		inline Vec3 ceilProduct( const Vec3& c ) const;
		inline Vec3 floorProduct( const Vec3& c ) const;

		///�õ�һ����������.ע��,��ʱ��������������ȷ��,���õ�׼ȷ����������,��ʹ��4Ԫ��.
		inline Vec3 perpendicular(void) const;


		/** Returns true if this vector is zero length. */
		inline bool isZeroLength(void) const;

		/** As normalise, except that this vector is unaffected and the
		normalised vector is returned as a copy. */
		inline Vec3 normalisedCopy(void) const;

		///���㷴������
		///ע��:
		///1.��ʱnormalҪΪ��λ����
		///2.ͨ����������׺���������Ū��,��ע����Եķ���.
		inline Vec3 reflect(const Vec3& normal) const;


		inline bool operator <= ( const Vec3& rhs ) const;
		inline bool operator >= ( const Vec3& rhs ) const;

		inline  void  reset( const float& X ,
			const float& Y ,const float& Z )
		{
			this->x = X;
			this->y = Y;
			this->z = Z;
		}

		String getDetails() const
		{
			std::ostringstream o;
			o << /*"Vec3:(" <<*/ "  ( " << x << "  ,  " << y << "  ,  " << z << " )";
			return o.str();
		}

		void setLength(float len)
		{
			float scale = (len / length());

			this->x *= scale;
			this->y *= scale;
			this->z *= scale;

		}

	public:			

		static const Vec3 ZERO;
		static const Vec3 UNIT_X;
		static const Vec3 UNIT_Y;
		static const Vec3 UNIT_Z;
		static const Vec3 NEGATIVE_UNIT_X;
		static const Vec3 NEGATIVE_UNIT_Y;
		static const Vec3 NEGATIVE_UNIT_Z;
		static const Vec3 UNIT_SCALE;

	};

	//std::ostream& operator<< ( std::ostream& o, Vec3 vec3 )
	//{
	//	o << vec3.getDetails();
	//	return o;
	//}

	///inline ����ʵ��.
	#include "vgkVec3.inl"

	
}// end of namespace vgKernel
	


#endif // end of __VGKVEC3_H__