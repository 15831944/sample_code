

#ifndef __VGSKYBOXNODE_H__
#define __VGSKYBOXNODE_H__




#include <vgCore/vgPrerequisites.h>
#include <vgMod/vgObjNode.h>
#include <vgCam/vgcaCamManager.h>


namespace vgMod {

	using namespace vgCore;

	/**
		@date 	2008/02/27  16:46	
		@author  leven
	
		@brief 	
	
		@see    
	*/

	/*
	5010 -- 5030֮��ΪQuadTreeSceneMananger�Ĺ�Ͻ��Χ֮��
	*/

	class SkyboxNode : public ObjectNode
	{

	public:
		SkyboxNode(int nVersionMod = VERSION_1);
		~SkyboxNode();
	
	
	public:

		void setDefault();
		//------------------------------------------
		// ģ������û��ɾ��.ֻ��ɾ��cg����Ϣ
		//------------------------------------------
		void destroy();

		virtual void Render();

		virtual	unsigned long PositDataToNode(char *data );
	
		virtual void WriteNodeToVG(CFile &fp);


		//bool recreate(CameraNode *camera , const StringVector& strvec , const String& bumpmap , 
		//	const String& decalmap , const float& bumpscale);
		
		virtual vgKernel::StringVector getTexNameVec();

	public:

		bool initImpl();

		void setCamera( vgCam::CamManager* camera )
		{
			this->mpEyeCamera = camera;
		}

		//vgCam::CamManager *mpEyeCamera;
		vgCam::CamManager *mpEyeCamera;


		float _oldPosX;
		float _oldPosZ;


		//std::string  m_StrEnvMap[6];
		//std::string m_StrBumpMap;
		//std::string m_StrDecalMap;
		//float m_fBrightness;

		// �Ƿ��ʼ����
		bool mIsInited;
	
	};
	
	
	
}// end of namespace vgCore



#endif // end of __VGSKYBOXNODE_H__