#include <vgStableHeaders.h>
#include "vgentry/vgSound3DEntry.h"
#include <vgUIController/vgPropertyPage.h>
#include <vgUIController/vgUIController.h>

#include <vgKernel/vgkVec3.h>
#include <vgMesh/vgmMeshManager.h>
//#include <vgMath/vgfVector3.h>
#include <vgKernel/vgkSelectManager.h>


PropertiesParam vgSound3DEntry::s_ParamArray[s_NumOfParam];

vgSound3DEntry::vgSound3DEntry(vgSound::Sound3D* renderer) 
	:vgBaseEntry(  renderer )
{

	
	sound= (vgSound::Sound3D *)renderer;

	if (sound)
	{
		m_sCaption = sound->getName();

		b_play = sound->getPlayFlag();

		x = sound->getSoundPos().x + vgKernel::CoordSystem::getSingleton().getProjectionCoord().x;
		y = sound->getSoundPos().y + vgKernel::CoordSystem::getSingleton().getProjectionCoord().y;
		z = sound->getSoundPos().z + vgKernel::CoordSystem::getSingleton().getProjectionCoord().z;
		zMinus = -z;
		sound->registerObserver( this );

		vgKernel::CoordSystem::getSingleton().registerObserver(this);

	}

}
vgSound3DEntry::~vgSound3DEntry(void)
{
	sound->unregisterObserver( this );
	sound = NULL;
}

void vgSound3DEntry::OnPropertyChanged(string paramName)
{
	z = - zMinus;
	vgSound::Sound3D *sound = (vgSound::Sound3D *)m_Renderer;

	vgKernel::Vec3 aa = vgKernel::CoordSystem::getSingleton().getProjectionCoord();

	sound->setAbsolutePos( x - aa.x, y - aa.y ,z - aa.z);

/*	sound->GenBoundaryBox(sound->getSoundPos());*/

	sound->setPlayFlag( b_play );

	if (b_play)
	{
		sound->startPlaying( true );
	}
	else
		sound->stopPlaying();

	// ����TREEITEM
	vgUI::UIController::getSingleton().GetWorkSpaceBar()->SetItemText(hTreeItem, m_Renderer->getName());

	vgKernel::SelectManager::getSingleton().updateBox();

}

void vgSound3DEntry::onChanged(int eventId, void *param)
{
	if (eventId == vgKernel::VG_OBS_PROPCHANGED)
	{
		vgSound::Sound3D *sound = (vgSound::Sound3D *)m_Renderer;


		vgKernel::Vec3 xx = sound->getSoundPos();

		x = xx.x + vgKernel::CoordSystem::getSingleton().getProjectionCoord().x;
		y = xx.y + vgKernel::CoordSystem::getSingleton().getProjectionCoord().y;
		z = xx.z + vgKernel::CoordSystem::getSingleton().getProjectionCoord().z;


		//TRACE("New Camera Position %.2f %.2f %.2f \n", posPtr->x, posPtr->y, posPtr->z);
		if (this == vgUI::UIController::getSingleton().GetCurrentSelectedNode())
		{
			s_ParamArray[1].pProp->SetValue(x);
			s_ParamArray[2].pProp->SetValue(y);
			zMinus = -z;
			s_ParamArray[3].pProp->SetValue(zMinus);
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

void vgSound3DEntry::AddNodeTabs()
{
	vgUI::UIController::getSingleton().RemoveAllPages();

	vgPropertiesViewBar* pageViewBar = vgUI::UIController::getSingleton().GetPropertiesViewBar();

	s_ParamArray[0].label = "����ֵ����";
	s_ParamArray[0].typeId = PROP_ITEM_GROUP;
	s_ParamArray[0].dataType = PROP_DATA_NONE;
	s_ParamArray[0].connectedPtr = NULL;
	s_ParamArray[0].comment = "�������������";

	s_ParamArray[1].label = "X ����";
	s_ParamArray[1].typeId = PROP_ITEM_DATA;
	s_ParamArray[1].dataType = PROP_DATA_FLOAT;
	s_ParamArray[1].connectedPtr = &x;
	s_ParamArray[1].comment = "����X����";


	s_ParamArray[2].label = "Y ����";
	s_ParamArray[2].typeId = PROP_ITEM_DATA;
	s_ParamArray[2].dataType = PROP_DATA_FLOAT;
	s_ParamArray[2].connectedPtr = &y;
	s_ParamArray[2].comment = "����Y����";

	s_ParamArray[3].label = "Z ����";
	s_ParamArray[3].typeId = PROP_ITEM_DATA;
	s_ParamArray[3].dataType = PROP_DATA_FLOAT;
	s_ParamArray[3].connectedPtr = &zMinus;
	s_ParamArray[3].comment = "����Z����";

	s_ParamArray[4].label = "��������";
	s_ParamArray[4].typeId = PROP_ITEM_GROUP;
	s_ParamArray[4].dataType = PROP_DATA_NONE;
	s_ParamArray[4].connectedPtr = NULL;
	s_ParamArray[4].comment = string();


	s_ParamArray[5].label = "��Ч����";
	s_ParamArray[5].typeId = PROP_ITEM_DATA;
	s_ParamArray[5].dataType = PROP_DATA_STR;
	s_ParamArray[5].connectedPtr = m_Renderer->getNamePtr();
	s_ParamArray[5].comment = "���������";

	s_ParamArray[6].label = "�Ƿ񲥷�";
	s_ParamArray[6].typeId = PROP_ITEM_DATA;
	s_ParamArray[6].dataType = PROP_DATA_BOOL;
	s_ParamArray[6].connectedPtr = &b_play;
	s_ParamArray[6].comment = "�Ƿ񲥷�";

	vgPropertyPage* propPage = vgUI::UIController::getSingleton().GetPropPage();
	propPage->Create(NIDD_PROPERTY, pageViewBar->GetTabControl());

	propPage->ConnectNode(this, s_ParamArray, s_NumOfParam);
	pageViewBar->AddTab("�Զ�����", propPage);
}


CMenu* vgSound3DEntry::GetContextMenu()
{
	CMenu *menu = new CMenu;

	VERIFY(menu->CreatePopupMenu());
//	VERIFY(menu->AppendMenu(MF_STRING, NID_MESH_GOTO, _T("ת��")));
	VERIFY(menu->AppendMenu(MF_STRING, NID_MESH_DELETE,_T("ɾ��")));

	return menu;
}
