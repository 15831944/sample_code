


#ifndef __VGTCUTSURFACEANALYSIS2_H__
#define __VGTCUTSURFACEANALYSIS2_H__


#include <vgTerritory/vgtDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkInputSystem.h>
#include <vgTerritory/vgtElevationBound.h>
#include <vgKernel/vgkPlane.h>

	
namespace vgTerritory {

	class TerrainManager;

	using namespace vgKernel;


	//typedef std::vector< Vec3 > Vec3Vector;

	/**
		@date 	2008/10/31  9:10	
		@author  leven
	
		@brief 	Ҫע��,Ҫȷ����opengl����ж��֮ǰ������������.
	
		@see    
	*/

	enum sectionType	//���������
	{
		SECT_VERTICAL,	//��ֱ�������
		SECT_HORIZONTAL,	//ˮƽ�������
		SECT_INCLINE,	//��б�������
		SECT_NONE,		//��
	};

	class  VGT_EXPORT CutSurfaceAnalysis2 : public vgKernel::InputHandler
	{
	public:
		CutSurfaceAnalysis2();
		virtual ~CutSurfaceAnalysis2();
	
	
		virtual void OnMouseMove(UINT nFlags, CPoint position);
		virtual void OnLButtonDown(UINT nFlags, CPoint position);
		virtual void OnLButtonUp(UINT nFlags, CPoint position) {}
		virtual void OnLButtonDbClick(UINT nFlags, CPoint position);
		virtual void OnRBottonDown(UINT nFlags, CPoint position);

		void render();

		float* getUserClipPlaneParam()
		{
			if ( _plane == NULL )
			{
				return NULL;
			}

			return _clipPlaneParam;
		}

	private:

		static void drawPoint( const Vec3& point );
		static void drawLines( const Vec3Vector& points , const Vec3& mousevec );
		static void drawHeight( Vec3Vector* points );
		static Vec3 get3dVecFrom2d( const CPoint& point_2d );


	private:

		Vec3 _mouseVec;
		

		Vec3Vector _clickPoints;


		TerrainManager* _terrainMgr;

		ElevationBound _sceneBound;

		vgKernel::Plane* _plane;

		TerrainGpuProgram* _gpuProgram;

		float _clipPlaneParam[4];

	public:
		sectionType _cutSufSecType;

		void setCutSurfAnaType( sectionType secType ){ _cutSufSecType = secType; }
		sectionType getCutSurfAnaType(){ return _cutSufSecType; }

	};
	
	
	
}// end of namespace vgTerritory
	


#endif // end of __VGTCUTSURFACEANALYSIS2_H__