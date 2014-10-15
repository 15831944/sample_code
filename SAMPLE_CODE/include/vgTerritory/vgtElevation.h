


#ifndef __VGTERRITORY_TELEVATION_H__
#define __VGTERRITORY_TELEVATION_H__


#include <vgTerritory/vgtDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgTerritory/vgtFileDefinitionVtr.h>
#include <vgAsyn/vgaFileInfo.h>
	
namespace vgTerritory {

	class ElevationItem;

	/**
		@date 	2008/09/08  19:24	
		@author  leven
	
		@brief 	�൱��vgMesh�е�Mesh��.

		�����﷢��IoRequest
	
		@see    
	*/
	class  VGT_EXPORT Elevation
	{
	public:
		Elevation( ElevationItem* father , 
			VtrBrief* brief , 
			vgAsyn::FileHandle* pfilehandle , int lod );

		~Elevation();

	public:

		const int getReferenceCount() const
		{
			return _referenceCount;
		}

		/**
			��ö�ȡ����IoRequest,ע��,��ͬ��ģʽ.
			Ŀǰֻ���ڱ������, �ͻ��˸���ɾ������
			_deleteBufferWhenDestruction��true��.�û������ֶ����á�
		*/
		vgAsyn::IoRequest* getIoRequest();

		void addReference();

		void releaseReference();

		ElevationItem* getFatherElevItem()
		{
			return _fatherItem;
		}

		vgAsyn::FileInfo* getFileInfoClone()
		{
			assert( _lodInfo != NULL );
			return _lodInfo->clone();
		}

		vgAsyn::FileInfo* getFileInfoPtr()
		{
			return _lodInfo;
		}

		const GLuint getHeightMapID() const
		{
			return _heightMapID;
		}

		const int getLod() const
		{
			return _lod;
		}

		/**
			ͨ���ڴ��е�buffer����,���Դ�.ע��
			buf_size�����ֽ�Ϊ��λ.
		*/
		bool setHeightMapID( void* buffer,  long buf_size );

		bool isEmptyHeightMap() const
		{
			return _heightMapID == VG_NULL_TEXTURE_ID;
		}

		/**
			���������Դ���ʱ,���Ի��buffer��ָ��.
			��֮,������NULL.
		*/
		float* getHeightMapBuffer();

		/**
			ע���ElevationItem��getHeightMapWidth������.
		*/
		long getBufferHeightMapWidth() const
		{
			assert( _heightMapBufWidth != 0 );
			return _heightMapBufWidth;
		}

		/**
			 �ѵ�ǰ��buffer���������dump���Դ�.
			 ע��,��_heightMapID == VG_NULL_TEXTURE_IDʱ
			 ����bufferΪ��ʱ,ֱ�ӷ���false
		*/
		bool dumpBufferToHeightMap();

	private:

		/**
			�����ύIoRequest��ʱ����Ҫ
		*/
		ElevationItem* _fatherItem;

		int _lod;

		vgAsyn::FileInfo* _lodInfo;

		int	 _referenceCount;

		/**
			�洢�߶�ͼ����Ϣ
		*/
		GLuint _heightMapID;

		float* _heightMapBuffer;
		GLint _heightMapBufWidth;
		GLint _heightMapBufHeight;

	};
	
	
}// end of namespace vgTerritory
	


#endif // end of __VGTERRITORY_TELEVATION_H__