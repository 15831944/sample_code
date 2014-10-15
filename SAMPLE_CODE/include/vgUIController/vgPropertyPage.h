#pragma once

#include <vgEntry/vgBaseEntry.h>
#include <vgUIController/vgBasedTabPage.h>
#include <vgUIControllerDLL/vgUIControllerResource.h>

#include <string>
#include "vgPropList.h"

using std::string;

typedef struct PropertiesParam_t
{
	unsigned int	typeId;				// �����������
	string			label;				// ����������
	string			comment;			// ������ע��
	unsigned int	dataType;			// ������������
	void*			connectedPtr;		// ԭ����ָ��
	CBCGPProp*		pProp;				// ���ɵ����Զ���ָ��
} PropertiesParam;


typedef std::vector<PropertiesParam> PropertiesParamVector;

/**
* vgPropertyPage ʵ������ҳ���Զ�����
* �ɸ��ݲ�ͬ��PropertiesParam������Ӧ
* ������ҳ.
*
* @see		   vgTestNode1
* @see		   vgCameraNode
*
* @author      ZSC
* @version     VRGIS FrameWork RC
* @since       VRGIS 1.0+
*/

// �Զ���Property Page����BasedTabPage��������
class  VGUI_EXPORT vgPropertyPage : public CDialog
{
	DECLARE_DYNAMIC(vgPropertyPage)

public:
	vgPropertyPage(CWnd* pParent = NULL); 
	virtual ~vgPropertyPage();

	enum { IDD = NIDD_PROPERTY };
 
	string GetTitle() { return "����"; }

	CWnd* GetWnd() { return this; }

	// ����Node�����PropertiesParam��������ҳ
	// @see vgTestNode1::AddNodeTabs
	void ConnectNode(vgBaseEntry* node, PropertiesParam* propParam, int numOfProp);

	// �����°汾 ��̬Vectorʵ��
	void ConnectNode(vgBaseEntry* node, PropertiesParamVector* propParam);

	void AdjustLayout();
	
	// ���Ը��Ļص�����������ԭ����ָ���������
	virtual LRESULT OnPropertyChanged(WPARAM w, LPARAM l);
	// void OnChangeSelection(CBCGPProp*,CBCGPProp*);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	//CBCGPPropList		m_wndPropList;
	PropList			m_wndPropList;
	vgBaseEntry*		m_pNode;
	PropertiesParam*	m_pPropParam;
	int					m_numOfParam;

	// modified by zsc @ 08/10/30
	PropertiesParamVector *m_pPropVector;
	bool m_useVectorParam;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
