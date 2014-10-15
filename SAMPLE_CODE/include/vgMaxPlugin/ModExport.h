
#ifndef _MODEXPORT_H_INCLUDED_
#define _MODEXPORT_H_INCLUDED_
/**********************************************************************
*<
ModExport.h:	.MOD��ʽ �����������ͷ�ļ�
��Ҫ�����ǣ� ȫ�ֱ�����ȫ�ֺ������ļ��������������
*>	
**********************************************************************/

#include "Max.h"
#include "istdplug.h"
#include "stdmat.h"
#include "decomp.h"
#include "helpsys.h"
#include "buildver.h"

#include <stdarg.h>

#include "vgModelExpres.h"
#include "vgMod.h"
#include "vgMtl.h"
#include "vgObjectSet.h"

#include "vgMaxPlugin/vgMaxPrerequisite.h"

//////////////////////////////////////////////////////////////////////////
//		ȫ�ֱ���	//////////////////////////////////////////////////////

HINSTANCE hInstance;

//static BOOL showPrompts;

//extern BOOL	exportSelected;

static Tab<TSTR*> msgList, msgListHead, msgListTexture, msgListObject;
static Tab<TSTR*> msgListZeroFace,msgListMulti2,msgListCadFace,msgListOtherType;

BitArray	bFaceOfMatOne;
BitArray	bFaceOfMatAll;
BitArray	vertFlag;
long		totleFaceWithMat;

Tab<int> mtlNumMap;
int mtlnum;
int bakedMtlNo ;
BOOL multiMtl;


static BOOL bTexture;

static	TSTR	g_strCenterX="0.0f", g_strCenterY="0.0f", g_strCenterZ=	"0.0f";
static	TSTR	g_strScale= "1.0f";
static	TSTR	g_strPrefix;//"Plan_i";

////////////////////////////////////////////////////////////	ȫ�ֱ���
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//   ȫ�ֺ���  ///////////////////////////////////////////////////////
TCHAR* GetString(int id)
{
	static TCHAR stBuf[SCHAR_MAX];
	if (hInstance)
		return LoadString(hInstance, id, stBuf, SCHAR_MAX) ? stBuf : NULL;
	return NULL;
}

static void MessageBox(int s1, int s2) {
	TSTR str1(GetString(s1));
	TSTR str2(GetString(s2));
	MessageBox(GetActiveWindow(), str1.data(), str2.data(), MB_OK);
}

static int MessageBox(int s1, int s2, int option = MB_OK) {
	TSTR str1(GetString(s1));
	TSTR str2(GetString(s2));
	return MessageBox(GetActiveWindow(), str1, str2, option);
}

static int Alert(int s1, int s2 = IDS_TH_VGMODEXP, int option = MB_OK) {
	return MessageBox(s1, s2, option);
}


DWORD WINAPI fn(LPVOID arg)
{
	return(0);
}

// ����������������ģ����ȡ���ˡ���ˡ���λ��
#define Mag(V) (sqrtf(V.x*V.x + V.y*V.y + V.z*V.z))

BOOL operator==(const Matrix3 &m1, const Matrix3 &m2) 
{
	for (int i=0; i<4; i++) {
		if (m1.GetRow(i) != m2.GetRow(i)) return FALSE;
	}
	return TRUE;
}

CVector3 SubVector(CVector3 vPoint1, CVector3 vPoint2)
{
	CVector3 vVector;

	vVector.x = vPoint1.x - vPoint2.x;
	vVector.y = vPoint1.y - vPoint2.y;
	vVector.z = vPoint1.z - vPoint2.z;
	return vVector;
}

float DotProduct(CVector3 vVector1, CVector3 vVector2)
{
	float resultDot = 0.0f;
	for(int index =0; index<3; index++)
		resultDot += vVector1.vert[index] * vVector2.vert[index]  ;
	return resultDot;
}

CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vCross;
	vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));
	return vCross;
}

CVector3 Normalize(CVector3 vNormal)
{
	double Magnitude;							

	Magnitude = Mag(vNormal);

	vNormal.x /= (float)Magnitude;				
	vNormal.y /= (float)Magnitude;				
	vNormal.z /= (float)Magnitude;				

	return vNormal;								
}

BOOL	UVVertEqual(UVVert tv0, UVVert tv1) 
{
	return (tv0.x == tv1.x &&
		tv0.y == tv1.y &&
		tv0.y == tv1.y);
}

//INT_PTR CALLBACK MsgListDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//void AddToMsgList(Tab<TSTR*>& mList, TCHAR* msg);
//void	ClearMsgList();

inline BOOL		ApproxEqual(float a, float b);
inline Matrix3	GetLocalNodeTM(INode* node, TimeValue t);
inline BOOL		UndoParentsOffset(INode* node, Point3& pt, Quat& rOff);




