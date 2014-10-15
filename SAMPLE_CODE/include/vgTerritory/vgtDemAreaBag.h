


#ifndef __VGTERRITORY_TDEMAREABAG_H__
#define __VGTERRITORY_TDEMAREABAG_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSharePtr.h>
#include <vgTerritory/vgtDemArea.h>
#include <vgkernel/vgkStringPairVector.h>
#include <vgTerritory/vgTerrainHugeImageUtil.h>
#include <vgKernel/vgkProcessObserver.h>

	
namespace vgTerritory {


	using namespace vgKernel;


	

	/**
		@date 	2008/08/26  16:53	
		@author  leven
	
		@brief 	
	
		@see    
	*/
	class DemAreaBag : public vgKernel::ProcessNotifier
	{
	public:
		DemAreaBag();
		virtual ~DemAreaBag();
	

		bool readFromFilePairs( const StringPairVector& inputlist );
	

		/**
			layer����ָ���㼶.
			layer == 0 ʱ,�Ƿ����ϸ��һ��
		*/
		fileBlockVectorPtr getFileBlockVec();

		bool isNeedToResizeImg();

		long getScaleOfDemToImg()
		{
			return _scaleOfDemToImg;
		}

		bool resizeImages( const long& new_scale , 
			const String& absolute_temp_folder );

		/**
			ִ�д˺���һ��,resizeͼ��1/4�Ĵ�С,����������һ��.
		*/
		bool resizeImageForLod( const String& absolute_temp_folder );

		/**
			����Ϊʲô����ddsfilenames���б�?��ΪĳЩ��Ӧ������û������
			��dds�б�����޳���Щ����
		*/
		bool saveToVtr( const vgKernel::StringVectorPtr ddsfilenames ,
			const String& terrain_name ,
			const String& absolute_output_filepath , const long& N );

		void clearTempFiles();

		/**
			 ������������Ҫ�Ĳ㼶�ĺ���.
			 ���ص�������dem rect���ܺ�,��������2��n�η�
			 ��ֵ��vgtr�е�widthһ��.
		*/
		long getDemWidth();

	private:
	

		DemAreaPtrVec* _dems;

		double _absOriginX;
		double _absOriginZ;
		float _cellsize;
		long _scaleOfDemToImg;
		
		vgKernel::StringVectorPtr _needToClearList;
	};

	typedef vgKernel::SharePtr<DemAreaBag> DemAreaBagPtr;
	
	
}// end of namespace vgTerritory
	


#endif // end of __VGTERRITORY_TDEMAREABAG_H__