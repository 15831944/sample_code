





#ifndef __VGITEXTUREIOREQUESTDISPATCHER_H__
#define __VGITEXTUREIOREQUESTDISPATCHER_H__

#include <vgImage/vgiDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSingleton.h>
#include <vgKernel/vgkTrace.h>
#include <vgImage/vgiTexture.h>

#include <vgKernel/vgkRenderCommandManager.h>

namespace vgImage {

	class IoRequest;
	class IoRequestTextureLod;

	/**
		@date 	2008/06/20  15:39	
		@author  leven
	
		@brief 	
			�൱��IoRequest�Ĺ�����.

			1.����texture������,�����ظ�������.
			2.����vgm������.
	
		@see    
	*/
	class  VGI_EXPORT TextureIoRequestDispatcher : 
		public vgKernel::Singleton<TextureIoRequestDispatcher>
	{
		typedef std::set<Texture*>	TexturePipe;
		typedef std::pair<TexturePipe::iterator, bool> TexPipeInsertResult;


		friend class vgKernel::Singleton<TextureIoRequestDispatcher>;
	private:
		TextureIoRequestDispatcher();

		~TextureIoRequestDispatcher();

	protected:
		virtual bool initialise();

		virtual bool shutdown();

	public:

		virtual void reset(){};//need to write

		bool initialize()
		{
			return true;
		}

		bool addTextureUpdate( Texture* texture );

		// set_asyn_mode == true �첽ģʽ
		void setAsynMode( const bool& set_asyn_mode );

		void dealWithUpdateInfoEveryFrame();

	private:

		void dealWithAllTextures();
		void dealWithTexture( Texture* curtex );
		bool loadTextureAndSendIoRequest( Texture* texture , const bool& asyn_mode);
		void unloadTexture( Texture* texture );


	private:
	
		// true --> �첽ģʽ
		bool _asynMode;

		TexturePipe _texPipe;

		vgKernel::RenderCommand *pTexIoCommand;
	};
		
	
	
	
}// end of namespace vgImage

#endif // end of __VGITEXTUREIOREQUESTDISPATCHER_H__