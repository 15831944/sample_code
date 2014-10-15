




#ifndef __VGMODEFFECTCONVERTOR_H__
#define __VGMODEFFECTCONVERTOR_H__

#include <vgKernel/vgkForward.h>
#include <vgGlobalDefinition.h>

#include <vgMod/vgObjNode.h>


namespace vgMod {

	/**
	@date 		2009/05/19  19:42	
	@author  	lss

	@brief 	

	@see    
	*/

	class  VGDEP_EXPORT ModEffectConvertor
	{
	private: 
		ModEffectConvertor()
		{

		}
		~ModEffectConvertor()
		{

		}

	public:
		/*!
		... תmodΪ��Ч ...
		*/		
		static ModItemPtrVec	convertSelectedObjectToEffectNode2();
		
		template <typename EFFECT_TYPE , typename DLG_TYPE/*, vgCore::IMAGETYPE EFFECT_IMAGE_TYPE */>
		static ModItemPtrVec convertSelectedObjectToEffectNode();
		
		/*!
		... תmodΪˮ��Ч ...
		*/		
		static ModItemPtrVec	convertSelectedObjectToWaterNode();

		/*!
		... תmodΪ������Ч ...
		*/		
		static ModItemPtrVec	convertSelectedObjectToGlassNode();

		/*!
		... תmodΪavi��Ƶ��Ч ...
		*/		
		static ModItemPtrVec	convertSelectedObjectToAviNode();

		/*!
		... תmodΪ������Ч ...
		*/		
		static ModItemPtrVec	convertSelectedObjectToMirrorNode();

		/*!
		... תmodΪ������Ч ...
		*/		
		static ModItemPtrVec	convertSelectedObjectToMetalNode();

		/*!
		... תmodΪ����������Ч ...
		*/		
		static ModItemPtrVec	convertSelectedObjectToFloorNode();


		/*!
		... תmodΪ��̬������Ч ...
		*/		
		static ModItemPtrVec	convertSelectedObjectToDynamicTexNode();

		/*!
		... תmodΪ�Զ���ģ��Ч ...
		*/		
		static ModItemPtrVec	convertSelectedObjectToAutoBuildNode();
		
		/*!
		... תmodΪ��պ���Ч ...
		*/		
		static ModItemPtrVec	convertSelectedObjectToSkyboxNode();
		
	public:
		static String&	getProDataPath();
		static	void setProDataPath(String& path);

	public:
		static bool changeWaterDirec();

	protected:
		static	String	m_strProDataPath;

	};


}// end of namespace vgMod



#endif // end of __VGMODEFFECTCONVERTOR_H__