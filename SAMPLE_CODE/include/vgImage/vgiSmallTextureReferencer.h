


#ifndef __VGISMALLTEXTUREREFERENCER_H__
#define __VGISMALLTEXTUREREFERENCER_H__
#include <vgImage/vgiDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgImage/vgiDefinition.h>

	
namespace vgImage {


	class TextureItem;
	class TextureObserver;

#define  VGI_LOADFILESIZETHRESHOLD	VGI_CONF_SMALLIMGREF_LOADTHRESHOLD_DEFAULT

	/**
		@date 	2009/04/28  10:00	
		@author  leven
	
		@brief 	����С�͵�textureһ�������뼴�ɡ�
	
		@see    
	*/
	class VGI_EXPORT SmallTextureReferencer
	{
		//typedef std::vector<TextureItem*> TextureItemPtrVector;
		typedef std::vector<TextureObserver*> TextureObserverPtrVector;
	public:
		SmallTextureReferencer();
		~SmallTextureReferencer();

	public:
	
		bool filterSmallTextures( TextureItem* itm );

		// unref����ɾ���������е�textureitm
		void unRefAllSmallTextures();
	
		

		int getLoadFileSizeThreshold() const
		{
			return _loadFileSizeThreshold;
		}

		void setLoadFileSizeThreshold( int new_threshold )
		{
			assert( new_threshold > 0 );
			_loadFileSizeThreshold = new_threshold;
		}

		const bool getFilterEnable() const { return _filterEnable; }
		bool getFilterEnable() { return _filterEnable; }
		void setFilterEnable( const bool& val ) { _filterEnable = val; }

	private:

		bool checkExist( TextureObserver* itm );

	private:
	
		// ��Ϊ���붼��һ���Եģ��������α���������Ч����ʧҲ����
		TextureObserverPtrVector _smallTextures;

		// �Ƿ�addref���ļ���С�����ֽڼ���
		// ����д��vgConf
		int _loadFileSizeThreshold;
	
		bool _filterEnable;


	};
	
	
}// end of namespace vgImage
	


#endif // end of __VGISMALLTEXTUREREFERENCER_H__