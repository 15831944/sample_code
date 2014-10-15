/*
-----------------------------------------------------------------------------
  VRES(Virtual Reality GIS Engine System)    
  -------------------------------------------------------------
  License details ... waiting to add...
  -------------------------------------------------------------
  Copyright  (C) 2007 , leven - All Rights Reserved
-----------------------------------------------------------------------------
*/

#ifndef __VGFRAY_H__
#define __VGFRAY_H__

#include <vgMath/vgfPrerequisites.h>


namespace vgMath {

	/**
		@date 	2007/02/22  18:02	
		@author  leven
	
		@brief 	������.ʵ�ֱȽϼ�.�����ཻ���Ժ����ڰ���������µó������ݲ��Ǻ�׼.
	
		Representation of a ray in space, ie a line with an origin and direction.

		@see    
	*/
	class VgfExport Ray
	{
	protected:

		///���ߵ���ʼ��.
		Vector3 mOrigin;		

		///����ָʾ�ķ���.
		Vector3 mDirection;
	public:

		///Ĭ������:
		///Ray(Origin = Vector3( 0 , 0 , 0 ) , Direction = Vector3( 0 , 1 , 0 ))
		Ray()
			:mOrigin(Vector3::ZERO), mDirection(Vector3::UNIT_Y) {}

		Ray(const Vector3& origin, const Vector3& direction)
			:mOrigin(origin), mDirection(direction) {}

		/// ����ԭ��.
		void setOrigin(const Vector3& origin) {mOrigin = origin;} 

		/// �õ�ԭ��ֵ.
		const Vector3& getOrigin(void) const {return mOrigin;} 

		/// ���÷���
		void setDirection(const Vector3& dir) {mDirection = dir;} 

		/// �õ���������.
		const Vector3& getDirection(void) const {return mDirection;} 

		///�õ������ϵ�һ��.mOriginΪgetPoint(0),ָ���ĩ��ΪgetPoint(1).
		///�����ϵķ�Χ[0-1].
		///ע��,����intersects���Եķ���ֵ�ٵõ���Ĺ�����,��������Խ��.
		///��ҪԽ����ʱ,ȡ��ע��.
		Vector3 getPoint(Real t) const 
		{ 
			//assert( (t <= 1) && (t >= 0) && "���ʹ����ϵĵ�Խ��" );
			return Vector3(mOrigin + (mDirection * t));
		}
		
		/** Gets the position of a point t units along the ray. */
		Vector3 operator*(Real t) const 
		{ 
			return getPoint(t);
		};

		/// ��ƽ����ཻ����.
		/** Tests whether this ray intersects the given plane. 
		@returns A pair structure where the first element indicates whether
			an intersection occurs, and if true, the second element will
			indicate the distance along the ray at which it intersects. 
			This can be converted to a point in space by calling getPoint().
		*/
		std::pair<bool, Real> intersects(const Plane& p) const
		{
			return Math::intersects(*this, p);
		}

		/** Tests whether this ray intersects the given plane bounded volume. 
		@returns A pair structure where the first element indicates whether
		an intersection occurs, and if true, the second element will
		indicate the distance along the ray at which it intersects. 
		This can be converted to a point in space by calling getPoint().
		*/
		//std::pair<bool, Real> intersects(const PlaneBoundedVolume& p) const
		//{
		//	return Math::intersects(*this, p.planes, p.outside == Plane::POSITIVE_SIDE);
		//}


		///������ཻ����.���ص�pair�ĵڶ���ֵ����getPoint(),���Եõ��ཻ��
		/** Tests whether this ray intersects the given sphere. 
		@returns A pair structure where the first element indicates whether
			an intersection occurs, and if true, the second element will
			indicate the distance along the ray at which it intersects. 
			This can be converted to a point in space by calling getPoint().
		*/
		std::pair<bool, Real> intersects(const Sphere& s) const
		{
			return Math::intersects(*this, s);
		}


		/// ���Χ�е��ཻ����.
		/** Tests whether this ray intersects the given box. 
		@returns A pair structure where the first element indicates whether
			an intersection occurs, and if true, the second element will
			indicate the distance along the ray at which it intersects. 
			This can be converted to a point in space by calling getPoint().
		*/
		std::pair<bool, Real> intersects(const AxisAlignedBox& box) const
		{
			return Math::intersects(*this, box);
		}

		inline friend StdOStream& operator << ( StdOStream& o , const Ray& s )
		{
			o << _T("Ray(Origin = ") << s.mOrigin << _T(" , Direction = ") <<  s.mDirection << _T(")");
			return o;
		}

		inline String getDetails() const
		{
			StdOStringStream o;
			o << (*this);
			return o.str();
		}

		// �õ�ĳһ��������ray�ϵĴ����ཻ��
		inline Vector3 getPerpendicularVertex( const Vector3& point )
		{
			Vector3 normalize_dir = mDirection.normalisedCopy();

			float len = normalize_dir.dotProduct( point - mOrigin );

			return  mOrigin + len * normalize_dir;
		}


		inline void getPerspNearAndFarFromBox(  AxisAlignedBox& int_box , 
			float& out_near , float& out_far )
		{
			Vector3 normalize_dir = mDirection.normalisedCopy();

			const Vector3 point = int_box.getCorner( 0 );
			Real len = normalize_dir.dotProduct( point - mOrigin );

			float max_len = len;
			float min_len = len;

			for ( int i = 1 ; i < 8 ; ++i )
			{
				const Vector3 point = int_box.getCorner( i );

				Real len = normalize_dir.dotProduct( point - mOrigin );

				if ( len <= 0.0f )
				{
					continue;
				}

				max_len = Math::Max( len , max_len );
				min_len = Math::Min( len , min_len );
			}

			out_near = min_len;
			out_far = max_len;

			return;
		}

	};

	
	
	
}

#endif // end of __VGFRAY_H__