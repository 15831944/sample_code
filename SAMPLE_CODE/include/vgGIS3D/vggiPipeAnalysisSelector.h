
#pragma once


#include <vggis/vggisDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkInputSystem.h>
#include <vgGis3d/vggiOGRPipeLineMap.h>
#include <vgGIS3D\vgPipeAttriDlg.h>
#include <vgGIS3D\vggiDlgPointAttri.h>

	
namespace vgGIS3D {


	using namespace vgKernel;


	typedef std::vector< Vec3d > Vec3dVector;

 
	class  VGG_EXPORT PipeAnalysisSelector : public vgKernel::InputHandler
	{
	public:
		PipeAnalysisSelector( OGRPipeLineMap* pipeLine );
		virtual ~PipeAnalysisSelector();


		virtual void OnMouseMove(UINT nFlags, CPoint position);
		virtual void OnLButtonDown(UINT nFlags, CPoint position);
		virtual void OnLButtonUp(UINT nFlags, CPoint position);
		virtual void OnLButtonDbClick(UINT nFlags, CPoint position);
		virtual void OnRBottonDown(UINT nFlags, CPoint position);

		void selectLayerGeometry( CPoint position , bool add_before_clear );

		void selectPipeGeoInVec(CPoint position, bool add_before_clear);

		void render();

		OGRPipeLineMap* getPipeLinePtr()
		{
			assert( _pipeLine != NULL );
			return _pipeLine;
		}
		

	private:

		static void drawPoint( const Vec3d& point );
		static void drawLines( const Vec3dVector& points , const Vec3d& mousevec );
		static void drawHeight( Vec3dVector* points );
		static Vec3d get3dVecFrom2d( const CPoint& point_2d );

		void SelectRectObject();

	private:

		OGRPipeLineMap* _pipeLine;

		bool _LBtnDown;
		bool  m_Already;

		float	m_start_x;
		float	m_start_y;
		float	m_drag_x;
		float	m_drag_y;
		float	m_end_x;
		float	m_end_y;

		int		m_window_width;
		int		m_window_height;


		Vec3d _mouseVec;

		vgPipeAttriDlg *PipeAtrdlg;
		vgDlgPointAttri *PointAtrdlg;

		Vec3dVector _clickPoints;

		Vec3dVector* _heightRender;

		//TerrainManager* _terrainMgr;


		//ElevationBound _sceneBound;

	};
	
	
}// end of namespace  
	
 