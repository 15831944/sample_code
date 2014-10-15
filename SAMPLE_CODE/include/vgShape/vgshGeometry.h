


#ifndef __VGSHGEOMETRY_H__
#define __VGSHGEOMETRY_H__



#include <vgShape/vgshDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgShape/vgshPrerequisite.h>

	
namespace vgShape {


	class Layer;

	class  VGSH_EXPORT Geometry
	{
	public:
		Geometry( OGRFeature* feature, 
			Layer* father_layer );
		virtual ~Geometry()
		{
			
		}
	

		//------------------------------------------
		// �õ�ÿ��field����ϢҪ
		//------------------------------------------
		//getFieldInfo

	public:
	
		// ʹ��display list������
		virtual void renderUsingDisplayList() = 0;


		OGRFeaturePtr getOGRFeaturePtr();

		// ע��, �ͻ��˸���ɾ��.
		OGRFeature* getOGRFeature();

		// ע��, ����Ҫ�ͻ���ɾ��.
		void setFeature( OGRFeature* feature );

		virtual int getNumOfPoints() = 0;

		virtual vgKernel::Box getEnvelope3f() = 0;

		virtual Box3d getEnvelope3d() = 0;

		// ��Ⱦһ��. Ϊ�˴���display list
		virtual void renderOnceForDisplayList() = 0;

		// ͨ��OGRFeature����ȡ����
		virtual bool readDataFromOGRFeature() = 0;


		// ɾ����ʱ����.
		// ע��, �����ٶ�ȡ��.
		virtual void clearRecoverableData() = 0;

		virtual bool testIntersect( const RayLine3d& rayline ) = 0;

		virtual void renderInSelectedMode() = 0;

		virtual bool attachToTerrain() = 0;

		virtual void invalidateRendering() = 0;

		Layer* getFatherLayer()
		{
			assert( _fatherLayer != NULL );
			return _fatherLayer;
		}

		DrawingDescriptor* getDrawingDescriptor()
		{
			return &_drawing;
		}

		//virtual OGREnvelope getEnvelope2f() = 0;

	private:
	
		long _ogrFeatureFID;
		Layer* _fatherLayer;
	
		DrawingDescriptor _drawing;
	};


	typedef std::vector<Geometry*> GeometryPointerVec;



#if 0

	/**
		@date 	2009/02/13  9:47	
		@author  leven
	
		@brief 	
	
		@see    
	*/
	class AAA
	{
	public:
		AAA()
		{
			a=100;
			b=434;
			c=9.0f;
			pp = NULL;

			vec.push_back( new AAA(1) );
			vec.push_back( new AAA(2) );
			vec.push_back( new AAA(3) );
		}

		AAA( int ddd)
		{
			a=ddd;
			b=ddd;
			c=9.0f;
			pp = NULL;

		}

		~AAA()
		{
			
		}
	
		AAA* pp;

		void reset()
		{
			a=16550;
			b=33333;
			c=1000.0f;
			pp = new AAA;
			pp->a = 320323;
			pp->b = 320323;
			pp->c = 320323;
		}
	
		virtual  void serialize( eternity::bin_archive &stream)
		{
			using namespace eternity;
			
			if (stream.is_loading())
			{
				//vec.clear();

				stream >> a;
				stream>> b;
				stream >> c;
				stream.get_object( &pp );

				binary_read( sequence <pointers> (), vec, vec.begin(), stream);
			}
			else
			{
				stream << a << b << c;
				stream.put_object( pp );

				binary_write(sequence <pointers>() ,vec.begin(), vec.end(), stream);

			}
		}
	

	public:	
		
		int a;
		int b;
		float c;
	

		std::vector<AAA*> vec;
		
	};
	

#endif
	
}// end of namespace vgShape
	


#endif // end of __VGSHGEOMETRY_H__