///////////////////////////////////////////////////////////		ȫ�ֺ���
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//  �ļ�������  //////////////////////////////////////////////////////////
class WorkFile {
private:
	FILE *stream;

public:
	WorkFile(const TCHAR *filename,const TCHAR *mode) 
	{ 
		stream = NULL; 
		Open(filename, mode); 
	};
	~WorkFile() { Close(); };

	FILE *	Stream() { return stream; };

	int		Close() 
	{ 
		int result=0; 
		if(stream) 
			result=fclose(stream); 
		stream = NULL; 
		return result; 
	}
	void	Open(const TCHAR *filename,const TCHAR *mode) 
	{ 
		Close(); 
		_tfopen_s( &stream, filename, mode); 
	}
};
//////////////////////////////////////////////////////////////	�ļ�������
//////////////////////////////////////////////////////////////////////////

typedef
enum	_enumCoordinateSystem
{
	XYZ_COORDINATE,
	XZminusY_COORDINATE,
	COUNT_COORDINATE
} ECoordinateSystem;


typedef 
enum	_enumObjectFilter	//  ___________________________________
{							//	|�˳�����		|�б��׼			|
							//	|				|					|
	FACE_ZERO,				//	|������			|����Ϊ0 �� ����Ϊ0	|
	MTL_MULTITEX_NEST,		//	|��ά��Ƕ��		|�Ӳ��ʲ���STANDARD	|
							//	|				|					|
	FACE_FROM_CAD,			//	|Cad ��ͼ��Ƭ	|���Ϊ0 �� �޲���	|
	OBJ_KEYFRAME_NO,		//	|���ؼ�֡		|֡����Ϊ0			|
	OBJ_KEYFRAME_HAS,		//	|�����ؼ�֡		|֡��Ϊ0			|
							//	|-----------------------------------|
	COUNT_FILTER_OBJECT		//	|��������Ŀ							|
} EObjectFilter;			//	|___________________________________|

typedef 
enum	_enumElementFilter	//  ___________________________________
{							//	|�˳�����		|�б��׼			|
							//	|				|					|
	OBJ_KEYFRAME_ELEMENT,	//	|�ؼ�֡��Ϣ		|֡����Ϊ0			|
	MTL_NAME_SUFFIX_ELEMENT,//	|��������׺��Ϣ	|��������			|
	MTL_EXT_TO_DDS_ELEMENT,	//	|������չ��תdds|��������			|
							//	|---------------|-------------------|
	COUNT_FILTER_ELEMENT	//	|��������Ŀ							|
} EElementFilter;			//	|___________________________________|

class	ObjectFilter
{
	//typedef	Tab<TSTR*>	MaxStrVec;
	//typedef	vector<MaxStrVec>	FilterMsgList;
	typedef	vector<string>		StrVec;
	typedef	vector<StrVec>		FilterMsgList;

public:
	FilterMsgList	m_strFilterMsgList;
	vector<BOOL>	m_bFilterFlag;
	
	ObjectFilter()
	{
		m_bFilterFlag.assign(COUNT_FILTER_OBJECT, FALSE);
		m_bFilterFlag[FACE_ZERO]			= FALSE;
		m_bFilterFlag[MTL_MULTITEX_NEST]	= FALSE;
	
		m_bFilterFlag[FACE_FROM_CAD]	= TRUE;
		m_bFilterFlag[OBJ_KEYFRAME_NO]	= TRUE;
		m_bFilterFlag[OBJ_KEYFRAME_HAS] = TRUE;

		initializeFilterMsgList();
	}

	BOOL&	getFilterFlag( EObjectFilter filter);
	void	initializeFilterMsgList();
	
};//ObjectFilter

class	ElementFilter
{
public:
	vector<BOOL>	m_bFilterFlag;

	ElementFilter()
	{
		m_bFilterFlag.assign(COUNT_FILTER_ELEMENT, FALSE);
		m_bFilterFlag[OBJ_KEYFRAME_ELEMENT]	= TRUE;
		m_bFilterFlag[MTL_NAME_SUFFIX_ELEMENT]	= TRUE;
		m_bFilterFlag[MTL_EXT_TO_DDS_ELEMENT]	= TRUE;
	}

	BOOL&	getFilterFlag( EElementFilter filter);

};//ObjectFilter

//static	tagObjectFilterFlag g_tagObjectFilterFlag;


//////////////////////////////////////////////////////////////////////////
//		���������	//////////////////////////////////////////////////////
class	vgModExportor : public SceneExport 
{

	friend INT_PTR CALLBACK ExportOptionsDlgProc(HWND hDlg, UINT message, 
		WPARAM wParam, LPARAM lParam);
	friend INT_PTR	CALLBACK MsgListDlgProc(HWND hDlg, UINT iMsg, 
		WPARAM wParam, LPARAM lParam);


public:
	void	reset();
	vgModExportor();
	~vgModExportor();


