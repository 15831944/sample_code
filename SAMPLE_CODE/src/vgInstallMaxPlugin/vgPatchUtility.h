



#ifndef __VGPatchUtility_H__
#define __VGPatchUtility_H__



	/**
		@date 	2009/11/23  10:39	
		@author ZhouZY
	
		@brief 	Ϊʹ�˹��߲�������vgLibrary���Ŀ���Ӻ��Ŀ�����ȡ�����Ľӿ�
	
		@see    
	*/

using namespace std;

typedef string String;
typedef vector<string> StringVector;

namespace	vgUtility
{
	class PatchUtility
	{
	public:
		PatchUtility() {}
		~PatchUtility() {}
	public:
		static void toLowerCase( String &str );
		static String getCurrentProcessPath();
		static String getCurrentProcessName();
		static String getFileExtension( const String &filename );
		static String getPathFromAbsoluteFilename( const String &abs_filename );

		static bool copyFile( const String &src_abs_filename, const String &dst_abs_filename );
		static bool createDirectory( const String &dir_path );

	};


	//----------------------------------------------------------------
}// namespace vgUtility


	


#endif // end of __VGPatchUtility_H__
