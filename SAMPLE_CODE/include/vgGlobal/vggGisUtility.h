

#ifndef __VGGGISUTILITY_H__
#define __VGGGISUTILITY_H__
#include <vgGlobal/vgglDefinition.h>

#include <vgGis/vgGisManager.h>
#include <vgGIS3D/vggiDataSource.h>

namespace vgGlobal{

	
	class	VGGL_EXPORT GisUtility
	{
	public:
		GisUtility();
		~GisUtility();

	public:
		static bool renderGisElements();

		/**
			zzy 
			2009/10/21   17:02
			view��Ӧ�˵���Ϣ�������°˸��ӿڣ�����ʹ��vgGlobal::SystemFacade�ṩ�Ľӿ�
		*/
		static bool	measureGisPoint();    //��ѯ����
		static bool	measureGisLength();   //��������
		static bool	measureGisArea();     //�������
		static bool	measureGisVolume();   //�������
		static bool matchRoad();          //��·ƥ��--VGMSGBOX2��ʱδ��ȡ���� 

		static bool measureGisVer_Hor_Slope();//��ֱ���������ˮƽ����������¶Ȳ���

		static bool FaecherAnalysis();//�������

		static bool	updateStateGisPoint();
		static bool	updateStateGisLength();
		static bool	updateStateGisArea();
		static bool updateGisVer_Hor_Slope();

		static bool updateStateGisFaecher();

		static void	reset();
		static void	uninitBeforeOpenGLDestroy();

	public:
		static	bool importShpFile(const string& strFilePath, bool bSpecial = false );

		static	bool readProject( const string& strFilePath );

		static	bool convertShpToEditState();

		//static  bool convertShpToEditState();
		static	bool getSelectFlag();

	private:
		static	bool addShpToUI(vgGIS3D::DataSource* psource);
	private:
		static bool setGisAnalysis(vgGIS3D::EGisState eType);
		static bool	updateGisState(vgGIS3D::EGisState eType);

	};
}//namespace vgGlobal

#endif//__VGGGISUTILITY_H__
