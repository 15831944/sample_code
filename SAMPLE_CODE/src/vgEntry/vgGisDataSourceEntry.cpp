



#include <vgStableHeaders.h>
#include "vgentry/vgGisDataSourceEntry.h"
#include <vgEntry/vgEntryUtility.h>
#include <vgUIController/vgUIController.h>
#include <vgGIS3d/vggiGisDataManager.h>

#include <vgKernel/vgkStringUtility.h>

//----------------------------------------------------------------
PropertiesParamVector& GisDataSourceEntry::pushPropertiesParam(
	PropertiesParamVector& params )
{
	VG_PUSH_PROP_GROUP( params , "Properties" );
	VG_PUSH_PROP_CONST_STR( params , "filepath" , &_filepath );

	return params;
}
//----------------------------------------------------------------
void GisDataSourceEntry::updateUIFromInternal()
{
	_filepath = _datasource->getSourcePath();

}
//----------------------------------------------------------------
void GisDataSourceEntry::updateInternalFromUI(
	const String& changed_param_name )
{

}
//----------------------------------------------------------------
GisDataSourceEntry::~GisDataSourceEntry()
{
	//assert(0);


}
//----------------------------------------------------------------
//bool GisDataSourceEntry::onDelete()
//{
//
//	vgShape::ShapeManager::getSingleton().clearDataSource( _datasource );
//
//	vgUI::UIController::getSingleton().DeleteNode( this , true );
//	return true;
//}

bool GisDataSourceEntry::onUserDefinedDeletion()
{
	//------------------------------------------
	// ����ɾ�����е�layers
	// ���ʱ����Զ�����entry��delete
	// �������е��ӽڵ�Ӧ�ö���ɾ������
	// ��ʱֱ��ɾ������.
	//------------------------------------------
	_datasource->deleteAllLayers();

	vgGIS3D::GisDataManager::getSingleton().clearDataSourcePtr( _datasource );

	return true;
}
//----------------------------------------------------------------
String GisDataSourceEntry::GetName()
{
	return vgKernel::StringUtility::getFilenameFromAbsolutePath( 
		_datasource->getSourcePath() );
}
//----------------------------------------------------------------
