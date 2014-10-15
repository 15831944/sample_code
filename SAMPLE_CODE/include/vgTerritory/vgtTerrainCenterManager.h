



#ifndef __VGTERRITORY_TTERRAINCENTERMANAGER_H__
#define __VGTERRITORY_TTERRAINCENTERMANAGER_H__


#include <vgTerritory/vgtDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSingleton.h>

#include <vgTerritory/vgtFileHandleVtr.h>
#include <vgImage/vgiFileHandleVgi.h>
#include <vgCam/vgcaCamManager.h>
#include <vgTerritory/vgtTerrainCenter.h>


	
namespace vgTerritory {

	/**
		@date 	2008/09/08  10:40	
		@author  leven
	
		@brief 	ע��,����һ��Ҫ��opengl����֮ǰ.
		����������Ҫ��TerrainManager��
	

		@see    
	*/
	class  VGT_EXPORT TerrainCenterManager : 
		public vgKernel::Singleton<TerrainCenterManager>
	{
		friend class vgKernel::Singleton<TerrainCenterManager>;
	private:
		TerrainCenterManager();

		~TerrainCenterManager()
		{
			VGK_TRACE(_T("TerrainCenterManager destroyed."));
		}

	protected:
		virtual bool initialise();
		virtual bool shutdown()
		{
			assert( _centerLayers.empty() == true && 
				"uninitBeforeOpenGLDestroyû��ִ��" );
			return true;
		}

	public:

		virtual void reset(){};

		/**
			������������㼶.
			����ˢ���޸�.
		*/
		bool reloadAllCenters();

		bool clearData();

		bool generateCenters( FileHandleVtr* vtrfilehandle );


		bool uninitBeforeOpenGLDestroy();
	
		/**
			��ÿ֡�и���centers.
			��Ҫ����TerrainManager��render��
			height_render_flag �Ǹ��ݸ߶�������centers.
			Ϊ0ʱ,ȫ������.
			Ϊ1ʱ,center=0������,��������,��������
		*/
		void updateCentersInEveryFrame( const int& height_render_flag );

		/**
			ͨ����ǰ��layer�����õ�TerrainCenter
			ע��,0����������һ��.
			����ע��,�������layer����layer��sizeʱ,���ص�
			����󼶵�center.
		*/
		TerrainCenter* getSuitableCenter( const int& layer );

		long getCurrentHeightLayerNumber() const
		{
			return _currentHeightLayer;
		}

		/**
			��ȡ��ǰ���в������е�items. ����filter
		*/
		ElevationItemVector getContainedElevationItems();

		/**
			��Elevation��buffer dump��opengl�Դ���
		*/
		void dumpElevationBufferToHeightMap();


	private:
	
		vgCam::CamManager* _camMgr;

		TerrainCenterPtrVec _centerLayers;

		// ��ǰ��Ҫ��Ⱦ��layer�߶�.
		// ��Ϊ0ʱ,��ʾ��Ⱦ�ϸһ��.
		long _currentHeightLayer;
	};
	
	
}// end of namespace vgTerritory
	


#endif // end of __VGTERRITORY_TTERRAINCENTERMANAGER_H__