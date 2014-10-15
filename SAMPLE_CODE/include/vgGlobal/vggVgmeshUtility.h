

#ifndef __VGMESHUTILITY_H__
#define __VGMESHUTILITY_H__

#include <vgGlobal/vgglDefinition.h>

namespace vgGlobal{

	//using namespace vgXXX;
	
	
	class	VGGL_EXPORT MeshUtility
	{
	public:
		MeshUtility();
		~MeshUtility();

		//ԭ��GlobalFacade�߸�����ת�Ƶ�����
	public:
		static bool importImageFile();
		static bool saveModelFileOnly();
		static bool saveModelAndImage();

		//���ζ�дvgm��vgi
		static bool importModelFileOnly( String VgmPathName);
		static bool importImageFileOnly(String VgiPathName);
		static void importModelAndImage(String readPath);

		static bool saveModelAndImage(String savePath);

		static bool collectModelAndImage(String savePath);
		static bool	importModelAndImagePreVersion(String readPath, String strheader);

		// �����п鱣��ģʽ--�м�����
		static void setSaveMode( bool bMode );
	};
}//namespace vgGlobal

#endif//__VGMESHUTILITY_H__