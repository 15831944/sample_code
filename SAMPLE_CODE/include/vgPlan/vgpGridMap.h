#pragma once

#include <vgKernel/vgkSharePtr.h>
#include <vgKernel/vgkVec2.h>
#include <vgKernel/vgkVec3.h>

#include <gdal/ogrsf_frmts.h>
#include <gdal/ogr_api.h>

//#define array_index(i, j, line) ( i*line + j)
using namespace vgKernel;

namespace vgPlan
{
	 
	struct GridStruct  
	{
		GridStruct()
		{
			ix = 0;
			iy = 0;
			px = 0;
			py = 0;
			s_sizex = 0;
			s_sizey = 0;

			availFlag = 0;
			
			availOnXHorizon = 0;
			availOnYHorizon = 0;
			availOnXVertical = 0;
			availOnYVertical = 0;

			usedBlockId = -1;
		}

		int ix;
		int iy;

		float px;
		float py;

		/*  
			0 - ��ǰ�����������ڣ�������
			1 - ��ǰ�����������ڣ���Ե�ǰblock��������վ����Ƿ��ܹ��ڷ�
			2 - ��ǰ�����������ڣ��Ѽ���������޷��ڷ�
			3 - ��ǰ�����������ڣ��Ѱڷ�����block
		*/
		int availFlag;

		int availOnXHorizon;
		int availOnYHorizon;
	
		int availOnXVertical;
		int availOnYVertical;

		int followOnXHorizon;
		int followOnYHorizon;

		int followOnXVertical;
		int followOnYVertical;

		int usedBlockId;

		static float s_sizex;
		static float s_sizey;
	};

	typedef vgKernel::SharePtr<GridStruct> GridStructSharedPtr;

	class GridMap
	{
	public:
		// ����դ�����Ͻǵ㣬դ���С��ˮƽ��ֵ�ֱ��ʳ�ʼ��GridMap
		GridMap();

		GridMap(Vec2 leftUpPoint, Vec2 gridSize, float xResolve, float yResolve);
		
		~GridMap();

		void initialise(Vec2 leftUpPoint, Vec2 gridSize, float xResolve, float yResolve);

		// ʹ��ǰ�������������õ�դ���趨0-1 flag
		void calculateAvailGrid(OGRGeometry *pGemo);


		// ��դ���ϰڷ�block
		int putBlockInGrid(int blockX, int blockY, int minAliasX, int minAliasY, int minAliasR);

		bool dumpToFile(string fileName = "d:\\GridDump.txt");

 		GridStruct* getGrid(int ix, int iy)
 		{
 			int index = array_index(ix, iy);
 
 			// assert (index < m_gridVec.size());
 			
 			return &m_gridVec[index];
 		}
		// ��Ⱦ����դ������ȷ�Լ��
		void render();

		vector<GridStruct>& getBlockGridVecRef()
		{
			return m_blockGridVec;
		}

	private:
		// �ڰڷ�ǰ��ʼ�����ָ�����դ��״̬��ɾ����ǰ�����
		void initGridBlockBeforePut();

		// �ָ�ix iy����û��ʹ�õ�դ���־����calculateGridBlock����.
		void prepareAvailGrid(int ix, int iy);

		// ʹ�ö�̬�滮��������İڷŷ�Χ
		bool calculateGridBlock(int ix=-1, int iy=-1);

		void cntAvailGrid();

		void quadCompute(int ix, int iy, int blockCntX, int blockCntY, OGRGeometry* pGemo);
		
		bool containGrid(int ix, int iy, int blockCntX, int blockCntY, OGRGeometry* pGemo);

		bool overlapeGrid(int ix, int iy, int blockCntX, int blockCntY, OGRGeometry* pGemo);
		
		inline int array_index(int i, int j)
		{
			return i * m_blockYNum + j;
		}

		// �û�����ɾ�����ص�Geometry.
		// inline OGRLinearRing* getRingsFormGrid(int ix, int iy, int blockCntX, int blockCntY);
		
		inline bool colorGrid(int ix, int iy, int blockCntX, int blockCntY, int flag, int blockId=-1);

		inline bool distanceAvail(int ix, int iy, int blockSizeX, int blockSizeY, int distInX, int distInY, int distInR);

	private:
		// vector<GridStructSharedPtr> m_gridVec;
		GridStruct *m_gridVec;
		
/*		GridStruct *m_oriGirdVec;*/

		int m_blockXNum;
		int m_blockYNum;

		vector<GridStruct> m_blockGridVec;
	};
}


