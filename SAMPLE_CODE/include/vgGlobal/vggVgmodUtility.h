


#ifndef __VGGMODUTILITY_H__
#define __VGGMODUTILITY_H__


#include <vgGlobal/vgglDefinition.h>
#include <vgMod/vgNodeManager.h>

#include <vgKernel/vgkRenderer.h>
#include <vgKernel/vgkAbstractEntry.h>


namespace  vgGlobal{

	using namespace vgMod;
	
	/**
		@date     2009/10/21  21:11
		@author   ZhouZY

		@brief    ��������Mod��ز������м��
				  ԭvgMod::ModUtility�л�����һЩ�ӿ���ʱ�޷�
				  ��ֲ�������Ա���ײ�DLL�����ϲ�DLL

		@see
	*/


	class	VGGL_EXPORT ModUtility
	{
	public:
		ModUtility();
		~ModUtility();

		//���⹫���ӿ�
	public:
		static bool importModFile(const String& strFilePath ); 
		
		static bool importVgFile(const String& strFilePath );
		static bool importVgFileDirectly(const String& strFilePath );

		static bool exportVgFile(const String& strFilePath );
		static bool exportVgFileDirectly(const String& strFilePath );

		static bool exportVgImgFile(const String& strFilePath );

		static bool responseLButtonDown(UINT nFlags, CPoint point);
		static bool responseLButtonDblClk(UINT nFlags, CPoint point, CWnd* pwnd );
		static bool responseLButtonUp(UINT nFlags, CPoint point);

		static bool convertSelectedObjectToEffectNode(int nEffectType);
		
		//�޲νӿ�����View����Ϣ��Ӧ����
		static bool importModFile();        //�ؼ�֡����--��ΪMod����

		static bool addModFileToScene(String newModFile, String vgFilePath);	// ����Mod�ļ�������

		static bool convertToWater();        //ת��Ϊˮ��
		static bool convertToGlass();        //ת��Ϊ����
		static bool convertToMirror();       //ת���ɾ���
		static bool convertToMetal();        //ת���ɽ���
		static bool convertToBrightFloor();  //ת��Ϊ��������
		static bool convertToSkybox();       //ת��Ϊ�����
		static bool convertToAviTex();       //ת����AVI����
		static bool convertToDynTex();       //ת��������֡����
		static bool convertToAutoBuild();    //ת��Ϊ�Զ���ģ
		static bool cloneAutoBuildObj();     //��¡�Զ���ģ����
		static void generateSolidBuild();	 //�����Զ������
		static bool getGenerateBuildFlag();  //���±��
		static void generateMarkBuild();     //������Ϣ��ע����
		static bool getAllLandMarkFlag();       //������Ϣ��Ǳ�־
		static void enableAllLandMark(bool enable);        //������Ϣ��ע��ʾ

		static bool addNodeToUI( CategoryType catType, NodePtrVec* pNodeList, const string& strFilePath );

		static bool getSaveMode();
		static void setSaveMode( bool bMode );

		static bool changeWaterDirection();	 //�ı�ˮ������ Added By FengYK @2010-03-23 14:57

		static bool importModData( const vgKernel::StringVector& strFilePaths );
	private:
		
		static bool importModBegin( const vgKernel::StringVector& strFilePaths );   
		static bool importModEnd( const String& strFilePath );


		static bool addNodeToUI( const string& strFilePath );
		static bool addModNodeToUI( ModItemPtrVec* pNodeList , const string& strFilePath);
		static vgKernel::AbstractEntry* rendererToEntry( vgKernel::Renderer* renderer );

	};
}//namespace vgGlobal

#endif//__VGGMODUTILITY_H__