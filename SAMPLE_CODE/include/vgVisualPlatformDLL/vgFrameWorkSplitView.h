// vgFrameWorkView.h : interface of the CvgFrameWorkView class
//


#pragma once



#include <vgEntry/vgBaseEntry.h>
#include <vector>

//#include <vgMFCApplication.h>
#include <IPDlg.h>
#include <afxsock.h>
using std::vector;



#include "vgFrameBuffer.h"
#include "vgcaAviFile.h"
#include "vgAviExportDialog.h"
#include <vgDatabase/vgDlgCondition.h>

#include <vgKernel/vgkRenderCommand.h>
#include "vgFrameWorkDoc.h"
//////////////////////////////////////////////////////////////////////////
//��ͨ��
//////////////////////////////////////////////////////////////////////////
//static ChannelClient *client;
//static ChannelServer *server;
#pragma   warning(disable:4099)

class CvgFrameWorkSplitView : public CView
{
public: // create from serialization only
	CvgFrameWorkSplitView();
	DECLARE_DYNCREATE(CvgFrameWorkSplitView)

	//////////////////////////////////////////////////////////////////////////

	//------------------------------------------
	// ��ǰ�İ汾��д������
	//------------------------------------------
	//vgCore::MFCApplication vgApp;

	//------------------------------------------
	// zhu raknet��ͨ��
	//------------------------------------------
	DWORD ThreadID;
	HANDLE hSendThread;

	static DWORD WINAPI threadFunc(LPVOID Data);

	vgKernel::RenderCommand *pClientRenderCmd;
// 	CMenu *RightMenu;
// 	CMenu menu;
	//////////////////////////////////////////////////////////////////////////
// Attributes
public:
	CvgFrameWorkDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CvgFrameWorkSplitView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
	bool initGLContext(void);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void SwitchView();
private:
	// device
	CDC*	m_pDC;
	HGLRC	m_hGLContext;

	CSplitterWnd m_wndsplitter;

public:
	afx_msg void OnDestroy();
	void releaseGLContext(void);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void renderScene(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnVgmImportVgm();
	afx_msg void OnVgiImportVgi();
	afx_msg void OnVgmSaveVgm();
	afx_msg void OnVgmiSaveVgmVgi();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnVgterrainWireframe();
	afx_msg void OnVgterrainNowireframe();
	afx_msg void OnVgterrainTurnon();
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);

	//afx_msg LRESULT OnRender(WPARAM wParam, LPARAM lParam); 
	virtual void OnInitialUpdate();
	afx_msg void OnVgtestFf();
	afx_msg void OnVgterrainConvertDemToVgtr();
	afx_msg void OnVgterrainConvertVgtrHeightmap();
	afx_msg void OnTtttttest();
	afx_msg void OnFileImportMod();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//afx_msg void OnCameraCapture();

	
	// ���&�������
	afx_msg void OnCreatcam();
	afx_msg void OnAniStart();
	afx_msg void OnAniEnd();
	afx_msg void OnAniPlay();
	afx_msg void OnAniResume();
	afx_msg void OnAniStop();

	afx_msg void OnSwitchcam();
	afx_msg void OnPositiontoobj();


	// ��Ч
	afx_msg void OnSeFountain();
	afx_msg void OnFireAdd();
	afx_msg void OnSeSmoke();
	afx_msg void OnAddflag();
	afx_msg void OnWaterConvert();

	afx_msg void OnGlassConvert();
	afx_msg void OnAviNodeConvert();
	afx_msg void OnSkyboxConvert();
	afx_msg void OnBeginplant();
	afx_msg void OnDyntextureConvert();
	afx_msg void OnRain();
	afx_msg void OnRainDel();
	afx_msg void OnSnow();
	afx_msg void OnSnowDel();
	afx_msg void OnFogDel();
	afx_msg void OnSeFog();
	afx_msg void OnMirrorConvert();
	afx_msg void OnMetalConvert();
	afx_msg void OnConvertBrigthfloor();
	afx_msg void On3dsoundAdd();
	afx_msg void OnBacksoundSet();
	afx_msg void OnFileImportMs3d();
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnAddanimation();
	afx_msg void OnSettree();
	afx_msg void OnSelect();
	afx_msg void OnPlantinline();
	afx_msg void OnConfigdynbillboard();
	afx_msg void OnDynamicbillboard();


	afx_msg void OnVgterrainNewVtr();
	
	// GIS
	afx_msg void OnZuobiao();
	afx_msg void OnJuli();
	afx_msg void OnMianji();
	afx_msg void OnTiji();

	// Tools
	afx_msg void OnToolMakerVgm();
	afx_msg void OnToolMakerVgi();

	afx_msg void OnToolMakerDds();

	afx_msg void OnShapefileDb();


