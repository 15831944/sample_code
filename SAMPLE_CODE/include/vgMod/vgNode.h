


#ifndef __VGNODE_H__
#define __VGNODE_H__


#include <vgMath/vgfBoundingBox.h>
#include <vgMod/vgModelStruct.h>
//#include <Referenced.h>

//#include <vgStableHeaders.h>
//#include <vgMod/vgQuadtreeSceneManager.h>
#include <vgKernel/vgkStreamWriter.h>
#include <vgTex/tvgImageRect.h>

#include <vgKernel/vgkStreamReader.h>


namespace vgMod{

	using namespace vgCore; 

	typedef int CategoryType;

	const CategoryType NODE_TYPE_MOD		= 1;
	const CategoryType NODE_TYPE_BB			= 2;

	const CategoryType NODE_TYPE_PARTICLE	= 30;
	const CategoryType NODE_TYPE_FOUNTAIN	= 31;
	const CategoryType NODE_TYPE_FIRE		= 32;
	const CategoryType NODE_TYPE_SMOKE		= 33;
	const CategoryType NODE_TYPE_FLAG		= 34;

	const CategoryType NODE_TYPE_MOD_EFFECT	= 4;

	class	Node;
	class	 VGDEP_EXPORT NodeAbstract/* : public CObject*//*, public Referenced*/
	{

	public:
		//zhu 2008-4-10 �ڵ����,������ʱ�ؾ�ֻ��
		bool	bHasRendered;
		int		mOcclusionQuery;
		int		mLastVisited;
		int		mLastRendered;
		void	SetOcclusionQuery(int occlusionQuery)
		{
			mOcclusionQuery = occlusionQuery;
		}
		int		GetOcclusionQuery()
		{
			return mOcclusionQuery;
		}
		void	SetLastRendered(int lastRendered)
		{
			mLastRendered = lastRendered;
		}

		int		LastVisited()
		{
			return mLastVisited;
		}
		void	SetLastVisited(int lastVisited)
		{
			mLastVisited = lastVisited;
		}



	public:
		//	���ԣ�8�
		int         m_id;			//	����ID
		TCHAR       m_name[20];		//	��������
		int         m_LayerId;		//	���ڲ�ID

		BoundingBox	m_BoundryBox;	//	��Χ��
		Vector3		m_Position;		//	����λ��

		int         m_numOfFaces;	//	����
		int         m_Type;			//	��������
		int			m_RenderTypeInt;		//	��������

	public:
		//	״̬
		unsigned int	m_offset;		//	�ļ���λ��
		unsigned int	m_size;		//	�ļ��д�С
		HTREEITEM		hitem;			//	�����б���

		bool			m_isInMemory;	//	�Ƿ��Ѵ����ڴ���
		Node*			m_pNodeData;

	public:
		NodeAbstract()
			:m_pNodeData(NULL)	{	}
		NodeAbstract(int id, int Rendertype, Vector3 Position,BoundingBox BBox, 
			int layerId, int Type, int vertsface,TCHAR name[20], Node* pNode = NULL)
		{
			m_id = id;
			m_RenderTypeInt = Rendertype;
			m_BoundryBox = BBox;
			m_Position = Position;
			m_LayerId = layerId;
			m_Type = Type;
			m_numOfFaces = vertsface;
			m_isInMemory = false;
			for (int i = 0 ; i < 20; i++)
			{
				m_name[i] = name[i];
			}

			m_pNodeData	= pNode;

		}
		NodeAbstract(const NodeAbstract& refNodeAbs)
		{
			*this = refNodeAbs;
		}

