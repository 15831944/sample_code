
#include <vgStableHeaders.h>
#include "gismatchentry.h"
#include <vgKernel/vgkSelectManager.h>

#include <vgUIController/vgUIController.h>

/**
@date 	 9/23/2008 9:20
@author  YX

@brief 	

@see    
*/


namespace vgCore{

	PropertiesParam vgGisMatchEntry::s_ParamArray[s_NumOfParam];


	void vgGisMatchEntry::OnPropertyChanged(string paramName) 
	{
		vgUI::UIController::getSingleton().GetWorkSpaceBar()->SetItemText(hTreeItem, 
			GetName() );
		m_posZ = - m_posZMinus;

		vgMod::GisRoadMatch* pObject = ( vgMod::GisRoadMatch*) m_Renderer;
		
 		Vector3 offset = Vector3( m_posX, m_posY, m_posZ)
 			- pObject->GetPosition() - vgKernel::CoordSystem::getSingleton().getProjectionCoord();
 		pObject->translate( offset.x, offset.y, offset.z );
		
		pObject->SetHeight(m_height);

//----------------------------------------------------------
		if ((m_width!= pObject->m_width)||(m_density!= pObject->m_texdensity)
								||(m_luya!= pObject->m_luya))
		{	
			pObject->SetWidth(m_width,pObject->GetPosition());

			pObject->SetDensity(m_density);

			pObject->SetLuyaHeight(m_luya);

			pObject->RenderPoints.clear();
			pObject->LuyaPoints1.clear();
			pObject->LuyaPoints2.clear();
			pObject->uv_u1.clear();
			pObject->uv_u2.clear();
		
			if (pObject->matchpath.size()>= 2)
			{
				pObject->ReGenRoadWithPoints();
			}
		}
		vgKernel::SelectManager::getSingleton().updateBox(); 
	}

