




#ifndef __VGSHPOINT_H__
#define __VGSHPOINT_H__



#include <vgShape/vgshDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgShape/vgshDefinition.h>
#include <vgShape/vgshGeometry.h>

	
namespace vgShape {

	class Layer;
	/**
		@action		creation
		@date 		2009/03/19  16:53	
		@author  	lss
	
		@brief 	
	
		@see    
		*/
	class  VGSH_EXPORT Point : public Geometry
	{

	public:
		Point( OGRFeature* feat , Layer* father );
		virtual ~Point()
		{
			clearRecoverableData();
		}
	

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

		

	protected:

		static bool readWkbData( uchar* buf , int buf_size , 
			double** ppPoint );



		// convert_to_opengl��Ŀ��coord
		static void converToCoordinate( bool& isopenglCoord ,double** ppPoint,
			const bool& convert_to_opengl );

		// �õ�shape�����wkb��ʽ.ע��,��3d,���ҿͻ��˸���ɾ��
		uchar* getShapeCoordinateBuf();

		void clearDisplayListID();

	protected:

		GLuint _callListId;

		bool _isOpenGLCoordinate;

		int _wkbSize;
		uchar* _wkbBuffer;
	
		// �洢������.
		double* _addrPoint;
		//Vec3d	_point;

		Box3d _box3d;
		
		//OGREnvelope _envelope;
	};
	
	
}// end of namespace vgShape
	


#endif // end of __VGSHPOINT_H__