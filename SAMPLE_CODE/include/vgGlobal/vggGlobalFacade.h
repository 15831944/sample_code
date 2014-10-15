

#if 0

#ifndef __VGGGLOBALFACADE_H__
#define __VGGGLOBALFACADE_H__
#include <vgGlobal/vgglDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSingleton.h>
#include <vgKernel/vgkTrace.h>


namespace vgGlobal {

	/**
	@date 	2008/08/05  19:31	
	@author  xy

	@brief 	ϵͳ���ܽӿڶ���

	@see    
	*/
	/**
	@action		change
	@date 		2009/04/22  15:30	
	@author  	lss

	@brief	���vg��mod�������;vg��д�ӿ�ת�Ƶ�class vgGlobal::MeshUtility

	@see    
	*/
	class VGGL_EXPORT GlobalFacade : public vgKernel::Singleton<GlobalFacade>
	{
		friend class vgKernel::Singleton<GlobalFacade>;
	private:
		GlobalFacade() 
			: Singleton<GlobalFacade>( VGK_SINGLETON_LEFE_GLOBALFACADE ) 
		{
			VGK_TRACE(_T("GlobalFacade created."));
			setDefault();
		}
		~GlobalFacade()
		{
			VGK_TRACE(_T("GlobalFacade destroyed."));
			reset();
		}

	protected:

		virtual bool initialise()
		{
			return true;
		}

		virtual bool shutdown()
		{
			return true;
		}

	public:
		void	setDefault()
		{
		}
		void	reset()
		{
		}

	public:

		/**
		����vgi�ļ�
		*/
		void importImageFile();

		/**
		ֻ����vgm�ļ�
		*/
		void saveModelFileOnly();

		/**
		���ڱ���һ��vgm��vgi
		*/
		void saveModelAndImage();

		/** ���� add by lss
		���ζ�дvgm��vgi
		*/
		bool saveModelAndImage(String savePath);

		bool importModelFileOnly( String VgmPathName);
		bool importImageFileOnly(String VgiPathName);
		void importModelAndImage(String readPath);

		

		bool	importVtrFile( const String& file_name);

		/*bool	import3DMusicFile( const String& file_name);

		bool	importBackgroundMusicFile( const String& file_name);*/

		bool	importVmdFileOnly( const String& file_name);
	};

}// end of namespace vgGlobal



#endif // end of __VGGGLOBALFACADE_H__

#endif