		NodeAbstract&	operator=(const NodeAbstract& refNodeAbs)
		{
			m_id = refNodeAbs.m_id;

			m_BoundryBox = refNodeAbs.m_BoundryBox;
			m_Position = refNodeAbs.m_Position;

			m_LayerId = refNodeAbs.m_LayerId;
			m_Type = refNodeAbs.m_Type;
			m_numOfFaces = refNodeAbs.m_numOfFaces;
			strcpy_s( m_name , 20, refNodeAbs.m_name);
			m_RenderTypeInt = refNodeAbs.m_RenderTypeInt;

			m_offset = refNodeAbs.m_offset;
			m_size = refNodeAbs.m_size;

			return *this;
		}
		void	Render();
		void	WriteToFile(CFile &fp)
		{
			fp.Write(&m_id, sizeof(int));

			fp.Write(&m_BoundryBox.maxPoint.x , sizeof(float));
			fp.Write(&m_BoundryBox.maxPoint.y , sizeof(float));
			fp.Write(&m_BoundryBox.maxPoint.z , sizeof(float));
			fp.Write(&m_BoundryBox.minPoint.x , sizeof(float));
			fp.Write(&m_BoundryBox.minPoint.y , sizeof(float));
			fp.Write(&m_BoundryBox.minPoint.z , sizeof(float));

			fp.Write(&m_Position.x, sizeof(float));
			fp.Write(&m_Position.y, sizeof(float));
			fp.Write(&m_Position.z, sizeof(float));

			fp.Write(&m_LayerId , sizeof(int));
			fp.Write(&m_Type , sizeof(int));
			fp.Write(&m_numOfFaces , sizeof(int));
			fp.Write(&m_name, 20);
			fp.Write(&m_RenderTypeInt, sizeof(int));

			fp.Write(&m_offset, sizeof(unsigned int));
			fp.Write(&m_size, sizeof(unsigned int));
		}
		void	ReadFromFile(CFile &fp)
		{
			fp.Read(&m_id, sizeof(int));

			fp.Read(&m_BoundryBox.maxPoint.x , sizeof(float));
			fp.Read(&m_BoundryBox.maxPoint.y , sizeof(float));
			fp.Read(&m_BoundryBox.maxPoint.z , sizeof(float));
			fp.Read(&m_BoundryBox.minPoint.x , sizeof(float));
			fp.Read(&m_BoundryBox.minPoint.y , sizeof(float));
			fp.Read(&m_BoundryBox.minPoint.z , sizeof(float));

			fp.Read(&m_Position.x, sizeof(float));
			fp.Read(&m_Position.y, sizeof(float));
			fp.Read(&m_Position.z, sizeof(float));

			fp.Read(&m_LayerId , sizeof(int));
			fp.Read(&m_Type , sizeof(int));
			fp.Read(&m_numOfFaces , sizeof(int));
			fp.Read(&m_name, 20);
			fp.Read(&m_RenderTypeInt, sizeof(int));

			fp.Read(&m_offset, sizeof(unsigned int));
			fp.Read(&m_size, sizeof(unsigned int));
		}
		void	WriteToFile(vgKernel::StreamWriterPtr	fp)
		{
			fp->write(&m_id, sizeof(int));

			fp->write(&m_BoundryBox.maxPoint.x , sizeof(float));
			fp->write(&m_BoundryBox.maxPoint.y , sizeof(float));
			fp->write(&m_BoundryBox.maxPoint.z , sizeof(float));
			fp->write(&m_BoundryBox.minPoint.x , sizeof(float));
			fp->write(&m_BoundryBox.minPoint.y , sizeof(float));
			fp->write(&m_BoundryBox.minPoint.z , sizeof(float));

			fp->write(&m_Position.x, sizeof(float));
			fp->write(&m_Position.y, sizeof(float));
			fp->write(&m_Position.z, sizeof(float));

			fp->write(&m_LayerId , sizeof(int));
			fp->write(&m_Type , sizeof(int));
			fp->write(&m_numOfFaces , sizeof(int));
			fp->write(&m_name, 20);
			fp->write(&m_RenderTypeInt, sizeof(int));

			fp->write(&m_offset, sizeof(unsigned int));
			fp->write(&m_size, sizeof(unsigned int));
		}
		void	ReadFromFile(vgKernel::StreamReaderPtr	fp)
		{
			fp->read(&m_id, sizeof(int));

			fp->read(&m_BoundryBox.maxPoint.x , sizeof(float));
			fp->read(&m_BoundryBox.maxPoint.y , sizeof(float));
			fp->read(&m_BoundryBox.maxPoint.z , sizeof(float));
			fp->read(&m_BoundryBox.minPoint.x , sizeof(float));
			fp->read(&m_BoundryBox.minPoint.y , sizeof(float));
			fp->read(&m_BoundryBox.minPoint.z , sizeof(float));

			fp->read(&m_Position.x, sizeof(float));
			fp->read(&m_Position.y, sizeof(float));
			fp->read(&m_Position.z, sizeof(float));

			fp->read(&m_LayerId , sizeof(int));
			fp->read(&m_Type , sizeof(int));
			fp->read(&m_numOfFaces , sizeof(int));
			fp->read(&m_name, 20);
			fp->read(&m_RenderTypeInt, sizeof(int));

			fp->read(&m_offset, sizeof(unsigned int));
			fp->read(&m_size, sizeof(unsigned int));
		}

