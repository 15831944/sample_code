


#ifndef __VGRENDERENVUTILITY_H__
#define __VGRENDERENVUTILITY_H__

#include <vgGlobal/vgglDefinition.h>
#include <vgKernel/vgkForward.h>


namespace vgGlobal{

	/**
		@date    2009/10/21  14:08
		@author  zzy

		@brief   

		@see
	*/	

	class	VGGL_EXPORT RenderEnvUtility
	{
	public:
		RenderEnvUtility()	{}
		~RenderEnvUtility()	{}

	public:
		//��δ��ȡ
		static  bool  initGLContext();      //---����thisָ����δ��ȡ
		static  bool  releaseGLContext();   //---����thisָ����δ��ȡ

		static  bool  resizeWindow();       
		static  bool  destoryWindow();
		static  bool  changeWindowPos();

		static  bool  renderScene();

	};
}//namespace vgGlobal

#endif//__VGEDITUTILITY_H__