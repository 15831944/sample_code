


#include <vgStableHeaders.h>
#include "vgMs3dEntry.h"
#include <vgKernel/vgkSelectManager.h>

#include <vgKernel/vgkRendererManager.h>

vgMs3dEntry::vgMs3dEntry(vgMs3d::CMs3d* pObject)
: vgBaseEntry( VG_ENTRY_MS3D , pObject )
{
	assert( pObject != NULL );
	if (pObject)
	{
		m_Positionx = pObject->getPosition().x + vgCore::vgCoordinate::getSingleton().GetSceneCood().x;
		m_Positiony = pObject->getPosition().y + vgCore::vgCoordinate::getSingleton().GetSceneCood().y;
		m_Positionz = pObject->getPosition().z + vgCore::vgCoordinate::getSingleton().GetSceneCood().z;
		//m_PositionzMinus = - m_Positionz;

		b_play = pObject->GetPlayState();
		m_startframe = pObject->GetStartFrame();
		m_endframe = pObject->GetEndFrame()	;
		b_loop = pObject->GetLoopState();


		m_pObject = pObject;

		m_sCaption = string( pObject->getName() );

		vgCore::vgCoordinate::getSingleton().registerObserver(this);

	}

}


//PropertiesParam vgMs3dEntry::s_ParamArray[s_NumOfParam];
vector<PropertiesParam> vgMs3dEntry::s_ParamArray;

void vgMs3dEntry::OnPropertyChanged(string paramName)
{
	//m_Positionz = - m_PositionzMinus;

	vgKernel::Vec3 temp=m_pObject->getPosition();
	Vector3 tmp=Vector3(temp.x,temp.y,temp.z);
	Vector3 offset = Vector3( m_Positionx, m_Positiony, m_Positionz)
		- tmp  - vgCore::vgCoordinate::getSingleton().GetSceneCood();


	m_pObject->SetPlayState( b_play );
	m_pObject->SetStartFrame(m_startframe);	
	m_pObject->SetEndFrame(m_endframe);
	m_pObject->SetLoopState(b_loop);		

	if(fabs(m_Positionx)>0.00001||fabs(m_Positiony)>0.00001||fabs(m_Positionz)>0.00001)
	{
		m_pObject->translate( offset );
	}

	vgUIController::GetInstance()->GetWorkSpaceBar()->SetItemText(hTreeItem, m_pObject->getName());

	vgKernel::SelectManager::getSingleton().updateBox();


	//m_pObject->SetPosition( Vector3(m_Positionx, m_Positiony, m_Positionz) );
}

