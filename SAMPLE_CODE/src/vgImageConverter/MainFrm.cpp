// MainFrm.cpp : CMainFrame ���ʵ��
//

#include <vgStableHeaders.h>
#include "VgimgReader.h"

#include "SaveVgimgDlg.h"

#include "MyListViewView.h"

#include "MainFrm.h"
#include "LeftView.h"
#include "VgimgReaderView.h"
#include "FileTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, &CMainFrame::OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, &CMainFrame::OnViewStyle)
	ON_COMMAND(ID_CREATE_VGIMG, &CMainFrame::OnCreateVgimg)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	m_wndToolBar.SetButtonText(0,"�½�");
	m_wndToolBar.SetButtonText(1,"��");
	m_wndToolBar.SetButtonText(3,"��ͼ��");
	m_wndToolBar.SetButtonText(4,"Сͼ��");
	m_wndToolBar.SetButtonText(5,"�б�");
	m_wndToolBar.SetButtonText(6,"��ϸ��Ϣ");
	m_wndToolBar.SetButtonText(8,"����");
	m_wndToolBar.SetButtonText(9,"ѡ��");
	m_wndToolBar.SetButtonText(10,"��������");
	m_wndToolBar.SetButtonText(11,"����");
	m_wndToolBar.SetSizes(CSize(55,45),CSize(24,24));


	// TODO: �������Ҫ��������ͣ������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	m_wndToolBar.SetFullColorImage(IDR_MAINFRAME_24, RGB(255, 0, 255));

	//m_ToolbarImageList.Create(24, 24, ILC_COLOR32, 1);

	//m_ToolbarImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	//m_ToolbarImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON4));
	//m_ToolbarImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON5));


	//m_wndToolBar.GetToolBarCtrl().SetImageList( &m_ToolbarImageList );

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// ������ִ���
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyListViewView), CSize(250, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CVgimgReaderView), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ��Ϣ�������


CVgimgReaderView* CMainFrame::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	CVgimgReaderView* pView = DYNAMIC_DOWNCAST(CVgimgReaderView, pWnd);
	return pView;
}

void CMainFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	if (!pCmdUI)
		return;

	// TODO: �Զ������չ�˴����Դ�����ͼ���˵��е�ѡ��

	CVgimgReaderView* pView = GetRightPane(); 

	// ����Ҵ�����δ�������߲�����ͼ��
	// ���ڷ�Χ�ڽ�������

	if (pView == NULL)
		pCmdUI->Enable(FALSE);
	else
	{
		DWORD dwStyle = pView->GetStyle() & LVS_TYPEMASK;

		// ��������� ID_VIEW_LINEUP����ֻ���ڴ���
		// LVS_ICON �� LVS_SMALLICON ģʽʱ����������

		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
			// ����ʹ�õ�������ӳ��ͼ����ʽ
			pCmdUI->Enable();
			BOOL bChecked = FALSE;

			switch (pCmdUI->m_nID)
			{
			case ID_VIEW_DETAILS:
				bChecked = (dwStyle == LVS_REPORT);
				break;

			case ID_VIEW_SMALLICON:
				bChecked = (dwStyle == LVS_SMALLICON);
				break;

			case ID_VIEW_LARGEICON:
				bChecked = (dwStyle == LVS_ICON);
				break;

			case ID_VIEW_LIST:
				bChecked = (dwStyle == LVS_LIST);
				break;

			default:
				bChecked = FALSE;
				break;
			}

			pCmdUI->SetRadio(bChecked ? 1 : 0);
		}
	}
}


void CMainFrame::OnViewStyle(UINT nCommandID)
{
	// TODO: �Զ������չ�˴����Դ�����ͼ���˵��е�ѡ��
	CVgimgReaderView* pView = GetRightPane();

	// ����Ҵ����Ѵ��������� CVgimgReaderView��
	// ����˵�����...
	if (pView != NULL)
	{
		DWORD dwStyle = -1;

		switch (nCommandID)
		{
		case ID_VIEW_LINEUP:
			{
				// Ҫ���б�ؼ����������
				CListCtrl& refListCtrl = pView->GetListCtrl();
				refListCtrl.Arrange(LVA_SNAPTOGRID);
			}
			break;

		// ������������б�ؼ��ϵ���ʽ
		case ID_VIEW_DETAILS:
			dwStyle = LVS_REPORT;
			break;

		case ID_VIEW_SMALLICON:
			dwStyle = LVS_SMALLICON;
			break;

		case ID_VIEW_LARGEICON:
			dwStyle = LVS_ICON;
			break;

		case ID_VIEW_LIST:
			dwStyle = LVS_LIST;
			break;
		}

		// ������ʽ�����ڽ��Զ����»���
		if (dwStyle != -1)
			pView->ModifyStyle(LVS_TYPEMASK, dwStyle);
	}
}


void CMainFrame::OnCreateVgimg()
{
	// TODO: Add your command handler code here
	//MessageBox("fff");

	CSaveVgimgDlg dlg;
	dlg.DoModal();

    dlg.GetDC();
	/*
	// ������ģ̬�Ի������ʱ��Ҫע�⣬������
	// onOK������DestroyWindow�����ſ��ԡ�
	CSaveVgimgDlg *pdlg = new CSaveVgimgDlg();
	pdlg->Create( IDD_SAVEVGIMGDLG );
	pdlg->ShowWindow( SW_SHOW );
	*/

}
