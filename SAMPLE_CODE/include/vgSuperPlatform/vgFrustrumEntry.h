#pragma once

#include "vgBaseEntry.h"
#include "vgUIController.h"
#include <vgKernel/vgkRendererObserver.h>

#include <vgCam/vgcaCamManager.h>
class vgFrustrumEntry :	public vgBaseEntry
{
public:
	const static int s_NumOfParam = 6;
	static PropertiesParam s_ParamArray[s_NumOfParam];

public:
	vgFrustrumEntry();
	virtual ~vgFrustrumEntry(void);

	void AddNodeTabs();

	virtual String GetName()
	{
#if VGK_LANG_CHS
		return "�Ӿ���";
#else
		return "Frustum";
#endif
	}

	int GetSelectedImage() { return 1; }

	int GetUnSelectedImage() { return 2; }

	// �̳���vgBasedNode�޸�Node����������Object ��TreeItem
	void OnPropertyChanged(string paramName);// { //TRACE("Camera Property Changed \n");}

	// �̳���Changed������Object�޸ĺ����Node
	void onChanged(int eventId, void *param);

	void UpDataProp() {} 

	CMenu* GetContextMenu()
	{
		return NULL;
	}

private:

	// ���޸����ݵĸ���
	GLfloat m_Near;
	GLfloat m_Far;
	GLfloat m_Fov;

	String m_sCaption;
};
