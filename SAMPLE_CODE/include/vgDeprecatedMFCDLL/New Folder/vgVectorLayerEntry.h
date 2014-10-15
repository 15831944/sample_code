#pragma once

#include "vgBaseEntry.h"
#include "vgUIController.h"
#include <vgKernel/vgkChangedObserver.h>

#include <vgVector/vgvLayer.h>

class vgVectorLayerEntry :	public vgBaseEntry
{
public:
	const static int s_NumOfParam = 8;
	static PropertiesParam s_ParamArray[s_NumOfParam];

public:
	vgVectorLayerEntry(vgVector::vgvLayer*	pLayer);
	virtual ~vgVectorLayerEntry(void);

	void AddNodeTabs();

	int GetSelectedImage() { return 1; }

	int GetUnSelectedImage() { return 2; }

	// �̳���vgBasedNode�޸�Node����������Object ��TreeItem
	void OnPropertyChanged(string paramName);// { //TRACE("Camera Property Changed \n");}

	// �̳���Changed������Object�޸ĺ����Node
	void OnChanged(int eventId, void *param);

	void UpDataProp() {} 

	CMenu* GetContextMenu()
	{
		return NULL;
	}

private:

	// ���޸����ݵĸ���
	vgKernel::Vec3	m_LayerCenterPoint;
	bool	m_bIsLayerVisible;

	float	m_posZMinus;

	String	m_strConstShapeCount;
};
