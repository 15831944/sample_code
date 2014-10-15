
#ifndef _VGGISMANAGER_H_INCLUDED_
#define _VGGISMANAGER_H_INCLUDED_

#include <vgGis/vggisDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSingleton.h>

#include <vgKernel/vgkRenderCommand.h>

#include <vgGIS/vggisAnalysisHandle.h>
#include <vgGIS3D/vggiOGRRouteMap.h>
#include <vgGIS3D/vggiOGRPipeLineMap.h>
#include <vgGIS3D/vggiRouteBuilder.h>
#include <vgGis3D/vggiBuffer.h>

#include <vgGIS3D/vggiProcessingDlg.h>

namespace vgGIS3D
{

	class  VGG_EXPORT GisManager: public vgKernel::Singleton<GisManager>
	{
		friend class vgKernel::Singleton<GisManager>;
	//��Ա����
	public :
		GisManager();
		virtual ~GisManager();

	protected:
		virtual bool initialise();

		virtual bool shutdown();
	
	public:
		void reset();
		void uninitBeforeOpenGLDestroy();
		void renderGisElements();
		
		void showProcessingDlg(bool show);
		void setProcessingInfo(string info);
		void setProcessingProgress(int pos);

	//GIS��������
	public:
		void setGisAnalysis( EGisState eType);
		EGisState getAnalysisType();
		GisAnalysis* getGisAnalysis();
		
		bool getMeasurePointFlag();
		bool getMeasureDistanceFlag();
		bool getMeasureAreaFlag();
		bool getVer_Hor_SlopeFlag();	// ��ֱ���������ˮƽ����������¶Ȳ���

		bool getFaecherAnalysisFlag();
		//��������������
		bool getBufferAnalysisFlag();
		bool getBufferShapeAnalysisFlag();
		bool getBufferModelAnalysisFlag();

	private:
		GisAnalysis*	_gisAnalysis;

	// ·����������
	public:
		void addRouteFromLayer(string layerName);
		
		int addRouteStarEndPoint(vgKernel::Vec3 point);

		void addRouteToBuilder(const vector<vgKernel::Vec3>& pointVec);
		bool onEditRoute();
		void addRouteFromFile(string filePath) {  m_ogrRouteMap.readFromFile(filePath); }
		void dumpRouteToFile(string filePath) { m_ogrRouteMap.saveToFile(filePath); }
		bool getEditRouteFlag();

	//���߷�������
	public:
		void ConstructPipeFromLayer(string layerName);

		void addPipeFromFile(string filePath) {  m_ogrPipeLineMap.readFromFile(filePath); }

		void dumpPipeToFile(string filePath) { m_ogrPipeLineMap.saveToFile(filePath); }

		OGRPipeLineMap* addPipeLineFromLayer(string layerName);

		OGRPipeLineMap* addPipeLineEntry(vgKernel::Renderer *pRenderer);

		//add by KingHJ @2010-01-08	
		std::vector <PointStruct>* getSelectPoints()
		{
			return m_ogrPipeLineMap.getSelectedPointVec();
		}

		GeometryPointerVec* getSelectedGeometry()
		{
			return m_ogrPipeLineMap.getSelectedGeometry();
		}

		vector<JointPointPtr>* getJointArray()
		{
			return m_ogrPipeLineMap.getJointArray();
		}

		OGRPipeLineMap* getPipeLineMap()
		{
			return &m_ogrPipeLineMap;
		}		

		void returnToInitialRender()
		{
			return m_ogrPipeLineMap.restoreInitialRender();
		}

		bool ifInPipeSelectMode()
		{
			return m_ogrPipeLineMap.isInEditMode();
		}

		bool ifInPipeAnalysisMode()
		{
			return m_ogrPipeLineMap.isInAnalysisMode();
		}

		void addBuildingToAnalysis(string layerName)
		{
			return m_ogrPipeLineMap.addBuildingToAnalysis(layerName);
		}
		//add by KingHJ @2010.04.22
		void pipeRouteAnalysis()
		{
			return m_ogrPipeLineMap.pipeRouteAnalysis();
		}

	private:
		OGRRouteMap m_ogrRouteMap;
		OGRPipeLineMap m_ogrPipeLineMap;

		RouteBuilder m_routeBuilder;
		bool m_onEditRoute;
		
		vgKernel::RenderCommand *m_pGisRenderCommand;
		
		vgProcessingDlg *m_processingDlg;

	//����ͶӰϵͳ	@FengYK 2009-12-22
	public:
		void vgCoordinateTransform();
		bool	getControlHtFlag();	// �ظ߷���
		
	}; //class GisManager
}//vgGIS3D�����ռ�


#endif//_VGGISMANAGER_H_INCLUDED_
