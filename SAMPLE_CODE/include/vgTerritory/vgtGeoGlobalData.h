




#ifndef __VGTGEOGLOBALDATA_H__
#define __VGTGEOGLOBALDATA_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSystemUtility.h>
#include <vgKernel/vgkImageRect.h>
#include <vgTerritory/vgVgtrFile.h>
#include <vgTerritory/vgtDefinition.h>
#include <vgThreads/CriticalSection.h>
#include <vgThreads/ScopedLock.h>

	
namespace vgTerritory {


	using namespace vgKernel;
	using namespace vgThreads;



	/**
		@date 	2008/09/22  14:29	
		@author  leven
	
		@brief 	
	
		@see    
	*/
	class GeoGlobalData
	{
	public:
		GeoGlobalData();
		~GeoGlobalData()
		{
			
		}
	public:

		bool isValid() const;

		void setValid( const bool& isvalid )
		{
			_isUpdated = isvalid;
		}

		String getDetails();

		void pushNeedToCleanFile( const String& filename );
		void pushNeedToCleanFile( const vgKernel::StringVectorPtr namelist );

		void cleanTempFiles();

		VgtrHeaderPtr getVgtrHeaderPtr();


	public:

		// ע��,��������ɺ�,�����־��false
		bool _genDebugImages;

		bool _genElevationOverview;
		bool _genImageOverview;

		bool _clearTempData;

		/**
			�������Ƿ������½�.
			��Ϊtrue,��ʾ�����½�.��ʱ_globalUpperLeftY > _globalLowerRightY
			��Ϊfalse,��ʾ�����Ͻ�,��ʱ_globalUpperLeftY < _globalLowerRightY
		*/
		bool _axisLeftDown;


		double _globalUpperLeftX;
		double _globalUpperLeftY;

		double _globalLowerRightX;
		double _globalLowerRightY;

		double _globalPixelSizeX;
		double _globalPixelSizeY;

		// ע��,��С�ĸ߶�ֵ���ᵱ���߼�ԭ��ĸ߳�offset
		double _globalMinElevation;
		double _globalMaxElevation;

		// Ҫ����GPU��scaleֵ. 
		float	 _globalElevationScale;

		// minPosӦ��Ϊ(0,0)
		ImageRect _globalRect;

		// ����2��n�η�ȡ�����rect
		ImageRect _globalAdjustRect;

		// ȫ�ֱ���tgaʱ����ʱĿ¼
		String _globalTempPath;

		// ȫ�����Ŀ¼
		String _globalOutputPath;

		/**
			һ��ΪN=8ʱ,256
		*/
		long _globalChunkSize;
		long _globalN;
		
		// ��Ϊ��γ������ʱ,���ֵ��Ҫ�ֶ�����
		float _globalAdjustCellsize;


		String _terrainName;

		// ���������Ķ�Ӧ��ϵ
		long _globalCellTexSize;

		// ���ﶼ�Ǽ���õľ���·��
		String _demDirectory;
		String _debugImageDirectory;
		String _imageDirectory;


		long _maxLayerNum;

		uchar _filledColor[4];

		uchar _filledBlackColor[4];

	private:

		CriticalSection _sect;

		vgKernel::StringVector _needToCleanFiles;

		bool _isUpdated;
	};
	
	
}// end of namespace vgTerritory
	


#endif // end of __VGTGEOGLOBALDATA_H__