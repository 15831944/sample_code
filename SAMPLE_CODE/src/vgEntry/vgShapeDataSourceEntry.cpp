



#include <vgStableHeaders.h>
#include "vgentry/vgShapeDataSourceEntry.h"
#include <vgEntry/vgEntryUtility.h>
#include <vgUIController/vgUIController.h>
#include <vgShape/vgshShapeManager.h>

#include <vgKernel/vgkStringUtility.h>

//----------------------------------------------------------------
PropertiesParamVector& ShapeDataSourceEntry::pushPropertiesParam(
	PropertiesParamVector& params )
{
	VG_PUSH_PROP_GROUP( params , "Properties" );
	VG_PUSH_PROP_CONST_STR( params , "filepath" , &_filepath );

	return params;
}
//----------------------------------------------------------------
void ShapeDataSourceEntry::updateUIFromInternal()
{
	_filepath = _datasource->getFilePath();

}
//----------------------------------------------------------------
void ShapeDataSourceEntry::updateInternalFromUI(
	const String& changed_param_name )
{

}
//----------------------------------------------------------------
ShapeDataSourceEntry::~ShapeDataSourceEntry()
{
	//assert(0);


}
//----------------------------------------------------------------
//bool ShapeDataSourceEntry::onDelete()
//{
//
//	vgShape::ShapeManager::getSingleton().clearDataSource( _datasource );
//
//	vgUI::UIController::getSingleton().DeleteNode( this , true );
//	return true;
//}

bool ShapeDataSourceEntry::onUserDefinedDeletion()
{
	//------------------------------------------
	// ����ɾ�����е�layers
	// ���ʱ����Զ�����entry��delete
	// �������е��ӽڵ�Ӧ�ö���ɾ������
	// ��ʱֱ��ɾ������.
	//------------------------------------------
	_datasource->deleteAllLayers();

	vgShape::ShapeManager::getSingleton().clearDataSourcePtr( _datasource );

	return true;
}
//----------------------------------------------------------------
String ShapeDataSourceEntry::GetName()
{
	return vgKernel::StringUtility::getFilenameFromAbsolutePath( 
		_datasource->getFilePath() );
}
//----------------------------------------------------------------