		void	deleteData();

		bool GetMemState()					{	return m_isInMemory; }
		void SetMemState(const bool& state)	{	this->m_isInMemory = state;}
		BoundingBox& GetBoundryBox()		{	return m_BoundryBox;}

		Node*	getNodeData()				{	return 		m_pNodeData;}
	};//class NodeAbstract


	typedef std::vector<NodeAbstract*>	NodeAbsPtrVec;	
	typedef std::vector<NodeAbstract*>::iterator	NodeAbsPtrVecItr;	

	class	 VGDEP_EXPORT Node /*: public CObject*/ /* , public Referenced*/
	{

	public:
		//	���ԣ�8�
		int			m_id;			//	����ID
		TCHAR		m_name[20];		//	��������
		TCHAR		m_LayerName[20];//	���ڲ�����   

		BoundingBox	m_BoundryBox;	//	��Χ��
		Vector3     m_Position;		//	����λ��         

		long		m_numOfFaces;	//	����
		int			TYPE;			//	��������
		int			m_RenderTypeInt;		//	�������ͣ�0����һ�����Ⱦ���У�1����blend���У�2����depth���У�

		CategoryType	m_category;		//  �������ࣻ1��mod��2��billboard��3��particle

	public:
		//	״̬
		long		nodelength;		//	�ļ���λ��
		long		Posinfile;
		int			m_propid;       //	�����б���ID��ΪLOD����*/

		bool		m_isSelected;        /*��ѡ��״̬*/
		bool		m_isCollideCheck;    /*��ײ���״̬*/
		bool		m_isVisible;         /*�Ƿ�ɼ�*/
		bool		m_isDeleted;         /*�Ƿ���ɾ�����*/


	public:
		//	���ݿ�
		int			m_numOfVerts;	//	����
		short		m_numOfMat;		//	������
		MaterialStruct*		pMats;	//	��������ָ��


	public:
		//	������ʱ����
		Vector3     m_Pivot;
		Vector3     m_prePositon;

		Vector3     m_Rotation;          /*����������ʾ������¼������redo,undo��*/
		Vector3     m_PreRotation;		

		Vector3     m_PreScale;
		Vector3     m_Scale;


	public:
		//	�������ԡ�״̬����������
		void       SetId(int idValue)                   {	m_id = idValue;			}
		int        GetId()                              {	return m_id;			}
		void       SetName(TCHAR* nameString)			{	strcpy_s(m_name, 20, nameString);}
		TCHAR*     GetName()                            {	return m_name;			}
		void       SetLayer(TCHAR layerName[20])        {	strcpy_s(m_LayerName, 20, layerName);}
		TCHAR*     GetLayer()                           {	return m_LayerName;		}

		BoundingBox GetBoundryBox()                     {	return m_BoundryBox;	}
		void       SetPosition(Vector3 position)        {	m_Position = position;	}
		Vector3    GetPosition()						{	return m_Position;		}

