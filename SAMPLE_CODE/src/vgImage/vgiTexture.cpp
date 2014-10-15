



#include <vgStableHeaders.h>
#include <vgImage/vgiTexture.h>
#include <vgImage/vgiImageMiniature.h>
#include <vgImage/vgiIoRequestTextureLod.h>
#include <vgImage/vgiTextureIoRequestDispatcher.h>
#include <vgKernel/vgkOpenGLSupport.h>


namespace vgImage {
	
	
	//----------------------------------------------------------------
	Texture::Texture( String texname , int lod ,
		ImageMiniature* img )
	{
		assert( img != NULL );

		_imgMini = img;
		_lod = lod;

		_texName = texname;

		_referenceCounter = 0;
		_textureID = VG_NULL_TEXTURE_ID;
	}
	//----------------------------------------------------------------
	Texture::~Texture()
	{
		unloadTextureData();
	}
	//----------------------------------------------------------------
	//----------------------------------------------------------------
	void Texture::unloadTextureData()
	{
		if ( _textureID != VG_NULL_TEXTURE_ID )
		{
			glDeleteTextures( 1, &_textureID );
			VGK_CHECK_OPENGL_ERROR();

			_textureID = VG_NULL_TEXTURE_ID;
		}

	}
	//----------------------------------------------------------------
	void Texture::addReference()
	{
		assert( _referenceCounter >= 0 );
		++_referenceCounter;

		// ���ʱ��˵���ǵ�һ���������
		if( _referenceCounter == 1 )
		{
			// ���ʱ������Ϣ���뼴��!!
			TextureIoRequestDispatcher::getSingleton().addTextureUpdate(
				this );
		}
	}
	//----------------------------------------------------------------
	void Texture::releaseReference()
	{
		if ( _referenceCounter <= 0 )
		{
			assert(0);
			return;
		}

		assert( _referenceCounter > 0 );
		--_referenceCounter;

		if( _referenceCounter == 0 )
		{
			// ���ʱ������Ϣж�ؼ���!!
			TextureIoRequestDispatcher::getSingleton().addTextureUpdate(
				this );
		}
	}
	//----------------------------------------------------------------
	vgAsyn::IoRequest* Texture::getIoRequest()
	{
		vgAsyn::FileInfo* fileinfo = _imgMini->getFileInfoByLod( _lod );

		if ( fileinfo == NULL )
		{
			return NULL;
		}

		return new IoRequestTextureLod( this, fileinfo );
	}
	//----------------------------------------------------------------
	bool Texture::createTextureByBuffer( char* buffer  , long size  )
	{
		assert( _textureID == VG_NULL_TEXTURE_ID );

		if ( _textureID != VG_NULL_TEXTURE_ID )
		{
			return false;
		}

		GLuint texid = _imgMini->createTextureID(
			buffer , size , _lod );

		assert( texid != VG_NULL_TEXTURE_ID );

		setTextureID( texid );

		return true;
	}
	//----------------------------------------------------------------




}// end of namespace vgImage
