/*
-----------------------------------------------------------------------------
  VRES(Virtual Reality GIS Engine System)    
  -------------------------------------------------------------
  License details ... waiting to add...
  -------------------------------------------------------------
  Copyright  (C) 2007 , leven - All Rights Reserved
-----------------------------------------------------------------------------
*/

#ifndef __VGFMATH_H__
#define __VGFMATH_H__

#include <vgMath/vgfPrerequisites.h>
#include <vgMath/vgfAsmMath.h>


namespace vgMath {

	/**
		@date 	2007/02/21  10:02	
		@author  leven
	
		@brief 	����Math��,Angle��,Radian��,Degree��.

		ע����붼����һ��ͷ�ļ�����,��Ϊʵ�ֵ�ʱ��ܶ�inline����,���ҽ�������ʵ�ֺܶ�.
	
		������ѧ����ķ���,�μ�Glossary.txt , ��ѧ�ʻ�Ӣ�﷭��.txt

		@see  
	*/

	#include <vgMath/vgfAngle.inl>
	#include <vgMath/vgfRadian.inl>	
	#include <vgMath/vgfDegree.inl>
	#include <vgMath/vgfMath.inl>

	#include <vgMath/vgfAngleImpl.inl>
	#include <vgMath/vgfRadianImpl.inl>
	#include <vgMath/vgfDegreeImpl.inl>

	
}

#endif // end of __VGFMATH_H__