	// File
	afx_msg void OnSceneRead();
	afx_msg void OnSceneSave();
	afx_msg void OnPublishCollect();

	// Edit
	afx_msg void OnEditSelect();
	afx_msg void OnEditSelectClear();
	afx_msg void OnFindObject();
	afx_msg void OnFileImportKfm();
	afx_msg void OnEditTranslate();
	afx_msg void OnEditRotate();
	afx_msg void OnEditScale();
	afx_msg void OnUpdateEditTranslate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRotate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditScale(CCmdUI *pCmdUI);

	// multi channel
	afx_msg void OnChannel1();
	afx_msg void OnChannel0();
	afx_msg void OnChannelm1();
	afx_msg void OnSetupServer();
	afx_msg void OnDestroyServer();
	afx_msg void OnNewClient();
	afx_msg void OnDelClient();

	// aboutDlg
	afx_msg void OnAppAbout();
	afx_msg void OnOccSwitch();
	afx_msg void OnFileImport();
	afx_msg void OnGismatch();


	//void OnSceneSave(string	strSavePath);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnFileExport();
	afx_msg void OnUndo();
	afx_msg void OnRedo();
	afx_msg void OnMeshOrLine();

	afx_msg void OnUpdateOccSwitch(CCmdUI *pCmdUI);
	afx_msg void OnEditTranall();
	//afx_msg void OnShowModels();
	//afx_msg void OnUpdateShowModels(CCmdUI *pCmdUI);

	afx_msg void OnVgtAttach();
	afx_msg void OnTerrainCollisionDetect();
	afx_msg void OnUpdateTerrainCollisionDetect(CCmdUI *pCmdUI);
	afx_msg void On32972();
	afx_msg void OnUpdate32972(CCmdUI *pCmdUI);
	afx_msg void On32973();
	afx_msg void OnUpdate32973(CCmdUI *pCmdUI);
	afx_msg void On32974();
	afx_msg void OnUpdate32974(CCmdUI *pCmdUI);
	afx_msg void OnVgtVisibility();
	afx_msg void OnUpdateVgtAttach(CCmdUI *pCmdUI);
	afx_msg void OnUpdateVgtVisibility(CCmdUI *pCmdUI);


	afx_msg void OnHideVec();
	afx_msg void OnNoselVec();

	//afx_msg void OnUpdateTerrainLeveling(CCmdUI *pCmdUI);
	afx_msg void OnVgtCutAnalysis2();
	afx_msg void OnUpdateVgtCutAnalysis2(CCmdUI *pCmdUI);
	afx_msg void OnVgtTerrainSave();
	afx_msg void OnUpdateVgtTerrainSave(CCmdUI *pCmdUI);
	afx_msg void OnVgtBlend();
	afx_msg void OnUpdateVgtBlend(CCmdUI *pCmdUI);
	//afx_msg void OnVgtVisibility2();
	//afx_msg void OnUpdateVgtVisibility2(CCmdUI *pCmdUI);
	//afx_msg void OnVgtCutarea();
	//afx_msg void OnUpdateVgtCutarea(CCmdUI *pCmdUI);
	afx_msg void OnNdbConnect();
	afx_msg void OnNdbCondition();
	afx_msg void OnCollisionOnoff();
	afx_msg void OnUpdateCollisionOnoff(CCmdUI *pCmdUI);
	afx_msg void OnVgtMappingOrigin();
	afx_msg void OnUpdateVgtMappingOrigin(CCmdUI *pCmdUI);
	afx_msg void OnVectorShpExport2db();


public:
	afx_msg void OnPdsPlay();
	afx_msg void OnPdsStop();
	afx_msg void OnPdsContinue();
	afx_msg void OnJkqPlay();
	afx_msg void OnJkqStop();
	afx_msg void OnJkqContinue();
	afx_msg void OnVgviBgConfig();
	afx_msg void OnNdbDisconnect();
	afx_msg void OnScriptRunfile();
	afx_msg void OnScriptTestscript();
	afx_msg void OnScriptTestmenu();
	afx_msg void OnScriptConsole();
	afx_msg void OnPublishPackageExe();
	afx_msg void OnSceneReset();
	afx_msg void OnVectorShpUnload();
	afx_msg void Onterraintesting();
	afx_msg void OnUpdate33109(CCmdUI *pCmdUI);
	
	
	afx_msg void OnVgscUserScriptConfig();
	afx_msg void OnTanslateSelected();
	afx_msg void OnGisSun();

private:
	// AVI �ļ��������
	int m_fps;

public:
	//���ݿ��ѯ������ģʽ�Ի���
	DBCondition* m_dbCondition;
	afx_msg LONG OnDelDlgMsg(WPARAM wP,LPARAM lP);
public:
	afx_msg void OnExpAvi();
	afx_msg void OnExpBmp();