void vgMs3dEntry::onChanged(int eventId, void *param)
{
	if (eventId == vgKernel::VG_OBS_PROPCHANGED)
	{
		vgKernel::Vec3 temp=m_pObject->getPosition();
		Vector3 pos=Vector3(temp.x,temp.y,temp.z);

		m_Positionx = pos.x + vgCore::vgCoordinate::getSingleton().GetSceneCood().x;
		m_Positiony = pos.y + vgCore::vgCoordinate::getSingleton().GetSceneCood().y;
		m_Positionz = pos.z + vgCore::vgCoordinate::getSingleton().GetSceneCood().z;

		m_startframe = m_pObject->GetStartFrame();
		m_endframe = m_pObject->GetEndFrame()	;

		//TRACE("New Camera Position %.2f %.2f %.2f \n", posPtr->x, posPtr->y, posPtr->z);
		if (this == vgUIController::GetInstance()->GetCurrentSelectedNode())
		{
			s_ParamArray[1].pProp->SetValue(m_Positionx);
			s_ParamArray[2].pProp->SetValue(m_Positiony);
			//m_PositionzMinus = - m_Positionz;
			s_ParamArray[3].pProp->SetValue(m_Positionz/*Minus*/);
			s_ParamArray[6].pProp->SetValue(m_startframe);
			s_ParamArray[7].pProp->SetValue(m_endframe);

		}
	}

	if (eventId == vgKernel::VG_OBS_SELECTCHAGNED)
	{
		vgUIController::GetInstance()->SelectNode(this);
	}

	if (eventId == vgKernel::VG_OBS_ADDSELECTION)
	{
		vgUIController::GetInstance()->AddSelection(this);

		return ;
	}

}
void vgMs3dEntry::AddNodeTabs()
{
	vgUIController::GetInstance()->RemoveAllPages();

	vgPropertiesViewBar* pageViewBar = vgUIController::GetInstance()->GetPropertiesViewBar();

	s_ParamArray.clear();
	PropertiesParam param;


#if	VGK_LANG_CHS	
	{
		param.label = "����ֵ����";
		param.typeId = PROP_ITEM_GROUP;
		param.dataType = PROP_DATA_NONE;
		param.connectedPtr = NULL;
		param.comment = "������������";
		s_ParamArray.push_back(param);

		param.label = "X ����";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_FLOAT;
		param.connectedPtr = &m_Positionx;
		param.comment = "����X����";
		s_ParamArray.push_back(param);


		param.label = "Y ����";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_FLOAT;
		param.connectedPtr = &m_Positiony;
		param.comment = "����Y����";
		s_ParamArray.push_back(param);

		param.label = "Z ����";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_FLOAT;
		param.connectedPtr = &m_PositionzMinus;
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

		param.label = "��ʼ֡";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_FLOAT;
		param.connectedPtr = &m_startframe;
		param.comment = "��ʼ֡";
		s_ParamArray.push_back(param);

		param.label = "����֡";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_FLOAT;
		param.connectedPtr = &m_endframe;
		param.comment = "����֡";
		s_ParamArray.push_back(param);


		param.label = "����";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_BOOL;
		param.connectedPtr = &b_play;
		param.comment = "����";
		s_ParamArray.push_back(param);

		param.label = "ѭ��";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_BOOL;
		param.connectedPtr = &b_loop;
		param.comment = "ѭ��";
		s_ParamArray.push_back(param);
	}
#else//VGK_LANG_ENG
	{
		param.label = "3D Coordinate";
		param.typeId = PROP_ITEM_GROUP;
		param.dataType = PROP_DATA_NONE;
		param.connectedPtr = NULL;
		param.comment = "Set Position Of Object";
		s_ParamArray.push_back(param);

		param.label = "X Coordinate";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_FLOAT;
		param.connectedPtr = &m_Positionx;
		param.comment = "Set X Coordinate";
		s_ParamArray.push_back(param);


		param.label = "Y Coordinate";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_FLOAT;
		param.connectedPtr = &m_Positiony;
		param.comment = "Set Y Coordinate";
		s_ParamArray.push_back(param);

		param.label = "Z Coordinate";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_FLOAT;
		param.connectedPtr = &m_PositionzMinus;
		param.comment = "Set Z Coordinate";
		s_ParamArray.push_back(param);

		param.label = "More Settings";
		param.typeId = PROP_ITEM_GROUP;
		param.dataType = PROP_DATA_NONE;
		param.connectedPtr = NULL;
		param.comment = string();
		s_ParamArray.push_back(param);

		param.label = "Name";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_STR;
		param.connectedPtr = m_Renderer->getNamePtr();
		param.comment = "Name Of Object";
		s_ParamArray.push_back(param);

		param.label = "Start Frame";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_INT;
		param.connectedPtr = &m_startframe;
		param.comment = "Start Frame";
		s_ParamArray.push_back(param);

		param.label = "End Frame";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_INT;
		param.connectedPtr = &m_endframe;
		param.comment = "End Frame";
		s_ParamArray.push_back(param);


		param.label = "Play";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_BOOL;
		param.connectedPtr = &b_play;
		param.comment = "Play";
		s_ParamArray.push_back(param);

		param.label = "Loop";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_BOOL;
		param.connectedPtr = &b_loop;
		param.comment = "Loop";
		s_ParamArray.push_back(param);

	}
#endif


	vgPropertyPage* propPage = vgUIController::GetInstance()->GetPropPage();
	propPage->Create(NIDD_PROPERTY, pageViewBar->GetTabControl());

	//propPage->ConnectNode(this, s_ParamArray, s_NumOfParam);
	propPage->ConnectNode(this, &s_ParamArray);

	pageViewBar->AddTab("�Զ�����", propPage);
}


CMenu* vgMs3dEntry::GetContextMenu()
{
	CMenu *menu = new CMenu;

	VERIFY(menu->CreatePopupMenu());
	VERIFY(menu->AppendMenu(MF_STRING, NID_MESH_GOTO, _T("ת��")));
	VERIFY(menu->AppendMenu(MF_STRING, NID_MESH_DELETE,_T("ɾ��")));

	return menu;
}