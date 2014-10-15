



#ifndef __VGSHRAY3D_H__
#define __VGSHRAY3D_H__



#include <vgShape/vgshDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkRay.h>

	
namespace vgShape {

	/**
		@date 	2009/02/20  20:03	
		@author  leven
	
		@brief 	
	
		@see    
	*/
	/** 
	*/
	class Ray3d
	{
	public:

		///���ߵ���ʼ��.
		Vec3d mOrigin;		

		///����ָʾ�ķ���.
		Vec3d mDirection;
	public:

		///Ĭ������:
		///Ray3d(Origin = Vector3( 0 , 0 , 0 ) , Direction = Vector3( 0 , 1 , 0 ))
		Ray3d()
			:mOrigin(Vec3d::ZERO), mDirection(Vec3d::UNIT_Y) {}

		Ray3d(const Vec3d& origin, const Vec3d& direction)
			:mOrigin(origin), mDirection(direction) {}

		/// ����ԭ��.
		void setOrigin(const Vec3d& origin) {mOrigin = origin;} 

		/// �õ�ԭ��ֵ.
		Vec3d getOrigin(void) const {return mOrigin;} 

		/// ���÷���
		void setDirection(const Vec3d& dir) {mDirection = dir;} 

		/// �õ���������.
		Vec3d getDirection(void) const {return mDirection;} 

		///�õ������ϵ�һ��.mOriginΪgetPoint(0),ָ���ĩ��ΪgetPoint(1).
		///�����ϵķ�Χ[0-1].
		///ע��,����intersects���Եķ���ֵ�ٵõ���Ĺ�����,��������Խ��.
		///��ҪԽ����ʱ,ȡ��ע��.
		Vec3d getPoint(float t) const 
		{ 
			//assert( (t <= 1) && (t >= 0) && "���ʹ����ϵĵ�Խ��" );
			return Vec3d(mOrigin + (mDirection * t));
		}

		/** Gets the position of a point t units along the ray. */
		Vec3d operator*(float t) const 
		{ 
			return getPoint(t);
		};

		inline String getDetails() const
		{
			std::ostringstream o;
			o << _T("Ray3d(Origin = ") << mOrigin.getDetails() <<
				_T(" , Direction = ") << 
				mDirection.getDetails() << _T(")");
			return o.str();
		}

		// �õ�ĳһ��������ray�ϵĴ����ཻ��
		Vec3d getPerpendicularVertex( const Vec3d& point )
		{
			Vec3d normalize_dir = mDirection.normalisedCopy();

			float len = (float)normalize_dir.dotProduct( point - mOrigin );

			return  mOrigin + len * normalize_dir;
		}

		vgKernel::Ray getRayFloat() const
		{
			return vgKernel::Ray( mOrigin.getVec3Float() , mDirection.getVec3Float() );
		}
	};
	
	
}// end of namespace vgShape
	


#endif // end of __VGSHRAY3D_H__