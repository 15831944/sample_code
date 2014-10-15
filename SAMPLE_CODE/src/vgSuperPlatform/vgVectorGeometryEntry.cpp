
#include <vgStableHeaders.h>
#include "vgVectorGeometryEntry.h"
#include "vgPropertyPage.h"

#include <vgKernel/vgkConvertUtility.h>
#include <vgDatabaseEntry.h>

//PropertiesParam vgVectorGeometryEntry::s_ParamArray[s_NumOfParam];

vgVectorGeometryEntry::vgVectorGeometryEntry(vgVector::vgvGeometry*	pGeometry) 
	:vgDatabaseEntry( VG_ENTRY_2DGEOMETRY , pGeometry)
{
	this->onChanged(vgKernel::VG_OBS_PROPCHANGED, NULL);
} 

vgVectorGeometryEntry::~vgVectorGeometryEntry(void)
{
	m_Renderer->unregisterObserver( this );
	m_strFieldPtrArray.clear();
}

//void vgVectorGeometryEntry::OnPropertyChanged(string paramName)
//{
//
//}

void vgVectorGeometryEntry::onChanged(int eventId, void *param)
{
	if (eventId == vgKernel::VG_OBS_PROPCHANGED)
	{
		vgUIController::GetInstance()->GetWorkSpaceBar()->SetItemText(hTreeItem, GetName());

		//vgVector::vgvLayer*	pLayer = (vgVector::vgvLayer*)m_Renderer;
		m_CenterPoint	= m_Renderer->getBoundingBox().getCenter();
		
		m_CenterPoint	+= vgKernel::Vec3(
			vgCore::vgCoordinate::getSingleton().GetSceneCood().x,
			vgCore::vgCoordinate::getSingleton().GetSceneCood().y,
			vgCore::vgCoordinate::getSingleton().GetSceneCood().z);
		
		m_bIsVisible	= m_Renderer->getVisible();

		
		if (this == vgUIController::GetInstance()->GetCurrentSelectedNode())
		{
			_params[1].pProp->SetValue(m_CenterPoint.x);
			_params[2].pProp->SetValue(m_CenterPoint.y);
			m_posZMinus	= -m_CenterPoint.z;
			_params[3].pProp->SetValue(m_posZMinus);
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

//void vgVectorGeometryEntry::AddNodeTabs()
//{
////	vgUIController::GetInstance()->RemoveAllPages();
////
////	vgPropertiesViewBar* pageViewBar = vgUIController::GetInstance()->GetPropertiesViewBar();
////
////	m_strFieldPtrArray.clear();
////	_params.clear();
////	PropertiesParam param;
////
////	param.label = "����������ֵ����";
////	param.typeId = PROP_ITEM_GROUP;
////	param.dataType = PROP_DATA_NONE;
////	param.connectedPtr = NULL;
////	param.comment = "���ò���������ֵ";
////	_params.push_back( param );
////
////	param.label = "X ����";
////	param.typeId = PROP_ITEM_DATA;
////	param.dataType = PROP_DATA_FLOAT;
////	param.connectedPtr = &m_CenterPoint.x;
////	param.comment = "����X����";
////	_params.push_back( param );
////
////	param.label = "Y ����";
////	param.typeId = PROP_ITEM_DATA;
////	param.dataType = PROP_DATA_FLOAT;
////	param.connectedPtr = &m_CenterPoint.y;
////	param.comment = "����Y����";
////	_params.push_back( param );
////
////	m_posZMinus = - m_CenterPoint.z;
////	param.label = "Z ����";
////	param.typeId = PROP_ITEM_DATA;
////	param.dataType = PROP_DATA_FLOAT;
////	param.connectedPtr = &m_posZMinus;
////	param.comment = "����Z����";
////	_params.push_back( param );
////
////	param.label = "��������";
////	param.
////
////	param.label = "���ݿ��¼";
////	param.typeId = PROP_ITEM_GROUP;
////	param.dataType = PROP_DATA_NONE;
////	param.connectedPtr = NULL;
////	param.comment = string();
////	_params.push_back( param );
////
////	vgVector::vgvGeometry*	pGeometry = dynamic_cast<vgVector::vgvGeometry*>(m_Renderer);
////	if (!pGeometry)
////	{
////		return;
////	}
////
////	vgVector::vgvAttribute*	pAttribute = pGeometry->getAttributePtr();
////
////	if (pAttribute)
////	{
////		int iField = 0;
////#ifndef	TEMP_BUFFER_SIZE
////#define TEMP_BUFFER_SIZE 80
////		char	szTempBuffer[TEMP_BUFFER_SIZE];
////		String	strRecord;
////		for (vgVector::FieldValueArrayItr itr = pAttribute->getFieldValueArrayPtr()->begin() ;
////			itr != pAttribute->getFieldValueArrayPtr()->end(); itr++ )
////		{
////			OGRFieldDefn        *poFDefn = pAttribute->getFeatureDefn()->GetFieldDefn( iField++ );
////			char    szFormat[TEMP_BUFFER_SIZE];
////
////			switch (poFDefn->GetType())
////			{
////			case OFTString:
////				{
////					if( 
////						( (*itr)->String == NULL )
////						|| ( (*itr)->Set.nMarker1 == OGRUnsetMarker ) 
////						)
////					{
////						snprintf( szTempBuffer, TEMP_BUFFER_SIZE -1,
////							"%s", "NULL" );
////					}
////					else
////					{
////						snprintf( szTempBuffer, TEMP_BUFFER_SIZE -1,
////							"%s", (*itr)->String );
////					}
////				}
////				break;
////
////			case OFTInteger:
////				{
////					snprintf( szTempBuffer, TEMP_BUFFER_SIZE -1,
////						"%d", (*itr)->Integer );
////				}				
////				break;
////
////			case OFTReal:
////				{
////					if( poFDefn->GetWidth() != 0 )
////					{
////						snprintf( szFormat, TEMP_BUFFER_SIZE -1, "%%-%d.%df",
////							poFDefn->GetWidth(), poFDefn->GetPrecision() );
////					}
////					else
////						strcpy( szFormat, "%.15g" );
////
////					snprintf( szTempBuffer, TEMP_BUFFER_SIZE -1,
////						szFormat, (*itr)->Real );
////				}				
////				break;
////
////			default:
////				{
////					snprintf( szTempBuffer, TEMP_BUFFER_SIZE -1,
////						"%s", "NULL" );
////				}
////				break;
////
////			}//switch
////			String	*pString = new String(szTempBuffer);
////			
////			m_strFieldPtrArray.push_back( ShareShringPtr(pString) );
////
////			param.label = poFDefn->GetNameRef();
////			param.typeId = PROP_ITEM_DATA;
////			param.dataType = PROP_DATA_STR;
////			param.connectedPtr = pString;
////			param.comment = poFDefn->GetNameRef();
////
////			_params.push_back( param );
////
////		}//for
////
////#endif//TEMP_BUFFER_SIZE
////	}
////	/*s_ParamArray[7].label = "ͼԪ��Ŀ";
////	s_ParamArray[7].typeId = PROP_ITEM_DATA;
////	s_ParamArray[7].dataType = PROP_DATA_CONST_STR;
////	s_ParamArray[7].connectedPtr = &m_strConstShapeCount;
////	s_ParamArray[7].comment = "���ڰ���ͼԪ������";*/
////
////	vgPropertyPage* propPage = vgUIController::GetInstance()->GetPropPage();
////	propPage->Create(NIDD_PROPERTY, pageViewBar->GetTabControl());
////
////	propPage->ConnectNode(this, &_params[0],  _params.size() );
////	pageViewBar->AddTab("�Զ�����", propPage);
//}

void vgVectorGeometryEntry::AddSingleTabPage()
{
	vgUIController::GetInstance()->RemoveAllPages();

	vgPropertiesViewBar* pageViewBar = vgUIController::GetInstance()->GetPropertiesViewBar();

	s_ParamArray.clear();
	PropertiesParam param;

	param.label = "����������ֵ����";
	param.typeId = PROP_ITEM_GROUP;
	param.dataType = PROP_DATA_NONE;
	param.connectedPtr = NULL;
	param.comment = "���ò���������ֵ";
	s_ParamArray.push_back( param );

	param.label = "X ����";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_FLOAT;
	param.connectedPtr = &m_CenterPoint.x;
	param.comment = "����X����";
	s_ParamArray.push_back( param );

	param.label = "Y ����";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_FLOAT;
	param.connectedPtr = &m_CenterPoint.y;
	param.comment = "����Y����";
	s_ParamArray.push_back( param );

	m_posZMinus = - m_CenterPoint.z;
	param.label = "Z ����";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_FLOAT;
	param.connectedPtr = &m_posZMinus;
	param.comment = "����Z����";
	s_ParamArray.push_back( param );

	param.label = "��������";
	param.typeId = PROP_ITEM_GROUP;
	param.dataType = PROP_DATA_NONE;
	param.connectedPtr = NULL;
	param.comment = string();
	s_ParamArray.push_back( param );

	param.label = "��������";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_STR;
	param.connectedPtr = m_Renderer->getNamePtr();
	param.comment = "���������";
	s_ParamArray.push_back( param );

	param.label = "�ɼ����";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_BOOL;
	param.connectedPtr = &m_bIsVisible;
	param.comment = "��ɼ��Ա��";
	s_ParamArray.push_back( param );

	// ���ݿ�profile���� ========================================================

	param.label = "���ݿ�";
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

	vector<vgDatabase::QueryProfile> profiles = vgDatabase::ProfileManager::getSingleton().GetProfile();

	for (INT i=0; i<profiles.size(); i++)
	{
		if (profiles[i].GetName() != m_profileName)
		{
			param.label = profiles[i].GetName();
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

	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_OPTION_END;
	s_ParamArray.push_back(param);

	// ===========================================================================
	vgPropertyPage* propPage = vgUIController::GetInstance()->GetPropPage();
	propPage->Create(NIDD_PROPERTY, pageViewBar->GetTabControl());

	//propPage->ConnectNode(this, s_ParamArray, s_NumOfParam);
	propPage->ConnectNode(this, &s_ParamArray);

	pageViewBar->AddTab("�Զ�����", propPage);

	// ���ø�������Database���Ե�Tabҳ
	vgDatabaseEntry::AddSingleTabPage();

	vgUIController::GetInstance()->SetCurrentSelectedNode(this);
}

void vgVectorGeometryEntry::AddGroupTabPage()
{
	vgUIController::GetInstance()->RemoveAllPages();

	vgPropertiesViewBar* pageViewBar = vgUIController::GetInstance()->GetPropertiesViewBar();

	s_GroupParamArray.clear();
	PropertiesParam param;

	param.label = "����������ֵ����";
	param.typeId = PROP_ITEM_GROUP;
	param.dataType = PROP_DATA_NONE;
	param.connectedPtr = NULL;
	param.comment = "���ò���������ֵ";
	s_GroupParamArray.push_back( param );

	param.label = "X ����";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_FLOAT;
	param.connectedPtr = &m_CenterPoint.x;
	param.comment = "����X����";
	s_GroupParamArray.push_back( param );

	param.label = "Y ����";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_FLOAT;
	param.connectedPtr = &m_CenterPoint.y;
	param.comment = "����Y����";
	s_GroupParamArray.push_back( param );

	m_posZMinus = - m_CenterPoint.z;
	param.label = "Z ����";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_FLOAT;
	param.connectedPtr = &m_posZMinus;
	param.comment = "����Z����";
	s_GroupParamArray.push_back( param );

	param.label = "��������";
	param.typeId = PROP_ITEM_GROUP;
	param.dataType = PROP_DATA_NONE;
	param.connectedPtr = NULL;
	param.comment = string();
	s_GroupParamArray.push_back( param );

	param.label = "��������";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_STR;
	param.connectedPtr = m_Renderer->getNamePtr();
	param.comment = "���������";
	s_GroupParamArray.push_back( param );

	param.label = "�ɼ����";
	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_BOOL;
	param.connectedPtr = &m_bIsVisible;
	param.comment = "��ɼ��Ա��";
	s_GroupParamArray.push_back( param );

	// Database ==========================================================
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

	vector<vgDatabase::QueryProfile> profiles = vgDatabase::ProfileManager::getSingleton().GetProfile();

	for (int i=0; i<profiles.size(); i++)
	{
		param.label = profiles[i].GetName();
		param.typeId = PROP_ITEM_DATA;
		param.dataType = PROP_DATA_OPTION_ITEM;
		s_GroupParamArray.push_back(param);
	}

	param.typeId = PROP_ITEM_DATA;
	param.dataType = PROP_DATA_OPTION_END;
	s_GroupParamArray.push_back(param);


	vgPropertyPage* propPage = vgUIController::GetInstance()->GetPropPage();
	propPage->Create(NIDD_PROPERTY, pageViewBar->GetTabControl());
	propPage->ConnectNode(this, &s_GroupParamArray);

	//propPage->ConnectNode(this, s_GroupParamArray, s_NumOfGroupParam);
	pageViewBar->AddTab("Group����", propPage);

	vgUIController::GetInstance()->SetCurrentSelectedNode(this);
}

void vgVectorGeometryEntry::OnSinglePropertyChanged( string paramName )
{
	vgDatabaseEntry::OnSinglePropertyChanged(paramName);

	//vgVector::vgvGe*	pLayer = (vgVector::vgvLayer*)m_Renderer;
	m_Renderer->setVisible(m_bIsVisible);
	// ����TREEITEM
	vgUIController::GetInstance()->GetWorkSpaceBar()->SetItemText(hTreeItem, GetName());
}

void vgVectorGeometryEntry::OnGroupPropertyChanged( string paramName )
{
	vgDatabaseEntry::OnGroupPropertyChanged(paramName);
}

void vgVectorGeometryEntry::OnGroupChanged( float sdx, float sdy, float sdz )
{
	return;
}