



#ifndef __VGTERRITORY_MESHCHUNKLEFTUP_H__
#define __VGTERRITORY_MESHCHUNKLEFTUP_H__


#include <vgTerritory/vgtDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgTerritory/vgMeshVertex.h>
#include <vgKernel/vgkSharePtr.h>
#include <vgTerritory/vgtTerrainCenter.h>
#include <vgTerritory/vgMeshDrawingData.h>
#include <vgCam/vgcaCamManager.h>
#include <vgTerritory/vgtElevationBound.h>

namespace vgTerritory {

	/**
		@date 	2008/04/17  19:01	
		@author  leven
	
		@brief 	
	
		@see    
	*/
	class MeshChunkLeftUp;

	class MeshChunkLeftUpData
	{
		friend class MeshChunkLeftUp;
	public:
		MeshChunkLeftUpData();
		~MeshChunkLeftUpData();
	
	

		//------------------------------------------
		// xnum��znum�ֱ�Ϊx���z�������,ע��!Ϊ2^n+1
		// xlength��zlength�ǳ���
		// centerx, centery, centerz�ֱ��������������ĵ�
		//------------------------------------------
		bool createFromParameters(
			const int& xnum,
			const int& znum , const float& xlength , const float& zlength,
			const float& centerx,const float& centery , const float& centerz);


		// rotate_anticlockwise ��ʱ����ת
		// ȡֵ: { 0, 90 , 270 , 180 };
		bool renderLargeArea( const int& texcenterx , 
			const int& texcenterz , 
			ElevationItem* leftup , 
			ElevationItem* leftdown ,
			ElevationItem* rightup ,
			ElevationItem* rightdown ,
			const bool& renderGaps );

	private:

		void setDefault();
		void destroy();

		void destroyVboData();
		void destroyMemData();
		void createMeshImpl();

	private:

		bool _initialised;


		int __xnum ;
		int __znum ;
		float __xLength ;
		float __zLength ;
		float __beginx ;
		float __beginy ;
		float __beginz ;

		int __vertCount;
		MeshVertex *__vertices;
		unsigned int __indexsCount ;
		unsigned int __indexsCount2;
		short unsigned int* __indexs;
		short unsigned int* __indexs2;

		GLuint __vertexBufferID;

		// ����������indexbuffer
		GLuint __indexBufferID;
		// ��������ȱ�ڵ�indexbuffer
		GLuint __indexBufferID2;


		float __nx ;
		float __nz ;


		static int _face2x2num;
		static int _face1x1num;

		static GLvoid*** _indicesTable;
		static GLsizei** _countTable;
		static GLsizei*	_wholeCount;

		MeshDrawingData _drawingData[4];

		//------------------------------------------
		// ����ѡ��
		//------------------------------------------
		static void buildIndicesTables( const int& face2x2num , 
			GLvoid*** &outIndicesTable , GLsizei** &outCountTable );

		static void getDrawingDataFromTable( const int& in_x , 
			const int& in_y , MeshDrawingData& leftup ,
			MeshDrawingData& rightup , MeshDrawingData& leftdown ,
			MeshDrawingData& rightdown );


	private:

		static void insertLeftUpFaceindex( int facexnum , 
			std::vector<short unsigned int>& outvec );

		static void insertFaceindex( int facei2x2 , int facexnum , 
			std::vector<short unsigned int>& outvec , const bool& insertemptyindex ,
			const bool& generate_gaps );

		static void insertSingleFace( int i, int facexnum, 
			std::vector<short unsigned int> &outvec ,const bool& generate_gaps );
	
	};

	typedef vgKernel::SharePtr<MeshChunkLeftUpData> MeshChunkLeftUpDataPtr;


	class MeshChunkLeftUp
	{
	public:
		MeshChunkLeftUp(MeshChunkLeftUpDataPtr data ,
			float stepsize,
			float offsetx , float offsetz ,
			float offset_unit, float scale , int rotate , 
			TerrainCenter* center,
			float* curposx , float* curposy , 
			float* curposz );
		~MeshChunkLeftUp();
	
	
		void update( const int& transX , const int& transZ );

		void render( const int& transX , const int& transZ );

		inline void setRenderGapsFlag(const bool& if_render )
		{
			_renderGaps = if_render;
		}
	
	private:
	
		ElevationBound _sceneBound;
		ElevationBound _tmpBound;

		float _stepSize;

		bool _renderGaps;

		float* _pCurrentPosX;
		float* _pCurrentPosY;
		float* _pCurrentPosZ;

		ElevationItem** _ppTexBlockLeftUp;
		ElevationItem** _ppTexBlockLeftDown;
		ElevationItem** _ppTexBlockRightUp;
		ElevationItem** _ppTexBlockRightDown;


		vgCam::ViewFrustum* _viewFrustum;



		TerrainCenter* _terrainCenter;


		int _centerX;
		int _centerZ;

		// ��Сoffset��λ
		float _offsetUnit;
		float _scale;

		float _offsetX;
		float _offsetZ;


		int _rotate;

		MeshChunkLeftUpDataPtr _data;
	
	};
	
	
	
	
}// end of namespace vgTerritory

#endif // end of __VGTERRITORY_MESHCHUNKLEFTUP_H__