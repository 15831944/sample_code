#include <vgStableHeaders.h>
#include "vgentry/vgCameraRecordEntry.h"
#include <vgCam/vgcaViewCam.h>
#include <vgKernel/vgkSelectManager.h>


#include <vgUIController/vgUIController.h>
#include <vgCam/vgcaCameraRecord.h>

PropertiesParam vgCameraRecordEntry::s_ParamArray[s_NumOfParam];



void vgCameraRecordEntry::AddNodeTabs()
{

	using namespace vgCam;

	CameraRecord* rec = ( CameraRecord* )getRender();
	vgKernel::Vec3 vec = rec->offset_vcr;
	m_offX = vec.x;
	m_offY = vec.y;
	m_offZ = vec.z;


	vgUI::UIController::getSingleton().RemoveAllPages();

	vgPropertiesViewBar* pageViewBar = vgUI::UIController::getSingleton().GetPropertiesViewBar();


#if VGK_LANG_CHS


	s_ParamArray[0].label = "��������";
	s_ParamArray[0].typeId = PROP_ITEM_GROUP;
	s_ParamArray[0].dataType = PROP_DATA_NONE;
	s_ParamArray[0].connectedPtr = NULL;
	s_ParamArray[0].comment = "����VCR����";

	vgCam::CameraRecord *pViewCam = ( vgCam::CameraRecord*) m_Renderer;
	s_ParamArray[1].label = "��������";
	s_ParamArray[1].typeId = PROP_ITEM_DATA;
	s_ParamArray[1].dataType = PROP_DATA_STR;
	s_ParamArray[1].connectedPtr = m_Renderer->getNamePtr();
	s_ParamArray[1].comment = "���������";



	// ����ֵ
	s_ParamArray[2].label = "λ��ƫ��";
	s_ParamArray[2].typeId = PROP_ITEM_GROUP;
	s_ParamArray[2].dataType = PROP_DATA_NONE;
	s_ParamArray[2].connectedPtr = NULL;
	s_ParamArray[2].comment = "����VCR��ƫ��";

	s_ParamArray[3].label = "X ƫ��";
	s_ParamArray[3].typeId = PROP_ITEM_DATA;
	s_ParamArray[3].dataType = PROP_DATA_FLOAT;
	s_ParamArray[3].connectedPtr = &m_offX;
	s_ParamArray[3].comment = "����Xƫ��";

	s_ParamArray[4].label = "Y ƫ��";
	s_ParamArray[4].typeId = PROP_ITEM_DATA;
	s_ParamArray[4].dataType = PROP_DATA_FLOAT;
	s_ParamArray[4].connectedPtr = &m_offY;
	s_ParamArray[4].comment = "����Yƫ��";

	s_ParamArray[5].label = "Z ƫ��";
	s_ParamArray[5].typeId = PROP_ITEM_DATA;
	s_ParamArray[5].dataType = PROP_DATA_FLOAT;
	s_ParamArray[5].connectedPtr = &m_offZ;
	s_ParamArray[5].comment = "����Zƫ��";

	vgPropertyPage* propPage = vgUI::UIController::getSingleton().GetPropPage();
	propPage->Create(NIDD_PROPERTY, pageViewBar->GetTabControl());

	propPage->ConnectNode(this, s_ParamArray, s_NumOfParam);
	pageViewBar->AddTab("�Զ�����", propPage);


#else

	s_ParamArray[0].label = "Config";
	s_ParamArray[0].typeId = PROP_ITEM_GROUP;
	s_ParamArray[0].dataType = PROP_DATA_NONE;
	s_ParamArray[0].connectedPtr = NULL;
	s_ParamArray[0].comment = "";

	vgCam::CameraRecord *pViewCam = ( vgCam::CameraRecord*) m_Renderer;
	s_ParamArray[1].label = "name";
	s_ParamArray[1].typeId = PROP_ITEM_DATA;
	s_ParamArray[1].dataType = PROP_DATA_STR;
	s_ParamArray[1].connectedPtr = m_Renderer->getNamePtr();
	s_ParamArray[1].comment = "";



	// ����ֵ
	s_ParamArray[2].label = "Offset";
	s_ParamArray[2].typeId = PROP_ITEM_GROUP;
	s_ParamArray[2].dataType = PROP_DATA_NONE;
	s_ParamArray[2].connectedPtr = NULL;
	s_ParamArray[2].comment = "";

	s_ParamArray[3].label = "X";
	s_ParamArray[3].typeId = PROP_ITEM_DATA;
	s_ParamArray[3].dataType = PROP_DATA_FLOAT;
	s_ParamArray[3].connectedPtr = &m_offX;
	s_ParamArray[3].comment = "";

	s_ParamArray[4].label = "Y";
	s_ParamArray[4].typeId = PROP_ITEM_DATA;
	s_ParamArray[4].dataType = PROP_DATA_FLOAT;
	s_ParamArray[4].connectedPtr = &m_offY;
	s_ParamArray[4].comment = "";

	s_ParamArray[5].label = "Z";
	s_ParamArray[5].typeId = PROP_ITEM_DATA;
	s_ParamArray[5].dataType = PROP_DATA_FLOAT;
	s_ParamArray[5].connectedPtr = &m_offZ;
	s_ParamArray[5].comment = "";

	vgPropertyPage* propPage = vgUI::UIController::getSingleton().GetPropPage();
	propPage->Create(NIDD_PROPERTY, pageViewBar->GetTabControl());

	propPage->ConnectNode(this, s_ParamArray, s_NumOfParam);
	pageViewBar->AddTab("Auto Prop", propPage);


#endif
	//	vgKernel::SelectManager::getSingleton().SetSelection(m_meshId);

}