	//  Part ��: ���ػ���SceneExport�Ĵ��麯��
	int				ExtCount();					// ֧���ļ���չ������
	const TCHAR *	Ext(int n);					// �ļ���չ��
	const TCHAR *	LongDesc();					// �ļ�������
	const TCHAR *	ShortDesc();				// �ļ�������
	const TCHAR *	AuthorName();				// �������
	const TCHAR *	CopyrightMessage();			// �����Ȩ������
	const TCHAR *	OtherMessage1();			// ���� #1
	const TCHAR *	OtherMessage2();			// ���� #2
	unsigned int	Version();					// �汾*100
	void			ShowAbout(HWND hWnd);		// ����
	BOOL			SupportsOptions(int ext, DWORD options);
	int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, 
		BOOL suppressPrompts = FALSE, DWORD options = 0);	// Export file

	void _vmd_save( const TCHAR * filename );

	//  Part 1: �������ݻ�ȡ����
public:
	int		_mod_save(const TCHAR *filename, ExpInterface *ei, vgModExportor* exp);
	bool	CollectDataForObjects();
	E_Return_Type	CollectGeoDataForEachObj(ObjectEntry *oe);
	E_Return_Type	CollectMtlDataForEachObj(Mtl*mtl);

	int		ComputerVerts(  MeshAndTMAndNode *mt ,t3DObject *newobj );
	int		ComputerBound( t3DObject* pObject);
	int		ComputerNormal( t3DObject * pObject);
	int		ComputerSWFaces(  t3DObject * pObject);
	int		sw_searchForAdjacent( t3DObject *newobj, int faceid , 
		int& point1 , int& point2 );
	BOOL	ComputerKeyFrames(INode* node );
	bool	HasKeyFrames(INode* node);
	bool	FilterFaultyObject(INode* pNode, E_Return_Type status);

	
	//	Part 2: �����ṹ��Ϣ�������
protected:
	void	AddToMsgList(Tab<TSTR*>& mList, TCHAR* msg);
	void	AddLineToListDlg(HWND hDlg, const TCHAR* buf);
	void	BuildMsgList(HWND hDlg);
	void	ClearMsgList();
	void	DumpMtlListBegin();
	void	DumpObjListBegin();
	void	DumpMtlListProcess();
	void	DumpObjListProcess();
	void	DumpMtlListEnd();
	void	DumpObjListEnd();

private:
	// ����ϵ
	bool	isNeedTranlateCoordinateSystem();
	void	setCoordinateSystem(ECoordinateSystem eCoordinate);

private:
	// ����ѡ������
	void	configExportFlag(DWORD options, bool bForceDfault = false);

protected:	// ��ʱ����
	unsigned int	m_nVersionMod;			
	//float			m_fltCenterX, m_fltCenterY, m_fltCenterZ;
	CVector3		m_oSceneCenter;
	float			m_fScale;
	//  �����ӿ�
	Interface *ip;

	ObjectFilter	m_eObjectFilter;		//	ģ�͹��� ��ǽṹ��
	ElementFilter	m_eElementFilter;	//	ģ��������Ԫ�ع��� ��ǽṹ��
	//bool	m_keyframeInfo;
	ECoordinateSystem	m_eCoordinateSystem;

protected:
	FILE*			pStream ;			//	ȫ���ļ�ָ��
	t3DModel*		pTheModels ;		//	ȫ��ģ��ָ��
	SceneEnumProc*	pTheSceneEnum ;		//	ȫ�ֳ����ӿ�
	MeshMtlList*	pTheMtls ;			//	ȫ�ֲ��ʽӿ�
	ObjectList*		pTheObjects ;		//	ȫ������ӿ�


private:	//	������ʱ����
	t3DObject*		newobj ;			//	��ǰ����ָ��
	tMaterialInfo*	newmat ;			//	��ǰ����ָ��
	tMatandFace*	newrender ;			//	��ǰ����������ָ��
	Mesh*			newmesh ;			//	��ǰ����ָ��
	SMtl*			savemtl ;			//	��ǰ��׼����ָ��

	vector<bool>	exportFlags;

};//class	vgModExportor
//////////////////////////////////////////////////////////		���������
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//		���������	//////////////////////////////////////////////////////
class vgModClassDesc : public ClassDesc 
{
public:
	int 			IsPublic() { return 1; }
	void *			Create(BOOL loading = FALSE) { return new	vgModExportor; }
	const TCHAR *	ClassName() { return GetString(IDS_TH_VGMOD); }
	SClass_ID		SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID() 
	{ 
#ifdef _DEBUG
		return	CLASS_ID_MOD_MAXPLUGIN_JPG_DEBUG;

#else
		return	CLASS_ID_MOD_MAXPLUGIN_JPG_RELEASE;
		
#endif//_DEBUG
	}
	const TCHAR* 	Category() { return GetString(IDS_TH_SCENEEXPORT);  }

};//class vgModClassDesc
/////////////////////////////////////////////////////////	���������
//////////////////////////////////////////////////////////////////////////

#endif	//_MODEXPORT_H_INCLUDED_