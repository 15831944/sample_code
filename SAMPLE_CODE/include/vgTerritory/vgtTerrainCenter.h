


#ifndef __VGTERRITORY_TTERRAINCENTER_H__
#define __VGTERRITORY_TTERRAINCENTER_H__


#include <vgTerritory/vgtDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgTerritory/vgtElevationItem.h>
#include <vgTerritory/vgtFileHandleVtr.h>
#include <vgTerritory/vgtElevationManager.h>

	
namespace vgTerritory {


	#define VGT_USE16X16		0




	/**
		@date 	2008/09/08  10:53	
		@author  leven
	
		@brief 	������ÿһ���center,��������͸߶�����
	
		@see    
	*/
	class  VGT_EXPORT TerrainCenter
	{
	public:

		/**
			layer == 0ʱ,��ʾ�����ϸ��һ��.
		*/
		TerrainCenter( FileHandleVtr* vtrfile , const int& layer ,
			bool islastlayer );
		~TerrainCenter();

	public:

		float getLogicCenterX();

		float getLogicCenterZ();

		/**
			�ֱ����ĸ�block����ĵ�ǰָ�� 
			ʹ������Ķ���:
			const int VGT_LEFT_UP	 = 0;
			const int VGT_LEFT_DOWN = 1;
			const int VGT_RIGHT_UP = 2;
			const int VGT_RIGHT_DOWN = 3;
		*/
		union
		{
			struct
			{ 
				ElevationItem* _pTexBlockLeftUp;
				ElevationItem* _pTexBlockLeftDown;
				ElevationItem* _pTexBlockRightUp;
				ElevationItem* _pTexBlockRightDown;
			};

			ElevationItem* _curElevItem[ 4 ];
		};

#if VGT_USE16X16
		ElevationItem* _cornerItem[ 4 ];
		ElevationItem* _prevCornerItem[ 4 ];
		ElevationItem* _upItem[ 8 ];
		ElevationItem* _prevUpItem[ 8 ];
#endif

		/**
			ͨ������ĵ�ǰ��λ������������.
		*/
		void update( const float& curpos_x,
			const float& curpos_z );

		bool isLoadedFully();
	

		/**
			��������.
		*/
		bool reload();

	private:

		void updateImpl( const int& xblockpos , const int& zblockpos );

	private:

		bool _isLastLayer;

		ElevationManager* _elevMgr;


		ElevationItem* _prevElevItem[4];

		/**
			ÿһ��Ķ�Ӧ���.���������,Ϊ����
			��Ϊ��������,���Գ�����һ����
			���һ��,N=8,cellsize= 2.0f,��Ϊ256*2.0f = 512.0f;
		*/
		float _blockWidth;

		/**
			 ��ǰ��λ��,��Ϊ-1ʱ����Ҫ���µ�״̬.
			 ����������Ӧ�ö�����ֵ.
		*/
		int _curBlockPosX;
		int _curBlockPosZ;

		float _absCenterX;
		float _absCenterZ;

		String _prefix;

		FileHandleVtr* _vtrFilehandle;

		int		_curLayer;
	};
	

	typedef std::vector<TerrainCenter*> TerrainCenterPtrVec;
	
}// end of namespace vgTerritory
	


#endif // end of __VGTERRITORY_TTERRAINCENTER_H__