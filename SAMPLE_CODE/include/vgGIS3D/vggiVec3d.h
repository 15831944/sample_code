
#pragma once

#include <vggis/vggisDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkVec3.h>

	
namespace vgGIS3D {

 
	class  VGG_EXPORT Vec3d
	{
	public:

		union
		{
			struct
			{  double x , y , z;  };
			double v[3];
		};

	public:

		inline Vec3d()
			: x( 0.0 ), y( 0.0 ), z( 0.0 )
		{
		}

		inline Vec3d( vgKernel::Vec3 vec3 )
			: x( vec3.x ), y( vec3.y ), z( vec3.z )
		{
		}


		inline Vec3d( const double fX, const double fY, const double fZ )
			: x( fX ), y( fY ), z( fZ )
		{
		}

		inline explicit Vec3d( const double vec[3] )
			: x( vec[0] ),
			y( vec[1] ),
			z( vec[2] )
		{
		}

		inline explicit Vec3d( const int vec[3] )
		{
			x = (double)vec[0];
			y = (double)vec[1];
			z = (double)vec[2];
		}

		inline explicit Vec3d( double* const r )
			: x( r[0] ), y( r[1] ), z( r[2] )
		{
		}

		inline explicit Vec3d( const double scaler )
			: x( scaler )
			, y( scaler )
			, z( scaler )
		{
		}

		inline Vec3d( const Vec3d& rkVector )
			: x( rkVector.x ), y( rkVector.y ), z( rkVector.z )
		{
		}

		inline double operator [] ( const size_t i ) const;
		inline double& operator [] ( const size_t i );


		///operator����������д
		inline Vec3d& operator = ( const Vec3d& rkVector );
		inline Vec3d& operator = ( const double fScaler );
		inline bool operator == ( const Vec3d& rkVector ) const;
		inline bool operator != ( const Vec3d& rkVector ) const;
		inline Vec3d operator + ( const Vec3d& rkVector ) const;
		inline Vec3d operator - ( const Vec3d& rkVector ) const;
		inline Vec3d operator * ( const double fScalar ) const;
		inline Vec3d operator / ( const double fScalar ) const;
		inline Vec3d operator * ( const Vec3d& rkVector) const;
		inline Vec3d operator / ( const Vec3d& rkVector) const;

		///Ӧ��ע������:��������Ϊ��ʱ.
		inline const Vec3d& operator + () const;
		inline Vec3d operator - () const;

		///��������Ϊ�������ʱ,ע��Ϊ��Ԫ����.
		inline friend Vec3d operator * ( const double fScalar, const Vec3d& rkVector );
		inline friend Vec3d operator / ( const double fScalar, const Vec3d& rkVector );
		inline friend Vec3d operator + (const Vec3d& lhs, const double rhs);
		inline friend Vec3d operator + (const double lhs, const Vec3d& rhs);
		inline friend Vec3d operator - (const Vec3d& lhs, const double rhs);
		inline friend Vec3d operator - (const double lhs, const Vec3d& rhs);

		///����߼���Ӧ��
		// arithmetic updates
		inline Vec3d& operator += ( const Vec3d& rkVector );
		inline Vec3d& operator += ( const double fScalar );
		inline Vec3d& operator -= ( const Vec3d& rkVector );
		inline Vec3d& operator -= ( const double fScalar );
		inline Vec3d& operator *= ( const double fScalar );
		inline Vec3d& operator *= ( const Vec3d& rkVector );
		inline Vec3d& operator /= ( const double fScalar );
		inline Vec3d& operator /= ( const Vec3d& rkVector );

		///���Ⱥ���
		inline double length () const;

		///ʱ���õ����ȵ�ƽ��,����Ƚϵ�ʱ��
		inline double squaredLength () const;


		///���뺯��,����2��������ľ���
		inline double distance(const Vec3d& rhs) const;
		inline double squaredDistance(const Vec3d& rhs) const;


		///����2���ĵ��.
		inline double dotProduct(const Vec3d& vec) const;
		inline double absDotProduct(const Vec3d& vec) const;

		///��׼������,ע��:			
		///1. ��ʱ���Զž�0���������.
		///2. ���з���ֵ,����ֵΪ��ǰ�����ĳ���,ע�ⲻ��vector3,��Ҫ����vector3 ,��ʹ��normalisedRef().
		inline double normalise();

		///���ؾ�����λ������������.
		inline Vec3d& normalisedRef();

		///������
		inline Vec3d crossProduct( const Vec3d& rkVector ) const;

		///������������͵�һ��.
		inline Vec3d midPoint( const Vec3d& vec = ZERO ) const;

		///�ȽϺ���
		inline bool operator < ( const Vec3d& rhs ) const;
		inline bool operator > ( const Vec3d& rhs ) const;

		///ȡ���������Ƚ϶��õ�����Сֵ,���Ҹ��豾��
		inline Vec3d makeFloor( const Vec3d& cmp );

		///ȡ���������Ƚ϶��õ������ֵ,���豾��,��������ֵ.
		inline Vec3d makeCeil( const Vec3d& cmp );
		inline Vec3d ceilProduct( const Vec3d& c ) const;
		inline Vec3d floorProduct( const Vec3d& c ) const;

		///�õ�һ����������.ע��,��ʱ��������������ȷ��,���õ�׼ȷ����������,��ʹ��4Ԫ��.
		inline Vec3d perpendicular(void) const;


		/** Returns true if this vector is zero length. */
		inline bool isZeroLength(void) const;

		/** As normalise, except that this vector is unaffected and the
		normalised vector is returned as a copy. */
		inline Vec3d normalisedCopy(void) const;

		///���㷴������
		///ע��:
		///1.��ʱnormalҪΪ��λ����
		///2.ͨ����������׺���������Ū��,��ע����Եķ���.
		inline Vec3d reflect(const Vec3d& normal) const;


		inline bool operator <= ( const Vec3d& rhs ) const;
		inline bool operator >= ( const Vec3d& rhs ) const;

		inline  void  reset( const double& X ,
			const double& Y ,const double& Z )
		{
			this->x = X;
			this->y = Y;
			this->z = Z;
		}

		String getDetails() const
		{
			std::ostringstream o;
			o << "Vec3d:(" << x << ", " << y << ", " << z << ")";
			return o.str();
		}

		void setLength(double len)
		{
			double scale = (len / length());

			this->x *= scale;
			this->y *= scale;
			this->z *= scale;

		}

		vgKernel::Vec3 getVec3Float() const
		{
			return vgKernel::Vec3( (float)x ,(float)y ,(float)z );
		}

	public:			

		static const Vec3d ZERO;
		static const Vec3d UNIT_X;
		static const Vec3d UNIT_Y;
		static const Vec3d UNIT_Z;
		static const Vec3d NEGATIVE_UNIT_X;
		static const Vec3d NEGATIVE_UNIT_Y;
		static const Vec3d NEGATIVE_UNIT_Z;
		static const Vec3d UNIT_SCALE;

	};

	//std::ostream& operator<< ( std::ostream& o, Vec3d vec3 )
	//{
	//	o << vec3.getDetails();
	//	return o;
	//}

	///inline ����ʵ��.
	#include "vggiVec3d.inl"
	
	
}// end of namespace  
	
 