		int			getNodeType()						{	return TYPE; }
		void		setNodeType(int nodetype)			{	TYPE = nodetype;}	

		CategoryType getCategory()						{	return m_category;	}
		void		setCategory(CategoryType cat)		{	m_category = cat;}	


		void       SetSelected(bool isSelected)         {	m_isSelected = isSelected;}
		bool       GetSelected()                        {	return m_isSelected;}
		void       SetCollideCheck(bool isCollideCheck) {	m_isCollideCheck = isCollideCheck;}
		bool       GetCollideCheck()                    {	return m_isCollideCheck;} 
		virtual		void   SetVisable(bool isVisable)           {	m_isVisible = isVisable;}
		virtual		bool&   GetVisable()                         {	return m_isVisible;}
		void       SetDeleted(bool isDeleted)           {	m_isDeleted = isDeleted;}
		bool       GetDeleted()                         {	return m_isDeleted;}

		Vector3    GetPivot()							{	return m_Pivot;}
		void       SetRotation(Vector3 rotation)        {	m_Rotation = rotation;}
		Vector3    GetRotation()						{	return m_Rotation;}
		void       SetScale(Vector3 scale)              {	m_Scale = scale;}
		Vector3    GetScale()							{	return m_Scale;}


	public:
		//	���麯����9�������������ʵ��
		virtual void	Initialize(){};
		virtual void	Render()	= 0;

		virtual void	WriteNodeToVG(CFile &fp)	= 0;
		virtual	unsigned long PositDataToNode(char *pBytes)	= 0;

		virtual void	Translate(Vector3 offset)
		{

		}
		virtual void	Rotate(short axis, float Angle)
		{

		}
		virtual void	Scale(Vector3 scale)	
		{

		}

		virtual bool	testIntersection( vgMath::Ray* ray )	= 0;// ���ھ�ȷ��ѡ(��box��ray�ཻ�ж�)
		virtual bool	testIntersection( const vgCore::ImageRect& rect ,
			int *viewport , GLdouble* proj_mat , GLdouble* model_mat )	= 0;// ���ھ�ȷ��ѡ(��box��8�����ж�)

	public:
		//	�麯�����������ѡ�������ط�ʽʵ��
		virtual void	ReadNodeFromVG(CFile &fp)			{	return;	}
		virtual BoundingBox		ComputeBoundBox()			{	return	m_BoundryBox;}

		virtual void			RenderShadows()				{	return;	}
		virtual void			render_ForShadow()			{	return;	}
		virtual void			update_ForShadowEveryTime()	{	return;	}

		virtual ~Node()	{	return;	}//	����Ϊ�麯���������Ժ�����ڴ�й©
		virtual void	UpdatePos();//	�Ѿ�����ʵ�֣�����ֻ���ڴ˻�������չ

		virtual void	RenderWithoutTex(float r, float g, float b)	{	return;	}//	������
		virtual void	Terminate()									{	return;	}//	������

		// �����п鱣���ռ�MOD��Ч��ͼ��Ϣ(water,glass,skybox,mirror,metal��)
		virtual vgKernel::StringVector getTexNameVec() 
		{ 
			vgKernel::StringVector texNameVec;
			return texNameVec;
		}

	public:
		//	��ȫʵ�ֵĺ���
		Node();	

		Node(CategoryType category);

		void setDefault();

		//	Χ��һ����ת��һ�㡣����Ϊ�ο��㣬��ת�Ƕȣ�Ҫ��ת�ĵ�
		void	RotatePoint(float baseU , float baseV , float angle , float *pointU , float *pointV);
		double	GetAngle(float baseU , float baseV , float pointU , float pointV);
		double	CallDisp(float U1, float V1, float U2, float V2);
	};//class Node


	typedef std::vector<Node*>	NodePtrVec;	
	typedef std::vector<Node*>::iterator	NodePtrVecItr;	
	typedef std::map< CategoryType, NodePtrVec > NodePtrPackage;


}//namespace vgMod


#endif // end of __VGNODE_H__