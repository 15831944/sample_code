



#ifndef __VGIIMAGEMANAGER_H__
#define __VGIIMAGEMANAGER_H__
#include <vgImage/vgiDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSingleton.h>
#include <vgKernel/vgkTrace.h>
#include <vgImage/vgiFileHandleVgi.h>
#include <vgAsyn/vgaFileInfo.h>


	
namespace vgImage {

	/**
		@date 	2008/06/23  21:33	
		@author  leven
	
		@brief 	ע��,Ŀǰֻ�Ƕ�vgi�ķ�װ.
		vgm��filehandle��������meshManager��.

		�Ժ������Ҫͳһ����.
	
		@see    
	*/
	class  VGI_EXPORT ImageManager : public vgKernel::Singleton<ImageManager>
	{
	public:
		typedef std::map<String, FileHandleVgi*> FileHandleVgiMap;
	private:
		typedef std::pair<FileHandleVgiMap::iterator, bool> FileHandleMapInsertResult;

		friend class vgKernel::Singleton<ImageManager>;
	private:
		ImageManager() 
			: Singleton<ImageManager>( VGK_SINGLETON_LEFE_IMAGEMGR ) 
		{
			VGK_TRACE(_T("ImageManager created."));
		}
		~ImageManager()
		{
			VGK_TRACE(_T("ImageManager destroyed."));
		}

	protected:

		virtual bool initialise()
		{
			return true;
		}

		virtual bool shutdown();

	public:

		void reset(){ clearAllFileHandles(); }

		void clearAllFileHandles();

		/**
			����Ҫע��,��vgAsyn:TextureManager��û�������Ӧ������,��Ҫ�ֶ����!
		*/
		FileHandleVgi* addImageAndAssociateWithIoServer( const String& filepathname );

		// Ҫȷ��handle�Ѿ�������!!
		FileHandleVgi* addImageAndAssociateWithIoServer( FileHandleVgi* handle );


		bool exist( const String& metafilename );
		
		ImageMiniature* getImageMiniature( const String& metafilename );

		// utility
		void extractVgiToMultipleFiles();
		void extractVgiToMultipleFiles(const String& vgimg_path, 
			const String& save_multiple_files_path );


		//bool deleteFileHandleVgi( FileHandleVgi* filehandle )
		//{

		//}

		FileHandleVgi* getFileHandleVgi( const String& vgipath );

		FileHandleVgiMap* getFileHandleVgiMap()
		{
			return &_filehandles;
		}

	private:

		bool addFileHandle( FileHandleVgi* filehandle );
	
		FileHandleVgiMap _filehandles;
	};

	
	
}// end of namespace vgImage
	


#endif // end of __VGIIMAGEMANAGER_H__