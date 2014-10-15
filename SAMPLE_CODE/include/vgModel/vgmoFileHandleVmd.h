



#ifndef __VGMO_FILEHANDLEVMD_H__
#define __VGMO_FILEHANDLEVMD_H__


#include <vgModel/vgmoDefinition.h>

#include <vgKernel/vgkForward.h>
#include <vgAsyn/vgaFileHandle.h>
#include <vgModel/vgmoFileReaderVmd.h>
#include <vgKernel/vgkStringVector.h>

	
namespace vgModel {



	/**
		@date 	2008/07/04  10:40	
		@author  leven
	
		@brief 	ע��������߼�.
		ModelItemʵ��Ĺ���(����ɾ��)������ModelManager��
	
		@see    
	*/
	class  VGMO_EXPORT FileHandleVmd : public vgAsyn::FileHandle
	{	
	public:
		FileHandleVmd( String filepath )
			: FileHandle( filepath )
		{
			_type = FILEHANDLE_TYPE_VGM;
			_isLinkedWithModelMgr = false;
		}

		virtual ~FileHandleVmd()
		{

		}

		virtual bool exist( const String& metafilename );

		virtual vgKernel::StringVector* listMetaFileNames();

		virtual bool open();

		virtual void close();

		virtual bool extractToMultipleFiles( const String& save_multiple_file_path );

		//virtual IoRequestVector* listIoRequests();

		ModelItemMap* getModelItemsPtr()
		{
			return &_itemPtrs;
		}

		void setLinkedFlag( const bool& flag )
		{
			_isLinkedWithModelMgr = flag;
		}

		bool getLinkedFlag() const
		{
			return _isLinkedWithModelMgr;
		}

	private:

		bool addModelItem( ModelItem* item );

	private:

		VmdHeader _header;

		bool _isLinkedWithModelMgr;

		ModelItemMap _itemPtrs;
	};
	
	

	

	
}// end of namespace vgModel
	



#endif // end of __VGMO_FILEHANDLEVMD_H__