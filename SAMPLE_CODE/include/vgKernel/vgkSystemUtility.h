

#ifndef __VGKSYSTEMUTILITY_H__
#define __VGKSYSTEMUTILITY_H__



#include <vgKernel/vgkForward.h>

	
namespace vgKernel {

	/**
		@date 	2008/08/01  19:34	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	
	class  VGK_EXPORT SystemUtility
	{
	private:
		SystemUtility()
		{
			
		}
	public:

		~SystemUtility()
		{
			
		}
	
	public:

		static String getCurrentProcessName();
		static String getCurrentProcessPath();

		static String getSystemTempPath();

		static bool getUUID( String& str ); 

		static String getUUIDStr();
	
		static bool checkFileExist( const String&  abs_filename );

		static bool copyFile( const String& src_abs_filename , 
			const String& dst_abs_filename );
	
		static bool moveFile( const String& src_abs_filename ,
			const String& dst_abs_filename );

		static bool removeFile( const String& abs_filename , 
			const bool& alert );


		static String getCurrentDirectory();

		static bool setCurrentDirectory( const String& filepath );

		// ���Ŀ¼�Ƿ��Ѵ���
		static bool checkDirectoryExist( const String& dir_path );

		// ������Ŀ¼
		static bool createDirectory( const String& dir_path );

		// ɾ����Ŀ¼
		static bool removeEmptyDirectory( const String& dir_path );

		// ����Ŀ¼
		static bool copyDirectory( const String& des_path, const String& src_path);

		// ɾ��Ŀ¼
		static bool removeDirectory( const String& dir_path );

		// �������������û�����,ע��,�����������256
		static String createInputBox( const String& title, 
			const String& content , 
			bool use_multiline = false );


		// ��ѡ�ļ��Ի���
		static StringVector	getFilesOpenByDialog( const StringVector& fileExts );

		static String	getFileSaveByDialog( const StringVector& fileExts );

		static bool	getFilesFromDirectory( const String& dir_path,
			String&	filesPathOut );

		static void	showModelDialog( const String& msg );

		static int	talkWithModelDialog( const String& msg );
	
		static String getStringByID(int stringID);
	
#ifndef VGK_NO_SUPPORT_MFC
		static CWnd* getCurrentFrameWnd();
		static CWnd* getCurrentViewWnd();
#endif

	private:

		static bool isRoot(LPCTSTR lpszPath);
	};
	
	
	

}// end of namespace vgKernel



#endif // end of __VGKSYSTEMUTILITY_H__