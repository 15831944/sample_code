


#ifndef __VGTGEOIMAGEBLOCK_H__
#define __VGTGEOIMAGEBLOCK_H__

#include <vgKernel/vgkForward.h>
#include <vgTerritory/vgtGeoGlobalData.h>
#include <vgImage/vgiCximageWrapper.h>
#include <vgTerritory/vgVgtrFile.h>
#include <vgTerritory/vgTerrainHugeImageUtil.h>

#include <vgTerritory/vgtHugeProcessHugeJobs.h>
#include <vgTerritory/vgtHugeProcessPieceJob.h>

#include <ximage.h>

	
namespace vgTerritory {


	using namespace vgImage;

	/**
		@date 	2008/09/23  15:28	
		@author  leven
	
		@brief 	
	
		@see    
	*/
	class GeoImageBlock
	{
	public:

		/**
			block_i��block_j�ֱ��Ƕ�Ӧ��block��ȫ���е�λ��.
			�����ļ����õ�level��Ϣ��block_i, block_j
		*/
		GeoImageBlock( GeoGlobalData* global_data , 
			String imgpath);

		
		~GeoImageBlock();
 
	public:

		bool open();

		/**
			��ͼ��Сһ��.
		*/
		bool resize();

		
		//fileBlock* getFileBlock();

		/**
			����Ƿ���ȫ�յ�ͼ��.
		*/
		bool isEmpty();
		
		fileBlock* getFileBlock();

		String getFilePath() const
		{
			return _imgFilepath;
		}

	private:

		void setDefault();
	
	private:

		bool _isOriginImage;
	
		long _level;

		long _width;
		long _height;



		long _blockPosX;
		long _blockPosY;

		String _imgFilepath;

		GeoGlobalData* _globalData;
	
	};


	class GeoImageResizeJob : public HugeProcess::PieceJob
	{
	public:
		GeoImageResizeJob( GeoImageBlock* img_block , 
			HugeProcess::HugeJobs* boss )
			: PieceJob( boss )
		{
			_imgBlock = img_block;
		}
		virtual ~GeoImageResizeJob()
		{
			
		}
	
	
		virtual void doJob()
		{
			if ( _boss->getNotifier() != NULL )
			{
				_boss->getNotifier()->setCurrentProcessPos( _imgBlock->getFilePath() , 
					_boss->getCounter()->getCount() );
			}

			_imgBlock->resize();
		}

	private:
	

		GeoImageBlock* _imgBlock;
	
	};
	

	typedef std::vector<GeoImageBlock*> GeoImageBlockPtrVec;

	
	
}// end of namespace vgTerritory
	


#endif // end of __VGTGEOIMAGEBLOCK_H__