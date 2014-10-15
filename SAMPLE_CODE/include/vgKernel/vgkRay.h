/*
-----------------------------------------------------------------------------
  VRES(Virtual Reality GIS Engine System)    
  -------------------------------------------------------------
  License details ... waiting to add...
  -------------------------------------------------------------
  Copyright  (C) 2007 , xy - All Rights Reserved
-----------------------------------------------------------------------------
*/

#ifndef __VGKRAY_H__
#define __VGKRAY_H__


#include <vgKernel/vgkForward.h>
#include <tchar.h>
#include <vgKernel/vgkVec3.h>


namespace vgKernel {


	/**
		@date 	2007/02/22  18:02	
		@author  xy
	
		@brief 	������.ʵ�ֱȽϼ�.�����ཻ���Ժ����ڰ���������µó������ݲ��Ǻ�׼.
	
		Representation of a ray in space, ie a line with an origin and direction.

		@see    
	*/
	class  VGK_EXPORT Ray
	{
	public:

		///���ߵ���ʼ��.
		Vec3 mOrigin;		

		///����ָʾ�ķ���.
		Vec3 mDirection;
	public:

		///Ĭ������:
		///Ray(Origin = Vector3( 0 , 0 , 0 ) , Direction = Vector3( 0 , 1 , 0 ))
		Ray()
			:mOrigin(Vec3::ZERO), mDirection(Vec3::UNIT_Y) {}

		Ray(const Vec3& origin, const Vec3& direction)
			:mOrigin(origin), mDirection(direction) {}

		/// ����ԭ��.
		void setOrigin(const Vec3& origin) {mOrigin = origin;} 

		/// �õ�ԭ��ֵ.
		Vec3 getOrigin(void) const {return mOrigin;} 

		/// ���÷���
		void setDirection(const Vec3& dir) {mDirection = dir;} 

		/// �õ���������.
		Vec3 getDirection(void) const {return mDirection;} 

		///�õ������ϵ�һ��.mOriginΪgetPoint(0),ָ���ĩ��ΪgetPoint(1).
		///�����ϵķ�Χ[0-1].
		///ע��,����intersects���Եķ���ֵ�ٵõ���Ĺ�����,��������Խ��.
		///��ҪԽ����ʱ,ȡ��ע��.
		Vec3 getPoint(float t) const 
		{ 
			//assert( (t <= 1) && (t >= 0) && "���ʹ����ϵĵ�Խ��" );
			return Vec3(mOrigin + (mDirection * t));
		}
		
		/** Gets the position of a point t units along the ray. */
		Vec3 operator*(float t) const 
		{ 
			return getPoint(t);
		};
		

#ifndef VGK_NO_OPENGL
		void render()
		{
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_TEXTURE_1D);
			glPushMatrix();
			glColor3f(1,0,0);
			//glColor3f(_colorRed, _colorYel, _colorBlu);

			glLineWidth(1.0);

			glBegin(GL_LINES);
			{
				glVertex3f(mOrigin.x, mOrigin.y, mOrigin.z);
				//glVertex3f(mDirection.x * 100, mDirection.y * 100, mDirection.z * 100);
				glVertex3f(mDirection.x + mOrigin.x , mDirection.y+ mOrigin.y, mDirection.z+ mOrigin.z);

			}
			glEnd();

			glPopMatrix();

			glEnable(GL_TEXTURE_2D);
			glEnable(GL_TEXTURE_1D);
		}
#endif

 		inline String getDetails() const
 		{
			std::ostringstream o;
 			o << _T("Ray(Origin = ") << mOrigin.getDetails() <<
				_T(" , Direction = ") << 
				mDirection.getDetails() << _T(")");
 			return o.str();
 		}

		// �õ�ĳһ��������ray�ϵĴ����ཻ��
		inline Vec3 getPerpendicularVertex( const Vec3& point )
		{
			Vec3 normalize_dir = mDirection.normalisedCopy();

			float len = normalize_dir.dotProduct( point - mOrigin );

			return  mOrigin + len * normalize_dir;
		}
	};

	
	
	
}

#endif // end of __VGFRAY_H__