	void vgGisMatchEntry::onChanged(int eventId, void *param)
	{
		if (eventId == vgKernel::VG_OBS_PROPCHANGED)
		{
			vgMod::GisRoadMatch *pObject = ( vgMod::GisRoadMatch*) m_Renderer;

			Vector3 pos = pObject->GetPosition();

			m_posX = pos.x + vgKernel::CoordSystem::getSingleton().getProjectionCoord().x;
			m_posY = pos.y + vgKernel::CoordSystem::getSingleton().getProjectionCoord().y;
			m_posZ = pos.z + vgKernel::CoordSystem::getSingleton().getProjectionCoord().z;

// 			m_height = pObject->GetHeight();
// 			m_width  = pObject->GetWidth();
// 			m_luya   = pObject->GetLuyaHeight();

			if (this == vgUI::UIController::getSingleton().GetCurrentSelectedNode())
			{	

				s_ParamArray[1].pProp->SetValue(m_posX);
				s_ParamArray[2].pProp->SetValue(m_posY);
				m_posZMinus = - m_posZ;
				s_ParamArray[3].pProp->SetValue(m_posZMinus/*m_posZ*/);
				
				s_ParamArray[6].pProp->SetValue(m_height);
 				s_ParamArray[7].pProp->SetValue(m_width);
				s_ParamArray[8].pProp->SetValue(m_density);
 				s_ParamArray[9].pProp->SetValue(m_luya);
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

	void vgGisMatchEntry::AddNodeTabs()
	{
		vgUI::UIController::getSingleton().RemoveAllPages();

		vgPropertiesViewBar* pageViewBar = vgUI::UIController::getSingleton().GetPropertiesViewBar();

#if VGK_LANG_CHS

		s_ParamArray[0].label = "ƥ���·����ֵ����";
		s_ParamArray[0].typeId = PROP_ITEM_GROUP;
		s_ParamArray[0].dataType = PROP_DATA_NONE;
		s_ParamArray[0].connectedPtr = NULL;
		s_ParamArray[0].comment = "�������������";

		s_ParamArray[1].label = "X ����";
		s_ParamArray[1].typeId = PROP_ITEM_DATA;
		s_ParamArray[1].dataType = PROP_DATA_FLOAT;
		s_ParamArray[1].connectedPtr = &m_posX;
		s_ParamArray[1].comment = "����X����";


		s_ParamArray[2].label = "Y ����";
		s_ParamArray[2].typeId = PROP_ITEM_DATA;
		s_ParamArray[2].dataType = PROP_DATA_FLOAT;
		s_ParamArray[2].connectedPtr = &m_posY;
		s_ParamArray[2].comment = "����Y����";

		s_ParamArray[3].label = "Z ����";
		s_ParamArray[3].typeId = PROP_ITEM_DATA;
		s_ParamArray[3].dataType = PROP_DATA_FLOAT;
		s_ParamArray[3].connectedPtr = &m_posZMinus/*m_posZ*/;
		s_ParamArray[3].comment = "����Z����";

		s_ParamArray[4].label = "��������";
		s_ParamArray[4].typeId = PROP_ITEM_GROUP;
		s_ParamArray[4].dataType = PROP_DATA_NONE;
		s_ParamArray[4].connectedPtr = NULL;
		s_ParamArray[4].comment = string();

		s_ParamArray[5].label = "��·����";
		s_ParamArray[5].typeId = PROP_ITEM_DATA;
		s_ParamArray[5].dataType = PROP_DATA_STR;
		s_ParamArray[5].connectedPtr = m_Renderer->getNamePtr();;
		s_ParamArray[5].comment = "��·������";

		s_ParamArray[6].label = "��·��Ե���߶�";
		s_ParamArray[6].typeId = PROP_ITEM_DATA;
		s_ParamArray[6].dataType = PROP_DATA_FLOAT;
		s_ParamArray[6].connectedPtr = &m_height;
		s_ParamArray[6].comment = "������Ը߶�";

		s_ParamArray[7].label = "��·���";
		s_ParamArray[7].typeId = PROP_ITEM_DATA;
		s_ParamArray[7].dataType = PROP_DATA_FLOAT;
		s_ParamArray[7].connectedPtr = &m_width;
		s_ParamArray[7].comment = "���õ�·�Ŀ��";

		s_ParamArray[8].label = "�����ܶ�";
		s_ParamArray[8].typeId = PROP_ITEM_DATA;
		s_ParamArray[8].dataType = PROP_DATA_FLOAT;
		s_ParamArray[8].connectedPtr = &m_density;
		s_ParamArray[8].comment = "����������ܶ�";

		s_ParamArray[9].label = "·Ѿ�߶�";
		s_ParamArray[9].typeId = PROP_ITEM_DATA;
		s_ParamArray[9].dataType = PROP_DATA_FLOAT;
		s_ParamArray[9].connectedPtr = &m_luya;
		s_ParamArray[9].comment = "����·Ѿ�ĸ߶�";




		vgPropertyPage* propPage = vgUI::UIController::getSingleton().GetPropPage();
		propPage->Create(NIDD_PROPERTY, pageViewBar->GetTabControl());

		propPage->ConnectNode(this, s_ParamArray, s_NumOfParam);
		pageViewBar->AddTab("�Զ�����", propPage);

#else

		s_ParamArray[0].label = "Coord";
		s_ParamArray[0].typeId = PROP_ITEM_GROUP;
		s_ParamArray[0].dataType = PROP_DATA_NONE;
		s_ParamArray[0].connectedPtr = NULL;
		s_ParamArray[0].comment = "";

		s_ParamArray[1].label = "X";
		s_ParamArray[1].typeId = PROP_ITEM_DATA;
		s_ParamArray[1].dataType = PROP_DATA_FLOAT;
		s_ParamArray[1].connectedPtr = &m_posX;
		s_ParamArray[1].comment = "";


		s_ParamArray[2].label = "Y";
		s_ParamArray[2].typeId = PROP_ITEM_DATA;
		s_ParamArray[2].dataType = PROP_DATA_FLOAT;
		s_ParamArray[2].connectedPtr = &m_posY;
		s_ParamArray[2].comment = "";

		s_ParamArray[3].label = "Z";
		s_ParamArray[3].typeId = PROP_ITEM_DATA;
		s_ParamArray[3].dataType = PROP_DATA_FLOAT;
		s_ParamArray[3].connectedPtr = &m_posZMinus/*m_posZ*/;
		s_ParamArray[3].comment = "";

		s_ParamArray[4].label = "Others";
		s_ParamArray[4].typeId = PROP_ITEM_GROUP;
		s_ParamArray[4].dataType = PROP_DATA_NONE;
		s_ParamArray[4].connectedPtr = NULL;
		s_ParamArray[4].comment = string();

		s_ParamArray[5].label = "Name";
		s_ParamArray[5].typeId = PROP_ITEM_DATA;
		s_ParamArray[5].dataType = PROP_DATA_STR;
		s_ParamArray[5].connectedPtr = m_Renderer->getNamePtr();;
		s_ParamArray[5].comment = "";

		s_ParamArray[6].label = "Height";
		s_ParamArray[6].typeId = PROP_ITEM_DATA;
		s_ParamArray[6].dataType = PROP_DATA_FLOAT;
		s_ParamArray[6].connectedPtr = &m_height;
		s_ParamArray[6].comment = "";

		s_ParamArray[7].label = "Road width";
		s_ParamArray[7].typeId = PROP_ITEM_DATA;
		s_ParamArray[7].dataType = PROP_DATA_FLOAT;
		s_ParamArray[7].connectedPtr = &m_width;
		s_ParamArray[7].comment = "";

		s_ParamArray[8].label = "Texture Density";
		s_ParamArray[8].typeId = PROP_ITEM_DATA;
		s_ParamArray[8].dataType = PROP_DATA_FLOAT;
		s_ParamArray[8].connectedPtr = &m_density;
		s_ParamArray[8].comment = "";

		s_ParamArray[9].label = "Curb height";
		s_ParamArray[9].typeId = PROP_ITEM_DATA;
		s_ParamArray[9].dataType = PROP_DATA_FLOAT;
		s_ParamArray[9].connectedPtr = &m_luya;
		s_ParamArray[9].comment = "";




		vgPropertyPage* propPage = vgUI::UIController::getSingleton().GetPropPage();
		propPage->Create(NIDD_PROPERTY, pageViewBar->GetTabControl());

		propPage->ConnectNode(this, s_ParamArray, s_NumOfParam);
		pageViewBar->AddTab("Auto Prop", propPage);
#endif
	}



	CMenu* vgGisMatchEntry::GetContextMenu()
	{
		CMenu *menu = new CMenu;

		VERIFY(menu->CreatePopupMenu());
		//VERIFY(menu->AppendMenu(MF_STRING, NID_MESH_GOTO, _T("ת��")));
		//VERIFY(menu->AppendMenu(MF_STRING, NID_MESH_DELETE,_T("ɾ��")));

		return menu;
	}

	vgGisMatchEntry::vgGisMatchEntry( vgMod::GisRoadMatch* gisMatch /*= NULL*/ ) : vgBaseEntry( VG_ENTRY_GISMATCH , gisMatch )
	{
		assert( gisMatch != NULL );

		Vector3 pos = gisMatch->GetPosition();
		m_posX = pos.x + vgKernel::CoordSystem::getSingleton().getProjectionCoord().x;
		m_posY = pos.y + vgKernel::CoordSystem::getSingleton().getProjectionCoord().y;
		m_posZ = pos.z + vgKernel::CoordSystem::getSingleton().getProjectionCoord().z;
		m_posZMinus = - m_posZ;

		m_height = gisMatch->GetHeight();
		m_density = gisMatch->GetDensity();
		m_luya = gisMatch->GetLuyaHeight();
		m_width = gisMatch->GetWidth();

		vgKernel::CoordSystem::getSingleton().registerObserver(this);
	}

}//namespace vgCore