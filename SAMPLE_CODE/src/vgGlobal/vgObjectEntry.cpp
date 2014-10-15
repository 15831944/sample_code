
#include <vgStableHeaders.h>
#include "vgObjectEntry.h"
#include <vgKernel/vgkSelectManager.h>

#include <vgUIController/vgUIController.h>
#include <vgVisualEffects/vgviColorValue.h>

#include <vgKernel/vgkPluginManager.h>
#include <vgDatabase/vgdDefinition.h>

namespace vgMod{

//	PropertiesParam vgObjectEntry::s_ParamArray[s_NumOfParam];

	bool vgObjectEntry::ms_collisionEnable = true;

void vgObjectEntry::OnSinglePropertyChanged(string paramName)
{
	vgDatabaseEntry::OnSinglePropertyChanged(paramName);
	
	ObjectNode *pObject = ( ObjectNode*) m_Renderer;	

	if (paramName == "X ����" 
		|| paramName == "Y ����"
		|| paramName == "Z ����")
	{
		m_posZ = - m_posZMinus;
		Vector3 offset = Vector3( m_posX, m_posY, m_posZ)
			- pObject->GetPosition() - vgKernel::CoordSystem::getSingleton().getProjectionCoord();
		pObject->translate( offset );
	}

	if (paramName == "��������")
	{
		pObject->SetName( (TCHAR*)m_Renderer->getName().c_str() );
	}

	if (paramName == "������͸��")
	{
		pObject->setOpacity(m_bOpacity);
	}

	if (paramName == "����˫����Ⱦ")
	{
		pObject->setRenderDoubleSidesState(m_bDoubleFaceRender);
	}

	if (paramName == "������ײ���")
	{
		pObject->setCollisionEnable(m_enableCollision);
	}

	if (paramName == "������ʾ")
	{
		pObject->setVisible(m_bVisible);
	}

	if (paramName == "������ɫ")
	{
		vgKernel::ColorVal colorval;
		colorval.setAsColorRef( _colorRef );
		vgKernel::RgbaQuad color;
		color = colorval.getAsRGBQUAD();  
		pObject->setColorableValue( color ); 
		pObject->getRgbaQuadRef()->alpha = m_fColorRatio * 255;
	}

	if (paramName == "��ϱ���")
	{
		pObject->getRgbaQuadRef()->alpha = m_fColorRatio * 255;
	}

	AviNode *pAvi = (AviNode*)m_Renderer;
	if (pAvi->mIsInited == true)
	{
		pAvi->setPlayState(m_bAviPlay);
	}

	if (paramName == "ʱ������ʼ��")
	{
		pObject->setStartTimeVal(m_startTime);
	}
	
	if (paramName == "ʱ������ֹ��")
	{
		pObject->setEndTimeVal(m_endTime);
	}

	// ����TREEITEM
	vgUI::UIController::getSingleton().GetWorkSpaceBar()->SetItemText(hTreeItem, m_Renderer->getName());

	vgKernel::SelectManager::getSingleton().updateBox();

}

void vgObjectEntry::onChanged(int eventId, void *param)
{

	if (eventId == vgKernel::VG_OBS_PROPCHANGED)
	{
		if (s_group_list.size() > 0)
		{
			return ;
		}
		
		getData();

		if (this == vgUI::UIController::getSingleton().GetCurrentSelectedNode())
		{
			s_ParamArray[1].pProp->SetValue(m_posX);
			s_ParamArray[2].pProp->SetValue(m_posY);
			s_ParamArray[3].pProp->SetValue(m_posZMinus/*m_posZ*/);

			s_ParamArray[6].pProp->SetValue(m_bOpacity);
			s_ParamArray[7].pProp->SetValue(m_bDoubleFaceRender);
			s_ParamArray[8].pProp->SetValue(m_enableCollision);
			s_ParamArray[9].pProp->SetValue(m_bVisible);

			AviNode *pAvi = (AviNode*)m_Renderer;
			if (pAvi->mIsInited == true)
			{
				s_ParamArray[11].pProp->SetValue(m_bAviPlay);
				s_ParamArray[17].pProp->SetValue(*m_pbUnderWater);

#if 0			//���Է��ִ˴���֧��colorref���ͺ�int���ͣ��д������֧��
				s_ParamArray[14].pProp->SetValue(_colorRef);
				s_ParamArray[15].pProp->SetValue(m_fColorRatio);

				s_ParamArray[19].pProp->SetValue(m_startTime);
				s_ParamArray[20].pProp->SetValue(m_endTime);
#endif
			}
			else
			{
				s_ParamArray[14].pProp->SetValue(*m_pbUnderWater);

#if 0			//���Է��ִ˴���֧��colorref���ͺ�int���ͣ��д������֧��
				s_ParamArray[11].pProp->SetValue(_colorRef);
				s_ParamArray[12].pProp->SetValue(m_fColorRatio);

				s_ParamArray[16].pProp->SetValue(m_startTime);
				s_ParamArray[17].pProp->SetValue(m_endTime);
#endif
			}					
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

	if (eventId == vgKernel::VG_OBS_GROUPUPDATE)
	{
		vgUI::UIController::getSingleton().RemoveAllPages();
		AddNodeTabs();
	}


}

void vgObjectEntry::AddSingleTabPage()
{

	getData();

	//��ɫ�༭
	ObjectNode *pObject = ( ObjectNode*) m_Renderer;

	AviNode *pAvi = (AviNode*) m_Renderer;
	
	vgUI::UIController::getSingleton().RemoveAllPages();

	vgPropertiesViewBar* pageViewBar = vgUI::UIController::getSingleton().GetPropertiesViewBar();
	
	s_ParamArray.clear();
	PropertiesParam param;

#if	VGK_LANG_CHS	
	{
		param.label = "MOD��������ֵ����";// index = 0
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

		param.label = "��������";// index = 4
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


		ObjectNode *pObject = ( ObjectNode*) m_Renderer;
		param.label = "������͸��";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_BOOL;
		param.connectedPtr = &m_bOpacity;
		param.comment = "��������İ�͸��Ч��";
		s_ParamArray.push_back(param);

		param.label = "����˫����Ⱦ";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_BOOL;
		param.connectedPtr = &m_bDoubleFaceRender;
		param.comment = "��������İ�͸��Ч��";
		s_ParamArray.push_back(param);

		param.label = "������ײ���";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_BOOL;
		param.connectedPtr = &m_enableCollision;
		param.comment = "����������ײ���";
		s_ParamArray.push_back(param);

		// add by liuss in version 1144
		param.label = "������ʾ";  
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_BOOL;
		param.connectedPtr = &m_bVisible;
		param.comment = "��ʾ/����MOD";
		s_ParamArray.push_back(param);

		if (pAvi->mIsInited == true)
		{
			param.label = "��Ч"; //index = 10
			param.typeId = PROP_ITEM_GROUP;
			param.dataType = PROP_DATA_NONE;
			param.connectedPtr = NULL;
			param.comment = string();
			s_ParamArray.push_back(param);

			param.label = "AVI����";  
			param.typeId = PROP_ITEM_DATA;
			param.dataType = PROP_DATA_BOOL;
			param.connectedPtr = &m_bAviPlay;
			param.comment = "����/�ر�AVI";
			s_ParamArray.push_back(param);	

			param.label = "AVI����";  
			param.typeId = PROP_ITEM_DATA;
			param.dataType = PROP_DATA_STR;
			param.connectedPtr = m_aviName;
			param.comment = "AVI����";
			s_ParamArray.push_back(param);
		}

		param.label = "�����������"; //index = 10
		param.typeId = PROP_ITEM_GROUP;
		param.dataType = PROP_DATA_NONE;
		param.connectedPtr = NULL;
		param.comment = string();
		s_ParamArray.push_back(param);

		param.label = "������ɫ";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_COLOR;
		param.connectedPtr = &_colorRef;
		param.comment = "��������Ĭ����ɫ";
		s_ParamArray.push_back(param);

		param.label = "��ϱ���";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_FLOAT;
		param.connectedPtr = &m_fColorRatio;
		param.comment = "����������ɫ���ϱ���";
		s_ParamArray.push_back(param);

		//Ocean set
		param.label = "������������"; //index = 13
		param.typeId = PROP_ITEM_GROUP;
		param.dataType = PROP_DATA_NONE;
		param.connectedPtr = NULL;
		param.comment = string();
		s_ParamArray.push_back(param);

		param.label = "������Ⱦ";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_BOOL;
		param.connectedPtr = m_pbUnderWater;
		s_ParamArray.push_back(param);

		param.label = "ʱ����"; 
		param.typeId = PROP_ITEM_GROUP;
		param.dataType = PROP_DATA_NONE;
		param.connectedPtr = NULL;
		param.comment = string();
		s_ParamArray.push_back(param);

		param.label = "ʱ������ʼ��";  
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_INT;
		param.connectedPtr = &m_startTime;
		param.comment = "����ʱ������ʼ��";
		s_ParamArray.push_back(param);	

		param.label = "ʱ������ֹ��";  
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_INT;
		param.connectedPtr = &m_endTime;
		param.comment = "����ʱ������ֹ��";
		s_ParamArray.push_back(param);

		// ���ݿ�profile���� ========================================================
		param.label = "���ݿ�����";
		param.typeId = PROP_ITEM_GROUP;
		param.dataType = PROP_DATA_NONE;
		param.connectedPtr = NULL;
		param.comment = string();
		s_ParamArray.push_back(param);

		param.label = "���ݿ�����";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_OPTION_START;
		param.connectedPtr = &m_profileName;
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
		param.connectedPtr = &m_posX;
		param.comment = "Set X Coordinate";
		s_ParamArray.push_back(param);

		param.label = "Y Coordinate";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_FLOAT;
		param.connectedPtr = &m_posY;
		param.comment = "Set Y Coordinate";
		s_ParamArray.push_back(param);

		param.label = "Z Coordinate";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_FLOAT;
		param.connectedPtr = &m_posZMinus/*m_posZ*/;
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

		ObjectNode *pObject = ( ObjectNode*) m_Renderer;
		param.label = "Turn Translucency On";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_BOOL;
		param.connectedPtr = pObject->getOpacityPtr();
		param.comment = "Set Translucent Flag";
		s_ParamArray.push_back(param);

		param.label = "Render Double Face";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_BOOL;
		param.connectedPtr = pObject->getRenderDoubleSidesStatePtr();
		param.comment = "Set Double-Face-Render Flag";
		s_ParamArray.push_back(param);

		if (pAvi->mIsInited == true)
		{
			param.label = "Effects"; //index = 10
			param.typeId = PROP_ITEM_GROUP;
			param.dataType = PROP_DATA_NONE;
			param.connectedPtr = NULL;
			param.comment = string();
			s_ParamArray.push_back(param);

			param.label = "AVI Play";  
			param.typeId = PROP_ITEM_DATA;
			param.dataType = PROP_DATA_BOOL;
			param.connectedPtr = &m_bAviPlay;
			param.comment = "Open/End AVI";
			s_ParamArray.push_back(param);	

			param.label = "AVI Name";  
			param.typeId = PROP_ITEM_DATA;
			param.dataType = PROP_DATA_STR;
			param.connectedPtr = m_aviName;
			param.comment = "AVI Name";
			s_ParamArray.push_back(param);
		}

		param.label = "Material Parameter";
		param.typeId = PROP_ITEM_GROUP;
		param.dataType = PROP_DATA_NONE;
		param.connectedPtr = NULL;
		param.comment = "Material Parameter Config";
		s_ParamArray.push_back(param);

		param.label = "Model Color";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_COLOR;
		param.connectedPtr = &_colorRef;
		param.comment = "set model color";
		s_ParamArray.push_back(param);

		param.label = "Enable Texture-Color Blend";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_FLOAT;
		param.connectedPtr = &m_fColorRatio;
		param.comment = "Texture-Color Blend Ratio";
		s_ParamArray.push_back(param);

		// ���ݿ�profile���� ========================================================
		param.label = "Database Configuration";
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_OPTION_START;
		param.connectedPtr = &m_profileName;
		s_ParamArray.push_back(param);
	}
#endif

	using namespace vgKernel;
	Plugin* plg = PluginManager::getSingleton().getPluginRef( VGD_DLL_NAME );


	if ( plg != NULL )
	{
		VGK_DLL_DECLARE_CLASSFUNC( plg, ProfileManager, GetProfileListSize, func_profilelistsize );
		VGK_DLL_DECLARE_CLASSFUNC( plg, ProfileManager, GetProfileNameByIndex, func_profilename );

		int profilesize = (*func_profilelistsize)();

		for (INT i=0; i<profilesize; i++)
		{
			String profilename = (*func_profilename)(i);

			if (profilename != m_profileName)
			{
				param.label = profilename;
				param.typeId = PROP_ITEM_DATA;
				param.dataType = PROP_DATA_OPTION_ITEM;
				s_ParamArray.push_back(param);
			}
		}

		if (m_profileName != "")
		{
			param.label = "";
			param.typeId = PROP_ITEM_DATA;
			param.dataType = PROP_DATA_OPTION_ITEM;
			s_ParamArray.push_back(param);
		}

	}

	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_OPTION_END;
	s_ParamArray.push_back(param);

	vgPropertyPage* propPage = vgUI::UIController::getSingleton().GetPropPage();
	propPage->Create(NIDD_PROPERTY, pageViewBar->GetTabControl());

	propPage->ConnectNode(this, &s_ParamArray);
	pageViewBar->AddTab("MOD", propPage);

	if ( !m_profileName.empty() )
	{
		vgDatabaseEntry::AddSingleTabPage();
	}
}

void vgObjectEntry::AddGroupTabPage()
{

	vgUI::UIController::getSingleton().RemoveAllPages();

	getGroupData();

	vgPropertiesViewBar* pageViewBar = vgUI::UIController::getSingleton().GetPropertiesViewBar();

	s_GroupParamArray.clear();
	PropertiesParam param;

	param.label = "������������";
	param.typeId = PROP_ITEM_GROUP;
	param.dataType = PROP_DATA_NONE;
	param.connectedPtr = NULL;
	param.comment = "������������";
	s_GroupParamArray.push_back(param);

	param.label = "������͸��";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_BOOL;
	param.connectedPtr = &m_bgroupOpacity;
	param.comment = "��ֵ���������͸��";
	s_GroupParamArray.push_back(param);

	param.label = "����˫����Ⱦ";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_BOOL;
	param.connectedPtr = &m_bGroupDbFaceRender;
	param.comment = "��ֵ��������˫����Ⱦ";
	s_GroupParamArray.push_back(param);

	param.label = "������ײ���";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_BOOL;
	param.connectedPtr = &m_enableCollision;
	param.comment = "��ֵ����������ײ���";
	s_GroupParamArray.push_back(param);

	param.label = "������ʾ";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_BOOL;
	param.connectedPtr = &m_bGroupVisible;
	param.comment = "��ֵ����������ʾ";
	s_GroupParamArray.push_back(param);

	if (m_bGroupHaveAvi == true)
	{
		param.label = "��Ч"; //index = 10
		param.typeId = PROP_ITEM_GROUP;
		param.dataType = PROP_DATA_NONE;
		param.connectedPtr = NULL;
		param.comment = string();
		s_GroupParamArray.push_back(param);

		param.label = "AVI����";  
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_BOOL;
		param.connectedPtr = &m_bGroupAviPlay;
		param.comment = "����/�ر�AVI";
		s_GroupParamArray.push_back(param);
	}

	// Database ==========================================================

#if VGK_LANG_CHS
	param.label = "�����������";
	param.typeId = PROP_ITEM_GROUP;
	param.dataType = PROP_DATA_NONE;
	param.connectedPtr = NULL;
	param.comment = "�����������";
	s_GroupParamArray.push_back(param);

	param.label = "������ɫ";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_COLOR;
	param.connectedPtr = &_colorRef;
	param.comment = "��ֵ����Ĭ����ɫ";
	s_GroupParamArray.push_back(param);

	param.label = "��ϱ���";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_FLOAT;
	param.connectedPtr = &m_fColorRatio;
	param.comment = "����������ɫ���ϱ���";
	s_GroupParamArray.push_back(param);

	param.label = "ʱ����"; 
	param.typeId = PROP_ITEM_GROUP;
	param.dataType = PROP_DATA_NONE;
	param.connectedPtr = NULL;
	param.comment = string();
	s_GroupParamArray.push_back(param);

	param.label = "ʱ������ʼ��";  
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_INT;
	param.connectedPtr = &m_groupStartTime;
	param.comment = "����ʱ������ʼ��";
	s_GroupParamArray.push_back(param);

	param.label = "ʱ������ֹ��";  
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_INT;
	param.connectedPtr = &m_groupEndTime;
	param.comment = "����ʱ������ֹ��";
	s_GroupParamArray.push_back(param);

	param.label = "���ݿ�";
	param.typeId = PROP_ITEM_GROUP;
	param.dataType = PROP_DATA_NONE;
	param.connectedPtr = NULL;
	param.comment = "���ݿ��������";
	s_GroupParamArray.push_back(param);

	s_groupProfileName = "";

	param.label = "���ݿ�����";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_OPTION_START;
	param.connectedPtr = &s_groupProfileName;
	s_GroupParamArray.push_back(param);
#else

	if (m_bGroupHaveAvi == true)
	{
		param.label = "Effects"; //index = 10
		param.typeId = PROP_ITEM_GROUP;
		param.dataType = PROP_DATA_NONE;
		param.connectedPtr = NULL;
		param.comment = string();
		s_GroupParamArray.push_back(param);

		param.label = "AVI Play";  
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_BOOL;
		param.connectedPtr = &m_bAviPlay;
		param.comment = "Open/End AVI";
		s_GroupParamArray.push_back(param);	

		param.label = "AVI Name";  
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_STR;
		param.connectedPtr = &m_aviName;
		param.comment = "AVI Name";
		s_GroupParamArray.push_back(param);
	}

	param.label = "Material Parameter";
	param.typeId = PROP_ITEM_GROUP;
	param.dataType = PROP_DATA_NONE;
	param.connectedPtr = NULL;
	param.comment = "Material Parameter Config";
	s_GroupParamArray.push_back(param);

	param.label = "Model Color";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_COLOR;
	param.connectedPtr = &_colorRef;
	param.comment = "set model color";
	s_GroupParamArray.push_back(param);

	param.label = "Texture-Color Blend Ratio";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_FLOAT;
	param.connectedPtr = &m_fColorRatio;
	param.comment = "Texture-Color Blend Ratio";
	s_GroupParamArray.push_back(param);

	param.label = "Database";
	param.typeId = PROP_ITEM_GROUP;
	param.dataType = PROP_DATA_NONE;
	param.connectedPtr = NULL;
	param.comment = "";
	s_GroupParamArray.push_back(param);

	s_groupProfileName = "";

	param.label = "Config";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_OPTION_START;
	param.connectedPtr = &s_groupProfileName;
	s_GroupParamArray.push_back(param);
#endif

	using namespace vgKernel;
	Plugin* plg = PluginManager::getSingleton().getPluginRef( VGD_DLL_NAME );


	if ( plg != NULL )
	{
		VGK_DLL_DECLARE_CLASSFUNC( plg, ProfileManager, GetProfileListSize, func_profilelistsize );
		VGK_DLL_DECLARE_CLASSFUNC( plg, ProfileManager, GetProfileNameByIndex, func_profilename );

		int profilesize = (*func_profilelistsize)();

		for (int i=0; i<profilesize; i++)
		{
			String profilename = (*func_profilename)(i);

			param.label = profilename;
			param.typeId = PROP_ITEM_DATA;
			param.dataType = PROP_DATA_OPTION_ITEM;
			s_GroupParamArray.push_back(param);
		}

	}

	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_OPTION_END;
	s_GroupParamArray.push_back(param);


	vgPropertyPage* propPage = vgUI::UIController::getSingleton().GetPropPage();
	propPage->Create(NIDD_PROPERTY, pageViewBar->GetTabControl());
	propPage->ConnectNode(this, &s_GroupParamArray);

	//propPage->ConnectNode(this, s_GroupParamArray, s_NumOfGroupParam);
	pageViewBar->AddTab("MOD Group", propPage);

	vgUI::UIController::getSingleton().SetCurrentSelectedNode(this);
}

void vgObjectEntry::OnGroupPropertyChanged(string paramName)
{
	vgObjectEntry* pObjectEntry;
	vgMod::ObjectNode*    pObject;
	vgMod::AviNode* pAvi;

	vgDatabaseEntry::OnGroupPropertyChanged(paramName);

	for (int i=0; i<s_group_list.size(); i++)
	{
		pObjectEntry = ((vgObjectEntry*)s_group_list[i]);
		pObject = (vgMod::ObjectNode*)(pObjectEntry->getRender());
		pAvi = (vgMod::AviNode*)(pObjectEntry->getRender());

		if (paramName == "������ɫ")
		{
			vgKernel::ColorVal colorval;
			colorval.setAsColorRef( _colorRef );
			vgKernel::RgbaQuad color;
			color = colorval.getAsRGBQUAD();  
			pObject->setColorableValue( color ); 
			pObject->getRgbaQuadRef()->alpha = m_fColorRatio * 255;
		}

		if (paramName == "��ϱ���")
		{
			pObject->getRgbaQuadRef()->alpha = m_fColorRatio * 255;
		}

		if (paramName == "������ײ���")
		{
			pObjectEntry->m_enableCollision = m_enableCollision;
			pObject->setCollisionEnable(m_enableCollision);
		}

		if (paramName == "������ʾ")
		{
			pObjectEntry->m_bGroupVisible = m_bGroupVisible;
			pObject->setVisible(m_bGroupVisible);
		}

		if (paramName == "����˫����Ⱦ")
		{
			pObjectEntry->m_bGroupDbFaceRender = m_bGroupDbFaceRender;		
			pObject->setRenderDoubleSidesState(m_bGroupDbFaceRender);
		}

		if (paramName == "������͸��")
		{
			pObjectEntry->m_bgroupOpacity = m_bgroupOpacity;
			pObject->setOpacity(m_bgroupOpacity);	
		}

		if (pAvi->mIsInited == true)
		{
			pObjectEntry->m_bGroupAviPlay = m_bGroupAviPlay;
			pAvi->setPlayState(m_bGroupAviPlay);
		}

		if (paramName == "ʱ������ʼ��")
		{
			pObjectEntry->m_groupStartTime = m_groupStartTime;
			pObject->setStartTimeVal(m_groupStartTime);
		}

		if (paramName == "ʱ������ֹ��")
		{
			pObjectEntry->m_groupEndTime = m_groupEndTime;
			pObject->setEndTimeVal(m_groupEndTime);
		}

	}

	return ;

}


CMenu* vgObjectEntry::GetContextMenu()
{
	CMenu *menu = new CMenu;

	VERIFY(menu->CreatePopupMenu());


#if VGK_LANG_CHS
	VERIFY(menu->AppendMenu(MF_STRING, NID_MESH_GOTO, _T("ת��")));
	VERIFY(menu->AppendMenu(MF_STRING, NID_MESH_DELETE,_T("ɾ��")));
#else
	VERIFY(menu->AppendMenu(MF_STRING, NID_MESH_GOTO, _T("delete")));

#endif

	return menu;
}

void vgObjectEntry::getData()
{	
	ObjectNode *pObject = ( ObjectNode*) m_Renderer;

	Vector3 pos = pObject->GetPosition();

	m_posX = pos.x + vgKernel::CoordSystem::getSingleton().getProjectionCoord().x;
	m_posY = pos.y + vgKernel::CoordSystem::getSingleton().getProjectionCoord().y;
	m_posZ = pos.z + vgKernel::CoordSystem::getSingleton().getProjectionCoord().z;
	m_posZMinus = - m_posZ;

	m_bOpacity = pObject->getOpacity();
	m_bDoubleFaceRender = *(pObject->getRenderDoubleSidesStatePtr() );

	m_enableCollision = pObject->getCollisionEnable();
	m_pbUnderWater=pObject->getUnderWaterPtr();
	
	m_bVisible = pObject->GetVisable();
	
	vgKernel::ColorVal val;
	val.setAsRGBQUAD( *pObject->getRgbaQuadRef()->getRGBQUAD());
	_colorRef = val.getAsColorRef();
	m_fColorRatio = pObject->getRgbaQuadRef()->alpha / 255.0f;

	AviNode *pAvi = ( AviNode*) m_Renderer;
	if (pAvi->mIsInited == true)
	{		
		m_bAviPlay = pAvi->getAviState();
		m_aviName = pAvi->getAviBaseName();
	}

	m_startTime = pObject->getStartTimeVal();
	m_endTime = pObject->getEndTimeVal();
}

void vgObjectEntry::getGroupData()
{
	vgObjectEntry* pObjectEntry;
	vgMod::ObjectNode*    pObject;	
	vgMod::AviNode* pAvi;

	//��ʱȡ���е�һ�������������Ϊ�����ʾ����
	pObjectEntry = ((vgObjectEntry*)s_group_list[0]);
	pObject = (vgMod::ObjectNode*)(pObjectEntry->getRender());
	pAvi = (vgMod::AviNode*)(pObjectEntry->getRender());

	m_bgroupOpacity = pObject->getOpacity();		
	m_bGroupDbFaceRender = *(pObject->getRenderDoubleSidesStatePtr() );
	m_bGroupVisible = pObject->GetVisable();
	if (pAvi->mIsInited == true)
	{
		m_bGroupHaveAvi = true;
		m_bGroupAviPlay = pAvi->getAviState();
		//m_GroupAviName = pAvi->getAviBaseName();
	}
	m_groupStartTime = pObject->getStartTimeVal();
	m_groupEndTime = pObject->getEndTimeVal();

}

vgObjectEntry::vgObjectEntry( ObjectNode* pObject ) : vgDatabaseEntry( pObject )
{
	assert( pObject != NULL );

	m_bGroupHaveAvi = false;

	m_aviName = NULL;

	getData();

	vgKernel::CoordSystem::getSingleton().registerObserver(this);
}
}//namespace vgCore
