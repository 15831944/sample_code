


#ifndef __VGTERRITORY_TFILEHANDLEVTR_H__
#define __VGTERRITORY_TFILEHANDLEVTR_H__


#include <vgTerritory/vgtDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgAsyn/vgaFileHandle.h>
#include <vgTerritory/vgtElevationItem.h>

	
namespace vgTerritory {

	/**
		@date 	2008/09/02  9:31	
		@author  leven
	
		@brief 	ע��!�ļ��Ĵ�Ϊ�ɶ���д�ķ�ʽ.
		�������ʽ��,�ļ����ܱ�����!
	
		@see    
	*/
	class  VGT_EXPORT FileHandleVtr : public vgAsyn::FileHandle
	{
	public:
		FileHandleVtr( String filepath )
			: FileHandle( filepath )
		{
			_nextHandle = NULL;
			_type = FILEHANDLE_TYPE_VTR;
		}
		virtual ~FileHandleVtr()
		{
			close();
		}
	

	public:

		FileHandleVtr* getNextFileHandle()
		{
			return _nextHandle;
		}

		virtual bool exist( const String& metafilename );

		virtual vgKernel::StringVector* listMetaFileNames();

		virtual bool open();

		virtual void close();

		
		virtual bool extractToMultipleFiles( const String& save_multiple_file_path );


		ElevationItem* getElevationItem( const String& metafilename );

		VtrHeaderPtr getHeader() const
		{
			assert( _vtrFileHeader.isNull() == false );
			return _vtrFileHeader;
		}

		ElevationItemMap* getElevationItemsPtr()
		{
			return &_elevItems;
		}

		// �õ�Y������scale,ͨ��������������Ż���Ⱦ
		float getMaxScaleY();

		/**
			�������һ��vtr�ļ�, ���Դ�
		*/
		FileHandleVtr* openNextVtrFile();

		String getNextFilePath();

		String getTerrainName() const
		{
			assert( _vtrFileHeader.isNull() == false );
			return _vtrFileHeader->name;
		}

	private:

		void dropItemsData();

	private:

		ElevationItemMap _elevItems;
	
		VtrHeaderPtr _vtrFileHeader;
		FileHandleVtr* _nextHandle;
	};
	

	typedef std::map<String, FileHandleVtr*> FileHandleVtrPtrMap;
	typedef std::pair<FileHandleVtrPtrMap::iterator , bool >  FileHandleVtrPtrMapInsertRes;

	
}// end of namespace vgTerritory
	


#endif // end of __VGTERRITORY_TFILEHANDLEVTR_H__