void vgCameraRecordEntry::OnPropertyChanged(string paramName)
{
	// ����TREEITEM

	//vgCam::CamManager::getSingleton().getCamRecordPtr()->offset_vcr=vgKernel::Vec3(m_offX,m_offY,m_offZ);
	// 	vgCam::CamManager::getSingleton().getCamRecordPtr()->translate(m_offX,m_offY,m_offZ);
	// 
	// 	vgUI::UIController::getSingleton().GetWorkSpaceBar()->SetItemText(hTreeItem, 
	// 		GetName() );
	vgUI::UIController::getSingleton().GetWorkSpaceBar()->SetItemText(hTreeItem, GetName() );

	vgCam::CameraRecord *pViewCam = ( vgCam::CameraRecord*) m_Renderer;
	pViewCam->translate(m_offX,m_offY,m_offZ);
}

void vgCameraRecordEntry::onChanged(int eventId, void* param)
{
	if (eventId == vgKernel::VG_OBS_PROPCHANGED)
	{
		using namespace vgCam;
		CameraRecord* rec = ( CameraRecord* )getRender();
		vgKernel::Vec3 vec = rec->offset_vcr;
		m_offX = vec.x;
		m_offY = vec.y;
		m_offZ = vec.z;

		////TRACE("New Camera Position %.2f %.2f %.2f \n", posPtr->x, posPtr->y, posPtr->z);
		if (this == vgUI::UIController::getSingleton().GetCurrentSelectedNode())
		{
			s_ParamArray[3].pProp->SetValue(m_offX);
			s_ParamArray[4].pProp->SetValue(m_offY);
			s_ParamArray[5].pProp->SetValue(m_offZ);

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
}



CMenu* vgCameraRecordEntry::GetContextMenu()
{
	CMenu *menu = new CMenu;

	VERIFY(menu->CreatePopupMenu());
	//  [11/4/2008 zhu]


#if VGK_LANG_CHS
	VERIFY(menu->AppendMenu(MF_STRING, NID_VCR_PLAY, _T("����")));

	//VERIFY(menu->AppendMenu(MF_STRING, NID_VCR_STOP, _T("ֹͣ")));

	VERIFY(menu->AppendMenu(MF_STRING, NID_VCR_GOON, _T("��ͣ/����")));
#else
	VERIFY(menu->AppendMenu(MF_STRING, NID_VCR_PLAY, _T("Play")));

	//VERIFY(menu->AppendMenu(MF_STRING, NID_VCR_STOP, _T("ֹͣ")));

	VERIFY(menu->AppendMenu(MF_STRING, NID_VCR_GOON, _T("Pause/continue")));

#endif

	//VERIFY(menu->AppendMenu(MF_STRING, NID_VCR_SHOW, _T("��ʾ/����")));

	//VERIFY(menu->AppendMenu(MF_STRING, NID_MESH_DELETE,_T("ɾ��")));

	return menu;
}

vgCameraRecordEntry::vgCameraRecordEntry( vgCam::CameraRecord* vcrpar ) : vgBaseEntry( vcrpar )
{
	assert( vcrpar != NULL );
	vgKernel::Vec3 vec = vcrpar->offset_vcr;
	m_offX = vec.x;
	m_offY = vec.y;
	m_offZ = vec.z;
}
