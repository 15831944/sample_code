
#include <vgStableHeaders.h>
#include "vgplan/vgBlockEntry.h"
#include <vgKernel/vgkSelectManager.h>
#include <vgUIController/vgPropertyPage.h>
#include <vgUIController/vgUIController.h>
#include <vgKernel/vgkVec3.h>

vgBlockEntry::vgBlockEntry(  vgPlan::vgPlanbox* pObject ) : vgGroupBasedEntry( pObject )
{
	assert( pObject != NULL );

	vgKernel::Vec3 pos = pObject->getPosition();
	
	// TRACE("%d Auto Build Node pos: %f, %f, %f \n", this, pos.x, pos.y, pos.z);

	m_posX = pos.x + vgKernel::CoordSystem::getSingleton().getProjectionCoord().x;
	m_posY = pos.y + vgKernel::CoordSystem::getSingleton().getProjectionCoord().y;
	m_posZ = pos.z + vgKernel::CoordSystem::getSingleton().getProjectionCoord().z;
	m_posZMinus = - m_posZ;

	// m_size = pObject->getSize();
	m_layer = pObject->getLayerNum();
	m_length = pObject->getLength();
	m_width = pObject->getWidth();
	m_heightPerLayer = pObject->getHeightPerLayer();
	m_angle = pObject->getAngle();

	vgKernel::CoordSystem::getSingleton().registerObserver(this);

	// TRACE("POS in Constructor : %f, %f, %f \n", m_posX, m_posY, m_posZ);

}

void vgBlockEntry::OnSinglePropertyChanged(string paramName)
{
/*	vgGroupBasedEntry::OnSinglePropertyChanged(paramName);*/

	vgPlan::vgPlanbox *pObject = ( vgPlan::vgPlanbox*) m_Renderer;

	if (paramName == "��鳤")
	{
		pObject->setLength(m_length);
		pObject->attachToTerrain();

		return ;
	}

	if (paramName == "����")
	{
		pObject->setWidth(m_width);
		pObject->attachToTerrain();

		return ;
	}

	if (paramName == "����߶�")
	{
		pObject->setHeightPerLayer(m_heightPerLayer);
		pObject->attachToTerrain();

		return ;
	}

	if (paramName == "������")
	{
		pObject->setLayerNum(m_layer);
		pObject->attachToTerrain();

		return ;
	}

	if (paramName == "��ת�Ƕ�")
	{
		pObject->setAngle(m_angle);

		return;
	}

	m_posZ = - m_posZMinus;
	vgKernel::Vec3 offset = vgKernel::Vec3( m_posX, m_posY, m_posZ)
		- pObject->getPosition() - vgKernel::CoordSystem::getSingleton().getProjectionCoord();
	pObject->translate( offset.x, offset.y, offset.z );

	// ����TREEITEM
	// vgUI::vgUIController::GetInstance()->GetWorkSpaceBar()->SetItemText(hTreeItem, m_Renderer->getName());
	// pObject->setName( (TCHAR*)m_Renderer->getName().c_str() );

	vgKernel::SelectManager::getSingleton().updateBox();

}

void vgBlockEntry::onChanged(int eventId, void *param)
{

	if (eventId == vgKernel::VG_OBS_PROPCHANGED)
	{
		if (s_group_list.size() > 0)
		{
			return ;
		}

		vgPlan::vgPlanbox *pObject = ( vgPlan::vgPlanbox* ) m_Renderer;

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

void vgBlockEntry::AddSingleTabPage()
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
	param.connectedPtr = &m_posZMinus/*m_posZ*/;
	param.comment = "����Z����";
	s_ParamArray.push_back(param);

	param.label = "��������";
	param.typeId = PROP_ITEM_GROUP;
	param.dataType = PROP_DATA_NONE;
	param.connectedPtr = NULL;
	param.comment = string();
	s_ParamArray.push_back(param);

	param.label = "��������";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_STR;
	param.connectedPtr = m_Renderer->getNamePtr();
	param.comment = "���������";
	s_ParamArray.push_back(param);

	param.label = "��鳤";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_FLOAT;
	param.connectedPtr = &m_length;
	param.comment = "������鳤��(�������򳤶�)";
	s_ParamArray.push_back(param);

	param.label = "����";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_FLOAT;
	param.connectedPtr = &m_width;
	param.comment = "���������(�ϱ����򳤶�)";
	s_ParamArray.push_back(param);

	param.label = "������";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_INT;
	param.connectedPtr = &m_layer;
	param.comment = "����������";
	s_ParamArray.push_back(param);

	param.label = "����߶�";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_FLOAT;
	param.connectedPtr = &m_heightPerLayer;
	param.comment = "������鵥��߶�";
	s_ParamArray.push_back(param);

	param.label = "��ת�Ƕ�";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_FLOAT;
	param.connectedPtr = &m_angle;
	param.comment = "���������ת�Ƕ�";
	s_ParamArray.push_back(param);

	vgPropertyPage* propPage = vgUI::UIController::getSingleton().GetPropPage();
	propPage->Create(NIDD_PROPERTY, pageViewBar->GetTabControl());

	propPage->ConnectNode(this, &s_ParamArray);
	pageViewBar->AddTab("AUTO Build", propPage);

	// vgGroupBasedEntry::AddSingleTabPage();

	TRACE("%d POS in AddSingleTabPage : %f, %f, %f \n", this, m_posX, m_posY, m_posZ);
	TRACE("PROP in AddSingleTabPage : %f, %f, %f \n", this, m_length, m_width, m_layer);
}

void vgBlockEntry::AddGroupTabPage()
{

}

void vgBlockEntry::OnGroupPropertyChanged(string paramName)
{
	//vgGroupBasedEntry::OnGroupPropertyChanged(paramName);
}


CMenu* vgBlockEntry::GetContextMenu()
{
	CMenu *menu = new CMenu;

	VERIFY(menu->CreatePopupMenu());
	//VERIFY(menu->AppendMenu(MF_STRING, NID_MESH_GOTO, _T("ת��")));
	//VERIFY(menu->AppendMenu(MF_STRING, NID_MESH_DELETE,_T("ɾ��")));

	return menu;
}

void vgBlockEntry::getData()
{
	vgPlan::vgPlanbox *pObject = ( vgPlan::vgPlanbox * ) m_Renderer;

	vgKernel::Vec3 pos = pObject->getPosition();

	m_posX = pos.x + vgKernel::CoordSystem::getSingleton().getProjectionCoord().x;
	m_posY = pos.y + vgKernel::CoordSystem::getSingleton().getProjectionCoord().y;
	m_posZ = pos.z + vgKernel::CoordSystem::getSingleton().getProjectionCoord().z;
	m_posZMinus = - m_posZ;
}