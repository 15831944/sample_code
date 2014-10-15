



#ifndef __VGTERRITORY_TERRAINGPUPROGRAM_H__
#define __VGTERRITORY_TERRAINGPUPROGRAM_H__



#include <vgTerritory/vgtDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkGpuManager.h>


namespace vgTerritory {

	using namespace vgKernel;

	/**
		@date 	2008/04/03  19:07	
		@author  leven
	
		@brief 	
	
		@see    
	*/
	class TerrainGpuProgram 
	{
	private:
		static char *vertex_source;
		static char *pixel_source;

	public:
		TerrainGpuProgram();
		~TerrainGpuProgram();

		void setParametersOnce();

		bool init();

		void setHeightMapIdEveryFrame( const GLuint& height_map_id );
		void setDecalUVParamsEveryFrame( const GLuint& decal_map_id , float* uvparam );

		void setViewMatrix( GLfloat* mat );


		void useBindBegin();

		void useBindEnd();

		void setDisplacementScaler( const float& scaler )
		{
			this->g_fDisplacementScaler = scaler;
		}

		void setHeightMapSize( const float& mapsize )
		{
			this->g_fHeightMapSize = mapsize;
		}

		void setUserClipParam( float* clip_param );

		void setClipPlaneParam( float* clip0 , 
			float* clip1 , float* clip2 , float* clip3 );

		void setFogColor( float* color );

		void setFogEnableFlag( const bool& enable_fog );

		void setFogStartAndEnd( const float& start , const float& end );

		//void setGlobalOffset( const float& offsetx , const float& offsetz )
		//{
		//	_globalOffset[0] = offsetx;
		//	_globalOffset[1] = offsetz;
		//	cgGLSetParameter2fv( g_CGparam_globalOffset , _globalOffset );
		//}
		//record if there is texture for the terrain -- livenson 20091015
		bool       _mbTexture;
		bool   m_bIsMirror;
		double m_WaterHeightClip;

	private:

		void destroy();

		void setDefault();


		//vgKernel::Vec3 mEyePosition;

		// ����ֻ�����id����Ⱦ,�����𴴽���ɾ��.
		// �����Ĺ�����TerrainRenderer��
		GLuint _heightMapID;
		GLuint g_decalTextureID;

		CGprofile mVertexProfile;
		CGprofile mPixelProfile;

		CGprogram mCgPixelProgram;
		CGprogram mCgVertexProgram;

		//float _globalOffset[2];

		/**
			���ܹ�ɾ��
		*/
		vgKernel::GpuSupport* mSupportPtr;

		float* _pUVParameter;

		GLfloat* _pReversedViewMat;

		CGparameter g_CGparam_ViewMat;

		
		CGparameter g_CGparam_displacementTexture;
		CGparameter g_CGparam_displacementScaler;
		CGparameter g_CGparam_decalTexture;
		CGparameter g_CGparam_uvParameter;

		CGparameter g_CGparam_userClipPlaneParam;
		CGparameter g_CGparam_clipPlaneParam0;
		CGparameter g_CGparam_clipPlaneParam1;
		CGparameter g_CGparam_clipPlaneParam2;
		CGparameter g_CGparam_clipPlaneParam3;

		CGparameter g_CGparam_fogColor;

		CGparameter g_CGparam_fogParam;

		// ע��,_fogColor[3]������ʶ�Ƿ���Ⱦ��.
		// ��Ϊ0ʱ,����Ҫ��Ⱦ��Ч
		float _fogColor[4];

		// _fogParam[0] ���fogStart
		// _fogParam[1] ���fogEnd
		float _fogParam[2];


		float g_fDisplacementScaler;
		float g_fHeightMapSize;

		bool mIsValid;
	};
	

	
	
	
}// end of namespace vgTerritory

#endif // end of __VGTERRITORY_TERRAINGPUPROGRAM_H__