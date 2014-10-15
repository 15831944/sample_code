#include <vgStableHeaders.h>
#include <vgGIS3D/vgPipeLineEntry.h>
#include <vgUIController/vgUIController.h>
#include <vgKernel/vgkSelectManager.h>
#include <vgUIController/vgPropertyPage.h>
#include <vgKernel/vgkVec3.h>


void vgPipeLineEntry::AddSingleTabPage()
{
	TRACE("1: %d POS in AddSingleTabPage : %f, %f, %f \n", this, m_posX, m_posY, m_posZ);

	getData();
	
	vgUI::UIController::getSingleton().RemoveAllPages();

	vgPropertiesViewBar* pageViewBar = vgUI::UIController::getSingleton().GetPropertiesViewBar();

	s_ParamArray.clear();
	PropertiesParam param;

	param.label = "����ֵ����";
	param.typeId = PROP_ITEM_GROUP;
	param.dataType = PROP_DATA_NONE;
	param.connectedPtr = NULL;
	param.comment = "�������������";
	s_ParamArray.push_back(param);

	param.label = "X ����";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_FLOAT;
	param.connectedPtr = &m_posX;
	param.comment = "����X����";
	s_ParamArray.push_back(param);

	param.label = "Y ����";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_FLOAT;
	param.connectedPtr = &m_posY;
	param.comment = "����Y����";
	s_ParamArray.push_back(param);

	param.label = "Z ����";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_FLOAT;
	param.connectedPtr = &m_posZ/*m_posZ*/;
	param.comment = "����Z����";
	s_ParamArray.push_back(param);

	param.label = "��������";
	param.typeId = PROP_ITEM_GROUP;
	param.dataType = PROP_DATA_NONE;
	param.connectedPtr = NULL;
	param.comment = string();
	s_ParamArray.push_back(param);

	param.label = "����ͼ������";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_STR;
	param.connectedPtr = m_Renderer->getNamePtr();
	param.comment = "����ͼ������";
	s_ParamArray.push_back(param);


	vgPropertyPage* propPage = vgUI::UIController::getSingleton().GetPropPage();
	propPage->Create(NIDD_PROPERTY, pageViewBar->GetTabControl());

	propPage->ConnectNode(this, &s_ParamArray);
	pageViewBar->AddTab("GIS���߷���", propPage);

	// vgGroupBasedEntry::AddSingleTabPage();

	TRACE("%d POS in AddSingleTabPage : %f, %f, %f \n", this, m_posX, m_posY, m_posZ);
}

CMenu* vgPipeLineEntry::GetContextMenu()
{
	CMenu *menu = new CMenu;

	VERIFY(menu->CreatePopupMenu());
	VERIFY(menu->AppendMenu(MF_STRING, NID_ATTRIBUTE_LIST, _T("�����б�")));
	VERIFY(menu->AppendMenu(MF_STRING, NID_MESH_GOTO, _T("ת��")));
	VERIFY(menu->AppendMenu(MF_STRING, NID_MESH_DELETE,_T("ɾ��")));

	return menu;
}

void vgPipeLineEntry::OnSinglePropertyChanged(string paramName)
{
	/*	vgGroupBasedEntry::OnSinglePropertyChanged(paramName);*/

	vgGIS3D::OGRPipeLineMap *pPipeLine = (vgGIS3D::OGRPipeLineMap*) m_Renderer;


	m_posZ = - m_posZMinus;
	vgKernel::Vec3 offset = vgKernel::Vec3( m_posX, m_posY, m_posZ)
		- pPipeLine->getPosition() - vgKernel::CoordSystem::getSingleton().getProjectionCoord();
	
	pPipeLine->translate( offset.x, offset.y, offset.z );

	//pPipeLine->setTriggerEnable(m_enableTrigger);
	//pPipeLine->setInnerVisible(m_innerVisible);

	// ����TREEITEM
	vgUI::UIController::getSingleton().GetWorkSpaceBar()->SetItemText(hTreeItem, m_Renderer->getName());
	pPipeLine->setName( (TCHAR*)m_Renderer->getName().c_str() );

	vgKernel::SelectManager::getSingleton().updateBox();
}


void vgPipeLineEntry::onChanged(int eventId, void *param)
{

	if (eventId == vgKernel::VG_OBS_PROPCHANGED)
	{
		if (s_group_list.size() > 0)
		{
			return ;
		}

		vgGIS3D::OGRPipeLineMap *pObject = ( vgGIS3D::OGRPipeLineMap* ) m_Renderer;

		vgKernel::Vec3 pos = pObject->getPosition();

		m_posX = pos.x + vgKernel::CoordSystem::getSingleton().getProjectionCoord().x;
		m_posY = pos.y + vgKernel::CoordSystem::getSingleton().getProjectionCoord().y;
		m_posZ = pos.z + vgKernel::CoordSystem::getSingleton().getProjectionCoord().z;

		////TRACE("New Camera Position %.2f %.2f %.2f \n", posPtr->x, posPtr->y, posPtr->z);
		if (this == vgUI::UIController::getSingleton().GetCurrentSelectedNode())
		{
			s_ParamArray[1].pProp->SetValue(m_posX);
			s_ParamArray[2].pProp->SetValue(m_posY);
			m_posZMinus = - m_posZ;
			s_ParamArray[3].pProp->SetValue(m_posZMinus/*m_posZ*/);

			s_ParamArray[6].pProp->SetValue(m_enableTrigger);
			s_ParamArray[7].pProp->SetValue(m_innerVisible);
		}
	}

	if (eventId == vgKernel::VG_OBS_SELECTCHAGNED)
	{
		vgUI::UIController::getSingleton().SelectNode(this);
	}

	if (eventId == vgKernel::VG_OBS_ADDSELECTION)
	{
		vgUI::UIController::getSingleton().AddSelection(this);

		return ;
	}

	TRACE("%d POS in OnChange : %f, %f, %f \n", this, m_posX, m_posY, m_posZ);
}

void vgPipeLineEntry::showAttriList()
{
	vgGIS3D::OGRPipeLineMap *pPipeLine = (vgGIS3D::OGRPipeLineMap*) m_Renderer;
	pPipeLine->showPipeLayerAtrriList();
	return;
}