	afx_msg void OnVgshapeReadshpfile();
	afx_msg void OnVgshapeSelectshparea();
	afx_msg void OnCamReheight();
	afx_msg void OnVgshapeSaveasshape();
	afx_msg void OnEditSetTransparent();
	afx_msg void OnVgshapeTranslatetoorigin();
	afx_msg void OnVgshapeAttachtoterrain();
	afx_msg void OnVgshapeSetrendererboxasfilter();
	afx_msg void OnAutobuild();
	afx_msg void OnVgshapeReadgridshpfile();

	afx_msg void OnVectorShpGreenRatio();
	afx_msg void OnUpdateVectorShpGreenRatio(CCmdUI *pCmdUI);
	/*afx_msg void OnVgtestSelectMode();
	afx_msg void OnUpdateVgtestSelectMode(CCmdUI *pCmdUI);*/
	afx_msg void OnUpdateVgshapeSelectshparea(CCmdUI *pCmdUI);
	afx_msg void OnAutobuildClone();
	afx_msg void OnEditSingleFaceRenderSel();
	afx_msg void OnEditSingleFaceRenderAll();
	afx_msg void OnUpdateEditSingleFaceRenderSelect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditSingleFaceRenderAll(CCmdUI *pCmdUI);
	afx_msg void OnTriggerSetAs();
	afx_msg void OnVgshapeImporttext();
	/*afx_msg void OnVgshapeExportshapeinfo();*/
	afx_msg void OnVgshapeCaculateposition();
	afx_msg void OnVgshapeClearplan();
	afx_msg void OnVgshapeGenerateselected();
	afx_msg void OnVgshapeImportinfofromtxt();
	afx_msg void OnAutobuildingCloseinfofile();
	afx_msg void OnTriggerConfig();
	afx_msg void OnQueryplaninfoQueryplotratio();
	afx_msg void OnUpdateQueryplaninfoQueryplotratio(CCmdUI *pCmdUI);
	afx_msg void OnVgshapeTranslateall();
	afx_msg void OnVgplanAddground();
	afx_msg void OnVgplanAddsecne();
	afx_msg void OnVgshadowTurnonshadow();
	afx_msg void OnVgshadowSelectshadowcastobjects();
	afx_msg void OnVgplanSetparameter();
	afx_msg void OnVgshadowChange();
	afx_msg void OnVgshadowTurnoffshadow();
	afx_msg void OnVgshadowSolaranalysis();
	afx_msg void OnUpdateVgshadowSolaranalysis(CCmdUI *pCmdUI);
	afx_msg void OnUpdateVgshadowTurnonshadow(CCmdUI *pCmdUI);
	afx_msg void OnEditSetUnTransparent();
	afx_msg void OnTesttestSelectchangecolor();
	afx_msg void OnVgplanAddstreetline();
	afx_msg void OnPlanExportscene();
	afx_msg void OnVgplanGenerateplanmap();
	afx_msg void OnVgplanAddregion();
	afx_msg void OnVgplanAddnew();
	afx_msg void OnVgplanCgrid();
	afx_msg void OnVgplanCalgrid();
	afx_msg void OnVgplanGenplan();
	afx_msg void OnVgplanOutput();
	/*afx_msg void OnCloseLogo();
	afx_msg void OnUpdateCloseLogo(CCmdUI *pCmdUI);*/
	afx_msg void OnVmdImport();
	afx_msg void OnLoadVtrDll();
	afx_msg void OnUnloadVtrDll();
	afx_msg void OnUpdateVgterrainNewVtr(CCmdUI *pCmdUI);
	afx_msg void OnScenePublish();
	afx_msg void OnUpdateZuobiao(CCmdUI *pCmdUI);
	afx_msg void OnUpdateJuli(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMianji(CCmdUI *pCmdUI);
	afx_msg void OnVgsoLoadDll();
	afx_msg void OnVgsoUnloadDll();
	afx_msg void OnUpdateScenePublish(CCmdUI *pCmdUI);
	
	void OnExportNewVg();
	void OnImportNewVg();
	afx_msg void OnVcrAcce();
	afx_msg void OnVcrDeacce();
	afx_msg void OnUpdateMeshOrLine(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditSetTransparent(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditSetUntransparent(CCmdUI *pCmdUI);
	afx_msg void OnSaveCutBlock();
	afx_msg void OnSwitchToTopView();
	afx_msg void OnBeginCutBlock();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnWaterDirection();
	afx_msg void OnSolidBuild();
	afx_msg void OnUpdateSolidBuild(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in vgFrameWorkView.cpp
inline CvgFrameWorkDoc* CvgFrameWorkSplitView::GetDocument() const
   { return reinterpret_cast<CvgFrameWorkDoc*>(m_pDocument); }
#endif

