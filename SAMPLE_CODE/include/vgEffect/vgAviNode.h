



#ifndef __VGAVINODE_H__
#define __VGAVINODE_H__






#pragma  once

#include <vgCore/vgPrerequisites.h>
#include <vgMod/vgObjNode.h>
#include "vgAviTexture.h"

namespace vgMod {

	using namespace vgCore;
	/**
		@date 	2008/02/27  16:46	
		@author  leven
	
		@brief 	
	
		@see    
	*/
	class VGDEP_EXPORT AviNode : public ObjectNode
	{

	public:
		AviNode(int nVersionMod = VERSION_1);
		~AviNode();
	
	
	public:

		void setDefault();
		//------------------------------------------
		// ģ������û��ɾ��.ֻ��ɾ��cg����Ϣ
		//------------------------------------------
		void destroy();

		virtual void Render();

		virtual	unsigned long PositDataToNode(char *data );
	
		virtual void WriteNodeToVG(CFile &fp);

		//����ObjectNode�ӿ�
		virtual void	setOGLTestStates();
		virtual void	resetOGLTestStates();

		bool recreate(const String& decalmap );

		void setPlayState(bool bPlay);

		//add by KingHJ 2010.03.16
		bool getAviState()
		{
			return m_bPlay;
		}

		string* getAviBaseName()
		{
			return &m_StrAvi_basename;
		}

		string& getAviName();

		bool getInitFlag();

		virtual vgKernel::StringVector getTexNameVec();

	public:

		AviTexture *m_avitexture;

		std::string m_StrAvi;

		std::string m_StrAvi_basename;


		// �Ƿ��ʼ����
		bool mIsInited;

		bool m_bPlay;
	
	};
	
	
	
}// end of namespace vgCore

#endif // end of __VGAVINODE_H__