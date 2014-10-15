




#ifndef __VGBRIGHTFLOORNODE_H__
#define __VGBRIGHTFLOORNODE_H__





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

	class BrightfloorNode : public ObjectNode
	{

	public:
		BrightfloorNode(int nVersionMod = VERSION_1);
		~BrightfloorNode();
	
	
	public:

		void setDefault();
		//------------------------------------------
		// ģ������û��ɾ��.ֻ��ɾ��cg����Ϣ
		//------------------------------------------
		void destroy();

		virtual void Render();

		virtual	unsigned long PositDataToNode(char *data );
	
		virtual void WriteNodeToVG(CFile &fp);


		bool recreate(vgCam::CamManager *camera , const vgCore::StringVector& strvec , const String& bumpmap , 
			const String& decalmap , const float& bumpscale);

		virtual void	assignTexture(int i );

		virtual vgKernel::StringVector getTexNameVec();

	public:

		

		std::string m_StrDecalMap;
		float m_fBumpScale;

		// �Ƿ��ʼ����
		bool mIsInited;

	private:

		GLuint _brightfloorspheremapid;
		float _constColor[4];
	
		bool initWaterImpl();

	};
	
	
	
}// end of namespace vgCore

#endif // end of __VGBRIGHTFLOORNODE_H__