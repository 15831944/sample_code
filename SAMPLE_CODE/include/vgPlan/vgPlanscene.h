#ifndef  __VGPLANSCENE__
#define  __VGPLANSCENE__

#include <vector>

/*#include <vgPlan/vgPlanground.h>*/
//#include <vgPlan/vgPlanstreet.h>
//#include <vgPlan/vgPlanstreetLine.h>
// #include <vgPlan/vgShpPlanground.h>
// #include <vgPlan/vgpExcludeArea.h>
#include <vgPlan/vgpGridMap.h>
#include <vgPlan/vgpBuildingArea.h>
#include <vgplan/vgpDefinition.h>

namespace vgPlan
{


//	typedef vector<vgShpPlangroundPtr> ShpGroundPtrVec;
	typedef vgKernel::SharePtr<OGRGeometry> OGRGeometrySharedPtr;

	/*!
		@date    2009/10/30  10:38
		@author  zzy
		@action  modify
		@brief   �����Ϊ�����࣬���˵�DLL���� 
	*/
	class VGP_EXPORT vgPlanScene
	{
	public:

		vgPlanScene(string name);

		~vgPlanScene();

		// һ��plan���ܰ������Ground���������ɹ滮
		bool addGroundFromSelectedShp();

		bool addStreetFromSelectedShp();

		bool addExclusiveAreaFromSelectedShp();
		
		bool addBuildingAreaFromLayer(string layerName);

		void addBuildingAreaFromPointVec(const vector<vgKernel::Vec3>& vec3Vector, bool bRender);

		bool addGroundFromLayer(string layerName);
		
		bool addStreetFromLayer(string layerName, float roadWidth);

		bool addExculsiveAreaFromLayer(string layerName);

//		vgShpPlangroundPtr getLastGroundPtr();

		void render();
		
		void renderShadow();

		bool generateMap();
		
		bool caculateGrid(int resolveX, int resolveY);

		bool updateGrid();

// 		const ShpGroundPtrVec& getShpGroundPtrVec()
// 		{
// 			return m_groundList;
// 		}

		void setName(string name)
		{
			m_sceneName = name;
		}

		string getName()
		{
			return m_sceneName;
		}

		vector<string> getPlangroundNameVector()
		{
			
			return std::vector<string>();
		}

		// ������������������.
		void generatePlan(BlockParam blockParam, SunlightParam sunlightParam)
		{
			m_blockParam = blockParam;

			m_maxFloor = blockParam.maxHeight / blockParam.heightPerFloor;
			m_minFloor = blockParam.minHeight / blockParam.heightPerFloor;

			for (int i=0; i<m_buildingAreaList.size(); i++)
			{
				m_buildingAreaList[i]->generatePlan(blockParam, sunlightParam);
			}
		}

		void updateShadowData()
		{
			for (int i=0; i<m_buildingAreaList.size(); i++)
			{
				m_buildingAreaList[i]->updateShadowData();
			}
		}

		// �õ�ָ��¥��߶ȵ��ݻ���
		float getFloorAreaRatio(int floor);
		
		void matchFloorAreaRatio(int method);

		// �õ���͵ĸ߶�
		int getMinFloor() { return m_minFloor; }

		// �õ���ߵĸ߶�
		int getMaxFloor() { return m_maxFloor; }

		// ���õ�ǰ��ʾ�ķ����߶�
		void setCurrentSchemFloor(int floor);

		// �õ������block����
		BlockParam getBlockParam() { return m_blockParam; }

		// ���ɵķ������ж������
		int getNumOfBuilding(int floor);

		int getGridResolveX() { return m_gridResolveX; }

		int getGridResolveY() { return m_gridResolveY; }

		bool saveCurrentSchemToShpFile(string filename);

		void setBlockColor(float red, float green, float blue, float alpha)
		{
			m_outColorAlpha = alpha;
			m_outColorBlue = blue;
			m_outColorGreen = green;
			m_outColorRed = red;
		}

		float getBlockRed()
		{
			return m_outColorRed;
		}

		float getBlockBlue()
		{
			return m_outColorBlue;
		}

		float getBlockGreen()
		{
			return m_outColorGreen;
		}

		float getBlockAlpha()
		{
			return m_outColorAlpha;
		}

		bool onDeleteRenderer(vgKernel::Renderer *pRenderer)
		{
			for (int i=0; i<m_buildingAreaList.size(); i++)
			{
				if (m_buildingAreaList[i].getPointer() != NULL)
				{
					if (m_buildingAreaList[i]->onDeleteRenderer(pRenderer))
					{
						return true;
					}
				}
			}

			return false;
		}
		
		void updateGemoData();

		bool collectGeometries(vector<OGRGeometry*>* pOGRGeometriesOut );

		int	getCurrentFloor();

	private:
// 		bool addGroundFromSelectedShp();
// 
// 		bool addStreetFromSelectedShp();
// 
// 		bool addExclusiveAreaFromSelectedShp();

	private:
		// С����ͼ
		//vector<vgShpPlangroundPtr> m_groundList;

		// С����·ͼ
		//vector<vgPlanstreetLinePtr> m_streetLineList;
	
		// С���̻���
		//vector<vgExcludeAreaPtr> m_excldAreaList;

		// ��������ͼ
		vector<vgBuildingAreaPtr> m_buildingAreaList;

		vector<string> m_groundNameVec;

		vector<string> m_streetNameVec;

		vector<string> m_areaNameVec;

// 		GridMap m_gridMap;

//		vector<OGRGeometrySharedPtr> m_excldAreaList;
//		Street List
// 		vector<vgPlanstreet*> m_streetList;


//		vector<vgPlanstreetLine*> m_streetLineList;
	
// ��������ܰڷŵķ���.
/*		int **gridMap;*/

		string m_sceneName;

//		int m_currentSchemFloor;

		int m_maxFloor;
		int m_minFloor;

		int m_currentFloor;

		BlockParam m_blockParam;

		int m_gridResolveX;
	
		int m_gridResolveY;

		float m_outColorRed;
		float m_outColorBlue;
		float m_outColorGreen;
		float m_outColorAlpha;
	};
}

#endif