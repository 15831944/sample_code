#pragma once


#include <vgGis/vggisDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgGis3d/vggiGeometry.h>

	
namespace vgGIS3D {

	class Layer;
 
	class  VGG_EXPORT Polygon : public Geometry
	{
	public:
		Polygon( OGRFeature* feat , Layer* father );
		virtual ~Polygon()
		{
			clearRecoverableData();
		}
	
		GeomtryType getType() { return GEOMTRY_POLY; }

	public:

		virtual void clearRecoverableData();

		virtual void renderUsingDisplayList();

		virtual int getNumOfPoints();

		virtual vgKernel::Box getEnvelope3f()
		{
			return _box3d.getBoxFloat();
		}

		virtual Box3d getEnvelope3d()
		{
			return _box3d;
		}
		
		//virtual OGREnvelope getEnvelope2f()
		//{
		//	return _envelope;
		//}

		// ��ʼ��ȡ����.
		virtual bool readDataFromOGRFeature();
	
		virtual void renderOnceForDisplayList();

		virtual bool testIntersect( const RayLine3d& rayline );


		virtual void renderInSelectedMode();

		virtual bool attachToTerrain();

		virtual void invalidateRendering();

		virtual std::vector<RingStruct>& getRings()
		{
			return _rings;
		}

	protected:

		static bool readWkbData( uchar* buf , int buf_size , 
			std::vector<RingStruct>& out_rings );



		// convert_to_opengl��Ŀ��coord
		static void converToCoordinate( bool& isopenglCoord , 
			uchar* buf , 
			std::vector<RingStruct>& rings, const bool& convert_to_opengl );

		// �õ�shape�����wkb��ʽ.ע��,��3d,���ҿͻ��˸���ɾ��
		uchar* getShapeCoordinateBuf();

		void clearDisplayListID();

	protected:

		GLuint _callListId;

		bool _isOpenGLCoordinate;

		int _wkbSize;
		uchar* _wkbBuffer;
	
		// �洢ÿ��rings��������ָ��.
		std::vector<RingStruct> _rings;

		Box3d _box3d;
		
		//OGREnvelope _envelope;
	};
	
	
}// end of namespace  
	

 