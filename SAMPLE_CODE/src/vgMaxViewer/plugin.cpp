

/**********************************************************************
*<
plugin.cpp:	����ӿڣ�����3ds max�Ĳ���淶

��Ҫ�����ǣ�
1��DLL���DLLMain����ʵ��
2�������5���ӿں���ʵ��
*>	
**********************************************************************/

#include "vgStableHeaders.h"
#include "vgUtility/vgModCommonUtility.h"
#include "vgModViewerUtility.h"
#include "vgMaxViewer.h"


//HINSTANCE hInstance;
int controlsInit = FALSE;

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved)
{
	//hInstance = hinstDLL;
	vgModUtility::setInstance(hinstDLL) ;

	if (!controlsInit) {
		controlsInit = TRUE;
		InitCustomControls(hinstDLL);
		InitCommonControls();
	}

	return (TRUE);
}

static CollectionClassDesc collectionDesc;

__declspec( dllexport ) const TCHAR* LibDescription()
{
	return vgModUtility::GetString(IDS_LIBDESCRIPTION);
}

__declspec( dllexport ) int LibNumberClasses()
{
	return 1;
}

__declspec( dllexport ) ClassDesc* LibClassDesc(int i)
{
	switch(i) {
		case 0: return &collectionDesc;//GetCollectionDesc();
		default: return 0;
	}
}

__declspec( dllexport ) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

