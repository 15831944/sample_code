


#ifndef __VGGIBUFFER_H__
#define __VGGIBUFFER_H__




#include <vgGis/vggisDefinition.h>
#include <vgGis3D/vggiVec3d.h>
#include <vgkernel/vgkMat4.h>
#include <vgGis3D/vggiGeometry.h>
#include <gdal/ogr_geometry.h>

#include <vgKernel/vgkRenderer.h>
#include <vgKernel/vgkRendererManager.h>
#include <vgGis3D/vggiLayer.h>
#include <gdal/ogrsf_frmts.h>
#include <vgKernel/vgkColorableObject.h>

namespace vgGIS3D {

	using namespace std;
	using namespace vgKernel;

	typedef vector< OGRGeometry* >               OGRGeoPtrVec;
	typedef vector< Vec3* >                      Vec3PtrVec;
	typedef vector< Vec3PtrVec >                 Vec3PtrVecVec;

	typedef vector< Vec3d* >                     Vec3dPtrVec;

	struct GeometryColor
	{
		Geometry* pGeometry;
		COLORREF  fillColor;
		COLORREF  penColor;
	};
	typedef vector<GeometryColor>                GeomColorVec;

	struct RendererColor 
	{
		Renderer* pRenderer;
		RgbaQuad  color;
		bool      colorState;
	};
	typedef vector<RendererColor>                RenderColorVec;


	class VGG_EXPORT Buffer
	{

	public:

		Buffer();
		virtual ~Buffer();


	private:
		enum SideType
		{
			LEFT,
			RIGHT
		};

	public:
	
		// ���������ӻ�
		void render();

		// ���ɻ�����
		bool genBuffer( const GeometryPointerVec* pGeoVec, double radius, int nQuadSegs = 30 );

		// �������һ���뾶����ȵĻ�����
		bool genLeftRightBuffer( const GeometryPointerVec* pGeoVec, double leftRadius, 
			double rightRadius, int nQuadSegs = 30 );

		// �ṩ�������õ�OGRGeometry
		OGRGeoPtrVec* getBufferVec();
		
		void setRenderOutline( bool bRender )
		{
			m_bRenderOutline = bRender;
		}

		void setRenderBuffer( bool bRender )
		{
			m_bRenderBuffer = bRender;
		}


		//��������������ӿ�
		void modelAnalysis(Layer* shapeLayer);
		//������ʸ�������ӿ�
		void shapeAnalysis(Layer* shapeLayer);

	private:

		void renderOnceForDisplayList();  // ������Ⱦ�б�

		bool bufferUnion();               // �ϲ�������

		void checkTerrainLoad();          // ����Ƿ����ش���

		void attachToTerrain();           // ����������

		void getDataFromOgrGeo( const OGRGeometry* pOgrGeo );  // ��OGRGeometry �ж�ȡ������

		bool readWkbData( uchar* buf , int buf_size );         // ��wkbBuffer�ж�����

		void interpolation( int ptNum, double* addr );         // ����ǰ��ֵ

		void clearDisplayList();                               // �����ʾ�б�

		void clearRecoverableData();                           // ���wkbBuffer ��ʱ����

	
		/////////////////////////�����������һ���뾶����ȵĻ�����

		// ���ɵ��߻�����
		void genSingleSideBuffer( Geometry* pGeo, Vec3PtrVec& vec, SideType type, 
			double radius, int nQuadSegs = 30 );

		// ��OGR��Buffer��������ȡ��Ҫ����һ��
		bool genOutlinePt( const double* addr, const RingStruct& rs, Vec3PtrVec& vec,
			SideType type, double radius, int numPts );

		// ��������Buffer��ԭʼ����Ϊ���������һ����
		void appendLinePt( Geometry* pGeo, Vec3PtrVec& vec );

		// ����Ŀ�������ߵ����˵�
		void calculateTowEndpoint( const RingStruct& rs, double radius, Vec3& firstPt, Vec3& lastPt );  

		// ������Ŀ�������ߵ����˵�����ĵ��������
		void findStartAndEndIndex( const double* addr, int numPts, const Vec3& firstPt, const Vec3 lastPt,
			int& sIndex, int& eIndex );

		// ����OGRGeometry���ڷ��� 
		void constructOgrGeo();

		// ��������ʱ��տռ�
		void unInit();  

		void clearPtDataVec();

		void destroyOgrGeo();


		/////////////////////////���¹���������������///////////////////////////
		//��������ʸ��ͼ�ཻ��������ʸ����������,����ֵ��ͬ
		OGRGeoPtrVec bufferShapeAnalysis(Layer* shapeLayer);
		GeometryPointerVec bufferShapeAnalysis2(Layer* shapeLayer);

		//ʸ��ͼ��ģ���ཻ�������������������
		void shapeModelAnalysis(OGRGeometry* bufferGeometry,Layer* shapeLayer);

		//��ʾ����������������������
		void renderModelAnalysisResult(vgKernel::RendererQueue& renderResult);

		//�ָ�ģ��ԭ��Ⱦ״̬���������������
		void unrenderModelAnalysisResult();

		//��ʾʸ�������������ʸ����������
		void renderShapeAnalysisResult(GeometryPointerVec& geoVec);

		//�ָ�ʸ��ԭ��Ⱦ״̬����ʸ����������
		void unrenderShapeAnalysisResult();

		//�õ�ʸ��ƽ�������������������
		Vec3 getTranslateModeltoShapeOffset();

	private:

		bool                m_bRenderOutline;    // �Ƿ���Ⱦ������
		
		bool                m_bRenderBuffer;     // �Ƿ���Ⱦ������

		GLuint              m_callListId;        // ��ʾ�б�ID

		OGRGeoPtrVec        m_ogrGeoPtrVec;      // ���������õ�OgrGeomtry����


		int                 m_wkbSize;
		uchar*              m_wkbBuffer;
		
		Vec3PtrVecVec       m_ptDataVec;         // ���ڿ��ӻ��ĵ�����


		vgKernel::Plugin*   m_pPlugin;

		bool           m_bLoadTerrainDLL;        // ƽ̨�Ƿ������TerrainDLL

		bool           m_bHasTerrain;            // �������Ƿ��е�������

		float          m_terrainCellSize;        // ���������С

		Vec3dPtrVec    m_vecForTess;             // ���ڶ�������

		vgKernel::Mat4 m_matrix;
		//////////////////////////////////////////////////////////////////////////
		bool      m_bNeedLine;        // ���������ɻ���������Ҳ��Ϊ�����ߵ�һ����

		bool      m_bLeftRightBuffer; // �������Ҳ���ȵĻ�����

		bool      m_bNeedDestroyOgr;  


		//��Ż������������////////////////////////////////////////////////////
		std::ostringstream o;
		
		vgKernel::RendererQueue resultQueue;
		
		GeometryPointerVec resultShapeGeomtrys;

		GeomColorVec geoColors;

		RenderColorVec rendererColors;


		// ���ڷ�����ʸ��ͼ��
		Layer*   m_pAnalsisLayer;
	};




}// end of namespace vgGIS3D



#endif // end of __VGGIBUFFER_H__
