#pragma once

#include "vgBaseEntry.h"
#include "vgUIController.h"
#include <vgKernel/vgkRendererObserver.h>

#include <vgVector/vgvGeometry.h>
#include <vgVector/vgvAttribute.h>



class vgVectorGeometryEntry :	public vgDatabaseEntry
{
public:
	//const static int s_NumOfParam = 7;
	//static PropertiesParam s_ParamArray[s_NumOfParam];
	//static std::vector<PropertiesParam> _params;
	//========================================//YXDB
	virtual String GetName()
	{
		String longname = m_Renderer->getName()	;

		return longname.substr( 0 , longname.find_last_of("_")-1 );
	}
	//==============================================================
public:
	vgVectorGeometryEntry(vgVector::vgvGeometry*	pGeometry);
	virtual ~vgVectorGeometryEntry(void);

	//void AddNodeTabs();

	int GetSelectedImage() { return 1; }

	int GetUnSelectedImage() { return 2; }

	// �̳���vgBasedNode�޸�Node����������Object ��TreeItem
	//void OnPropertyChanged(string paramName);// { //TRACE("Camera Property Changed \n");}

	// �̳���Changed������Object�޸ĺ����Node
	void onChanged(int eventId, void *param);

	void UpDataProp() {} 

	CMenu* GetContextMenu()
	{
		return NULL;
	}

	// ��ѡʱ����Tabҳ�棬��DatabaseTabPage
	virtual void AddSingleTabPage();

	// ��ѡʱ����Tabҳ��
	virtual void AddGroupTabPage();

protected:
	// ��д������PropertyChanged�������ֱ���single��Group��PropertyChanged
	virtual void OnGroupPropertyChanged(string paramName);

	virtual void OnSinglePropertyChanged(string paramName);

private:
	// �û��Զ��庯����ʵ��Mesh�ض���Group�༭����
	void OnGroupChanged(float sdx, float sdy, float sdz);

private:

	// ���޸����ݵĸ���
	vgKernel::Vec3	m_CenterPoint;
	bool			m_bIsVisible;

	float			m_posZMinus;

	typedef vgKernel::SharePtr<String>	ShareShringPtr;
	std::vector<ShareShringPtr>	m_strFieldPtrArray;

	//String	m_strConstShapeCount;
	std::vector<PropertiesParam> _params;
};
