
#ifndef   _VGSTRUCT_MOD_H_INCLUDED_
#define   _VGSTRUCT_MOD_H_INCLUDED_	
/**********************************************************************
*<
vgStruct_Mod.h:	��ģ�ͼ�����Ϣ��ص����ݽṹ����
��Ҫ�����ǣ� �㡢�桢ģ�͵Ƚṹ��
*>	
**********************************************************************/

#include "vgStableHeaders.h"


using namespace vgMath;

//#define	VERSION_MOD(year, updateTime)		(	( (year)<<16 )	+	(updateTime)	)
/************************************************************************/
/* MOD�汾���¼�¼                                                        
/*																		
/*	�汾��	|	����ʱ��					|	������	|	����ԭ��				 
/*          |                           |           |      	
/*	2009.1	|	2009��2��5��14:20:22		|	lss		|	����������ö��(����) ��Ϊ ��������	          
/*	6		|	2008��10��17��20:58:54	|	lss		|	vgmд�ļ� vgObjFrameNodeдvg��������	          
/*	5		|	2008��10��9��10:11:06	|	lss		|	vgmд�ļ� �ɺϲ���Ϊ���ϲ�	          
/*	4		|	2008��6��26��14:01:41	|	lss		|	vrgis2.0  vg�ļ��Ļ����汾	          
/*                                                         
/************************************************************************/
#if 0
// ��������ֵ����
enum	E_Return_Type
{
	RETURN_FALSE,
	RETURN_TRUE,

	RETURN_MTL_MULTITEX_NEST,//��������Ƕ�׶�������
	RETURN_SCENE_EMPTY
};
#endif

class  MeshMtlList;
//////////////////////////////////////////////////////////////////////////
// ��ṹ
	// class CVector2
	// class CVector3
	// struct vgVertexTNP
	// struct Vert3D


struct Vert3D {
	vgKernel::Vec3 pt;
	vgKernel::Vec3 tv1;
	vgKernel::Vec3 tv3;
};

//2008��3��5��16:36:25 ���
struct  Vert3DIndex {
	unsigned int vertIndex;
	unsigned int tvertIndex;
};

struct vgVertexTNP
{

public:	
	CVector2	texcoord2D;	
	CVector3	normal3D;	
	CVector3	postion3D;
};

struct vgVertexTNP2
{

public:	
	CVector2	texcoord2D;	
	CVector3	normal3D;	
	CVector3	postion3D;
	// 2008��3��6��15:25:01 ���
	CVector2	texcoord2DLit;
};

////////////////////////////////////////////////////////// ��ṹ
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// ��ṹ
	// struct tFace
	// struct tFace2
	// struct FaceMod
struct tFace
{
	int vertIndex[3];			// indicies for the verts that make up this triangle
	bool bHasTexture;
};

struct tFace2 
{
	union
	{
		struct{int x, y, z;};
		int vertIndex[3];
	};
};

struct FaceMod {
	int vNum[3];
	int flags;
};
////////////////////////////////////////////////////////////// ��ṹ
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	����
	// struct sw_FaceIndex
	// struct tMaterialInfo
	// struct tMatandFace
	// struct MeshAndTMAndNode
struct sw_FaceIndex
{
	int	p1;
	int p2;
	int p3;

	int	normal;

	int	p1p2AdjFaceIndex;
	int	p2p3AdjFaceIndex;
	int	p3p1AdjFaceIndex;

	bool faceVisible;

	bool p1p2Visible;
	bool p2p3Visible;
	bool p3p1Visible;

};

struct tMaterialInfo
{
	bool aphlatext;
	char  strName[255];			// The texture name
	char  strFile[255];			// The texture file name (If this is set it's a texture map)
	char  optexname[255];
	unsigned char  color[3];				// The color of the object (R, G, B)
	unsigned int   texureId;				// the texture ID

} ;

struct tMatandFace2 
{
	char strName[255];
	int numOfFace;
	tFace2 *RenderFace;
	unsigned int textureId;
	short *FaceID;
	bool bHasTexture;			// This is TRUE if there is a texture map for this object
	unsigned char red;
	unsigned char green;
	unsigned char blue;

	// 2008��3��6��15:52:08 ���
	char strNameBaked[255];

};

struct tMatandFace3 
{
	char strName[255];
	int numOfFace;
	tFace2 *RenderFace;
	unsigned int textureId;
	short *FaceID;
	bool bHasTexture;			// This is TRUE if there is a texture map for this object
	unsigned char red;
	unsigned char green;
	unsigned char blue;

	// 2008��3��6��15:52:08 ���
	char strNameBaked[255];

};
#if 0
struct MeshAndTMAndNode{
	Matrix3 *tm;
	Mesh *mesh;
	INode *node;
} ;
#endif
struct ModKeyFrame
{
	int			m_fTime;		//	Time at which keyframe is started
	CVector3	key_translate;	//	Translation or Rotation values
	float		key_rotate[4];
	CVector3	key_scale;

	ModKeyFrame()
	{
		for (int index = 0 ; index < 4 ; index ++)
		{
			key_rotate[index] = 1.0f;
		}
	}
};
////////////////////////////////////////////////////////////// ����
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// ģ�ͽṹ
	// struct t3DObject
	// struct t3DModel
	// class SceneEntry
	// class ObjectEntry
#define OBTYPE_MESH 0
#define OBTYPE_CAMERA 1
#define OBTYPE_OMNILIGHT 2
#define OBTYPE_SPOTLIGHT 3
#define OBTYPE_DUMMY 5
#define OBTYPE_CTARGET 6
#define OBTYPE_LTARGET 7

#endif	//_VGSTRUCT_MOD_H_INCLUDED_	