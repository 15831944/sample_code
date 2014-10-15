



#ifndef __VGMO_SHADOWMETAFACE_H__
#define __VGMO_SHADOWMETAFACE_H__
//
//#include <vgModel/vgmoDefinition.h>
//#include <vgKernel/vgkForward.h>
//#include <vgKernel/vgkVec3.h>
//
//	
//namespace vgModel {
//
//
////#define VMD_NULL_SHADOW_FACEID	-1
//
//	struct MetaFaceStruct;
//	struct VmdFaceIndex;
//	class ShadowMetaFace;
//
//	class  VGMO_EXPORT ShadowMetaFaceData
//	{
//	public:
//		ShadowMetaFaceData()
//		{
//			_fatherShadowFaceStruct = NULL;
//			_faceindex = NULL;
//
//			p1p2AdjFace = NULL;
//			p2p3AdjFace = NULL;
//			p3p1AdjFace = NULL;
//
//			faceVisible = false;
//
//			p1p2Visible = false;
//			p2p3Visible = false;
//			p3p1Visible = false;	
//		}
//		~ShadowMetaFaceData()
//		{
//			
//		}
//	
//
//	public:
//
//		ShadowMetaFace* _fatherShadowFaceStruct;
//
//		// ���Դ�MetaFaceStruct* _metaFaceStruct;��ȡ��
//		//int	 p1; // �����vertexelement�еĵ�
//		//int p2;
//		//int p3;
//
//		VmdFaceIndex*	_faceindex; 
//
//		vgKernel::Vec3 _normal; // ֱ�Ӵ洢
//
//		// ��������������һ��vgm������˵
//		// ע�⣬Ҫʹ��getMetaFaceByIndex()����ȡ��
//		//  ��ΪNULLʱ����ʾ��û�в����档
//		//  ��Ϊ�����ֵʱ����ʾû���ڽ��档
//		ShadowMetaFaceData*	p1p2AdjFace; 
//		ShadowMetaFaceData*	p2p3AdjFace;
//		ShadowMetaFaceData*	p3p1AdjFace;
//
//		bool faceVisible;
//
//		bool p1p2Visible;
//		bool p2p3Visible;
//		bool p3p1Visible;	
//	
//	};
//
//	class ModelRenderWayVertexArray;
//	class ModelDataPlus;
//
//	/**
//		@date 	2009/03/24  16:04	
//		@author  leven
//	
//		@brief 	
//	
//		@see    
//	*/
//	class  VGMO_EXPORT ShadowMetaFace
//	{
//	public:
//		ShadowMetaFace();
//
//		~ShadowMetaFace()
//		{
//			if ( _shadowFaceData != NULL )
//			{
//				delete[] _shadowFaceData;
//				_shadowFaceData = NULL;
//			}
//		}
//	
//		void setFromMetaFaceStruct( ModelRenderWayVertexArray* vaway , 
//			MetaFaceStruct* metaface ,ModelDataPlus* dataplus );
//
//		void searchAndSetAdj();
//	
//		void debugCheckAdj();
//
//	private:
//
//		void searchP1P2Adj( ShadowMetaFaceData* sw_face_data );
//		void searchP2P3Adj( ShadowMetaFaceData* sw_face_data );
//		void searchP3P1Adj( ShadowMetaFaceData* sw_face_data );
//
//		void searchP1P2Adj_element2( ShadowMetaFaceData* sw_face_data );
//		void searchP2P3Adj_element2( ShadowMetaFaceData* sw_face_data );
//		void searchP3P1Adj_element2( ShadowMetaFaceData* sw_face_data );
//	public:
//
//		ModelDataPlus* _fatherDataPlus;
//
//		// ע�⣬��һһ��Ӧ��
//		MetaFaceStruct* _metaFaceStruct;
//		ShadowMetaFaceData* _shadowFaceData;
//	
//		ModelRenderWayVertexArray* _vaway;
//		
//	};
//	
//	
//}// end of namespace vgModel
//	


#endif // end of __VGMO_SHADOWMETAFACE_H__