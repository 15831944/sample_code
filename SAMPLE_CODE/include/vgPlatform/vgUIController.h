// vgUIController 

#pragma once

#include "vgPropertiesViewBar.h"
#include "vgWorkSpaceBar.h"
#include "vgBasedNode.h"
#include "vgBasedTabPage.h"
#include "vgTabDef.h"
#include "vgPropertyPage.h"

#include "vgCameraNode.h"
#include "vgRootNode.h"
#include "vgTestNode1.h"

/**
* vgUIController ��UI�ĺ��Ŀ�����������ڹ���Ӧ�ó������
* ������ͼ��������ͼ��vgUIController ʹ�õ�̬��װ���û���
* ͨ����̬��������UI���.
* ��ǰ�汾vgUIController�ɹ�������ͼvgWorkSpaceBar,������
* ͼvgPropertyViewBar.
*
* @see		   vgWorkSpaceBar
* @see		   vgPropertiesViewBar
*
* @author      ZSC
* @version     VRGIS FrameWork RC
* @since       VRGIS 1.0+
*/

//enum	E_IMAGE_TYPE
//{
//	NONE_SELECT,				// 0
//	
//	BASE_SELECT,				// 1
//	BASE_UNSELECT,
//	
//	OBJECT_SELECT,				// 3
//	OBJECT_UNSELECT,
//	OBJECT_KEYFRAME_SELECT,		// 5
//	OBJECT_KEYFRAME_UNSELECT,
//	OBJECT_BAKED_SELECT,		// 7
//	OBJECT_BAKED_UNSELECT
//};

class vgUIController
{
public:
	static vgUIController* GetInstance();
	static void DestoryInstance();

protected:
	static vgUIController* s_pInstance;

public:
	vgPropertiesViewBar* GetPropertiesViewBar();

	void AddTabPage(string pageName, CWnd* page);
	
	void RemoveAllPages();

	vgWorkSpaceBar* GetWorkSpaceBar();

	HTREEITEM AddNode(HTREEITEM parent, vgBasedNode* node); 
	
	HTREEITEM AddNode(vgBasedNode* node); 
	
	HTREEITEM AddNode(vgBasedNode* parentNode, vgBasedNode* node); 
	
	// ����ע��ID �õ���������ҳ
	// @see vgTabDef.h
	vgBasedTabPage* GetTabPage(long ID);
	
	// �õ��Զ�����ҳ�����Զ�����
	// @see vgPropertyPage
	vgPropertyPage* GetPropPage();

	vector<vgBasedNode*>& GetNodeList(); 
	
	// ���ݽڵ����ƻ�ýڵ�
	vgBasedNode* GetNode(string nameID); //{ return m_pWorkSpaceBar->GetNode(nameID); }
	
	// ֪ͨ����ͼѡ��pNodeָ��Ľڵ�
	void SelectNode(vgBasedNode* pNode);// { m_pWorkSpaceBar->SelectNode(pNode); }

	// �õ���ǰѡ�еĽڵ�
	// ���½ڵ�����ʱ��ͨ���˺����ж��Ƿ�Ӧ�ø���������ʾ
	// @see vgTestNode1::SetPosX
	vgBasedNode* GetCurrentSelectedNode(); //{ return m_currentSelected; }

	// FrameWork�������õ�ǰѡ��ڵ�
	// @see vgEditTreeCtrl::OnSelectedChanged
	void SetCurrentSelectedNode(vgBasedNode* pNode); //{ m_currentSelected = pNode; }

protected:
	vgUIController(void);
	
	~vgUIController(void);
	
	vgBasedNode					*m_currentSelected;
	
	// ������ͼ
	vgPropertiesViewBar			*m_pTabView;
	
	// ����ͼ
	vgWorkSpaceBar				*m_pWorkSpaceBar;	
	
	// ��ע���Tabҳ
	vgBasedTabPage*				m_listTabPage[TAB_ARRAY_SIZ];
	
	// ���Զ����ɵ�����ҳ
	vgPropertyPage*				m_propPage;

	//ui////////////////////////////////////////////////////////////////////////
	HTREEITEM		m_hRootNode;	//rootnode��

	HTREEITEM		m_hRootVGM;		//vgm��
	HTREEITEM		m_hRootVG;		//vg��
	HTREEITEM		m_hRootCamera;	//camera��
public:
	bool			InitUIControl(void);
	HTREEITEM&	GetRootNode()		{	return m_hRootNode; }
	HTREEITEM&	GetRootVGM()		{	return m_hRootVGM; }
	HTREEITEM&	GetRootVG()			{	return m_hRootVG; }
	HTREEITEM&	GetRootCamera()		{	return m_hRootCamera; }
};

inline vector<vgBasedNode*>& vgUIController::GetNodeList()
{
	return m_pWorkSpaceBar->GetNodeList();
}

inline HTREEITEM vgUIController::AddNode(HTREEITEM parent, vgBasedNode* node) 
{ 
	return m_pWorkSpaceBar->AddNode(parent, node); 
}

inline HTREEITEM vgUIController::AddNode(vgBasedNode* node) 
{ 
	return m_pWorkSpaceBar->AddNode(node); 
}

inline HTREEITEM vgUIController::AddNode(vgBasedNode* parentNode, vgBasedNode* node) 
{ 
	return m_pWorkSpaceBar->AddNode(parentNode, node); 
}

inline vgBasedNode* vgUIController::GetNode(string nameID)
{ 
	return m_pWorkSpaceBar->GetNode(nameID); 
}

inline void vgUIController::SelectNode(vgBasedNode* pNode)
{ 
	m_pWorkSpaceBar->SelectNode(pNode); 
}

inline vgBasedNode* vgUIController::GetCurrentSelectedNode() 
{ 
	return m_currentSelected; 
}

inline void vgUIController::SetCurrentSelectedNode(vgBasedNode* pNode) 
{ 
	m_currentSelected = pNode; 
}
