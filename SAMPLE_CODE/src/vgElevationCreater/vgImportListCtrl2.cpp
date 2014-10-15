






#include <vgStableHeaders.h>
#include "vgImportListCtrl2.h"





CImportListCtrl2::CImportListCtrl2()
{
	_items = new ListCtrlItemMap2;
}

CImportListCtrl2::~CImportListCtrl2()
{

	if ( _items != NULL )
	{
		delete _items;
		_items = NULL ;
	}
}

void CImportListCtrl2::updateDems( const StringVectorPtr& dems )
{

	assert( dems.isNull() == false );

	StringVector::iterator iter = dems->begin();
	StringVector::iterator iter_end = dems->end();

	for ( ; iter != iter_end ; ++iter )
	{
		String metaname = (*iter ).substr(
			(*iter).find_last_of("\\/") + 1 , String::npos );

		ListCtrlItem2 tmpitem;

		tmpitem._demFilepath = *iter;
		tmpitem._imageFilePath = "";
		tmpitem._metaDemFilename = metaname;
		tmpitem._metaImgFilename = "";

		ListCtrlItemInsertRes2 result = _items->insert( std::make_pair(
			tmpitem._metaDemFilename ,tmpitem )) ;

		if ( result.second == false )
		{
			std::ostringstream o;
			o << "���в����ظ�������" << metaname << ",�������е���.";
			MessageBox( o.str().c_str() , "��ʾ");
		}
	}

	invalidate();
}

void CImportListCtrl2::updateImage( const StringVectorPtr& imgs )
{

	assert( imgs.isNull() == false );

	StringVector::iterator iter = imgs->begin();
	StringVector::iterator iter_end = imgs->end();

	for ( ; iter != iter_end ; ++iter )
	{
		String curname = *iter;

		String demname = curname.substr(
			curname.find_last_of("\\/") + 1 ,  String::npos );

		demname = demname.substr( 0 , demname.find_last_of(".") ) + ".dem";

		//------------------------------------------
		// ���Ȳ��Ҷ�Ӧ��item
		//------------------------------------------
		ListCtrlItemMap2::iterator found = _items->find( demname );

		if ( found == _items->end() )
		{
			std::ostringstream o;
			o << "ͼ��:" << *iter << "δ�ҵ���Ӧ��dem����,�������е���.";
			MessageBox( o.str().c_str() ,"��ʾ");
			continue;
		}

		found->second._imageFilePath = *iter;
		found->second._metaImgFilename = 
			(*iter).substr( 
			(*iter).find_last_of("\\/") + 1 , String::npos );
	}

	invalidate();
}

void CImportListCtrl2::invalidate()
{
	DeleteAllItems();

	int itemid = 1;

	ListCtrlItemMap2::iterator iter = _items->begin();
	ListCtrlItemMap2::iterator iter_end = _items->end();

	for ( ; iter != iter_end ; ++ iter )
	{
		std::ostringstream o;
		o << itemid++;

		InsertItem(INT_MAX, 
			o.str().c_str() ,
			iter->first.c_str(),
			iter->second._demFilepath.c_str()
			);
	}

	UpdateData( FALSE );
}

void CImportListCtrl2::initialise()
{
	SetHeadings(
		_T(", 35;tif�ļ�, 100;tif�ļ�·��, 150"));


	SetGridLines(TRUE); // SHow grid lines
	//_importList.SetEditable(TRUE); // Allow sub-text edit
	SortItems(0, TRUE); // sort the 1st column, ascending

	//m_bSortable = _importList.IsSortable();
}

bool CImportListCtrl2::checkValid()
{
	if ( _items->empty() == true )
	{
		return false;
	}

	ListCtrlItemMap2::iterator iter = _items->begin();
	ListCtrlItemMap2::iterator iter_end = _items->end();

	for ( ; iter != iter_end ; ++ iter )
	{
		if ( iter->first == "" )
		{
			return false;
		}

		//if ( iter->second._imageFilePath == "" )
		//{
		//	return false;
		//}

		vgKernel::StreamReaderPtr preader1 = 
			vgKernel::StreamReaderFactory::createFromFile( iter->second._demFilepath );

		if ( preader1.isNull() == true )
		{
			std::ostringstream o;
			o << "��ȡ�ļ�" << iter->second._demFilepath << 
				"ʱʧ��,��ȷ���ļ����ڲ��ҿ�����������.";
			MessageBox( o.str().c_str() , "����" );
			return false;
		}

		//vgKernel::StreamReaderPtr preader2 = 
		//	vgKernel::StreamReaderFactory::createFromFile( iter->second._imageFilePath );

		//if ( preader2.isNull() == true )
		//{
		//	std::ostringstream o;
		//	o << "��ȡ�ļ�" << iter->second._imageFilePath << 
		//		"ʱʧ��,��ȷ���ļ����ڲ��ҿ�����������.";
		//	MessageBox( o.str().c_str() , "����" );
		//	return false;
		//}
	}

	return true;
}

void CImportListCtrl2::clearAll()
{
	_items->clear();
	invalidate();

}

vgKernel::StringPairVector* CImportListCtrl2::getStringPairVec()
{
	assert( _items->empty() == false );


	vgKernel::StringPairVector* ret = new vgKernel::StringPairVector;

	ListCtrlItemMap2::iterator iter = _items->begin();
	ListCtrlItemMap2::iterator iter_end = _items->end();

	for ( ; iter != iter_end ; ++ iter )
	{
		ret->push_back( 
			std::make_pair(
			iter->second._demFilepath , iter->second._imageFilePath ));
	}


	return ret;
}
//----------------------------------------------------------------
vgKernel::StringVectorPtr CImportListCtrl2::getDemFileNameVector()
{
	if ( _items->empty() == true )
	{
		return vgKernel::StringVectorPtr();
	}

	assert( _items->empty() == false );

	vgKernel::StringVectorPtr ret( new StringVector );

	ListCtrlItemMap2::iterator iter = _items->begin();
	ListCtrlItemMap2::iterator iter_end = _items->end();

	for ( ; iter != iter_end ; ++ iter )
	{
		ret->push_back( 	iter->second._demFilepath );
	}

	return ret;
}