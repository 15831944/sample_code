#pragma once

#include <vgEntry/vgBaseEntry.h>
#include <vgUIController/vgPropertyPage.h>

#include <cmath>
#include <vgKernel/vgkRendererObserver.h>

#include <vgCam/vgcaViewCam.h>

#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define PI 3.1415926535

class  VGUI_EXPORT vgCamMgrEntry :	public vgBaseEntry
{
public:
	const static int s_NumOfParam = 18;
	static PropertiesParam s_ParamArray[s_NumOfParam];

public:
	vgCamMgrEntry();
	virtual ~vgCamMgrEntry(void);

	void AddNodeTabs();

	virtual String GetName()
	{
#if VGK_LANG_CHS
		return "��ǰ���";
#else
		return "Current Camera";
#endif
	}

	int GetSelectedImage() { return 1; }

	int GetUnSelectedImage() { return 2; }

// 	void SetPosX(float posX);
// 
// 	void SetPosY(float posY);
// 
// 	void SetPosZ(float posZ);

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

	void getData();

	// ���޸����ݵĸ���
	GLfloat m_eyePosX;
	GLfloat m_eyePosY;
	GLfloat m_eyePosZ, m_eyePosZMinus;

	GLfloat m_tarPosX;
	GLfloat m_tarPoxY;
	GLfloat m_tarPoxZ;

	GLfloat m_upPosX;
	GLfloat m_upPosY;
	GLfloat m_upPoxZ;

	GLfloat m_hAngle;
	GLfloat m_vAngle;

	GLfloat m_speend;
	GLfloat m_turningSpeed;
	GLfloat m_pitchSpeed;

	GLfloat m_turningDegree;
	GLfloat m_pitchDegree;

	GLfloat m_upAndDownMultiply;

	String m_sCaption;

	float m_eillipX;
	float m_eillipY;
	float m_eillipZ;
};

