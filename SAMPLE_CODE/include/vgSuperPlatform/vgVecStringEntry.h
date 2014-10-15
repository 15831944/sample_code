#pragma once

#include "vgBaseEntry.h"
#include <vgKernel/vgkUniqueID.h>
#include <vgGIS3D/vgVertexString.h >
#include <vgKernel/vgkRendererObserver.h>
#include "vgPropertyPage.h"



class vgVecStringEntry : public vgBaseEntry
{
public:
	const static int s_NumOfParam = 3;
	static PropertiesParam s_ParamArray[s_NumOfParam];

public:
	vgVecStringEntry(vgGIS3D::VertexStringClass *vsc);


	virtual ~vgVecStringEntry(void);


	void AddNodeTabs();
	virtual String GetName()
	{
		return "ʸ��";
	}


	int GetSelectedImage() { return 1; }

	int GetUnSelectedImage() { return 2; }

	// �̳���vgBasedNode,��vgUIController���� �޸�Node����������Object ��TreeItem
	void OnPropertyChanged(string paramName);// { //TRACE("Camera Property Changed \n");}

	// �̳���Changed������Object�޸ĺ����Node
	void onChanged(int eventId, void* param);

	void UpDataProp() {}

	CMenu* GetContextMenu()
	{
		return NULL;
	}

private:
	// group�б�
	//vector<vgMeshEntry*> s_mesh_item_list;

	//string  s_group_item_name;
	vgGIS3D::VertexStringClass *m_VecStringClass;

	String m_sCaption;

	BOOL m_visible;
};

