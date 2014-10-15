// vgFrameWorkView.cpp : implementation of the CvgFrameWorkView class
//



#include <vgStableHeaders.h>
#include "vgFrameWork.h"
#include "vgFrameWorkDoc.h"
#include "vgFrameWorkView.h"
#include "vgUIController.h"
#include "vgBaseEntry.h"

#include "vgTestNode1.h"
#include "vgTerrainEntry.h"


#include <sys/timeb.h>
#include <MainFrm.h>


#include <vgKernel/vgkOpenGLSupport.h>
#include <vgMesh/vgmMeshManager.h>
#include <vgGlobal/vggGlobalFacade.h>
#include <vgGlobal/vggGlobalConsole.h>
#include <vgKernel/vgkRenderer.h>

#include <vgKernel/vgkRay.h>
#include <vgKernel/vgkMath.h>
#include <vgKernel/vgkInputSystem.h>
#include <vgKernel/vgkSelectManager.h>


#include <vgDlgTestMultisample.h>


#include <vgMesh/vgmFileHandleVgm.h>

#include <vgCam/vgcaCamManager.h>
#include "vgEntryFactory.h"
#include <vgCam/vgcaViewCam.h>
#include <vgCam/vgcaCameraRecord.h>

#include "vgTree/vgtTreeFactory.h"
#include <vgKernel/vgkRendererManager.h>
#include <vgGIS3D/vgGIS3D.h>
#include <vgEffect/vgAviNode.h>
#include <vgDlgAviNode.h>
#include <vgQuadtreeSceneManager.h>


#include <vgOcc/vgoOcclusion.h>


#include <vgTerritory/vgtTerrainManager.h>
#include <vgTerritory/vgtElevationFilter.h>

#include "DlgAbout.h"
#include "DlgTranslateAll.h"
#include "DlgTranslateVector.h"
#include "DlgShpToDBEx.h"

#include <vgGlobal/vggGlobalUtility.h>

#include <vgVector/vgvVectorManager.h>

#include <vgDatabase/vgdDatabaseManager.h>
#include <vgDatabase/vgdOracleConnector.h>
#include <vgDatabase/vgdBasedConnector.h>
#
#include <vgDatabase/vgDlgDBConnect.h>
#include <vgDatabase/vgDlgCondition.h>

#include <vgSound/vgSoundManager.h>
#include <vgVisualEffects/vgviBackgroundManager.h>
#include <vgScript/vgscScriptManager.h>

#include <vgCam/vgcaCamFacade.h>
#include <vgScript/vgscScriptManager.h>
#include <vgGlobal/vggSystemFacade.h>

#include <vgTerritory/vgtTerrainCenterManager.h>
#include <vgMod/vgModTerrainManager.h>

#include "vgDlgSun.h"
#include <vgSuperPlatform/vgaviexportdialog.h>
#include <vgSuperPlatform/vgExportBMPDialog.h>

#include <vgFont/vgFont.h>
#include <vgTerritory/vgtCutSurfaceLevelling.h>

#include <vgUIController.h>
#include <vgImage/vgiCximageWrapper.h>

#include <vgShape/vgshShapeManager.h>
#include "vgVcrHeightDlg.h"
#include "vgShapeDataSourceEntry.h"

#include "vgShapeLayerEntry.h"

#include <vgVector/vgAutoBuildNode.h>
#include <vgVector/vgvAutoBuildManager.h>
#include <vgShape/vgshSpecialPolygon.h>
#include <vgShape/vgshPolygon.h>

#include <vgMod/vgTriggerManager.h>

#include "DlgAutoBuildInput.h"
#include "DlgAutoIn.h"
#include <vgLogo/vgloLogoManager.h>
#include "DlgTriggerConfig.h"
#include "vgDlgDynamicTexNode.h"
#include <vgPlan/vgPlanmanager.h>
#include "DlgPlanParam.h"
#include "DlgPlanShpIn.h"
#include "DlgPlanBlock.h"
#include "DlgPlanOutput.h"

#include <vgSolar/vgsoSolarDirectionCalc.h>
#include <vgSolar/vgsoSolarManager.h>
#include <vgSolar/SolarConfig.h>
#include <vgGIS3D/vgSunAngle.h>

using std::list;
using std::vector;
using namespace vgDatabase;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define VG_MSG_RENDER WM_USER + 100

void TestData(); 

struct THREADFLAG g_threadFlag;

static UINT RenderThread(LPVOID pParam);
CCriticalSection g_renderCriticalArea;


#define VGMSGBOX2(A) {\
	this->SetTimer(1, 150, NULL);\
	A; \
	this->SetTimer(1, 15, NULL);}

// ---------------------------------------------------------

// CvgFrameWorkView

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CvgFrameWorkView, CView)

BEGIN_MESSAGE_MAP(CvgFrameWorkView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(NID_VGM_IMPORT_VGM, &CvgFrameWorkView::OnVgmImportVgm)
	ON_COMMAND(NID_VGI_IMPORT_VGI, &CvgFrameWorkView::OnVgiImportVgi)
	ON_COMMAND(NID_VGM_SAVE_VGM, &CvgFrameWorkView::OnVgmSaveVgm)
	ON_COMMAND(NID_VGMI_SAVE_VGM_VGI, &CvgFrameWorkView::OnVgmiSaveVgmVgi)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(NID_VGTERRAIN_WIREFRAME, &CvgFrameWorkView::OnVgterrainWireframe)
	ON_COMMAND(NID_VGTERRAIN_NOWIREFRAME, &CvgFrameWorkView::OnVgterrainNowireframe)
	ON_COMMAND(NID_VGTERRAIN_TURNON, &CvgFrameWorkView::OnVgterrainTurnon)
	ON_WM_ACTIVATEAPP()

	ON_MESSAGE(VG_MSG_RENDER, &CvgFrameWorkView::OnRender)
	ON_COMMAND(NID_VGTERRAIN_CONVERT_DEM_TO_VGTR, &CvgFrameWorkView::OnVgterrainConvertDemToVgtr)
	ON_COMMAND(NID_VGTERRAIN_CONVERT_VGTR_HEIGHTMAP, &CvgFrameWorkView::OnVgterrainConvertVgtrHeightmap)
	ON_COMMAND(ID_TTTTTTEST, &CvgFrameWorkView::OnTtttttest)
	ON_COMMAND(NID_FILE_IMPORT_MOD, &CvgFrameWorkView::OnFileImportMod)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_CAMERA_CAPTURE, &CvgFrameWorkView::OnCameraCapture)
	ON_COMMAND(ID_TOOL_TRAVERSESWITCH, &CvgFrameWorkView::OnToolTraverseswitch)
	ON_UPDATE_COMMAND_UI(ID_TOOL_TRAVERSESWITCH, &CvgFrameWorkView::OnUpdateToolTraverseswitch)
	ON_COMMAND(ID_CREATCAM, &CvgFrameWorkView::OnCreatcam)
	ON_COMMAND(ID_ANI_Start, &CvgFrameWorkView::OnAniStart)
	ON_COMMAND(ID_ANI_END, &CvgFrameWorkView::OnAniEnd)
	ON_COMMAND(ID_ANI_PLAY, &CvgFrameWorkView::OnAniPlay)
	ON_COMMAND(ID_ANI_RESUME, &CvgFrameWorkView::OnAniResume)
	ON_COMMAND(ID_CREATCAM, &CvgFrameWorkView::OnCreatcam)
	ON_COMMAND(ID_ANI_STOP, &CvgFrameWorkView::OnAniStop)

	ON_COMMAND(ID_SWITCHCAM, &CvgFrameWorkView::OnSwitchcam)
	ON_COMMAND(ID_POSITIONTOOBJ, &CvgFrameWorkView::OnPositiontoobj)

	ON_COMMAND(id_fountianset, &CvgFrameWorkView::Onfountianset)
	ON_COMMAND(ID_SE_FOUNTAIN, &CvgFrameWorkView::OnSeFountain)
	ON_COMMAND(ID_FIRE_ADD, &CvgFrameWorkView::OnFireAdd)
	ON_COMMAND(ID_SE_SMOKE, &CvgFrameWorkView::OnSeSmoke)
	ON_COMMAND(ID_ADDFLAG, &CvgFrameWorkView::OnAddflag)
	ON_COMMAND(ID_WATER_CONVERT, &CvgFrameWorkView::OnWaterConvert)
	ON_COMMAND(ID_WATER_EDIT, &CvgFrameWorkView::OnWaterEdit)
	ON_COMMAND(ID_GLASS_CONVERT, &CvgFrameWorkView::OnGlassConvert)
	ON_COMMAND(ID_AVI_NODE_CONVERT, &CvgFrameWorkView::OnAviNodeConvert)
	ON_COMMAND(ID_SKYBOX_CONVERT, &CvgFrameWorkView::OnSkyboxConvert)
	ON_COMMAND(ID_BEGINPLANT, &CvgFrameWorkView::OnBeginplant)

	ON_COMMAND(ID_RAIN, &CvgFrameWorkView::OnRain)
	ON_COMMAND(ID_SNOW, &CvgFrameWorkView::OnSnow)
	ON_COMMAND(ID_RAIN_DEL, &CvgFrameWorkView::OnRainDel)
	ON_COMMAND(ID_SNOW_DEL, &CvgFrameWorkView::OnSnowDel)
	ON_COMMAND(ID_SE_FOG_DEL, &CvgFrameWorkView::OnFogDel)
	ON_COMMAND(ID_SE_FOG, &CvgFrameWorkView::OnSeFog)
	ON_COMMAND(ID_MIRROR_CONVERT, &CvgFrameWorkView::OnMirrorConvert)
	ON_COMMAND(ID_METAL_CONVERT, &CvgFrameWorkView::OnMetalConvert)
	ON_COMMAND(ID_CONVERT_BRIGTHFLOOR, &CvgFrameWorkView::OnConvertBrigthfloor)
	ON_COMMAND(ID_3DSOUND_ADD, &CvgFrameWorkView::On3dsoundAdd)
	ON_COMMAND(ID_BACKSOUND_SET, &CvgFrameWorkView::OnBacksoundSet)
	ON_COMMAND(ID_FILE_IMPORT_MS3D, &CvgFrameWorkView::OnFileImportMs3d)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_ADDANIMATION, &CvgFrameWorkView::OnAddanimation)
	ON_COMMAND(ID_SETTREE, &CvgFrameWorkView::OnSettree)
	ON_COMMAND(ID_SELECT, &CvgFrameWorkView::OnSelect)
	ON_COMMAND(ID_PLANTINLINE, &CvgFrameWorkView::OnPlantinline)
	ON_COMMAND(ID_VGTERRAIN_NEW_VTR, &CvgFrameWorkView::OnVgterrainNewVtr)

	ON_COMMAND(IDM_Zuobiao, &CvgFrameWorkView::OnZuobiao)
	ON_COMMAND(IDM_Juli, &CvgFrameWorkView::OnJuli)
	ON_COMMAND(IDM_Mianji, &CvgFrameWorkView::OnMianji)
	ON_COMMAND(IDM_Tiji, &CvgFrameWorkView::OnTiji)
	ON_COMMAND(ID_TOOL_MAKER_VGM, &CvgFrameWorkView::OnToolMakerVgm)
	ON_COMMAND(ID_TOOL_MAKER_VGI, &CvgFrameWorkView::OnToolMakerVgi)
	ON_COMMAND(ID_SCENE_READ, &CvgFrameWorkView::OnSceneRead)
	ON_COMMAND(ID_SCENE_SAVE, &CvgFrameWorkView::OnSceneSave)
	ON_COMMAND(ID_PUBLISH_COLLECT, &CvgFrameWorkView::OnPublishCollect)
	ON_COMMAND(ID_33014, &CvgFrameWorkView::OnShapefileDb)//YXDB

	ON_COMMAND(ID_TOOL_MAKER_DDS, &CvgFrameWorkView::OnToolMakerDds)

	ON_COMMAND(ID_DB_SHOW,  &CvgFrameWorkView::OnDbShow)
	ON_COMMAND(ID_DB_LINK, &CvgFrameWorkView::OnDbLink)
	ON_COMMAND(ID_DB_DATABASE, &CvgFrameWorkView::OnDbDatabase)
	ON_COMMAND(ID_DB_TABLE, &CvgFrameWorkView::OnDbTable)
	ON_COMMAND(ID_DB_RECORD, &CvgFrameWorkView::OnDbRecord)

	ON_COMMAND(ID_EDIT_SELECT, &CvgFrameWorkView::OnEditSelect)
	ON_COMMAND(ID_EDIT_SELECT_CLEAR, &CvgFrameWorkView::OnEditSelectClear)/*
	ON_COMMAND(ID_FILE_EXPORT_VGM, &CvgFrameWorkView::OnFileExportVgm)
	ON_COMMAND(ID_FILE_EXPORT_VGI, &CvgFrameWorkView::OnFileExportVgi)*/
	ON_COMMAND(NID_FILE_IMPORT_KFM, &CvgFrameWorkView::OnFileImportKfm)

	ON_COMMAND(ID_CHANNEL_1, &CvgFrameWorkView::OnChannel1)
	ON_COMMAND(ID_CHANNEL_0, &CvgFrameWorkView::OnChannel0)
	ON_COMMAND(ID_CHANNEL_m1, &CvgFrameWorkView::OnChannelm1)
	ON_COMMAND(ID_SETUP_SERVER, &CvgFrameWorkView::OnSetupServer)
	ON_COMMAND(ID_DESTROY_SERVER, &CvgFrameWorkView::OnDestroyServer)
	ON_COMMAND(ID_NEW_CLIENT, &CvgFrameWorkView::OnNewClient)
	ON_COMMAND(ID_DEL_CLIENT, &CvgFrameWorkView::OnDelClient)
	ON_COMMAND(ID_APP_ABOUT, &CvgFrameWorkView::OnAppAbout)
//	ON_WM_SETFOCUS()
//	ON_WM_KILLFOCUS()
	ON_COMMAND(ID_OCC_SWITCH, &CvgFrameWorkView::OnOccSwitch)
	ON_COMMAND(ID_FILE_IMPORT, &CvgFrameWorkView::OnFileImport)
	ON_COMMAND(IDM_GISMATCH, &CvgFrameWorkView::OnGismatch)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_FILE_EXPORT, &CvgFrameWorkView::OnFileExport)
	ON_COMMAND(ID_UNDO, &CvgFrameWorkView::OnUndo)
	ON_COMMAND(ID_REDO, &CvgFrameWorkView::OnRedo)
	ON_COMMAND(ID_MESH_OR_LINE, &CvgFrameWorkView::OnMeshOrLine)
	ON_COMMAND(ID_TREE_SWITCH, &CvgFrameWorkView::OnTreeSwitch)
	ON_UPDATE_COMMAND_UI(ID_TREE_SWITCH, &CvgFrameWorkView::OnUpdateTreeSwitch)
	ON_UPDATE_COMMAND_UI(ID_OCC_SWITCH, &CvgFrameWorkView::OnUpdateOccSwitch)
	ON_COMMAND(ID_EDIT_TRANALL, &CvgFrameWorkView::OnEditTranall)
	ON_COMMAND(ID_SHOW_MODELS, &CvgFrameWorkView::OnShowModels)
	ON_UPDATE_COMMAND_UI(ID_SHOW_MODELS, &CvgFrameWorkView::OnUpdateShowModels)
	ON_COMMAND(ID_NEWPLAN, &CvgFrameWorkView::OnNewplan)
	ON_COMMAND(ID_VGT_ATTACH, &CvgFrameWorkView::OnVgtAttach)
	ON_COMMAND(ID_32971, &CvgFrameWorkView::On32971)
	ON_UPDATE_COMMAND_UI(ID_32971, &CvgFrameWorkView::OnUpdate32971)
	ON_COMMAND(ID_32972, &CvgFrameWorkView::On32972)
	ON_UPDATE_COMMAND_UI(ID_32972, &CvgFrameWorkView::OnUpdate32972)
	ON_COMMAND(ID_32973, &CvgFrameWorkView::On32973)
	ON_UPDATE_COMMAND_UI(ID_32973, &CvgFrameWorkView::OnUpdate32973)
	ON_COMMAND(ID_32974, &CvgFrameWorkView::On32974)
	ON_UPDATE_COMMAND_UI(ID_32974, &CvgFrameWorkView::OnUpdate32974)
	ON_COMMAND(ID_VGT_VISIBILITY, &CvgFrameWorkView::OnVgtVisibility)
	ON_UPDATE_COMMAND_UI(ID_VGT_ATTACH, &CvgFrameWorkView::OnUpdateVgtAttach)
	ON_UPDATE_COMMAND_UI(ID_VGT_VISIBILITY, &CvgFrameWorkView::OnUpdateVgtVisibility)
	ON_COMMAND(ID_VEC_POINTS, &CvgFrameWorkView::OnVecPoints)
	ON_COMMAND(ID_VEC_LINES, &CvgFrameWorkView::OnVecLines)
	ON_COMMAND(ID_VEC_POLYGON, &CvgFrameWorkView::OnVecPolygon)
	ON_COMMAND(ID_VEC_EDIT, &CvgFrameWorkView::OnVecEdit)
	ON_COMMAND(ID_TERRAIN_LEVELING, &CvgFrameWorkView::OnTerrainLeveling)
	ON_COMMAND(ID_HIDE_VEC, &CvgFrameWorkView::OnHideVec)
	ON_COMMAND(ID_NOSEL_VEC, &CvgFrameWorkView::OnNoselVec)
	ON_UPDATE_COMMAND_UI(ID_TERRAIN_LEVELING, &CvgFrameWorkView::OnUpdateTerrainLeveling)
	ON_COMMAND(ID_VGT_CUT_ANALYSIS2, &CvgFrameWorkView::OnVgtCutAnalysis2)
	ON_UPDATE_COMMAND_UI(ID_VGT_CUT_ANALYSIS2, &CvgFrameWorkView::OnUpdateVgtCutAnalysis2)
	ON_COMMAND(ID_VGT_TERRAIN_SAVE, &CvgFrameWorkView::OnVgtTerrainSave)
	ON_UPDATE_COMMAND_UI(ID_VGT_TERRAIN_SAVE, &CvgFrameWorkView::OnUpdateVgtTerrainSave)
	ON_COMMAND(ID_VGT_BLEND, &CvgFrameWorkView::OnVgtBlend)
	ON_UPDATE_COMMAND_UI(ID_VGT_BLEND, &CvgFrameWorkView::OnUpdateVgtBlend)
	ON_COMMAND(ID_VGT_VISIBILITY2, &CvgFrameWorkView::OnVgtVisibility2)
	ON_UPDATE_COMMAND_UI(ID_VGT_VISIBILITY2, &CvgFrameWorkView::OnUpdateVgtVisibility2)
	ON_COMMAND(ID_VGT_CUTAREA, &CvgFrameWorkView::OnVgtCutarea)
	ON_UPDATE_COMMAND_UI(ID_VGT_CUTAREA, &CvgFrameWorkView::OnUpdateVgtCutarea)
	ON_COMMAND(ID_NDB_CONNECT, &CvgFrameWorkView::OnNdbConnect)
	ON_COMMAND(ID_NDB_CONDITION, &CvgFrameWorkView::OnNdbCondition)
	ON_COMMAND(ID_COLLISION_ONOFF, &CvgFrameWorkView::OnCollisionOnoff)
	ON_UPDATE_COMMAND_UI(ID_COLLISION_ONOFF, &CvgFrameWorkView::OnUpdateCollisionOnoff)
	ON_COMMAND(ID_VGT_MAPPING_ORIGIN, &CvgFrameWorkView::OnVgtMappingOrigin)
	ON_UPDATE_COMMAND_UI(ID_VGT_MAPPING_ORIGIN, &CvgFrameWorkView::OnUpdateVgtMappingOrigin)
	ON_COMMAND(ID_VECTOR_SHP_IMPORT, &CvgFrameWorkView::OnVectorShpImport)
	ON_COMMAND(ID_VECTOR_SHP_SHOW, &CvgFrameWorkView::OnVectorShpShow)
	ON_COMMAND(ID_VECTOR_SHP_EXPORT2DB, &CvgFrameWorkView::OnVectorShpExport2db)
	ON_COMMAND(ID_VECTOR_SHP_ATTACH_TERRAIN, &CvgFrameWorkView::OnVectorShpAttachTerrain)
	ON_COMMAND(ID_VECTOR_SHP_TRAN_LAYER, &CvgFrameWorkView::OnVectorShpTranLayer)
	ON_COMMAND(ID_VECTOR_SHP_EDIT_STATE, &CvgFrameWorkView::OnVectorShpEditState)
	ON_UPDATE_COMMAND_UI(ID_VECTOR_SHP_SHOW, &CvgFrameWorkView::OnUpdateVectorShpShow)
	ON_UPDATE_COMMAND_UI(ID_VECTOR_SHP_EDIT_STATE, &CvgFrameWorkView::OnUpdateVectorShpEditState)

	ON_COMMAND(ID_VGVI_BG_CONFIG, &CvgFrameWorkView::OnVgviBgConfig)
	ON_COMMAND(ID_NDB_DISCONNECT, &CvgFrameWorkView::OnNdbDisconnect)
	ON_COMMAND(ID_SCRIPT_RUNFILE, &CvgFrameWorkView::OnScriptRunfile)
	ON_COMMAND(ID_SCRIPT_TESTSCRIPT, &CvgFrameWorkView::OnScriptTestscript)
	ON_COMMAND(ID_SCRIPT_TESTMENU, &CvgFrameWorkView::OnScriptTestmenu)
	ON_COMMAND(ID_SCRIPT_CONSOLE, &CvgFrameWorkView::OnScriptConsole)
	ON_COMMAND(ID_PUBLISH_PACKAGE_EXE, &CvgFrameWorkView::OnPublishPackageExe)
	ON_COMMAND(ID_SCENE_RESET, &CvgFrameWorkView::OnSceneReset)
	ON_COMMAND(ID_VECTOR_SHP_UNLOAD, &CvgFrameWorkView::OnVectorShpUnload)
	ON_COMMAND(ID_33109, &CvgFrameWorkView::Onterraintesting)
	ON_COMMAND(ID_MODTERRAIN_CONVERT, &CvgFrameWorkView::OnSetTerrain)
	ON_UPDATE_COMMAND_UI(ID_33109, &CvgFrameWorkView::OnUpdate33109)
	ON_UPDATE_COMMAND_UI(ID_SET_TERRAIN, &CvgFrameWorkView::OnUpdateSetTerrain)
	ON_COMMAND(ID_MODTERRAIN_LOAD, &CvgFrameWorkView::OnLoadModterrain)
	ON_COMMAND(ID_MODTERRAIN_RELOAD, &CvgFrameWorkView::OnModterrainReload)
	ON_COMMAND(ID_MODTERRAIN_SAVE, &CvgFrameWorkView::OnModterrainSave)
	ON_UPDATE_COMMAND_UI(ID_MODTERRAIN_LOAD, &CvgFrameWorkView::OnUpdateModterrainLoad)
	ON_COMMAND(ID_MODTERRAIN_LEVELING, &CvgFrameWorkView::OnModterrainLeveling)
	ON_UPDATE_COMMAND_UI(ID_MODTERRAIN_LEVELING, &CvgFrameWorkView::OnUpdateModterrainLeveling)
	ON_UPDATE_COMMAND_UI(ID_MODTERRAIN_RELOAD, &CvgFrameWorkView::OnUpdateModterrainReload)
	ON_UPDATE_COMMAND_UI(ID_MODTERRAIN_SAVE, &CvgFrameWorkView::OnUpdateModterrainSave)
	ON_COMMAND(ID_VGSC_USER_SCRIPT_CONFIG, &CvgFrameWorkView::OnVgscUserScriptConfig)
	ON_COMMAND(ID_TEST_UNLOADMOD, &CvgFrameWorkView::OnTestUnloadmod)
	ON_COMMAND(ID_GIS_SUN, &CvgFrameWorkView::OnGisSun)
	ON_COMMAND(ID_TANSLATE_SELECTED, &CvgFrameWorkView::OnTanslateSelected)
	ON_COMMAND(ID_EXP_AVI, &CvgFrameWorkView::OnExpAvi)
	ON_COMMAND(ID_EXP_BMP, &CvgFrameWorkView::OnExpBmp)
	ON_COMMAND(ID_VGSHAPE_QUERY_GREEN_RATIO, &CvgFrameWorkView::OnVectorShpGreenRatio)
	ON_UPDATE_COMMAND_UI(ID_VGSHAPE_QUERY_GREEN_RATIO, &CvgFrameWorkView::OnUpdateVectorShpGreenRatio)
	ON_COMMAND(ID_VGSHAPE_READSHPFILE, &CvgFrameWorkView::OnVgshapeReadshpfile)
	ON_COMMAND(ID_VGSHAPE_SELECTSHPAREA, &CvgFrameWorkView::OnVgshapeSelectshparea)
	ON_COMMAND(ID_CAM_REHEIGHT, &CvgFrameWorkView::OnCamReheight)
	ON_COMMAND(ID_VGSHAPE_SAVEASSHAPE, &CvgFrameWorkView::OnVgshapeSaveasshape)
	ON_COMMAND(ID_TEST_MOD_OPACITY, &CvgFrameWorkView::OnTestModOpacity)
	ON_COMMAND(ID_VGSHAPE_TRANSLATETOORIGIN, &CvgFrameWorkView::OnVgshapeTranslatetoorigin)
	ON_COMMAND(ID_VGSHAPE_ATTACHTOTERRAIN, &CvgFrameWorkView::OnVgshapeAttachtoterrain)
	ON_COMMAND(ID_VGSHAPE_SETRENDERERBOXASFILTER, &CvgFrameWorkView::OnVgshapeSetrendererboxasfilter)
	ON_COMMAND(ID_AUTOBUILD, &CvgFrameWorkView::OnAutobuild)
	ON_COMMAND(ID_VGSHAPE_READGRIDSHPFILE, &CvgFrameWorkView::OnVgshapeReadgridshpfile)
	ON_UPDATE_COMMAND_UI(ID_VGSHAPE_SELECTSHPAREA, &CvgFrameWorkView::OnUpdateVgshapeSelectshparea)
	ON_COMMAND(ID_AUTOBUILD_CLONE, &CvgFrameWorkView::OnAutobuildClone)
	ON_COMMAND(ID_EDIT_SINGLE_FACE_RENDER, &CvgFrameWorkView::OnEditSingleFaceRender)
	ON_COMMAND(ID_EDIT_SINGLE_FACE_RENDER_SEL, &CvgFrameWorkView::OnEditSingleFaceRenderSel)
	ON_COMMAND(ID_EDIT_SINGLE_FACE_RENDER_ALL, &CvgFrameWorkView::OnEditSingleFaceRenderAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SINGLE_FACE_RENDER_SEL, &CvgFrameWorkView::OnUpdateEditSingleFaceRenderSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SINGLE_FACE_RENDER_ALL, &CvgFrameWorkView::OnUpdateEditSingleFaceRenderAll)
	ON_COMMAND(ID_TRIGGER_SET_AS, &CvgFrameWorkView::OnTriggerSetAs)
	ON_COMMAND(ID_VGSHAPE_IMPORTTEXT, &CvgFrameWorkView::OnVgshapeImporttext)
	ON_COMMAND(ID_VGSHAPE_EXPORTSHAPEINFO, &CvgFrameWorkView::OnVgshapeExportshapeinfo)
	ON_COMMAND(ID_VGSHAPE_CACULATEPOSITION, &CvgFrameWorkView::OnVgshapeCaculateposition)
	ON_COMMAND(ID_VGSHAPE_CLEARPLAN, &CvgFrameWorkView::OnVgshapeClearplan)
	ON_COMMAND(ID_VGSHAPE_GENERATESELECTED, &CvgFrameWorkView::OnVgshapeGenerateselected)
	ON_COMMAND(ID_VGSHAPE_IMPORTINFOFROMTXT, &CvgFrameWorkView::OnVgshapeImportinfofromtxt)
	ON_COMMAND(ID_AUTOBUILDING_CLOSEINFOFILE, &CvgFrameWorkView::OnAutobuildingCloseinfofile)
	ON_COMMAND(ID_TRIGGER_CONFIG, &CvgFrameWorkView::OnTriggerConfig)
	ON_COMMAND(ID_QUERYPLANINFO_QUERYPLOTRATIO, &CvgFrameWorkView::OnQueryplaninfoQueryplotratio)
	ON_UPDATE_COMMAND_UI(ID_QUERYPLANINFO_QUERYPLOTRATIO, &CvgFrameWorkView::OnUpdateQueryplaninfoQueryplotratio)
	ON_COMMAND(ID_DYNTEXTURE_CONVERT, &CvgFrameWorkView::OnDyntextureConvert)
	ON_COMMAND(ID_VGSHAPE_TRANSLATEALL, &CvgFrameWorkView::OnVgshapeTranslateall)
	ON_COMMAND(ID_VGPLAN_ADDGROUND, &CvgFrameWorkView::OnVgplanAddground)
	ON_COMMAND(ID_VGPLAN_ADDSECNE, &CvgFrameWorkView::OnVgplanAddsecne)
	ON_COMMAND(ID_VGSHADOW_TURNONSHADOW, &CvgFrameWorkView::OnVgshadowTurnonshadow)
	ON_COMMAND(ID_VGSHADOW_SELECTSHADOWCASTOBJECTS, &CvgFrameWorkView::OnVgshadowSelectshadowcastobjects)
	ON_COMMAND(ID_VGPLAN_SETPARAMETER, &CvgFrameWorkView::OnVgplanSetparameter)
	ON_COMMAND(ID_VGSHADOW_CHANGE, &CvgFrameWorkView::OnVgshadowChange)
	ON_COMMAND(ID_VGSHADOW_TURNOFFSHADOW, &CvgFrameWorkView::OnVgshadowTurnoffshadow)
	ON_COMMAND(ID_VGSHADOW_SOLARANALYSIS, &CvgFrameWorkView::OnVgshadowSolaranalysis)
	ON_UPDATE_COMMAND_UI(ID_VGSHADOW_SOLARANALYSIS, &CvgFrameWorkView::OnUpdateVgshadowSolaranalysis)
	ON_UPDATE_COMMAND_UI(ID_VGSHADOW_TURNONSHADOW, &CvgFrameWorkView::OnUpdateVgshadowTurnonshadow)
	ON_COMMAND(ID_TEST_MOD_UNOPACITY, &CvgFrameWorkView::OnTestModUnopacity)
	ON_COMMAND(ID_TESTTEST_SELECTCHANGECOLOR, &CvgFrameWorkView::OnTesttestSelectchangecolor)
	ON_COMMAND(ID_VGPLAN_ADDSTREETLINE, &CvgFrameWorkView::OnVgplanAddstreetline)
	ON_COMMAND(ID_PLAN_EXPORTSCENE, &CvgFrameWorkView::OnPlanExportscene)
	ON_COMMAND(ID_VGPLAN_GENERATEPLANMAP, &CvgFrameWorkView::OnVgplanGenerateplanmap)
	ON_COMMAND(ID_VGPLAN_ADDREGION, &CvgFrameWorkView::OnVgplanAddregion)
	ON_COMMAND(ID_VGPLAN_ADDNEW, &CvgFrameWorkView::OnVgplanAddnew)
	ON_COMMAND(ID_VGPLAN_CGRID, &CvgFrameWorkView::OnVgplanCgrid)
	ON_COMMAND(ID_VGPLAN_CALGRID, &CvgFrameWorkView::OnVgplanCalgrid)
	ON_COMMAND(ID_VGPLAN_GENPLAN, &CvgFrameWorkView::OnVgplanGenplan)
	ON_COMMAND(ID_VGPLAN_OUTPUT, &CvgFrameWorkView::OnVgplanOutput)
	END_MESSAGE_MAP()

// CvgFrameWorkView construction/destruction

CvgFrameWorkView::CvgFrameWorkView()
: m_pDC(NULL), testVector(100000), pNodeList(
	vgUIController::GetInstance()->GetNodeList())
{
	// TODO: add construction code here
	Rainon = true;
	Snowon = true;
	firenum = 0;
	//fognum  = 0;

	//m_bIsActive = true;
	//m_bIsFirstRead  = true;
	m_bQuadtree		= false;
	//m_bRenderBox	= false;

	m_bShowShapeLayers	= true;
	m_bEditLayers		= false;

	checkDl = FALSE;
	checkPf = FALSE;
	checkPs = FALSE;
	checkXf = FALSE;

	paviFile = NULL;

	m_bDoubleFaceRenderSelected = true;
	m_bDoubleFaceRenderAll		= true;

	m_bSetAsTrigger = false;
}

CvgFrameWorkView::~CvgFrameWorkView()
{
}

BOOL CvgFrameWorkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CvgFrameWorkView drawing

void CvgFrameWorkView::OnDraw(CDC* /*pDC*/)
{
	CvgFrameWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	//RenderScene();



}

// CvgFrameWorkView printing

void CvgFrameWorkView::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL CvgFrameWorkView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CvgFrameWorkView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CvgFrameWorkView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CvgFrameWorkView diagnostics

#ifdef _DEBUG
void CvgFrameWorkView::AssertValid() const
{
	CView::AssertValid();
}

void CvgFrameWorkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CvgFrameWorkDoc* CvgFrameWorkView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CvgFrameWorkDoc)));
	return (CvgFrameWorkDoc*)m_pDocument;
}
#endif //_DEBUG


// CvgFrameWorkView message handlers
void CvgFrameWorkView::renderScene(void)
{
#if 0
	static bool contrl =true ;

	if ( contrl == true )
	{
		contrl = false ;
		String strSrcFilePath = vgApp.vgInitialProData(this);
		OnSceneRead(strSrcFilePath);
	}
#endif

	VGK_CHECK_OPENGL_ERROR();

	vgKernel::InputSystem::getSingleton().UpdateInput();



	static float SunSlfRot;
	static float EarthCommonRot;
	static float EarthSlfRot;
	static float MoonCommonRot;
	static float MoonSlfRot;

	VGK_CHECK_OPENGL_ERROR();

	if (vgCam::CamManager::getSingleton().getAviRecordCnt() == 0)
	{
		if (paviFile != NULL)
		{
			LONG fileSize = paviFile->GetCurrentFileSize();

			delete paviFile;
			paviFile = NULL;
			
			CString outString;
			outString.Format("����AVI�ļ��ɹ�.�ļ���С %ul", fileSize);
			AfxMessageBox(outString);
			vgUIController::getSingleton().DisableProgressBar();
			
		}

		VGK_CHECK_OPENGL_ERROR();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		glLoadIdentity();	

		VGK_CHECK_OPENGL_ERROR();

		vgGlobal::GlobalConsole::getSingleton().renderBegin();

		VGK_CHECK_OPENGL_ERROR();

		vgApp.run();

		vgGlobal::GlobalConsole::getSingleton().renderPrimitives();

		//vgCore::MovingManager::getSingleton().Render();

		VGK_CHECK_OPENGL_ERROR();

		// ��Ⱦ��ѡ�еĽڵ�
		vgKernel::SelectManager::getSingleton().renderSelected();
		// ��ӦInput�¼�
		vgKernel::SelectManager::getSingleton().updateSelectedItem();

		VGK_CHECK_OPENGL_ERROR();

		vgGlobal::GlobalConsole::getSingleton().renderEnd();

		VGK_CHECK_OPENGL_ERROR();

		vgGIS3D::GIS3D::getSingleton().GisRender();//  [11/6/2008 zhu]

		VGK_CHECK_OPENGL_ERROR();

		//��������
		if (vgTerritory::TerrainManager::getSingleton().getLevellingClassPtr()!=NULL)
		{
			if(vgTerritory::TerrainManager::getSingleton().getLevellingClassPtr()->btm)
				vgFont::FontManager::getSingleton().DrawHeight(vgTerritory::TerrainManager::getSingleton().getLevellingClassPtr()->avh+ vgCore::vgCoordinate::getSingleton().GetSceneCood().y);
		}

		VGK_CHECK_OPENGL_ERROR();

		//������
		if ( vgMod::TriggerManager::getSingleton().hasTrigger() )
		{
			vgMod::TriggerManager::getSingleton().testAnyTriggered(
				vgCam::CamManager::getSingleton().getCurrentPosition());
		}

		VGK_CHECK_OPENGL_ERROR();

		glFinish();

		VGK_CHECK_OPENGL_ERROR();

		HDC handledevice = m_pDC->GetSafeHdc();

		if ( handledevice == INVALID_HANDLE_VALUE )
		{
			assert( 0 );
		}

		SwapBuffers( handledevice );

		vgKernel::OpenGLSupport::makeErrorStackEmpty();

		try
		{
			VGK_CHECK_OPENGL_ERROR();
		}
		catch (...)
		{
			int dff=0;

			::MessageBox( NULL , "SwapBuffers ����!" , "ERROR" , MB_OK );
		}

		

		showFps();

		VGK_CHECK_OPENGL_ERROR();
	}
	else if (vgCam::CamManager::getSingleton().getAviRecordCnt() == -1)
	{
		vgCam::CamManager::getSingleton().setAviRecordCnt(0);

		exportFrameBufferToPath(m_bmpFilePath, m_bmpWidth, m_bmpHeight);

	}
	else if (vgCam::CamManager::getSingleton().getAviRecordCnt() > 0)
	{
		int totalFrame = vgCam::CamManager::getSingleton().getCameraRecordByName(m_aviCameraName)->framenum;
		int frameId = totalFrame - vgCam::CamManager::getSingleton().getAviRecordCnt();
		
		if (paviFile->GetCurrentFileSize() > 1800000000L)
		{
			delete paviFile;
			paviFile = NULL;
			
			int compressId = m_expDialog.getCompressId();
			CString filePrefix;

			if (AfxExtractSubString(filePrefix, m_aviFilePath.GetString(), 0, _T('.')))
			{	
				CString tmp;
				tmp.Format("%d", ++m_fileId);
				filePrefix += tmp;
				filePrefix += ".avi";			
			}
			else
			{
				AfxMessageBox("���ʧ��");
				vgCam::CamManager::getSingleton().setAviRecordCnt(0);

				return ;
			}
			
			if (compressId == 1)
			{
				//paviFile = new vgCam::CAviFile(m_aviFilePath, mmioFOURCC('M','S','V','C'), fps);

				paviFile = new vgCam::CAviFile(filePrefix, mmioFOURCC('M','S','V','C'), m_fps);
			}
			else if (compressId == 2)
			{
				paviFile = new vgCam::CAviFile(filePrefix, mmioStringToFOURCC("CVID", 0), m_fps);
			}
			else if (compressId == 3)
			{
				paviFile = new vgCam::CAviFile(filePrefix, mmioFOURCC('M','P','G','4'), m_fps);
			}
			else
			{
				paviFile = new vgCam::CAviFile(filePrefix, 0, m_fps);
			}

			paviFile->SetQuality(m_expDialog.getQuality());
		}

		exportFrameBufferToAVI(frameId);
	}
}

void CvgFrameWorkView::exportFrameBufferToAVI(int frameId)
{
	int frameWidth = m_frameBufferWidth, frameHeight = m_frameBufferHeight;

	vgCore::FrameBuffer frameBuffer(frameWidth, frameHeight);

	glPushAttrib(GL_VIEWPORT_BIT);

	frameBuffer.create();

	frameBuffer.bind();

	vgGlobal::GlobalConsole::getSingleton().renderBegin();

	vgApp.run();

	vgGlobal::GlobalConsole::getSingleton().renderPrimitives();

	vgGlobal::GlobalConsole::getSingleton().renderEnd();

	vgGIS3D::GIS3D::getSingleton().GisRender();//  [11/6/2008 zhu]


	// ��������
	if (vgTerritory::TerrainManager::getSingleton().getLevellingClassPtr()!=NULL)
	{
		if(vgTerritory::TerrainManager::getSingleton().getLevellingClassPtr()->btm)
			vgFont::FontManager::getSingleton().DrawHeight(vgTerritory::TerrainManager::getSingleton().getLevellingClassPtr()->avh+ vgCore::vgCoordinate::getSingleton().GetSceneCood().y);
	}

	BYTE* buf = new BYTE[ 3 * frameWidth * frameHeight ];
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadPixels(0, 0, frameWidth, frameHeight , GL_BGR , GL_UNSIGNED_BYTE , buf);

	//m_pAviGenerator->AddFrame(buf);

	if (FAILED(paviFile->AppendNewFrame(frameWidth, frameHeight, buf,24)))
	{
		AfxMessageBox(paviFile->GetLastErrorMessage());
	}


	frameBuffer.unbind();

	frameBuffer.destroy();

	glPopAttrib();

	int progress = (float)(m_totalFrame - vgCam::CamManager::getSingleton().getAviRecordCnt()) / m_totalFrame * 100;
	vgUIController::getSingleton().SetProgress(progress);

	delete[] buf;
}

void CvgFrameWorkView::exportFrameBufferToPath(string filePath, int width, int height)
{
	int frameWidth = width, frameHeight = height;

	vgCore::FrameBuffer frameBuffer(frameWidth, frameHeight);

	glPushAttrib(GL_VIEWPORT_BIT);

	frameBuffer.create();

	frameBuffer.bind();

	vgGlobal::GlobalConsole::getSingleton().renderBegin();

	vgApp.run();

	vgGlobal::GlobalConsole::getSingleton().renderPrimitives();

	vgGlobal::GlobalConsole::getSingleton().renderEnd();

	vgGIS3D::GIS3D::getSingleton().GisRender();//  [11/6/2008 zhu]


	// ��������
	if (vgTerritory::TerrainManager::getSingleton().getLevellingClassPtr()!=NULL)
	{
		if(vgTerritory::TerrainManager::getSingleton().getLevellingClassPtr()->btm)
			vgFont::FontManager::getSingleton().DrawHeight(vgTerritory::TerrainManager::getSingleton().getLevellingClassPtr()->avh+ vgCore::vgCoordinate::getSingleton().GetSceneCood().y);
	}

	char* buf = new char[ 3 * frameWidth * frameHeight ];
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadPixels(0, 0, frameWidth, frameHeight , GL_BGR , GL_UNSIGNED_BYTE , buf);

	vgImage::CximageWrapper bmpimg;

	bmpimg.createFromArray(buf, frameWidth, frameHeight);

	bmpimg.saveToFile(filePath.c_str());

	delete[] buf;

	frameBuffer.unbind();

	frameBuffer.destroy();

	glPopAttrib();

	return ;
}

bool CvgFrameWorkView::initGLContext(void)
{

// 	DlgTestMultisample test_multisample_dlg( AfxGetApp()->m_pMainWnd );
// 
// 	test_multisample_dlg.DoModal();


	GLuint PixelFormat;	

	static PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),    // pfd�ṹ�Ĵ�С 
		1,                                // �汾�� 
		PFD_DRAW_TO_WINDOW |              // ֧���ڴ����л�ͼ 
		PFD_SUPPORT_OPENGL |              // ֧�� OpenGL 
		PFD_DOUBLEBUFFER,                 // ˫����ģʽ 
		PFD_TYPE_RGBA,                    // RGBA ��ɫģʽ 
		24,                               // 24 λ��ɫ��� 
		0, 0, 0, 0, 0, 0,                 // ������ɫλ 
		1,                                // û�з�͸���Ȼ��� 
		0,                                // ������λλ 
		0,                                // ���ۼӻ��� 
		0, 0, 0, 0,                       // �����ۼ�λ 
		32,                               // 32 λ��Ȼ���     
		8,                                // ��ģ�建�� //�Ķ���Ӱ
		0,                                // �޸������� 
		PFD_MAIN_PLANE,                   // ���� 
		0,                                // ���� 
		0, 0, 0                           // ���Բ�,�ɼ��Ժ������ģ 
	};

	HWND hWND = this->GetSafeHwnd();
	HDC hDC = ::GetDC(hWND);
	m_pDC = new CClientDC(this);
	// ѡ����ʵ�Device Context���ص��ʽ
	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))		// Did Windows Find A Matching Pixel Format?
	{							
		::MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

// 	if ( *test_multisample_dlg._parbMultisampleSupported )
// 	{
// 		PixelFormat = *test_multisample_dlg._parbMultisampleFormat;
// 	}
	if ( *(((CMainFrame *)GetParent())->test_multisample_dlg)._parbMultisampleSupported )
	{
		PixelFormat = *((((CMainFrame *)GetParent())->test_multisample_dlg)._parbMultisampleFormat);
	}



	// �����µ�Device Context���ص��ʽ
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))			// Are We Able To Set The Pixel Format?
	{
		::MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}
	// ���ݵ�ǰ��Device Context�õ�rending context
	if (!(this->m_hGLContext=wglCreateContext(hDC)))	// Are We Able To Get A Rendering Context?
	{
		//KillGLWindow();								// Reset The Display
		::MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}
	// ���õ�ǰDevice context ��rendering context
	if(!wglMakeCurrent(hDC,this->m_hGLContext))			// Try To Activate The Rendering Context
	{
		//KillGLWindow();								// Reset The Display
		::MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.5f,0.6f,0.8f,1.0f);					// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations


	vgFont::FontManager::getSingleton();
	vgFont::FontManager::getSingleton().BuildFont(hDC);//��ʼ����������
	return true;
}

int CvgFrameWorkView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	vgScript::ScriptManager::getSingleton();


	this->initGLContext();

	vgKernel::OpenGLSupport::initialiseOpenGLExtension();


	HWND hWND = this->GetSafeHwnd();

	vgApp.initialize( hWND , m_pDC );


	vgGlobal::GlobalConsole::getSingleton().initAfterOpenGLSetup();

	this->SetTimer(1, 15, NULL);

	vgKernel::SelectManager::getSingleton();

	vgCam::CamManager::getSingleton().setAviRecordCnt(0);

	vgLogo::vgLogoManager::getSingleton().initialise();

	return 0;
}

void CvgFrameWorkView::OnDestroy()
{
	CView::OnDestroy();

	vgUIController::DestoryInstance();

	vgGlobal::GlobalConsole::getSingleton().uninitBeforeOpenGLDestroy();

	vgApp.exit();
	

	this->releaseGLContext();
	// TODO: �ڴ˴������Ϣ����������
}

void CvgFrameWorkView::releaseGLContext(void)
{
	if (this->m_hGLContext)										// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))							// Are We Able To Release The DC And RC Contexts?
		{
			::MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(this->m_hGLContext))				// Are We Able To Delete The RC?
		{
			::MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		this->m_hGLContext=NULL;								// Set RC To NULL
	}

	if (m_pDC)
		delete m_pDC;
}

void CvgFrameWorkView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	int height = cy;
	int width = cx;

	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	//glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	//glLoadIdentity();					 				// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	//gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,10000.0f);
	//gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,10000.0f);

// 	s_window_width = width;
// 	s_window_height = height;

	vgKernel::SelectManager::getSingleton().SetViewSize(cx, cy);

	vgCam::CamManager::getSingleton().setAspectRatioByWindowSize(cx, cy);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	//glDrawBuffer(GL_BACK);
}

BOOL CvgFrameWorkView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//return CView::OnEraseBkgnd(pDC);
	return true;
}

void CvgFrameWorkView::showFps()
{

	//------------------------------------------
	// FPS���
	//------------------------------------------


	static bool  g_bFirstRendering = true;
	static timeb g_lastTime;
	static float g_fTimeSinceLastReport = 0.0;
	static int   g_nFrameCount = 0;
	//RenderScene();
	timeb currentTime;
	float fElapsed = 0.0f;

	if( g_bFirstRendering == true )
	{
		ftime( &g_lastTime );
		currentTime = g_lastTime;
		g_bFirstRendering = false;
	}
	else
	{
		ftime( &currentTime );

		// This is accurate to one second
		fElapsed  = (float)(currentTime.time - g_lastTime.time);
		// This gets it down to one ms
		fElapsed += (float)((currentTime.millitm - g_lastTime.millitm) / 1000.0f);
	}

	static char fpsString[50];

	++g_nFrameCount;


	if( fElapsed - g_fTimeSinceLastReport > 1.0f )
	{

		vgUIController::GetInstance()->SetFrameSpeed( g_nFrameCount );

		g_fTimeSinceLastReport = fElapsed;
		g_nFrameCount = 0;
	}
}

void CvgFrameWorkView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (1 == nIDEvent)
	{
		VGK_CHECK_OPENGL_ERROR();
		renderScene();
		VGK_CHECK_OPENGL_ERROR();
	}
	CView::OnTimer(nIDEvent);
}

void CvgFrameWorkView::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: �ڴ˴������Ϣ����������
}

void TestData()
{
// 	float i, j;
// 	GLfloat rad  = 10;
// 	GLfloat space = 10;
// 	int pa = 100;
// 
// 	glTranslated(0.f, 0.f, -pa);
// 
// 	for(j=-pa; j<=pa; j+=5)
// 	{
// 		glTranslatef(0.f, 0.f, 10);
// 		glPushMatrix();
// 		glTranslatef(-pa, 0.f, 0.f);
// 		for (i=-pa; i<=pa; i+=5)
// 		{
// 			//glutSolidSphere(2.0, 20.0, 20.0);
// 			glTranslatef(5, 0.0f, 0.f);		//ƽ̨�Ķ�λ
// 		}
// 		glPopMatrix();
// 
// 	}
}

// vgm
void CvgFrameWorkView::OnVgmImportVgm()
{
	
	//vgGlobal::GlobalFacade::getSingleton().importModelFile();
	std::ostringstream o;
	o	<< "-----------------------------------------------------------\n"; 
#if VGK_LANG_CHS
	o << "\t\t����ִ�� ����VGM����:\n";
#else
	o << "\t\tImporting VGM:\n";
#endif
	o	<< "-----------------------------------------------------------\n" ;
	VGK_SHOW( o.str() );

	o.clear();

	CString DefExt;

	DefExt.Format("%s","vgm(*.vgm)|*.vgm|");


	CFileDialog dlgmodelFile(true,"vgm",
		NULL,OFN_HIDEREADONLY|
		OFN_CREATEPROMPT|
		OFN_NONETWORKBUTTON|
		OFN_ALLOWMULTISELECT|   
		OFN_FILEMUSTEXIST,
		DefExt);

	DWORD   MAXFILE = 0xFFFF;   
	dlgmodelFile.m_ofn.nMaxFile=MAXFILE; 

	char*  pc = new char[MAXFILE];   


	dlgmodelFile.m_ofn.lpstrFile = pc;   
	dlgmodelFile.m_ofn.lpstrFile[0] = NULL;   
	dlgmodelFile.m_ofn.lpstrTitle="vgm";


	if( dlgmodelFile.DoModal() != IDOK )
	{
		std::ostringstream oEnd;
		oEnd<< "-----------------------------------------------------------\n" ;
#if VGK_LANG_CHS
		oEnd<< "\t\t�˳�ִ�� ����VGM����\n";
#else
		oEnd<< "\t\tQuit importing vgm.\n";
#endif
		oEnd<< "-----------------------------------------------------------\n" ;
		VGK_SHOW( oEnd.str() );
		return;
	}

		POSITION posi;   
		vgKernel::StringVector strVec;
		HTREEITEM	hRootVGM = vgUIController::GetInstance()->GetRootVGM();

		posi = dlgmodelFile.GetStartPosition();   

		while( posi != NULL )
		{   
			strVec.push_back( dlgmodelFile.GetNextPathName(posi).GetBuffer(0));
		}   

		delete[] pc;

		//------------------------------------------
		// ��ʼ����
		//------------------------------------------

		std::ostringstream ovgm;
#if VGK_LANG_CHS
		ovgm << "\n��ʼ����vgm�ļ�... \n";
#else
		ovgm << "\nBegin to import vgm... \n";
#endif
		VGK_SHOW( ovgm.str() );
		int nErrorFileCount = 0;

		vgKernel::StringVector::iterator iter = strVec.begin();
		vgKernel::StringVector::iterator iter_end = strVec.end();

		using namespace vgMesh;
		for ( ; iter != iter_end ; ++ iter )
		{
			FileHandleVgm* vgmhandle 
				= new FileHandleVgm( *iter );

			bool opened = vgmhandle->open();

			if( opened == false )
			{
				delete vgmhandle;

				string err;
#if VGK_LANG_CHS
				err = "�����ļ�ʧ��";
#else
				err = "failed to import";
#endif
				MessageBox( err.c_str() );


				std::ostringstream oEnd;
#if VGK_LANG_CHS
				oEnd << "\t�����ļ�:"<< *iter << " ʧ��\n";
#else
				oEnd << "\timport:\t"<< *iter << " failed\n";
#endif
				VGK_SHOW( oEnd.str() );
				nErrorFileCount++;
				continue;
			}

			bool addres = 
				vgMesh::MeshManager::getSingleton().addItemsAndRegisterFileHandlerVgm( vgmhandle );


			if ( addres == false)
			{
				delete vgmhandle;
				string err;
#if VGK_LANG_CHS
				err = "�����ļ�ʧ��";
#else
				err = "failed to import";
#endif
				MessageBox( err.c_str() );


				std::ostringstream oEnd;
#if VGK_LANG_CHS
				oEnd << "\t�����ļ�:"<< *iter << " ʧ��\n";
#else
				oEnd << "\tFail: "<< *iter << " \n";
#endif
				VGK_SHOW( oEnd.str() );
				nErrorFileCount++;
				continue;
			}

			//------------------------------------------
			// ��UI�����vgBaseEntry
			//------------------------------------------
			std::ostringstream o;
#if VGK_LANG_CHS
			o << "\t�ɹ������ļ�:\t"<< vgmhandle->getFilePath() << " \n";
#else
			o << "\tSuccess: "<< vgmhandle->getFilePath() << " \n";
#endif
			VGK_SHOW( o.str() );
			vgMesh::MeshItemMap* items = vgmhandle->getMeshItemsPtr();
			assert( items != NULL );

			vgMesh::MeshItemMap::iterator iter = items->begin();
			vgMesh::MeshItemMap::iterator iter_end = items->end();

			for ( ; iter != iter_end ; ++ iter )
			{
				String shortname = vgKernel::StringUtility::getFilenameFromAbsolutePath(
					vgmhandle->getFilePath() );
				vgBaseEntry* entry = vgEntryFactory::createEntryFromRenderer( iter->second );
				vgUIController::GetInstance()->AddNode( shortname, entry, hRootVGM);
				//vgUIController::GetInstance()->OnChanged(VG_OBS_NEWMESHITEM, vgmhandle->getMeshItemsPtr());


				RendererQueue* queue = 
					vgKernel::RendererManager::getSingleton().addRenderer( iter->second );

				assert( queue != NULL );
			}

		}

		std::ostringstream oEndvgm;
#if VGK_LANG_CHS
		oEndvgm << "...\n" << "vgm�ļ��������,�ɹ������ļ�" 
			<< strVec.size()- nErrorFileCount << "��,"
			<< nErrorFileCount << "������ʧ��\n";
#else
		oEndvgm << "...\nSuccess: " << strVec.size()- nErrorFileCount << ", Fail:"
			<< nErrorFileCount << "\n";
#endif			
		VGK_SHOW( oEndvgm.str() );


		if (hRootVGM != NULL)
		{
			vgUIController::GetInstance()->GetWorkSpaceBar()->Expand(hRootVGM );
			vgUIController::GetInstance()->GetWorkSpaceBar()->EnsureVisible(hRootVGM);
		}
		// Ensure the item is visible.
//		GetTreeCtrl().EnsureVisible(hmyItem);

		vgKernel::RendererManager::getSingleton().invalidate();

	
	

	std::ostringstream oEnd;
	oEnd<< "-----------------------------------------------------------\n"; 
#if VGK_LANG_CHS
	oEnd<< "\t\t���ִ�� ����VGM����\n";
#else
	oEnd<< "\t\tImport VGM Completed!\n";
#endif
	oEnd<< "-----------------------------------------------------------\n" ;
	VGK_SHOW( oEnd.str() );

}

void CvgFrameWorkView::OnVgiImportVgi()
{
	std::ostringstream o;
	o	<< "-----------------------------------------------------------\n"; 
#if VGK_LANG_CHS
	o << "\t\t����ִ�� ����VGI����:\n";
#else
	o << "\t\tImporting VGI:\n";
#endif
	o	<< "-----------------------------------------------------------\n" ;
	VGK_SHOW( o.str() );

	
	vgGlobal::GlobalFacade::getSingleton().importImageFile();

	std::ostringstream oEnd;
	oEnd<< "-----------------------------------------------------------\n"; 
#if VGK_LANG_CHS
	oEnd<< "\t\t���ִ�� ����VGI����\n";
#else
	oEnd<< "\t\tImport VGI Completed!\n";
#endif
	oEnd<< "-----------------------------------------------------------\n" ;
	VGK_SHOW( oEnd.str() );

}

void CvgFrameWorkView::OnVgmSaveVgm()
{
	vgGlobal::GlobalFacade::getSingleton().saveModelFileOnly();
}

void CvgFrameWorkView::OnVgmiSaveVgmVgi()
{
	vgGlobal::GlobalFacade::getSingleton().saveModelAndImage();
}


// ����¼���Ӧ
void CvgFrameWorkView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	////TRACE("On mouse db click position -- %d %d \n", point.x, point.y);
	vgKernel::InputSystem::getSingleton().OnLButtonDbClick(nFlags,point);

	// 2008��9��1��16:31:09 add
	vgApp.LButtonDblClk(nFlags,point, this);

	CView::OnLButtonDblClk(nFlags, point);
}

void CvgFrameWorkView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	vgKernel::InputSystem::getSingleton().OnLButtonDown(nFlags,point);

	vgApp.LButtonDown( nFlags, point, this);

	CView::OnLButtonDown(nFlags, point);
}

void CvgFrameWorkView::OnMove(int x, int y)
{
	CView::OnMove(x, y);
	// TODO: �ڴ˴������Ϣ����������
}

void CvgFrameWorkView::OnLButtonUp(UINT nFlags, CPoint point)
{
	vgKernel::InputSystem::getSingleton().OnLButtonUp(nFlags,point);

	vgApp.LButtonUp(nFlags, point);

	CView::OnLButtonUp(nFlags, point);
}

void CvgFrameWorkView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	vgKernel::InputSystem::getSingleton().OnMouseMove(nFlags, point);

	vgApp.OnMouseMove( point, this);

	CView::OnMouseMove(nFlags, point);
}

 BOOL CvgFrameWorkView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
 {
 	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
 
 	vgKernel::InputSystem::getSingleton().OnMouseWheel(nFlags, zDelta, pt);
 	return CView::OnMouseWheel(nFlags, zDelta, pt);
 }

// vgtr
 void CvgFrameWorkView::OnVgterrainWireframe()
{
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}

void CvgFrameWorkView::OnVgterrainNowireframe()
{
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void CvgFrameWorkView::OnVgterrainTurnon()
{
 
}

void CvgFrameWorkView::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	CView::OnActivateApp(bActive, dwThreadID);
	
	if (bActive)
	{
		VGK_TRACE("View ��ý���");
	}
	else
	{
		VGK_TRACE("View ʧȥ����");
	}
	// TODO: �ڴ˴������Ϣ����������
}


// 
LRESULT CvgFrameWorkView::OnRender(WPARAM wParam, LPARAM lParam)
{
	// TODO: �����û��Զ�����Ϣ
	renderScene();
	//TRACE("Render \n");
	SendMessage(VG_MSG_RENDER, 0, 0);
	return 0;
} 
void CvgFrameWorkView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
}

static UINT RenderThread(LPVOID pParam)
{
	while (true)
	{
		g_renderCriticalArea.Lock();
		if (g_threadFlag.bModifyFlag)
		{
			if (g_threadFlag.bResize)
			{

			}

			if (g_threadFlag.bTerminate)
			{
				g_renderCriticalArea.Unlock();
				return 0;
			}
		}
		g_renderCriticalArea.Unlock();

		//RenderScene();
	}
	
}
void CvgFrameWorkView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CView::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	//String	strFilePath = vgCore::MFCApplication::getSingleton().readFilePath;
	//if (!strFilePath.empty())
	//{
	//	//MessageBox("Hello World!");
	//	//MessageBox( strFilePath.c_str() );
	//	OnSceneRead(strFilePath);
	//}
}

//
void CvgFrameWorkView::OnVgterrainConvertDemToVgtr()
{
	//vgTerrain::TerrainManager::getSingleton().selectDemsToVgtr();
}

void CvgFrameWorkView::OnVgterrainConvertVgtrHeightmap()
{
	//vgTerrain::TerrainManager::getSingleton().convertVgtrToHeightImage();
}

void CvgFrameWorkView::OnTtttttest()
{
	// TODO: Add your command handler code here


	//vgKernel::RendererQueue& selected = 
	//	vgKernel::SelectManager::getSingleton().getSelectedItems();

	//vgKernel::RendererQueue::iterator iter = selected.begin();
	//vgKernel::RendererQueue::iterator iter_end = selected.end();

	//for ( ; iter != iter_end ; ++ iter )
	//{
	//	vgKernel::RendererManager::getSingleton().deleteRenderer( *iter );
	//	if ( (*iter)->getType() == vgMesh::RENDERER_TYPE_MESH )
	//	{
	//		vgMesh::MeshManager::getSingleton().deleteMeshItem( 
	//			(vgMesh::MeshItem*)*iter );
	//	}
	//}

	vgTerritory::TerrainManager::getSingleton();

}

// vg
void CvgFrameWorkView::OnFileImportMod()
{
	std::ostringstream o;
	o	<< "-----------------------------------------------------------\n"; 
#if VGK_LANG_CHS
	o << "\t\t����ִ�� ����MOD����:\n";
#else
	o << "\t\tImporting MOD:\n";
#endif
	o	<< "-----------------------------------------------------------\n" ;
	VGK_SHOW( o.str() );


	vgApp.appProc(VG_COMMAND_MODEL, VG_COMMAND_MODEL_ADD,this);	


	std::ostringstream oEnd;
	oEnd<< "-----------------------------------------------------------\n"; 
#if VGK_LANG_CHS
	oEnd<< "\t\t���ִ�� ����MOD����\n";
#else
	oEnd<< "\t\tImport MOD Completed!\n";
#endif
	oEnd<< "-----------------------------------------------------------\n" ;
	VGK_SHOW( oEnd.str() );

}


void CvgFrameWorkView::OnCameraCapture()
{
	vgCam::CamManager::getSingleton().captureToFile();
}

void CvgFrameWorkView::OnToolTraverseswitch()
{
	// TODO: �ڴ���������������
	//vgTree::TreeFactory::getTree()->switchTreeStruct();
}

void CvgFrameWorkView::OnUpdateToolTraverseswitch(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������

	//pCmdUI->SetCheck(vgTree::TreeFactory::getTree()->isUseTree());
}


// keyframe
void CvgFrameWorkView::OnFileImportKfm()
{
	// TODO: �ڴ���������������
	vgApp.appProc(VG_COMMAND_MODEL, VG_COMMAND_KEYFRAME_ADD,this);	

}
// ms3d
void CvgFrameWorkView::OnFileImportMs3d()
{
	// TODO: �ڴ���������������
	CString DefExt;

	DefExt.Format("%s","ms3d�ļ�(*.ms3d)|*.ms3d|");

	///CFileDialog��һ��������Ϊtrue��Ϊ���ļ��Ի���,�öλ���mfc
	CFileDialog dlgmodelFile(true,"ms3d",NULL,OFN_HIDEREADONLY|OFN_CREATEPROMPT|OFN_NONETWORKBUTTON,DefExt);

	dlgmodelFile.m_ofn.lpstrTitle="VG_MS3D";

	if(dlgmodelFile.DoModal()==IDOK)
	{
		CString FilePathName = dlgmodelFile.GetPathName();
		vgCore::QuadtreeSceneManager::getSingleton().AddSkelecton(FilePathName);
	}

	SetCurrentDirectoryA(vgApp.scenetree->pathbuffer);
}

void CvgFrameWorkView::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CView::OnWindowPosChanging(lpwndpos);

	CView::OnWindowPosChanging(lpwndpos);

	CRect rect;

	GetWindowRect(&rect);

	vgCam::CamManager::getSingleton().setCenterOfScreen(
		rect.TopLeft().x+ rect.Width()/2, rect.TopLeft().y+ rect.Height()/2);

	// TODO: �ڴ˴������Ϣ����������
}

// ���&�������
void CvgFrameWorkView::OnCreatcam()
{
	// TODO: �ڴ���������������

	vgCam::ViewCam* items =
		vgCam::CamManager::getSingleton().saveCurrentCameraView();


	if ( items == NULL )
	{
		return;
	}


	{
		HTREEITEM	hRootCamera = vgUIController::GetInstance()->GetRootStaticCamera();
		vgBaseEntry* entry = vgEntryFactory::createEntryFromRenderer( items );
		vgUIController::GetInstance()->AddNode( hRootCamera,entry );

		vgKernel::RendererManager::getSingleton().addRenderer( items );
	
		if (hRootCamera != NULL)
		{
			vgUIController::GetInstance()->GetWorkSpaceBar()->Expand(hRootCamera );
			vgUIController::GetInstance()->GetWorkSpaceBar()->EnsureVisible(hRootCamera);
		}	
	
	}
}

void CvgFrameWorkView::OnAddanimation()
{
	// TODO: �ڴ���������������

	vgCam::CameraRecordPtr newone(new vgCam::CameraRecord());

	//�ĳɴ��ļ��ж�ȡ����Ҳ����
	//std::ostringstream o;
	//o << "VCR" <<vgCam::CamManager::getSingleton().getVCRvectorsize();

	//newone->setName( o.str() );

	//���������ݳ�ʼ����Ӧ���Ǵ��ļ��ж�ȡ��
	//newone->cloneCurrentCameraView();

	CString DefExt,filepath;
	DefExt.Format("%s","vcr�ļ�(*.vcr)|*.vcr|");	
	CFileDialog dlgmodelFile(FALSE,"vcr",NULL,OFN_HIDEREADONLY|
		OFN_CREATEPROMPT|OFN_NONETWORKBUTTON,DefExt);	
	dlgmodelFile.m_ofn.lpstrTitle="���������vcr�ļ�";	
	if(dlgmodelFile.DoModal()==IDOK)
	{
		filepath = dlgmodelFile.GetPathName();	

		std::ostringstream o;
		string  filepath0= filepath;
		string _filename = filepath0.substr( filepath0.find_last_of("\\/")+1,std::string::npos);
		_filename=_filename.substr(0,_filename.length()-4);
		o << _filename;
		newone->setName( o.str() );

		newone->setVcrFilePath(filepath.GetBuffer(0));
		newone->loadVCR(filepath.GetBuffer(0));

		vgCam::CamManager::getSingleton().addCameraRecord(newone);

		assert( !newone.isNull()/* != NULL */);
		{
			HTREEITEM	hRootAni = vgUIController::GetInstance()->GetRootAnimation();
			vgBaseEntry* entry = vgEntryFactory::createEntryFromRenderer( newone.getPointer() );
			vgUIController::GetInstance()->AddNode( hRootAni, entry );

			if (hRootAni != NULL)
			{
				vgUIController::GetInstance()->GetWorkSpaceBar()->Expand(hRootAni );
				vgUIController::GetInstance()->GetWorkSpaceBar()->EnsureVisible(hRootAni);
			}	
		}
	}
}

void CvgFrameWorkView::OnAniStart()
{
	// TODO: �ڴ���������������
	vgCam::CamManager::getSingleton().getCurrentCameraRecord()->beginRecording();
}

void CvgFrameWorkView::OnAniEnd()
{
	// TODO: �ڴ���������������
	{
		vgCam::CamManager::getSingleton().getCurrentCameraRecord()->finishRecording();
		CString DefExt,filepath;
		DefExt.Format("%s","vcr�ļ�(*.vcr)|*.vcr|");	
		CFileDialog dlgmodelFile(FALSE,"vcr",NULL,OFN_HIDEREADONLY|
			OFN_CREATEPROMPT|OFN_NONETWORKBUTTON,DefExt);	
		dlgmodelFile.m_ofn.lpstrTitle="�����������vcr�ļ�";	
		if(dlgmodelFile.DoModal()==IDOK)
		{
			filepath = dlgmodelFile.GetPathName();	
			vgCam::CamManager::getSingleton().getCurrentCameraRecord()->saveVCR(filepath.GetBuffer(0));


			///
			vgCam::CameraRecordPtr newone(new vgCam::CameraRecord());

			//�ĳɴ��ļ��ж�ȡ����Ҳ����
			//std::ostringstream o;
			//o << "VCR" <<vgCam::CamManager::getSingleton().getVCRvectorsize();

			//newone->setName( o.str() );
			std::ostringstream o;
			string  filepath0= filepath;
			string _filename = filepath0.substr( filepath0.find_last_of("\\/")+1,std::string::npos);
			_filename=_filename.substr(0,_filename.length()-4);
			o << _filename;
			newone->setName( o.str() );

			//���������ݳ�ʼ�������ļ��ж�ȡ
			//newone->cloneCurrentCameraView();
			newone->setVcrFilePath(filepath.GetBuffer(0));
			newone->loadVCR(filepath.GetBuffer(0));

			vgCam::CamManager::getSingleton().addCameraRecord(newone);

			assert( !newone.isNull()/* != NULL */);
			{
				HTREEITEM	hRootAni = vgUIController::GetInstance()->GetRootAnimation();

				vgBaseEntry* entry = vgEntryFactory::createEntryFromRenderer( newone.getPointer() );
				vgUIController::GetInstance()->AddNode( hRootAni, entry );

				vgKernel::RendererManager::getSingleton().addRenderer( newone.getPointer() );

				if (hRootAni != NULL)
				{
					vgUIController::GetInstance()->GetWorkSpaceBar()->Expand(hRootAni );
					vgUIController::GetInstance()->GetWorkSpaceBar()->EnsureVisible(hRootAni);
				}	
			}
		}

	}

}

void CvgFrameWorkView::OnAniPlay()
{
	// TODO: �ڴ���������������
	{
		//�õ���ǰѡ�е�Ŀ�갡���ж��Ƿ���VCR�����Ƿ���Ч����Ч������������

		/*vector<vgKernel::Renderer*>*/ 
		vgKernel::RendererQueue sel=vgKernel::SelectManager::getSingleton().getSelectedItems();
		vgCam::CameraRecord *to;
		if (sel.size()>0)//��ǰ��ѡ�е�Ŀ��
		{	
			vgKernel::Renderer* tmp=sel[0];
			//����ж��Ƿ�����Ч��VCR�����أ�
			if(tmp->getType()==vgCam::RENDERER_TYPE_VCR)
			{
				to=(vgCam::CameraRecord *)tmp;
				vgCam::CamManager::getSingleton().setCurrentCameraRecord(to);
				vgCam::CamManager::getSingleton().getCurrentCameraRecord()->startPlaying();
				//vgCam::CamManager::getSingleton().getCurrentCameraRecord()->b_play=true;
				//vgCam::CamManager::getSingleton().getCurrentCameraRecord()->uiFrame=0;
				//vgCam::CamManager::getSingleton().getCurrentCameraRecord()->m_Timer.Init();
				//vgCam::CamManager::getSingleton().getCurrentCameraRecord()->fTime=0;
			}

		}
		else
			return;
	}
}

void CvgFrameWorkView::OnAniResume()
{
	// TODO: �ڴ���������������
	vgCam::CamManager::getSingleton().getCurrentCameraRecord()->continuePlaying();
	//vgCam::CamManager::getSingleton().getCurrentCameraRecord()->b_play=true;
	//vgCam::CamManager::getSingleton().getCurrentCameraRecord()->uiFrame=0;
	//vgCam::CamManager::getSingleton().getCurrentCameraRecord()->m_Timer.Init();
	//vgCam::CamManager::getSingleton().getCurrentCameraRecord()->fTime=0;
}

void CvgFrameWorkView::OnAniStop()
{
	// TODO: �ڴ���������������
	vgCam::CamManager::getSingleton().getCurrentCameraRecord()->stopPlaying();

	//if (vgCam::CamManager::getSingleton().getCurrentCameraRecord()->b_play==true)
	//{
	//	int nowframenum=vgCam::CamManager::getSingleton().getCurrentCameraRecord()->uiFrame;
	//	vgCam::CameraRecord* nowVCR=vgCam::CamManager::getSingleton().getCurrentCameraRecord();
	//	vgCam::CamManager::getSingleton().setCurrentPosition(nowVCR->frameptr[nowframenum].pos);


	//	vgKernel::Quat qt0=nowVCR->frameptr[nowframenum].qt,Aq;
	//	Aq.createFromAxisRadian(qt0.m_x,qt0.m_y,qt0.m_z,qt0.m_w);
	//	Aq.normalize();
	//	vgKernel::Vec3 from(1,0,0),from1(0,0,-1);
	//	vgKernel::Vec3 to,to1;
	//	vgKernel::Mat4 mat;
	//	Aq.createMatrix(mat._m);

	//	to=mat*from;
	//	to1=mat*from1;

	//	float sethead;
	//	if (to.v[0]>0)
	//	{
	//		sethead=180/PI*asin(to.v[2]/sqrt(to.v[0]*to.v[0]+to.v[2]*to.v[2]));
	//	}
	//	else
	//	{
	//		sethead=180-180/PI*asin(to.v[2]/sqrt(to.v[0]*to.v[0]+to.v[2]*to.v[2]));
	//	}

	//	float setpitch=180/PI*asin(-to1.v[1]/*/to.length()*/);

	//	vgCam::CamManager::getSingleton().setHeadingDegrees(sethead);
	//	vgCam::CamManager::getSingleton().setPitchDegrees(setpitch);

	//	vgCam::CamManager::getSingleton().getCurrentCameraRecord()->b_play=false;
	//	vgCam::CamManager::getSingleton().getCurrentCameraRecord()->uiFrame=0;
	//	//vgCam::CamManager::getSingleton().getCurrentCameraRecord()->m_Timer.Init();
	//	vgCam::CamManager::getSingleton().getCurrentCameraRecord()->fTime=0;
	//}
}




void CvgFrameWorkView::OnSwitchcam()
{
	// TODO: �ڴ���������������

	vgCam::CamFacade::gotoSelectedCamera();

	return;

	vgKernel::RendererQueue& sel=vgKernel::SelectManager::getSingleton().getSelectedItems();
	//�����Ƕ�̬�л�
	if (sel.size()>0)
	{	
		if (sel[0]->getType() !=  vgCam::RENDERER_TYPE_VIEWCAMERA)
		{
			MessageBox( "��ѡ�����!","VR-GIS",MB_OK );

			return;
		}
		vgCam::ViewCam *to,from;
		to=(vgCam::ViewCam *)sel[0];
		//to.setPosition(vgKernel::SelectManager::getSingleton().GetSelectedCenter());
		from.cloneCurrentCameraView();
		to->setToUnvisible();
		vgCam::CamManager::getSingleton().getCurrentCameraRecord()->switchCamera(&from,to);
		//vgCam::CamManager::getSingleton().ImmediateToViewCam(to);// ��ֱ̬����ת

		vgKernel::SelectManager::getSingleton().clearSelection();
	}

	//vgKernel::RendererQueue& sel=vgKernel::SelectManager::getSingleton().getSelectedItems();
	////�����Ƕ�̬�л�
	//if (sel.size()>0)
	//{	
	//	vgCam::ViewCam *to,from;to=(vgCam::ViewCam *)sel[0];
	//	//to.setPosition(vgKernel::SelectManager::getSingleton().GetSelectedCenter());
	//	from.cloneCurrentCameraView();
	//	to->setToUnvisible();
	//	vgCam::CamManager::getSingleton().getCurrentCameraRecord()->SwitchCamera(&from,to);
	//	vgKernel::SelectManager::getSingleton().ClearSelection();
	//}

	//�����Ǿ�̬�л�
	//if (sel.size()>0)
	//{	
	//	vgCam::ViewCam *to,from;to=(vgCam::ViewCam *)sel[0];
	//	from.cloneCurrentCameraView();
	//	to->setToUnvisible();
	//	vgKernel::SelectManager::getSingleton().ClearSelection();
	//	vgCam::CamManager::getSingleton().ImmediateToViewCam(to);
	//}

}

void CvgFrameWorkView::OnPositiontoobj()
{
	// TODO: �ڴ���������������
//	vgCam::ViewCam to,from;
//	to.setPosition(vgKernel::SelectManager::getSingleton().GetSelectedCenter());
//	from.cloneCurrentCameraView();
//	vgCam::CamManager::getSingleton().getCurrentCameraRecord()->SwitchCamera(&from,&to);
	//vgKernel::SelectManager::getSingleton().ClearSelection();

	vgCam::CamFacade::gotoSelectedObject();

	return;

	vgCam::ViewCam to,from;
	vgKernel::Box nowbox=vgKernel::SelectManager::getSingleton().getSelectedBox();
	to.setPosition(1.5*nowbox._maxVertex/*GetSelectedCenter()*/-0.5*nowbox._minVertex);

	vgKernel::Vec3 vec1=nowbox._minVertex-nowbox._maxVertex;


	float sethead;
	sethead=270-180/PI*asin(vec1[2]/sqrt(vec1[0]*vec1[0]+vec1[2]*vec1[2]));
	if (sethead>180)
	{
		sethead-=360;
	}
	to.setHeadingDegrees(sethead);
	float setpitch=180/PI*asin(-vec1[1]/vec1.length());
	to.setPitchDegrees(setpitch);
	from.cloneCurrentCameraView();
		vgCam::CamManager::getSingleton().setDefaultCamRecordPtr();
	vgCam::CamManager::getSingleton().getCurrentCameraRecord()->switchCamera(&from,&to);
}


// ��Ч
void CvgFrameWorkView::Onfountianset()
{
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_FOUNTAIN_SET, this);

	// TODO: �ڴ���������������
}

void CvgFrameWorkView::OnSeFountain()
{
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_FOUNTAIN_ADD, this);

	// TODO: �ڴ���������������
}

void CvgFrameWorkView::OnFireAdd()
{
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_FIRE, this);

	// TODO: �ڴ���������������
}

void CvgFrameWorkView::OnSeSmoke()
{
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_SMOKE, this);
	// TODO: �ڴ���������������
}

void CvgFrameWorkView::OnAddflag()
{
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_FLAG_ADD,this);
	// TODO: �ڴ���������������
}

void CvgFrameWorkView::OnWaterConvert()
{
	// TODO: �ڴ���������������
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_WATER_CONVERT, this);
	SetCurrentDirectoryA(vgApp.scenetree->pathbuffer);
}

void CvgFrameWorkView::OnWaterEdit()
{
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_WATER_EDIT, this);
	SetCurrentDirectoryA(vgApp.scenetree->pathbuffer);
	// TODO: �ڴ���������������
}

void CvgFrameWorkView::OnGlassConvert()
{

	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_GLASS_CONVERT, this);
	SetCurrentDirectoryA(vgApp.scenetree->pathbuffer);
}

void CvgFrameWorkView::OnAviNodeConvert()
{
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_AVI_CONVERT, this);
	SetCurrentDirectoryA(vgApp.scenetree->pathbuffer);
	//vgApp.scenetree->model_pick_set.insert( 1 );
	//vgCore::QuadtreeSceneManager::getSingleton().convertSelectedObjectToEffectNode
	//	< vgCore::AviNode , DlgAviNode , /*vgCore::*/IMG_AVINODE >();
	// TODO: �ڴ���������������
}

void CvgFrameWorkView::OnSkyboxConvert()
{
	// TODO: �ڴ���������������
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_SKY_CONVERT, this);
	SetCurrentDirectoryA(vgApp.scenetree->pathbuffer);
}

void CvgFrameWorkView::OnBeginplant()
{
	vgApp.appProc(VG_COMMAND_TREE, VG_COMMAND_TREE_PLANT, this);

	// TODO: �ڴ���������������
}



void CvgFrameWorkView::OnRain() 
{
	// TODO: Add your command handler code here
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_RAIN, this);

	//vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_FOG, this);
	//Update();
	Rainon = false;

	//fognum++;
}

void CvgFrameWorkView::OnSnow() 
{
	// TODO: Add your command handler code here
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_SNOW, this);

	//vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_FOG, this);
	//Update();
	Snowon = false;
	//fognum++;	
}
void CvgFrameWorkView::OnRainDel() 
{
	// TODO: Add your command handler code here
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_RAIN_DEL, this);
	//Update();

	/*fognum-=1;	
	if (fognum == 0)
	{
		vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_FOG_DEL, this);	

	}*/
	Rainon = true;
}

void CvgFrameWorkView::OnSnowDel() 
{
	// TODO: Add your command handler code here
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_SNOW_DEL, this);
	//Update();
	/*fognum-=1;
	if (!fognum)
	{
		vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_FOG_DEL, this);	

	}*/
	Snowon = true;
}

void CvgFrameWorkView::OnSeFog()
{
	// TODO: �ڴ���������������
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_FOG, this);

}
void CvgFrameWorkView::OnFogDel() 
{
	// TODO: Add your command handler code here
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_FOG_DEL, this);

}

void CvgFrameWorkView::OnMirrorConvert()
{
	// TODO: �ڴ���������������
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_MIRROR_CONVERT, this);
	SetCurrentDirectoryA(vgApp.scenetree->pathbuffer);
}

void CvgFrameWorkView::OnMetalConvert()
{
	// TODO: �ڴ���������������
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_METAL_CONVERT, this);
	SetCurrentDirectoryA(vgApp.scenetree->pathbuffer);
}

void CvgFrameWorkView::OnConvertBrigthfloor()
{
	// TODO: �ڴ���������������
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_FLOOR_CONVERT, this);
	SetCurrentDirectoryA(vgApp.scenetree->pathbuffer);
}


// ��Ч
void CvgFrameWorkView::On3dsoundAdd()
{
	// TODO: �ڴ���������������
	char strFilter[]=
		"WAV Files (*.wav)|*.wav|";

	CFileDialog  OpenDlg(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		strFilter);


	int nFlags=OpenDlg.DoModal();

	if( nFlags != IDOK )
	{
		return ;
	}

	String filepath = OpenDlg.GetPathName();

	vgCore::SoundManager::getSingleton().create3DSoundFromCurrentPos( filepath );
}

void CvgFrameWorkView::OnBacksoundSet()
{
	// TODO: �ڴ���������������
	vgCore::SoundManager::getSingleton().createDlgToSetBackgroundMusic( );
	
	HTREEITEM	h3DSound = vgUIController::GetInstance()->GetRoot3DSound();
	if (h3DSound != NULL)
	{
		vgUIController::GetInstance()->GetWorkSpaceBar()->Expand(h3DSound );
		vgUIController::GetInstance()->GetWorkSpaceBar()->EnsureVisible(h3DSound);
	}
}

// �Ҽ�
void CvgFrameWorkView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	vgKernel::InputSystem::getSingleton().OnRButtonUp(nFlags, point);
	CView::OnRButtonUp(nFlags, point);
}

void CvgFrameWorkView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	vgKernel::InputSystem::getSingleton().OnRButtonDown(nFlags, point);
	CView::OnRButtonDown(nFlags, point);
}

// billboard tree
void CvgFrameWorkView::OnSettree()
{
	vgApp.appProc(VG_COMMAND_TREE, VG_COMMAND_TREE_SETTING, this);
	// TODO: �ڴ���������������
}

void CvgFrameWorkView::OnSelect()
{
	vgApp.appProc(VG_COMMAND_EDIT , VG_COMMAND_SELECT, this);
	// TODO: �ڴ���������������
}

void CvgFrameWorkView::OnPlantinline()
{
	vgApp.appProc(VG_COMMAND_TREE, VG_COMMAND_TREE_PLANTINLINE, this);
	// TODO: �ڴ���������������
}

// terrain
void CvgFrameWorkView::OnVgterrainNewVtr()
{
	// TODO: �ڴ���������������
	//vgTerritory::TerrainManager::getSingleton().turnOnRenderFlag();
	//vgTerritory::TerrainManager::getSingleton().openVtrFile();

	String filepath;

	CString DefExt;

	DefExt.Format("%s","vtr(*.vtr)|*.vtr|");

	CFileDialog dlgvgtr(true,"vtr",
		NULL,OFN_HIDEREADONLY|OFN_CREATEPROMPT|OFN_NONETWORKBUTTON,DefExt);

#if VGK_LANG_CHS
	dlgvgtr.m_ofn.lpstrTitle = "�򿪵����ļ�";
#else
	dlgvgtr.m_ofn.lpstrTitle = "Open Terrain File";
#endif


	if( dlgvgtr.DoModal() == IDOK )
	{
		filepath = dlgvgtr.GetPathName();
	}

	if ( filepath.empty() == false )
	{
		// ����·����ά�� [11/18/2008 zhu]
		//vgCore::QuadtreeSceneManager::getSingleton().vtrfilepath=filepath;
		bool has_create_node = 
			vgTerritory::TerrainManager::getSingleton().hasTerrainData();

		bool openok = 
			vgTerritory::TerrainManager::getSingleton().openVtrFile( filepath );

		if ( openok == true )
		{
			if ( has_create_node == false )
			{
				// ��ӵ��νڵ�
				vgUIController::GetInstance()->ClearSelection();
			}
		}
	}
}

// GIS
void CvgFrameWorkView::OnZuobiao() 
{
	// TODO: Add your command handler code here
	vgApp.appProc(VG_COMMAND_GIS, VG_COMMAND_GIS_ZUOBIAO, this);
}

void CvgFrameWorkView::OnJuli() 
{
	// TODO: Add your command handler code here
	vgApp.appProc(VG_COMMAND_GIS, VG_COMMAND_GIS_JULI, this);	
}

void CvgFrameWorkView::OnMianji() 
{
	// TODO: Add your command handler code here
	vgApp.appProc(VG_COMMAND_GIS, VG_COMMAND_GIS_MIANJI, this);
}

void CvgFrameWorkView::OnTiji() 
{
	// TODO: Add your command handler code here
	vgApp.appProc(VG_COMMAND_GIS, VG_COMMAND_GIS_TIJI, this);
}

// Tools
void CvgFrameWorkView::OnToolMakerVgm()
{
	// TODO: �ڴ���������������vgModelGenerator_Debug.exe
	CString	externelEXE = CString(vgApp.scenetree->pathbufferStatic) + "vgModelGenerator_Release.exe";
	
	VGMSGBOX2(
	ShellExecute(NULL, "open", externelEXE, NULL, NULL, SW_SHOWNORMAL)	;
	)

}

void CvgFrameWorkView::OnToolMakerVgi()
{
	// TODO: �ڴ���������������vgImagePacker_Debug.exe
	CString	externelEXE = CString(vgApp.scenetree->pathbufferStatic) + "vgImagePacker_Release.exe";
	
	VGMSGBOX2(
		ShellExecute(NULL, "open", externelEXE, NULL, NULL, SW_SHOWNORMAL)	;
	)
}
void CvgFrameWorkView::OnToolMakerDds()
{
	// TODO: �ڴ���������������ResizeTool
	CString	externelEXE = CString(vgApp.scenetree->pathbufferStatic) + "vgImageConverter_Release.exe";
	VGMSGBOX2(
		ShellExecute(NULL, "open", externelEXE, NULL, NULL, SW_SHOWNORMAL)	
		)
}

// Database
void CvgFrameWorkView::OnDbShow() 
{
	// TODO: Add your command handler code here
}


void CvgFrameWorkView::OnDbLink() 
{
	// TODO: Add your command handler code here
}

void CvgFrameWorkView::OnDbDatabase() 
{
}

void CvgFrameWorkView::OnDbTable() 
{
	// TODO: Add your command handler code here
}

void CvgFrameWorkView::OnDbRecord() 
{
	// TODO: Add your command handler code here
}

// Edit
void CvgFrameWorkView::OnEditSelect()
{
	// TODO: �ڴ���������������
	vgCore::QuadtreeSceneManager::getSingleton().vgCurrentMode = 1;
}

void CvgFrameWorkView::OnEditSelectClear()
{
	// TODO: �ڴ���������������
	vgKernel::SelectManager::getSingleton().clearSelection();
}





// Multi Channel
void CvgFrameWorkView::OnChannel1()
{
	// TODO: �ڴ���������������
	vgCam::CamManager::getSingleton().setChannelNum(1);
}

void CvgFrameWorkView::OnChannel0()
{
	// TODO: �ڴ���������������
	vgCam::CamManager::getSingleton().setChannelNum(0);
}

void CvgFrameWorkView::OnChannelm1()
{
	// TODO: �ڴ���������������
	vgCam::CamManager::getSingleton().setChannelNum(-1);
}
DWORD WINAPI  CvgFrameWorkView::threadFunc(LPVOID Data)//��������
{
assert(0);
#if 0
	//	m_bSendRun=TRUE;m_bSendRun
	while(1)
	{

		ViewerControl nn;		
		nn.pos=vgCam::CamManager::getSingleton().getCurrentPosition();

		nn.headdegrees=vgCam::CamManager::getSingleton().getHeadingDegrees();
		nn.pitchdegrees=vgCam::CamManager::getSingleton().getPitchDegrees();
		RakNet::BitStream sendStream;
		//zhu,������BitStream��д�벿�֣����������ͣ����ĳ���
		unsigned char typeId = ID_CAMERA_INFO;//�������Ժ�ģ���Ӧ�����������߳�ʱ��Ӧ�������Ӧ�Ĵ�������֧
		sendStream.Write(typeId);
		sendStream.Write((const char*) &nn, sizeof(nn));

		SystemAddress address;

		map<string, SystemAddress>::iterator iter,end;

		iter = ChannelServer::getInstance()->m_userList.begin();
		end =  ChannelServer::getInstance()->m_userList.end();
		while (iter != end) {
			if ((*iter).second != ChannelServer::getInstance()->m_serverAddress)  //���ظ��Լ������ӵ���Ϣ��
			{	
				address = (*iter).second;
				server->m_peer->Send(&sendStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, address, false);
			}
			++iter;
		}
		Sleep(1);//�������
	}
	

#endif

	return 0;
}

void CvgFrameWorkView::OnSetupServer()
{
	assert(0);

#if 0
	// TODO: �ڴ���������������
	server = ChannelServer::getInstance();
	//vgApp.IsServer=true;
	vgCam::CamManager::getSingleton().IsServer=true;
	vgCam::CamManager::getSingleton().b3Channel=true;
	// start message loop
	AfxBeginThread(ServerThread,NULL,THREAD_PRIORITY_NORMAL);// ����ͨ�ŵİ�����������̣߳����մ�������
	//ͬʱ�����̵߳�ѭ����Ӧ�ÿ�ʼѭ�����Ͱ�
	/*(LPTHREAD_START_ROUTINE)*/
	//long i;
	//vgApp.bFirstRunSever=true;
	hSendThread=CreateThread(NULL,0,threadFunc,0,0,&ThreadID);
	//vgApp.b3Channel=TRUE;
	//�����������ͬ���Ŀͻ��ˣ�����������
	client = ChannelClient::getInstance();

	//-----------------------------------------
	//begin
	//-----------------------------------------
	if (!AfxSocketInit())
	{
		AfxMessageBox("socket init error!");
		return ;
	}
	char host_name[256]; 
	gethostname(host_name,sizeof(host_name)); 
	struct hostent *hp; 
	static struct in_addr sa; 
	char *buf; 
	hp = gethostbyname(host_name); 
	if (hp != NULL) 
	{ 
		//ѭ����ȡ���������� 
		for (int i = 0; hp->h_addr_list[i]; i++) 
		{ 
			memcpy (&sa, hp->h_addr_list[i],hp->h_length); 

			buf= inet_ntoa(sa); 
		} 
	} 
	//-----------------------------------------
	//end
	//-----------------------------------------
	CString strIP=buf;
	//MessageBox(strIP);
	/*client->*/ChannelClient::getInstance()->ConnectServer(strIP);	/*address*/
	AfxBeginThread(ClientThread,NULL,THREAD_PRIORITY_NORMAL);// ����ͨ�ŵİ�����������̣߳�

#endif

}
void CvgFrameWorkView::OnDestroyServer()
{
	assert(0);

#if 0
	// TODO: �ڴ���������������
	vgCam::CamManager::getSingleton().b3Channel=false;
	SuspendThread(hSendThread);//�������߳�
	ChannelServer::destoryInstance();
	ChannelClient::destoryInstance();	
#endif
}

void CvgFrameWorkView::OnNewClient()
{
	assert(0);

#if 0
	// TODO: �ڴ���������������
	vgCam::CamManager::getSingleton().b3Channel=true;
	client = ChannelClient::getInstance();

	CIPDlg dlg;

	VGMSGBOX2( dlg.DoModal())
	
	BYTE IP[4];
	IP[0]=dlg.IP1;
	IP[1]=dlg.IP2;
	IP[2]=dlg.IP3;
	IP[3]=dlg.IP4;

	unsigned char  *pIP;  
	CString  strIP;   
	pIP  =  (unsigned char*)IP;  
	strIP.Format("�����ӷ�����:%u.%u.%u.%u", *pIP,  *(pIP+1),*(pIP+2)  ,*(pIP+3) );
	MessageBox(strIP);

	ChannelClient::getInstance()->ConnectServer(strIP);	/*address*/
	AfxBeginThread(ClientThread,NULL,THREAD_PRIORITY_NORMAL);// ����ͨ�ŵİ�����������̣߳�
	vgCam::CamManager::getSingleton().b3Channel=TRUE;
#endif

}

void CvgFrameWorkView::OnDelClient()
{
	assert(0);
#if 0
	// TODO: �ڴ���������������
	vgCam::CamManager::getSingleton().b3Channel=false;
	ChannelClient::destoryInstance();	
#endif
}

// about
void CvgFrameWorkView::OnAppAbout()
{
	// TODO: �ڴ���������������
	
	CAboutDlg	dlgAbout;
	VGMSGBOX2(dlgAbout.DoModal() );
}


// gismatch
void CvgFrameWorkView::OnGismatch()
{
	// TODO: �ڴ���������������
	VGMSGBOX2( vgCore::QuadtreeSceneManager::getSingleton().AddGismatch());

	vgCore::QuadtreeSceneManager::getSingleton().vgCurrentMode = VG_STATUS_GIS_MATCH;
}



// File
void CvgFrameWorkView::OnSceneRead()
{
	
	String	strVGReadPath;
	CString DefExt;

	DefExt.Format("%s","vg(*.vg)|*.vg|");

	CFileDialog dlgmodelFile(true,"vg",NULL,OFN_HIDEREADONLY|OFN_CREATEPROMPT|OFN_NONETWORKBUTTON,DefExt);

#if VGK_LANG_CHS
	dlgmodelFile.m_ofn.lpstrTitle="�򿪳����ļ�";
#else
	dlgmodelFile.m_ofn.lpstrTitle="Open Scene File";
#endif

	if(dlgmodelFile.DoModal()==IDOK)
	{
		CString strFilePath = dlgmodelFile.GetPathName();

		strVGReadPath = strFilePath.GetString();

		vgGlobal::SystemFacade::openSceneFile( strVGReadPath );

	}
}

void CvgFrameWorkView::OnSceneSave()
{

	CString DefExt;

	DefExt.Format("%s","vg(*.vg)|*.vg|");

	CFileDialog dlgmodelFile(false,"vg",NULL,OFN_HIDEREADONLY|OFN_CREATEPROMPT|OFN_NONETWORKBUTTON,DefExt);

#if VGK_LANG_CHS
	dlgmodelFile.m_ofn.lpstrTitle="���泡���ļ�";
#else
	dlgmodelFile.m_ofn.lpstrTitle="Save Scene File";
#endif


	if(dlgmodelFile.DoModal()==IDOK)
	{

		// �½�Ŀ¼��׼�����vg��vgimg�ļ�
		string FilePathName = dlgmodelFile.GetPathName();
		string Filename = dlgmodelFile.GetFileName();

		string Filefolderpath = FilePathName;

		vgGlobal::SystemFacade::saveScene( FilePathName );

	}

	return	;
}

void CvgFrameWorkView::OnSceneSave(string	strSavePath)
{

	vgGlobal::SystemFacade::saveScene( strSavePath );

	return;


	//std::ostringstream o;
	//o	<< "-----------------------------------------------------------\n" 
	//	<< "\t\t����ִ�� ���泡������:\n"
	//	<< "-----------------------------------------------------------\n" ;
	//VGK_SHOW( o.str() );

	//// Part 1��vg��vgimg
	////string	strSavePath = strSavePath;
	//	vgCore::QuadtreeSceneManager::getSingleton().saveSceneToVG(strSavePath);


	//vgGlobal::GlobalConsole::getSingleton().saveProject( 
	//	vgKernel::StringUtility::getPathFromAbsoluteFilename(strSavePath),
	//	vgKernel::StringUtility::getFilenameWithoutExtFromAbsPath( strSavePath )
	//	);

	////	Part 2��vgm��vgi
	//vgGlobal::GlobalFacade::getSingleton().saveModelAndImage(strSavePath);

	////	Part 3�����й滮����д��vg
	//vgGlobal::GlobalFacade::getSingleton().writeExtraDataToVG( strSavePath );

	////	Part 4�����ݿ������ļ� *.vgp		
	//String	filePathVgp = vgKernel::StringUtility::getPathFromAbsoluteFilename(strSavePath);
	//vgDatabase::ProfileManager::getSingleton().SaveRendererProfile(filePathVgp);


	////	Part 5: ʸ������д *.vsg
	//String filePathVsg = strSavePath.substr( 0 , strSavePath.find_last_of(".") );
	//filePathVsg += ".vsg";	
	//vgVector::VectorManager::getSingleton().writeLayersToVSGFile(filePathVsg);

	////MessageBox("���泡�������ɹ�!");

	//std::ostringstream oEnd;
	//oEnd<< "-----------------------------------------------------------\n" 
	//	<< "\t\t���ִ�� ���泡������\n"
	//	<< "-----------------------------------------------------------\n" ;
	//VGK_SHOW( oEnd.str() );

	//return;
}

void CvgFrameWorkView::OnPublishCollect()
{
	// Part 1��vg��vgimg
	string	strSavePath = 
		vgCore::QuadtreeSceneManager::getSingleton().saveSceneToVG();

	if (strSavePath.empty())
	{
		return;
	}

	// Part 2��vgm��vgi
	vgGlobal::GlobalFacade::getSingleton().collectModelAndImage(strSavePath);

	MessageBox("�ռ���Դ�����ɹ�!");
	return;

}

void CvgFrameWorkView::OnFileImport()
{
	// TODO: �ڴ���������������
	CString DefExt;

	DefExt.Format("%s","vgm�ļ�(*.vgm)|*.vgm|vgi�ļ�(*.vgi)|*.vgi|mod�ļ�(*.mod)|*.mod\
					   |shp�ļ�(*.shp)|*.shp|�����ļ�(*.*)|*.*|");

	CFileDialog dlgmodelFile(true,"vgm|vgi|mod|shp",
		NULL,OFN_HIDEREADONLY|
		OFN_CREATEPROMPT|
		OFN_NONETWORKBUTTON|
		OFN_ALLOWMULTISELECT|   
		OFN_FILEMUSTEXIST,
		DefExt);

	DWORD   MAXFILE = 0xFFFF;   
	dlgmodelFile.m_ofn.nMaxFile=MAXFILE; 

	char*  pc = new char[MAXFILE];   


	dlgmodelFile.m_ofn.lpstrFile = pc;   
	dlgmodelFile.m_ofn.lpstrFile[0] = NULL;   
	dlgmodelFile.m_ofn.lpstrTitle="�ļ�����";

	if( dlgmodelFile.DoModal() == IDOK )
	{
		POSITION posi;   
		vgKernel::StringVector strVec;

		posi = dlgmodelFile.GetStartPosition();   

		while( posi != NULL )
		{   
			strVec.push_back( dlgmodelFile.GetNextPathName(posi).GetBuffer(0));
		}   

		delete[] pc;

		vgGlobal::SystemFacade::importFiles( strVec );

	}//if

}

void CvgFrameWorkView::OnFileExport()
{
	// TODO: �ڴ���������������
	CString DefExt;

	DefExt.Format("%s","vgm�ļ�(*.vgm)|*.vgm");

	CFileDialog dlgmodelFile(false,"vgm",
		NULL,OFN_HIDEREADONLY|
		OFN_CREATEPROMPT|
		OFN_NONETWORKBUTTON|
		OFN_ALLOWMULTISELECT|   
		OFN_FILEMUSTEXIST,
		DefExt);

	DWORD   MAXFILE = 0xFFFF;   
	dlgmodelFile.m_ofn.nMaxFile=MAXFILE; 

	char*  pc = new char[MAXFILE];   


	dlgmodelFile.m_ofn.lpstrFile = pc;   
	dlgmodelFile.m_ofn.lpstrFile[0] = NULL;   
	dlgmodelFile.m_ofn.lpstrTitle="�ļ�����";

	if( dlgmodelFile.DoModal() == IDOK )
	{
		POSITION posi;   
		vgKernel::StringVector strVec;

		posi = dlgmodelFile.GetStartPosition();   

		while( posi != NULL )
		{   
			strVec.push_back( dlgmodelFile.GetNextPathName(posi).GetBuffer(0));
		}   

		delete[] pc;

		assert( strVec.size() == 1 );

		//------------------------------------------
		// ��ʼ����
		//------------------------------------------
		vgKernel::StringVector::iterator iter = strVec.begin();
		vgKernel::StringVector::iterator iter_end = strVec.end();

		using namespace vgMesh;
		for ( ; iter != iter_end ; ++ iter )
		{
			vgGlobal::SystemFacade::exportModelFile( *iter );
		}// for

	}//if
}

// Redo/Undo
void CvgFrameWorkView::OnUndo()
{
	// TODO: �ڴ���������������
	vgKernel::SelectManager::getSingleton().Undo();
}

void CvgFrameWorkView::OnRedo()
{
	// TODO: �ڴ���������������
	vgKernel::SelectManager::getSingleton().Redo();
}

//
void CvgFrameWorkView::OnMeshOrLine()
{
	// TODO: �ڴ���������������
	static bool dd = false;

	dd = !dd;

	if ( dd == true )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	else
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	}
}

// �Ż�
void CvgFrameWorkView::OnTreeSwitch()
{
	// TODO: �ڴ���������������
	m_bQuadtree = !m_bQuadtree;
	if (m_bQuadtree)
	{
		vgKernel::RendererManager::getSingleton().switchTreeType(vgTree::TREE_QUAD);
	}
	else
	{
		vgKernel::RendererManager::getSingleton().switchTreeType(vgTree::TREE_LINEAR);
	}
}

void CvgFrameWorkView::OnUpdateTreeSwitch(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(m_bQuadtree);
}

void CvgFrameWorkView::OnOccSwitch()
{
	// TODO: �ڴ���������������
	if (vgOcclusion::vgOcclusionManager::getSingleton().GetOccState())
	{
		vgOcclusion::vgOcclusionManager::getSingleton().DisableOcc();
	}
	else
		vgOcclusion::vgOcclusionManager::getSingleton().EnableOcc();
}

void CvgFrameWorkView::OnUpdateOccSwitch(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(vgOcclusion::vgOcclusionManager::getSingleton().GetOccState());
}

void CvgFrameWorkView::OnEditTranall()
{
	// TODO: �ڴ���������������
	CDlgTranslateAll	tranDlg;
	VGMSGBOX2( tranDlg.DoModal() );

	vgKernel::RendererManager::getSingleton().translateAllRenderers(
		 tranDlg.getTranslateVec() );
}

void CvgFrameWorkView::OnShowModels()
{
	assert(0);

	// TODO: �ڴ���������������
	//m_bRenderBox = !m_bRenderBox;
	//vgKernel::RendererManager::getSingleton().setBoxRender(m_bRenderBox);
}

void CvgFrameWorkView::OnUpdateShowModels(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	//pCmdUI->SetCheck(m_bRenderBox);
}

void CvgFrameWorkView::OnNewplan()
{
	//vgCore::CityplanManager::getSingleton().AddPlanblueprint();
	// TODO: �ڴ���������������
}

void CvgFrameWorkView::OnVgtAttach()
{
	// TODO: �ڴ���������������
	vgGlobal::GlobalUtility::attachRendererToTerrain( 
		&vgKernel::SelectManager::getSingleton().getSelectedItems() , true );
}

void CvgFrameWorkView::On32971()
{
	vgTerritory::TerrainManager::getSingleton().setCollisionDetectedFlag(
		!vgTerritory::TerrainManager::getSingleton().getCollisionDetectedFlag() );
}

void CvgFrameWorkView::OnUpdate32971(CCmdUI *pCmdUI)//zhu������������ײ
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( vgTerritory::TerrainManager::getSingleton().hasTerrainData() );
	pCmdUI->SetCheck(
		vgTerritory::TerrainManager::getSingleton().getCollisionDetectedFlag() );
}

void CvgFrameWorkView::On32972()
{
	vgTerritory::TerrainManager::getSingleton().setRenderFlag(
		!vgTerritory::TerrainManager::getSingleton().getRenderFlag() );
}

void CvgFrameWorkView::OnUpdate32972(CCmdUI *pCmdUI)//����������Ⱦ
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( vgTerritory::TerrainManager::getSingleton().hasTerrainData() );
	pCmdUI->SetCheck(
		vgTerritory::TerrainManager::getSingleton().getRenderFlag() );
}

void CvgFrameWorkView::On32973()
{
	vgTerritory::TerrainManager::getSingleton().dropTerrianData();
}

void CvgFrameWorkView::OnUpdate32973(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( vgTerritory::TerrainManager::getSingleton().hasTerrainData() );
}

void CvgFrameWorkView::On32974()//�������
{
	// TODO: Add your command handler code here


	vgTerritory::TerrainManager::getSingleton().setCutSurfaceAnalysisFlag(
		!vgTerritory::TerrainManager::getSingleton().getCutSurfaceAnaysisFlag() );
}

void CvgFrameWorkView::OnUpdate32974(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( vgTerritory::TerrainManager::getSingleton().hasTerrainData() );

	pCmdUI->SetCheck( 
		vgTerritory::TerrainManager::getSingleton().getCutSurfaceAnaysisFlag() );
}

void CvgFrameWorkView::OnVgtVisibility()//ͨ͸����
{
	// TODO: Add your command handler code here


	vgTerritory::TerrainManager::getSingleton().setCutSurfaceVisibilityFlag(
		!vgTerritory::TerrainManager::getSingleton().getCutSurfaceVisibilityFlag() );

}
void CvgFrameWorkView::OnUpdateVgtVisibility(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( vgTerritory::TerrainManager::getSingleton().hasTerrainData() );

	pCmdUI->SetCheck( 
		vgTerritory::TerrainManager::getSingleton().getCutSurfaceVisibilityFlag() );

}

void CvgFrameWorkView::OnUpdateVgtAttach(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( vgTerritory::TerrainManager::getSingleton().hasTerrainData() );

}

void CvgFrameWorkView::OnUpdateTerrainLeveling(CCmdUI *pCmdUI)//����ƽ������������
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable( vgTerritory::TerrainManager::getSingleton().hasTerrainData() );
	pCmdUI->SetCheck( 
		vgTerritory::TerrainManager::getSingleton().getCutSurfaceLevellingFlag() );
}

void CvgFrameWorkView::OnVgtCutAnalysis2()//��������������
{
	// TODO: Add your command handler code here
	vgTerritory::TerrainManager::getSingleton().setCutSurfaceAnalysisFlag2(
		!vgTerritory::TerrainManager::getSingleton().getCutSurfaceAnaysisFlag2() );
}

void CvgFrameWorkView::OnUpdateVgtCutAnalysis2(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( vgTerritory::TerrainManager::getSingleton().hasTerrainData() );
	pCmdUI->SetCheck(
		vgTerritory::TerrainManager::getSingleton().getCutSurfaceAnaysisFlag2() );
}

void CvgFrameWorkView::OnVgtTerrainSave()
{
	CString DefExt;

	DefExt.Format("%s","vg�ļ�(*.vg)|*.vg|");

	CFileDialog dlgmodelFile(false,"vg",NULL,OFN_HIDEREADONLY|OFN_CREATEPROMPT|OFN_NONETWORKBUTTON,DefExt);

	dlgmodelFile.m_ofn.lpstrTitle="���泡���ļ�";

	//		GetCurrentCamera()._useKeyControl = false;

	if(dlgmodelFile.DoModal()==IDOK)
	{

		// �½�Ŀ¼��׼�����vg��vgimg�ļ�
		string FilePathName = dlgmodelFile.GetPathName();
		string Filename = dlgmodelFile.GetFileName();

		string Filefolderpath = FilePathName;

		

		//------------------------------------------
		// ���
		//------------------------------------------
		vgTerritory::ElevationFilter::getSingleton().doFilterAndSave();

		vgGlobal::SystemFacade::saveScene( FilePathName );
	}
}

void CvgFrameWorkView::OnUpdateVgtTerrainSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( vgTerritory::TerrainManager::getSingleton().hasTerrainData() 
		&& vgTerritory::ElevationFilter::getSingleton().needToSave() );
}
void CvgFrameWorkView::Onterraintesting()//���λ�ԭ
{
	using namespace vgTerritory;

	ElevationFilter::getSingleton().clearPolygons();

	TerrainCenterManager::getSingleton().reloadAllCenters();

	vgMod::ModTerrainManager::getSingleton().reloadModTerrain();
}
void CvgFrameWorkView::OnUpdate33109(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable( vgTerritory::TerrainManager::getSingleton().hasTerrainData() 
		&& vgTerritory::ElevationFilter::getSingleton().needToSave() );
}


void CvgFrameWorkView::OnVecPoints()
{
	// TODO: �ڴ���������������
	vgApp.appProc(VG_COMMAND_GIS, VG_COMMAND_GIS_VEC_VERTEX, this);	
}

void CvgFrameWorkView::OnVecLines()
{
	// TODO: �ڴ���������������
	vgApp.appProc(VG_COMMAND_GIS, VG_COMMAND_GIS_VEC_LINE, this);
}

void CvgFrameWorkView::OnVecPolygon()
{
	// TODO: �ڴ���������������
		vgApp.appProc(VG_COMMAND_GIS, VG_COMMAND_GIS_VEC_POLY, this);	
}

void CvgFrameWorkView::OnVecEdit()
{
	// TODO: �ڴ���������������
	vgApp.appProc(VG_COMMAND_GIS, VG_COMMAND_GIS_VECEDIT, this);
}

void CvgFrameWorkView::OnTerrainLeveling()
{
	// TODO: �ڴ���������������
	vgApp.appProc(VG_COMMAND_GIS, VG_COMMAND_GIS_TER_LEVELING, this);	
}

void CvgFrameWorkView::OnHideVec()
{
	// TODO: �ڴ���������������
	vgApp.appProc(VG_COMMAND_GIS, VG_COMMAND_GIS_HIDE_VEC, this);
}

void CvgFrameWorkView::OnNoselVec()
{
	// TODO: �ڴ���������������
	vgApp.appProc(VG_COMMAND_GIS, VG_COMMAND_GIS_SELNONE_VEC, this);
}


void CvgFrameWorkView::OnVgtBlend()
{
	vgTerritory::TerrainManager::getSingleton().setBlendFlag(
		!vgTerritory::TerrainManager::getSingleton().getBlendFlag() );
}

void CvgFrameWorkView::OnUpdateVgtBlend(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( vgTerritory::TerrainManager::getSingleton().hasTerrainData() );
	pCmdUI->SetCheck(
		vgTerritory::TerrainManager::getSingleton().getBlendFlag() );

}
void CvgFrameWorkView::OnVgtVisibility2()
{
	vgTerritory::TerrainManager::getSingleton().setCutSurfaceVisibilityFlag2(
		!vgTerritory::TerrainManager::getSingleton().getCutSurfaceVisibilityFlag2() );
}

void CvgFrameWorkView::OnUpdateVgtVisibility2(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( vgTerritory::TerrainManager::getSingleton().hasTerrainData() );
	pCmdUI->SetCheck(
		vgTerritory::TerrainManager::getSingleton().getCutSurfaceVisibilityFlag2() );

}

void CvgFrameWorkView::OnVgtCutarea()
{
	vgTerritory::TerrainManager::getSingleton().setCutAreaFlag(
		!vgTerritory::TerrainManager::getSingleton().getCutAreaFlag() );
}

void CvgFrameWorkView::OnUpdateVgtCutarea(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( vgTerritory::TerrainManager::getSingleton().hasTerrainData() );
	pCmdUI->SetCheck(
		vgTerritory::TerrainManager::getSingleton().getCutAreaFlag() );

}

void CvgFrameWorkView::OnNdbConnect()
{
	// TODO: �ڴ���������������
	DBConnectDlg dialog;

	VGMSGBOX2(dialog.DoModal());
}

void CvgFrameWorkView::OnNdbCondition()
{
	// TODO: �ڴ���������������
	if (! DatabaseManager::getSingleton().IsOpen())
	{
		DBConnectDlg dialog;

		VGMSGBOX2(dialog.DoModal());

		if (! DatabaseManager::getSingleton().IsOpen())
		{
			return ;
		}
	}

	DBCondition dlg;

	VGMSGBOX2(dlg.DoModal());
}

void CvgFrameWorkView::OnShapefileDb()
{
	// TODO: �ڴ���������������
	if (!vgDatabase::DatabaseManager::getSingleton().IsOpen())
	{
		AfxMessageBox("�����������ݿ⣡");
		return;
	}
	//vgDatabase::DatabaseManager::getSingleton().CreateShapeTable();
}
void CvgFrameWorkView::OnCollisionOnoff()
{
	// TODO: �ڴ���������������
	vgCam::CamManager::getSingleton().setCollisionOnOff(
		! vgCam::CamManager::getSingleton().getCollisionOnOff() );
}

void CvgFrameWorkView::OnUpdateCollisionOnoff(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(
		vgCam::CamManager::getSingleton().getCollisionOnOff() );
}

void CvgFrameWorkView::OnVgtMappingOrigin()
{
	vgTerritory::VtrHeaderPtr header = 
		vgTerritory::ElevationManager::getSingleton().getMainFileHandleVtr()->getHeader();

	if ( header.isNull() == true )
	{
		return;
	}

	vgCore::vgCoordinate::getSingleton().SetSceneCood( 
		vgCore::Vector3( header->originX , header->originY , -header->originZ) );

}

void CvgFrameWorkView::OnUpdateVgtMappingOrigin(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( vgTerritory::TerrainManager::getSingleton().hasTerrainData() );
}

// ShapeFile -----------------------------------------------------------------
void CvgFrameWorkView::OnVectorShpImport()
{
	// TODO: Add your command handler code here
	CString DefExt;

	DefExt.Format("%s","shp�ļ�(*.shp)|*.shp|(*.*)|*.*|");

	CFileDialog dlgmodelFile(true,"shp",
		NULL,OFN_HIDEREADONLY|
		OFN_CREATEPROMPT|
		OFN_NONETWORKBUTTON|
		OFN_ALLOWMULTISELECT|   
		OFN_FILEMUSTEXIST,
		DefExt);

	DWORD   MAXFILE = 0xFFFF;   
	dlgmodelFile.m_ofn.nMaxFile=MAXFILE; 

	char*  pc = new char[MAXFILE];   


	dlgmodelFile.m_ofn.lpstrFile = pc;   
	dlgmodelFile.m_ofn.lpstrFile[0] = NULL;   
	dlgmodelFile.m_ofn.lpstrTitle="�ļ�����";

	if( dlgmodelFile.DoModal() == IDOK )
	{
		POSITION posi;   
		vgKernel::StringVector strVec;

		posi = dlgmodelFile.GetStartPosition();   

		while( posi != NULL )
		{   
			strVec.push_back( dlgmodelFile.GetNextPathName(posi).GetBuffer(0));
		}   

		delete[] pc;

		//------------------------------------------
		// ��ʼ����
		//------------------------------------------
		vgKernel::StringVector::iterator iter = strVec.begin();
		vgKernel::StringVector::iterator iter_end = strVec.end();

		using namespace vgMesh;
		for ( ; iter != iter_end ; ++ iter )
		{
			vgVector::VectorManager::getSingleton().open(*iter);
		}// for

		// ���¹���
		vgKernel::RendererManager::getSingleton().invalidate();

		vgVector::VectorManager::getSingleton().setVisibleLayers(true);

	}//if

	return;
}

void CvgFrameWorkView::OnVectorShpShow()
{
	// TODO: Add your command handler code here
	bool	bShowShapeLayers = vgVector::VectorManager::getSingleton().getVisibleSelectLayers();
	bShowShapeLayers = !bShowShapeLayers;
	vgVector::VectorManager::getSingleton().setVisibleLayers(bShowShapeLayers);
	return;

}

void CvgFrameWorkView::OnVectorShpExport2db()
{
	if (! DatabaseManager::getSingleton().IsOpen())
	{
		DBConnectDlg dialog;

		VGMSGBOX2(dialog.DoModal());

		if (! DatabaseManager::getSingleton().IsOpen())
		{
			return ;
		}
	}

	CDlgShpToDBEx dialog;

	VGMSGBOX2(dialog.DoModal());
}

void CvgFrameWorkView::OnVectorShpAttachTerrain()
{
	// TODO: Add your command handler code here
	vgGlobal::GlobalUtility::attachShapeLayerToTerrain(
		&vgKernel::SelectManager::getSingleton().getSelectedItems(), true );
}

void CvgFrameWorkView::OnVectorShpTranLayer()
{
	// TODO: Add your command handler code here
	CDlgTranslateVector dialog;

	VGMSGBOX2(dialog.DoModal());
}

void CvgFrameWorkView::OnVectorShpEditState()
{
	// TODO: Add your command handler code here
	m_bEditLayers = !m_bEditLayers;
	if(m_bEditLayers)
	{
		vgVector::VectorManager::getSingleton().switchLayersModeRenderWay(
			vgVector::vgRenderNormal );
	}
	else
	{
		vgVector::VectorManager::getSingleton().switchLayersModeRenderWay(
			vgVector::vgRenderDisplay );


	}
}

void CvgFrameWorkView::OnUpdateVectorShpShow(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	bool	bShowShapeLayers = vgVector::VectorManager::getSingleton().getVisibleSelectLayers();
	pCmdUI->SetCheck( bShowShapeLayers );
}

void CvgFrameWorkView::OnUpdateVectorShpEditState(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_bEditLayers );
}



void CvgFrameWorkView::gotoCamera(string cameraName)
{
	vgKernel::Renderer* pRenderer = vgKernel::RendererManager::getSingleton().getRendererbyName(cameraName);

	if (pRenderer != NULL)
	{	
		if (pRenderer->getType() !=  vgCam::RENDERER_TYPE_VIEWCAMERA)
		{
			MessageBox( "��ѡ�����!","VR-GIS",MB_OK );

			return;
		}

		vgCam::ViewCam *to,from;
		to=(vgCam::ViewCam *)pRenderer;

		from.cloneCurrentCameraView();
		to->setToUnvisible();

		vgCam::CamManager::getSingleton().setDefaultCamRecordPtr();
		vgCam::CamManager::getSingleton().getCurrentCameraRecord()->switchCamera(&from,to);

		vgKernel::SelectManager::getSingleton().clearSelection();
	}
}

void CvgFrameWorkView::playVCR(string vcrName)
{
	vgCam::CameraRecord* pVCR = vgCam::CamManager::getSingleton().getCameraRecordByName(vcrName);

	if (pVCR != NULL )
	{
		vgCam::CamManager::getSingleton().setCurrentCameraRecord(pVCR);

		vgCam::CamManager::getSingleton().getCurrentCameraRecord()->startPlaying();
	}
	else
	{
		AfxMessageBox(vcrName.c_str());
	}
}

void CvgFrameWorkView::stopVCR(string vcrName)
{
	vgCam::CameraRecord* pVCR = vgCam::CamManager::getSingleton().getCameraRecordByName(vcrName);

	if (pVCR != NULL )
	{
		vgCam::CamManager::getSingleton().setCurrentCameraRecord(pVCR);
		int nowframenum=vgCam::CamManager::getSingleton().getCurrentCameraRecord()->uiFrame;
		vgCam::CameraRecord* nowVCR=vgCam::CamManager::getSingleton().getCurrentCameraRecord();
		vgCam::CamManager::getSingleton().setPosByCurVcrSpecificFrame(nowframenum);

		vgCam::CamManager::getSingleton().getCurrentCameraRecord()->stopPlaying();
	}
}

void CvgFrameWorkView::continueVCR(string vcrName)
{
	vgCam::CameraRecord* pVCR = vgCam::CamManager::getSingleton().getCameraRecordByName(vcrName);

	if (pVCR != NULL )
	{
		vgCam::CamManager::getSingleton().setCurrentCameraRecord(pVCR);

		vgCam::CamManager::getSingleton().getCurrentCameraRecord()->continuePlaying();
	}
}

void CvgFrameWorkView::hideShowObject(string objName)
{
	vgKernel::Renderer* pRenderer = vgKernel::RendererManager::getSingleton().getRendererbyName(objName);

	if (pRenderer != NULL)
	{
		pRenderer->setVisible(! pRenderer->getVisible());
	}
}

void CvgFrameWorkView::hideShowObject(const vector<string>& objectNames)
{

	for (int i=0; i<objectNames.size(); i++)
	{
		vgKernel::Renderer* pRenderer = vgKernel::RendererManager::getSingleton().getRendererbyName(objectNames[i]);

		if (pRenderer != NULL)
		{
			pRenderer->setVisible(! pRenderer->getVisible());
		}
	}

}

void CvgFrameWorkView::OnMusicOnoff()
{
	// TODO: �ڴ���������������

	if(vgCore::SoundManager::getSingleton().getCurrentBackgroundMusic().getPlayFlag())
	{
		vgCore::SoundManager::getSingleton().getCurrentBackgroundMusic().stopPlaying();
	}
	else
		vgCore::SoundManager::getSingleton().getCurrentBackgroundMusic().startPlaying();

	// 	vgKernel::Renderer* pRenderer = vgKernel::RendererManager::getSingleton().getRendererbyName("������Ч");
	// 
	// 	if (pRenderer != NULL)
	// 	{
	// 		vgCore::Sound3D *sound = (vgCore::Sound3D *)pRenderer;
	// 		sound->SetPlay(! sound->getPlayFlag());
	// 	}
	// 	else
	// 	{
	// 		AfxMessageBox("Null Pointer");
	// 	}
}

void CvgFrameWorkView::OnUpdateMusicOnoff(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(
		vgCore::SoundManager::getSingleton().getCurrentBackgroundMusic().getPlayFlag()
		);
}
// Renfang Dingzhi add-------------------------------------------------------------
void CvgFrameWorkView::OnVgviBgConfig()
{
	vgVisualEffects::BackgroundManager::getSingleton().configBackground();
}

void CvgFrameWorkView::OnNdbDisconnect()
{
	// TODO: �ڴ���������������
	if (! vgDatabase::DatabaseManager::getSingleton().CloseDatabase())
		AfxMessageBox("���ݿ�δ���ӻ�ر�ʧ��.");
	else
		AfxMessageBox("�ѹر����ݿ�����.");
}

// �ű�			-------------------------------------------------------------
void CvgFrameWorkView::OnScriptRunfile()
{
	// TODO: Add your command handler code here

	vgScript::ScriptManager::getSingleton().createMultiSelectDailogAndRunScript();

}

void CvgFrameWorkView::OnScriptTestscript()
{
	//vgCam::CamFacade::gotoCameraByName( "ViewCam0" );
	AfxGetApp()->m_pMainWnd->SetWindowText( "���ǵķ��������������" );  
}

void CvgFrameWorkView::OnScriptTestmenu()
{

	//vgScript::ScriptManager::getSingleton().runScriptConsole();

	//return;
	//CWnd *wnd = AfxGetMainWnd();


	//CMainFrame* mainwnd = ( CMainFrame* )wnd;
	//mainwnd->loadScriptMenu( "d:\\TestFile.dat" , "fasdf" , 0 );

	//mainwnd->loadScriptMenu( "d:\\TestFile2.dat" , "��ʿ���df" , 0 );


	//std::wstring wstr = 
	//	vgKernel::StringUtility::convertStringToWString( "��������ˮ" );
	//VGK_SHOW( 
	//	vgKernel::StringUtility::convertWStringToString( wstr ) );

}

void CvgFrameWorkView::OnScriptConsole()
{
	vgScript::ScriptManager::getSingleton().runScriptConsole();

	return;

}

// �����EXE			-------------------------------------------------------------
void CvgFrameWorkView::OnPublishPackageExe()
{// TODO: �ڴ���������������
	
	//-----Step 1����ȡ����İ��ľ���·�� �� ʹ�ð��������½����������ʱĿ¼	------------------
	String	strPackageSrcPathTemp, strPackageDestiPath, 
		strPackageName,strPackageFileName;
	
	//��ȡ·��
	CString DefExt;
	DefExt.Format("%s","EXE�ļ�(*.EXE)|*.EXE|");

	CFileDialog dlgmodelFile(false,"EXE",NULL,
	OFN_HIDEREADONLY|OFN_CREATEPROMPT|OFN_NONETWORKBUTTON|OFN_OVERWRITEPROMPT,DefExt);

	dlgmodelFile.m_ofn.lpstrTitle="�����EXE";

	if(dlgmodelFile.DoModal()!=IDOK)
	{
		return;
	}
	
	strPackageDestiPath = dlgmodelFile.GetPathName();
	strPackageName = dlgmodelFile.GetFileTitle();
	strPackageFileName = dlgmodelFile.GetFileName();
	

	//�½�Ŀ¼
	strPackageSrcPathTemp = vgCore::QuadtreeSceneManager::getSingleton().getBinaryPath();
	strPackageSrcPathTemp += "\\" + strPackageName;
	CreateDirectory( strPackageSrcPathTemp.c_str(), 0 );
	
	//-----Step 2������	------------------------------------------------------------------------
	String strSrcFilePath = strPackageSrcPathTemp + "\\" + strPackageFileName;
	OnSceneSave(strSrcFilePath);

	//-----Step 3�����	------------------------------------------------------------------------
	vgGlobal::GlobalFacade::getSingleton().packageToEXE(strPackageSrcPathTemp
		,strPackageDestiPath);

	return;
}

// ���ó���			-------------------------------------------------------------
void CvgFrameWorkView::OnSceneReset()
{
	// TODO: �ڴ���������������
	
	//vgVector::VectorManager::getSingleton().reset();
	
	//vgCore::QuadtreeSceneManager::getSingleton().ResetScene();

	std::ostringstream o;

	o << "�Ƿ���и�λ����? ������ɾ����������.";

	int ret = VGK_WARN( vgKernel::Warning::WARN_INTERNAL_WARNING , 
		vgKernel::Warning::BUTTON_YES_NO, 
		o.str(),
		"CvgFrameWorkView::OnSceneReset");

	if ( ret == vgKernel::Warning::RETURN_NO )
	{
		return;
	}

	vgGlobal::SystemFacade::resetScene();

}

// ж��ʸ��ģ��
void CvgFrameWorkView::OnVectorShpUnload()
{
	// TODO: �ڴ���������������
	vgVector::VectorManager::getSingleton().reset();
}



// ����MODģ��Ϊ����
void CvgFrameWorkView::OnSetTerrain()
{
	// TODO: �ڴ���������������
	vgKernel::RendererQueue& selectedBuffer = vgKernel::SelectManager::getSingleton().getSelectedItems();
	if (selectedBuffer.empty())
	{
		AfxMessageBox("��ѡ����Ҫת��Ϊ���ε�ģ�ͣ�");
		vgKernel::SelectManager::getSingleton().clearSelection();
		return;
	}
	else if (selectedBuffer.size()>1)
	{
		AfxMessageBox("ÿ��ֻ����ѡ��һ��ģ�ͣ�");
		vgKernel::SelectManager::getSingleton().clearSelection();
		return;
	}

	vgCore::ObjectNode*	pMod = dynamic_cast<vgCore::ObjectNode*>(selectedBuffer[0]);
	
	if (pMod == NULL)
	{
		AfxMessageBox("ֻ����MODģ��ת��Ϊ���Σ�");
		vgKernel::SelectManager::getSingleton().clearSelection();
		return;
	}

	vgMod::ModTerrain*	pModTerrain = dynamic_cast<vgCore::ModTerrain*>(pMod);

	vgMod::ModTerrainManager::getSingleton().setModTerrain(pModTerrain);
	
}

void CvgFrameWorkView::OnUpdateSetTerrain(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	//pCmdUI->SetCheck(vgMod::ModTerrainManager::getSingleton().hasModTerrain());
}

void CvgFrameWorkView::OnLoadModterrain()
{
	// TODO: �ڴ���������������
	CString DefExt;

	DefExt.Format("%s","mod�ļ�(*.mod)|*.mod|");

	///CFileDialog��һ��������Ϊtrue��Ϊ���ļ��Ի���,�öλ���mfc
	CFileDialog dlgmodelFile(true,"mod",NULL,OFN_HIDEREADONLY|OFN_CREATEPROMPT|OFN_NONETWORKBUTTON,DefExt);

	dlgmodelFile.m_ofn.lpstrTitle="VG_MOD";

	if(dlgmodelFile.DoModal()==IDOK)
	{
		CString FilePathName = dlgmodelFile.GetPathName();
		vgMod::ModTerrainManager::getSingleton().loadModTerrain(String(FilePathName));
	}
}

//


void CvgFrameWorkView::OnModterrainReload()
{
	// TODO: �ڴ���������������
	vgMod::ModTerrainManager::getSingleton().updateModTerrain(false);
}

void CvgFrameWorkView::OnModterrainSave()
{
	// TODO: �ڴ���������������
	vgMod::ModTerrainManager::getSingleton().updateModTerrain(true);
}

void CvgFrameWorkView::OnUpdateModterrainLoad(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(vgMod::ModTerrainManager::getSingleton().hasModTerrain());
}

void CvgFrameWorkView::OnModterrainLeveling()
{
	// TODO: �ڴ���������������
	vgApp.appProc(VG_COMMAND_GIS, VG_COMMAND_GIS_MOD_TER_LEVELING, this);	
}

void CvgFrameWorkView::OnUpdateModterrainLeveling(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(vgMod::ModTerrainManager::getSingleton().hasModTerrain());
	pCmdUI->SetCheck(vgMod::ModTerrainManager::getSingleton().getCutSurfaceLevellingFlag());
}

void CvgFrameWorkView::OnUpdateModterrainReload(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(vgMod::ModTerrainManager::getSingleton().hasModTerrain());
}

void CvgFrameWorkView::OnUpdateModterrainSave(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(vgMod::ModTerrainManager::getSingleton().hasModTerrain());
}

void CvgFrameWorkView::OnVgscUserScriptConfig()
{
	vgScript::ScriptManager::getSingleton().runUserScriptConfig();
}

void CvgFrameWorkView::OnTestUnloadmod()
{
	// TODO: �ڴ���������������
	vgMod::ModTerrainManager::getSingleton().unloadModTerrain();
}

void CvgFrameWorkView::OnTanslateSelected()
{
	// TODO: �ڴ���������������
	CDlgTranslateAll	tranDlg;
	VGMSGBOX2( tranDlg.DoModal() );

	vgGlobal::SystemFacade::translateRenderers( 
		&vgKernel::SelectManager::getSingleton().getSelectedItems() , tranDlg.getTranslateVec() );

}

void CvgFrameWorkView::OnGisSun()
{
	//zhu ���շ����Ի���
	vgDlgSun *dlg=new vgDlgSun;
	dlg->Create(IDD_DLG_SUN,NULL);			
	dlg->SetWindowPos(AfxGetMainWnd(),400,300,NULL,NULL,SWP_NOSIZE);
	dlg->ShowWindow(SW_SHOW);
	//vgCore::MFCApplication::getSingleton().b_DrawShadow=true;//������Ӱ
}

void CvgFrameWorkView::OnExpAvi()
{
	// TODO: �ڴ���������������

	if (m_expDialog.DoModal() != IDOK)
	{
		return;
	}
	m_fileId = 0;

	m_aviFilePath = m_expDialog.getFilePath();
	m_aviCameraName = m_expDialog.getCameraName();

	m_frameBufferHeight = m_expDialog.getFrameHeight();
	m_frameBufferWidth  = m_expDialog.getFrameWidth();
	
	if (m_aviFilePath.GetLength() == 0)
	{
		AfxMessageBox("��·��");
	}

	m_frameBufferWidth = m_frameBufferWidth / 4 * 4;
	m_frameBufferHeight = m_frameBufferHeight / 4 * 4;

	vgCam::CameraRecord* record = vgCam::CamManager::getSingleton().getCameraRecordByName(m_aviCameraName);

	if (record == NULL)
	{
		AfxMessageBox("���������");
		return ;
	}

	float totalTime = record->m_fTotalTime;
	float fps = record->fps;
	m_fps = fps;

	m_totalFrame = record->framenum;

	if (paviFile != NULL)
	{
		delete paviFile;
		paviFile = NULL;
	}

	// ��ʹ��ѹ��������֡��Ϊfps�������VCR�еõ���һ��Ϊ32֡
	int compressId = m_expDialog.getCompressId();
	if (compressId == 1)
	{
		//paviFile = new vgCam::CAviFile(m_aviFilePath, mmioFOURCC('M','S','V','C'), fps);
	
		paviFile = new vgCam::CAviFile(m_aviFilePath, mmioFOURCC('M','S','V','C'), fps);
	}
	else if (compressId == 2)
	{
		paviFile = new vgCam::CAviFile(m_aviFilePath, mmioStringToFOURCC("CVID", 0), fps);
	}
	else if (compressId == 3)
	{
		paviFile = new vgCam::CAviFile(m_aviFilePath, mmioFOURCC('M','P','G','4'), fps);
	}
	else
	{
		paviFile = new vgCam::CAviFile(m_aviFilePath, 0, fps);
	}
	
	paviFile->SetQuality(m_expDialog.getQuality());

	vgCam::CamManager::getSingleton().setAviRecordCnt(m_totalFrame); // = vgCam::CamManager::getSingleton().getCameraRecordByName("Camera1")->framenum;

	vgCam::CamManager::getSingleton().setCurrentAviCameraName(m_aviCameraName);

	vgUIController::getSingleton().EnableProgressBar();
}

void CvgFrameWorkView::OnExpBmp()
{
	// TODO: �ڴ���������������
	BMPExportDialog bmpExpDialog;

	if ( bmpExpDialog.DoModal() != IDOK)
	{
		return ;
	}


	m_bmpHeight = bmpExpDialog.m_bmpHeight;
	m_bmpWidth = bmpExpDialog.m_bmpWidth;
	m_bmpFilePath = bmpExpDialog.m_bmpFilePath;
	
	if (m_bmpFilePath.length() == 0)
	{
		AfxMessageBox("��·��");

		return ;
	}
	
	m_bmpWidth = m_bmpWidth / 4 * 4;
	m_bmpHeight = m_bmpHeight / 4 * 4;

	vgCam::CamManager::getSingleton().setAviRecordCnt(-1);
}
void CvgFrameWorkView::OnVectorShpGreenRatio()
{
	// TODO: �ڴ���������������
	vgKernel::InputSystem::getSingleton().setCurrentStatus(
		VG_INPUT_VECTOR_ANALYSIS_GREEN);

	if( !vgShape::ShapeManager::getSingleton().setQueryRatioFlag(vgShape::GREEN_RATIO) )
	{
		vgKernel::InputSystem::getSingleton().setDefaultStatus();
	}

}

void CvgFrameWorkView::OnVgshapeReadshpfile()
{
	std::ostringstream o;

#if VGK_LANG_CHS
	o	<< "-----------------------------------------------------------\n" 
		<< "\t\t����ִ�� ����shp����:\n"
		<< "-----------------------------------------------------------\n" ;
#else
	o	<< "-----------------------------------------------------------\n" 
		<< "\t\timporting shp:\n"
		<< "-----------------------------------------------------------\n" ;
#endif
	VGK_SHOW( o.str() );

	CString DefExt;

	DefExt.Format("%s","shp(*.shp)|*.shp|");

	CFileDialog dlgmodelFile(true,"shp",
		NULL,OFN_HIDEREADONLY|
		OFN_CREATEPROMPT|
		OFN_NONETWORKBUTTON|
		OFN_ALLOWMULTISELECT|   
		OFN_FILEMUSTEXIST,
		DefExt);

	DWORD   MAXFILE = 0xFFFF;   
	dlgmodelFile.m_ofn.nMaxFile=MAXFILE; 

	char*  pc = new char[MAXFILE];   


	dlgmodelFile.m_ofn.lpstrFile = pc;   
	dlgmodelFile.m_ofn.lpstrFile[0] = NULL;   
	dlgmodelFile.m_ofn.lpstrTitle="shp";

	if( dlgmodelFile.DoModal() == IDOK )
	{
		POSITION posi;   
		vgKernel::StringVector strVec;

		posi = dlgmodelFile.GetStartPosition();   

		while( posi != NULL )
		{   
			strVec.push_back( dlgmodelFile.GetNextPathName(posi).GetBuffer(0));
		}   

		delete[] pc;

		//------------------------------------------
		// ��ʼ����
		//------------------------------------------

		using namespace vgShape;

		std::ostringstream o;
#if VGK_LANG_CHS
		o << "\n��ʼ����shp�ļ�... \n";
#else
		o << "\nbegin to import shp... \n";
#endif
		VGK_SHOW( o.str() );
		int nErrorFileCount = 0;

		vgKernel::StringVector::iterator iter = strVec.begin();
		vgKernel::StringVector::iterator iter_end = strVec.end();

		for ( ; iter != iter_end ; ++ iter )
		{
			vgShape::DataSourcePtr psource = 
				vgShape::ShapeManager::getSingleton().openShapeFileWithUpdateUI(
				*iter  );

			if( psource.isNull() == true )
			{
				nErrorFileCount++;
			}
		}

#if VGK_LANG_CHS
		std::ostringstream oEnd;
		oEnd << "...shp�ļ��������,�ɹ������ļ�" << strVec.size()- nErrorFileCount << "��,"
			<< nErrorFileCount << "������ʧ��\n";
#else
		std::ostringstream oEnd;
		oEnd << "...shp import done. sucess: " << strVec.size()- nErrorFileCount 
			<< ","
			<< nErrorFileCount << " failed\n";
#endif
		VGK_SHOW( oEnd.str() );

		vgKernel::RendererManager::getSingleton().invalidate();

	}
	else
	{
#if VGK_LANG_CHS
		std::ostringstream oEnd;
		oEnd<< "-----------------------------------------------------------\n" 
			<< "\t\t�˳�ִ�� ����shp����\n"
			<< "-----------------------------------------------------------\n" ;
#else
		std::ostringstream oEnd;
		oEnd<< "-----------------------------------------------------------\n" 
			<< "\t\tquit import shp\n"
			<< "-----------------------------------------------------------\n" ;

#endif
		VGK_SHOW( oEnd.str() );
		return;
	}

#if VGK_LANG_CHS
	std::ostringstream oEnd;
	oEnd<< "-----------------------------------------------------------\n" 
		<< "\t\t���ִ�� ����shp����\n"
		<< "-----------------------------------------------------------\n" ;
#else
	std::ostringstream oEnd;
	oEnd<< "-----------------------------------------------------------\n" 
		<< "\t\tdone.\n"
		<< "-----------------------------------------------------------\n" ;
#endif
	VGK_SHOW( oEnd.str() );
}

void CvgFrameWorkView::OnVgshapeSelectshparea()
{
	using namespace vgKernel;
	RendererQueue* queue = 
		&SelectManager::getSingleton().getSelectedItems();

	if ( queue->size() != 1 )
	{
#if VGK_LANG_CHS
		::MessageBox( 
			AfxGetApp()->m_pMainWnd->m_hWnd,
			"��ѡ��һ��shp����", "��ʾ" , MB_OK );
#else
		::MessageBox( 
			AfxGetApp()->m_pMainWnd->m_hWnd,
			"Please select a shp object", "Tips" , MB_OK );
#endif
		return ;
	}

	Renderer* ren = (*queue)[0];
	if ( ren->getType() !=  vgShape::RENDERER_TYPE_SHAPE_LAYER )
	{
#if VGK_LANG_CHS
		::MessageBox( 
			AfxGetApp()->m_pMainWnd->m_hWnd,
			"��ѡ��shp����", "��ʾ" , MB_OK );
#else
		::MessageBox( 
			AfxGetApp()->m_pMainWnd->m_hWnd,
			"Please select shp object", "Tips" , MB_OK );
#endif
		return ;
	}

	vgShape::ShapeManager::getSingleton().setSelectMode( (vgShape::Layer*)ren,
		!vgShape::ShapeManager::getSingleton().getSelectFlag() );
}

void CvgFrameWorkView::OnCamReheight()
{
	// TODO: �ڴ���������������
// 	if (vgKernel::SelectManager::getSingleton().getSelectedItems().size() == 0)
// 	{
// 		AfxMessageBox("û��ѡ������.");
// 	
// 		return ;
// 	}
// 
// 	vgKernel::Renderer *pCurrentRenderer = vgKernel::SelectManager::getSingleton().getSelectedItems().front();
// 
// 
// 	if (pCurrentRenderer->getType() != vgCam::RENDERER_TYPE_VCR)
// 	{
// 		AfxMessageBox("��ѡ��һ�����.");
// 		return ;
// 	}

	vgCam::CameraRecord *pCurrentVCR = vgCam::CamManager::getSingleton().getCurrentCameraRecord();

	if (pCurrentVCR == NULL)
	{
		 AfxMessageBox("��ѡ��һ�����.");
		 return ;
	}

	vgVcrHeightDlg dlg;

	this->SetTimer(1, 150, NULL);

	if (dlg.DoModal() != IDOK)
	{
		this->SetTimer(1, 15, NULL);
		return ;
	}
	this->SetTimer(1, 15, NULL);

	float y = dlg.getTransHeight();
	
	Vec3 transVec = pCurrentVCR->offset_vcr;
	pCurrentVCR->translate(transVec.x, transVec.y+y, transVec.z);
	
}

void CvgFrameWorkView::OnVgshapeSaveasshape()
{
	vgBaseEntry* entry = 
		vgUIController::GetInstance()->GetCurrentSelectedNode();

	if ( entry == NULL )
	{
		MessageBox( "��ѡ�е���shp����" );
		return;
	}

	ShapeDataSourceEntry* ds = dynamic_cast<ShapeDataSourceEntry*>
		( entry );

	if ( ds == NULL )
	{
		MessageBox( "��ѡ��shp����" );
		return;
	}

	vgShape::DataSourcePtr psource = 
		ds->getShapeDataSource();
	assert( psource.isNull() == false );

	//------------------------------------------
	// �����Ի���
	//------------------------------------------
	CString DefExt;

	DefExt.Format("%s","shp�ļ�(*.shp)|*.shp|");

	CFileDialog dlgmodelFile(false,"shp",NULL,OFN_HIDEREADONLY|OFN_CREATEPROMPT|OFN_NONETWORKBUTTON,DefExt);

	dlgmodelFile.m_ofn.lpstrTitle="����shp�ļ�";

	if(dlgmodelFile.DoModal()==IDOK)
	{
		String FilePathName = dlgmodelFile.GetPathName();

		bool saveok = psource->saveToFilePath( FilePathName );

		if ( saveok == false )
		{
			::MessageBox(NULL , "����ʧ��" , "Error" , MB_OK );
		}
	}

	return;
}
void CvgFrameWorkView::OnTestModOpacity()
{
	// TODO: �ڴ���������������

	vgKernel::RendererQueue& selectedBuffer = vgKernel::SelectManager::
		getSingleton().getSelectedItems();
	if (selectedBuffer.empty())
	{
		AfxMessageBox("��ѡ����Ҫ��͸����ģ�ͣ�");
		return;
	}

	vgKernel::RendererQueue::iterator iter = selectedBuffer.begin();
	vgKernel::RendererQueue::iterator end = selectedBuffer.end();
	for (; iter != end; iter++)
	{
		vgKernel::TransparentObject* pNode = 
			dynamic_cast<vgKernel::TransparentObject*>(*iter);
		
		if ( pNode == NULL )
		{
			continue;
		}
		assert( pNode != NULL );

		pNode->setTransparent( true );
	}


}



void CvgFrameWorkView::OnVgshapeTranslatetoorigin()
{
	vgBaseEntry* selectentry = 
		vgUIController::GetInstance()->GetCurrentSelectedNode();

	if ( selectentry == NULL )
	{
		::MessageBox(NULL , "��ѡ��layer��" , "��ʾ" , MB_OK );
		return;
	}

	ShapeLayerEntry* layerentry = dynamic_cast<ShapeLayerEntry*>( selectentry );

	if ( layerentry == NULL )
	{
		::MessageBox(NULL , "ѡ�����,��ѡ��layer��" , "��ʾ" , MB_OK );
		return;
	}

	vgShape::Layer* layer = layerentry->getLayerPtr();

	//------------------------------------------
	// �õ�ԭ������
	//------------------------------------------

	vgTerritory::FileHandleVtr* vtrhandle =
		vgTerritory::ElevationManager::getSingleton().getMainFileHandleVtr();


	if ( vtrhandle == NULL )
	{
		::MessageBox(NULL , "�뵼�����" , "��ʾ" , MB_OK );
		return;
	}

	assert( vtrhandle != NULL );

	vgTerritory::VtrHeaderPtr pheader = vtrhandle->getHeader();

	assert( pheader.isNull() == false );
	
	//------------------------------------------
	// ��ʼλ��
	//------------------------------------------

	layer->translate( -pheader->originX , 
		pheader->originY , 
		pheader->originZ );
	
	return;
}

void CvgFrameWorkView::OnVgshapeAttachtoterrain()
{
	vgBaseEntry* selectentry = 
		vgUIController::GetInstance()->GetCurrentSelectedNode();

	if ( selectentry == NULL )
	{
		::MessageBox(NULL , "��ѡ��layer��" , "��ʾ" , MB_OK );
		return;
	}

	ShapeLayerEntry* layerentry = dynamic_cast<ShapeLayerEntry*>( selectentry );

	if ( layerentry == NULL )
	{
		::MessageBox(NULL , "ѡ�����,��ѡ��layer��" , "��ʾ" , MB_OK );
		return;
	}

	vgShape::Layer* layer = layerentry->getLayerPtr();

	//------------------------------------------
	// �õ�ԭ������
	//------------------------------------------

	bool attachok = layer->attachGeometriesToTerrain();

	if ( attachok == false )
	{
		::MessageBox(NULL , "ƴ��ʱ���ִ���" , "��ʾ" , MB_OK );
		return;
	}

	return;
}

void CvgFrameWorkView::OnVgshapeSetrendererboxasfilter()
{
	bool em = vgKernel::SelectManager::getSingleton().getSelectedItems().empty();

	if ( em == true )
	{
		::MessageBox(NULL , "��ѡ������" , "��ʾ" , MB_OK );
		return;
	}

	vgKernel::Box box = 
		vgKernel::SelectManager::getSingleton().getSelectedBox();

	//------------------------------------------
	// ע��Ҫȡ��!!
	//------------------------------------------
	vgShape::ShapeManager::getSingleton().setShpLoadingFilter(
		box._minVertex.x , 
		-box._maxVertex.z ,
		box._maxVertex.x , 
		-box._minVertex.z , 
		0.1 );

}

void CvgFrameWorkView::OnAutobuild()
{
	// TODO: �ڴ���������������
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_AUTOBUILD_CONVERT, this);
} 


void CvgFrameWorkView::OnUpdateVectorShpGreenRatio(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck( vgShape::GREEN_RATIO ==
		vgShape::ShapeManager::getSingleton().getQueryType());
}

void CvgFrameWorkView::OnVgshapeReadgridshpfile()
{
	std::ostringstream o;
	o	<< "-----------------------------------------------------------\n" 
		<< "\t\t����ִ�� ����shp����:\n"
		<< "-----------------------------------------------------------\n" ;
	VGK_SHOW( o.str() );

	CString DefExt;

	DefExt.Format("%s","shp�ļ�(*.shp)|*.shp|");

	CFileDialog dlgmodelFile(true,"shp",
		NULL,OFN_HIDEREADONLY|
		OFN_CREATEPROMPT|
		OFN_NONETWORKBUTTON|
		OFN_ALLOWMULTISELECT|   
		OFN_FILEMUSTEXIST,
		DefExt);

	DWORD   MAXFILE = 0xFFFF;   
	dlgmodelFile.m_ofn.nMaxFile=MAXFILE; 

	char*  pc = new char[MAXFILE];   


	dlgmodelFile.m_ofn.lpstrFile = pc;   
	dlgmodelFile.m_ofn.lpstrFile[0] = NULL;   
	dlgmodelFile.m_ofn.lpstrTitle="shp�ļ�����";

	if( dlgmodelFile.DoModal() == IDOK )
	{
		POSITION posi;   
		vgKernel::StringVector strVec;

		posi = dlgmodelFile.GetStartPosition();   

		while( posi != NULL )
		{   
			strVec.push_back( dlgmodelFile.GetNextPathName(posi).GetBuffer(0));
		}   

		delete[] pc;

		//------------------------------------------
		// ��ʼ����
		//------------------------------------------

		using namespace vgShape;

		std::ostringstream o;
		o << "\n��ʼ����shp�ļ�... \n";
		VGK_SHOW( o.str() );
		int nErrorFileCount = 0;

		vgKernel::StringVector::iterator iter = strVec.begin();
		vgKernel::StringVector::iterator iter_end = strVec.end();

		for ( ; iter != iter_end ; ++ iter )
		{
			vgShape::DataSourcePtr psource = 
				vgShape::ShapeManager::getSingleton().openShapeFileSpeciallyWithUpdateUI( *iter  );

			if( psource.isNull() == true )
			{
				nErrorFileCount++;
				continue;
			}
		}

		std::ostringstream oEnd;
		oEnd << "...shp�ļ��������,�ɹ������ļ�" << strVec.size()- nErrorFileCount << "��,"
			<< nErrorFileCount << "������ʧ��\n";
		VGK_SHOW( oEnd.str() );

		vgKernel::RendererManager::getSingleton().invalidate();
	}
	else
	{
		std::ostringstream oEnd;
		oEnd<< "-----------------------------------------------------------\n" 
			<< "\t\t�˳�ִ�� ����shp����\n"
			<< "-----------------------------------------------------------\n" ;
		VGK_SHOW( oEnd.str() );
		return;
	}

	std::ostringstream oEnd;
	oEnd<< "-----------------------------------------------------------\n" 
		<< "\t\t���ִ�� ����shp����\n"
		<< "-----------------------------------------------------------\n" ;
	VGK_SHOW( oEnd.str() );
}

void CvgFrameWorkView::OnVgtestSelectMode()
{
	// TODO: �ڴ���������������
	vgVector::VectorManager::getSingleton().toggleSelectMode();
}

void CvgFrameWorkView::OnUpdateVgtestSelectMode(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	String	strSelectMode = 
	vgVector::VectorManager::getSingleton().getSelectModeString();;
	
	pCmdUI->SetText(strSelectMode.c_str());
}

void CvgFrameWorkView::OnUpdateVgshapeSelectshparea(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( 
		vgShape::ShapeManager::getSingleton().getSelectFlag()
		);
}

void CvgFrameWorkView::OnAutobuildClone()
{
	// TODO: �ڴ���������������
	if (vgKernel::SelectManager::getSingleton().getSelectedItems().size() == 0)
	{
		return ;
	}

	vgKernel::Renderer *pRenderer = vgKernel::SelectManager::getSingleton().getSelectedItems().front();
	
	if (pRenderer->getType() == vgVector::RENDERER_TYPE_AUTOBUILD)
	{
		vgVector::AutoBuildNode *pNewNode = new vgVector::AutoBuildNode;

		vgVector::AutoBuildNode *pSrcNode = dynamic_cast<vgVector::AutoBuildNode*>(pRenderer);
		
		if (pSrcNode == NULL)
		{
			return ;
		}

		pNewNode->cloneFromAutobuildNode(pSrcNode);
		
		vgVector::AutoBuildManager::getSingleton().addAutoBuildNode(pNewNode);
	}

	vgKernel::SelectManager::getSingleton().clearSelection();
}

void CvgFrameWorkView::OnEditSingleFaceRender()
{
	// TODO: �ڴ���������������

	/*vgKernel::RendererQueue& selectedBuffer = vgKernel::SelectManager::
		getSingleton().getSelectedItems();
	if (selectedBuffer.empty())
	{
		AfxMessageBox("��ѡ������Ⱦ��MODģ�ͣ�");
		vgKernel::SelectManager::getSingleton().clearSelection();
		return;
	}

	m_bDoubleFaceRenderSelected = !m_bDoubleFaceRenderSelected;


	vgKernel::RendererQueue::iterator iter = selectedBuffer.begin();
	vgKernel::RendererQueue::iterator end = selectedBuffer.end();
	for (; iter != end; iter++)
	{
		vgCore::ObjFrameNode* pNode = dynamic_cast<vgCore::ObjFrameNode*>(*iter);

		if ( pNode == NULL )
		{
			continue;
		}
		assert( pNode != NULL );

		pNode->setRenderDoubleSidesState(m_bDoubleFaceRenderSelected);
	}*/

}

void CvgFrameWorkView::OnEditSingleFaceRenderSel()
{
	// TODO: �ڴ���������������

	vgKernel::RendererQueue& selectedBuffer = vgKernel::SelectManager::
		getSingleton().getSelectedItems();
	if (selectedBuffer.empty())
	{
		AfxMessageBox("��ѡ������Ⱦ��MODģ�ͣ�");
		vgKernel::SelectManager::getSingleton().clearSelection();
		return;
	}

	m_bDoubleFaceRenderSelected = !m_bDoubleFaceRenderSelected;


	vgKernel::RendererQueue::iterator iter = selectedBuffer.begin();
	vgKernel::RendererQueue::iterator end = selectedBuffer.end();
	for (; iter != end; iter++)
	{
		vgCore::ObjFrameNode* pNode = dynamic_cast<vgCore::ObjFrameNode*>(*iter);

		if ( pNode == NULL )
		{
			continue;
		}
		assert( pNode != NULL );

		pNode->setRenderDoubleSidesState(m_bDoubleFaceRenderSelected);
	}

}

void CvgFrameWorkView::OnEditSingleFaceRenderAll()
{
	// TODO: �ڴ���������������
	vgKernel::RendererQueue*	selectedBuffer = vgKernel::RendererManager::
		getSingleton().getRendererQueueByType(vgCore::RENDERER_TYPE_MODOJBECT,false);

	if ( !selectedBuffer || selectedBuffer->empty())
	{
		//AfxMessageBox("��ѡ����Ҫ��͸����MODģ�ͣ�");
		//vgKernel::SelectManager::getSingleton().clearSelection();
		return;
	}

	m_bDoubleFaceRenderAll = !m_bDoubleFaceRenderAll;

	vgKernel::RendererQueue::iterator iter = selectedBuffer->begin();
	vgKernel::RendererQueue::iterator end = selectedBuffer->end();
	for (; iter != end; iter++)
	{
		vgCore::ObjFrameNode* pNode = dynamic_cast<vgCore::ObjFrameNode*>(*iter);

		if ( pNode == NULL )
		{
			continue;
		}
		assert( pNode != NULL );

		pNode->setRenderDoubleSidesState(m_bDoubleFaceRenderAll);
	}

}

void CvgFrameWorkView::OnUpdateEditSingleFaceRenderSel(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(m_bDoubleFaceRenderSelected);
}

void CvgFrameWorkView::OnUpdateEditSingleFaceRenderAll(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(m_bDoubleFaceRenderAll);
}

void CvgFrameWorkView::OnTriggerSetAs()
{
	// TODO: �ڴ���������������
	vgKernel::RendererQueue& selectedBuffer = vgKernel::SelectManager::
		getSingleton().getSelectedItems();
	if (selectedBuffer.empty())
	{
		AfxMessageBox("��ѡ������Ⱦ��MODģ�ͣ�");
		vgKernel::SelectManager::getSingleton().clearSelection();
		return;
	}

	//m_bSetAsTrigger = !m_bSetAsTrigger;


	vgKernel::RendererQueue::iterator iter = selectedBuffer.begin();
	vgKernel::RendererQueue::iterator end = selectedBuffer.end();
	for (; iter != end; iter++)
	{
		vgMod::TriggerManager::getSingleton().addTrigger(*iter);
		/*if (m_bSetAsTrigger)
		{
			vgMod::TriggerManager::getSingleton().addTrigger(*iter);
		}
		else
		{
			vgMod::TriggerManager::getSingleton().deleteTrigger(*iter);
		}*/
	}
}

void CvgFrameWorkView::OnVgshapeImporttext()
{
	// TODO: �ڴ���������������
// 
// 	if (vgVector::AutoBuildManager::getSingleton().saveInfoToShpFile())
// 		AfxMessageBox("Success");
	vgVector::AutoBuildManager::getSingleton().importShpInfo();
}

void CvgFrameWorkView::OnVgshapeExportshapeinfo()
{
	// TODO: �ڴ���������������

	vgShape::Layer *pLayer = vgShape::ShapeManager::getSingleton().getLayerByShortName("Grid");

	if (pLayer == NULL)
	{
		AfxMessageBox("�����ڵ�Layer");
	}

	vgShape::GeometryPointerVec* geomVec = pLayer->getGeometries();

	std::stringstream outString;

	outString << "����Grid "<< (*geomVec).size()<<"\n";
	VGK_SHOW(outString.str());

	outString.str("");

	for ( int i=0; i<(*geomVec).size(); i++ )
	// for ( int i=204; i<205; i++ )
	
	{
		int pointCnt = (*geomVec)[i]->getNumOfPoints();
		vgShape::Geometry *pPoly = (*geomVec)[i];

		vgShape::SpecialPolygon *pSpoly = dynamic_cast<vgShape::SpecialPolygon*>(pPoly);

		if (pSpoly == NULL)
		{
			VGK_SHOW("ת��ʧ��!\n");		
			continue ;
		}
		else
		{
			pSpoly->constructInfoShp();
		}

		outString.str("");
		outString << "�Ѿ��������"<< i << "/" << (*geomVec).size() << "\n";
		VGK_SHOW(outString.str());
	}

	vgVector::AutoBuildManager::getSingleton().saveInfoToShpFile();

	VGK_SHOW("����IntersectionVec�ɹ�\n");
}

void CvgFrameWorkView::OnVgshapeCaculateposition()
{
	// TODO: �ڴ���������������
	int schem, year;
	DlgAutoIn dlg;

	this->SetTimer(1, 150, NULL);
		
	if (dlg.DoModal() == IDOK)
	{
 		schem = dlg.getSchemId();
 		year = dlg.getYear();
	}
	vgVector::AutoBuildManager::getSingleton().generateBuildingAll(schem, year);

	this->SetTimer(1, 15, NULL);
}

void CvgFrameWorkView::OnVgshapeClearplan()
{
	// TODO: �ڴ���������������
	vgVector::AutoBuildManager::getSingleton().clearPlan();
}

void CvgFrameWorkView::OnVgshapeGenerateselected()
{
	// TODO: �ڴ���������������
	int schem, year;
	DlgAutoIn dlg;

	this->SetTimer(1, 150, NULL);

	if (dlg.DoModal() == IDOK)
	{
 		schem = dlg.getSchemId();
 		year = dlg.getYear();

		vgVector::AutoBuildManager::getSingleton().generateBuildingSelected(schem, year);
	}

	this->SetTimer(1, 15, NULL);
}

void CvgFrameWorkView::OnVgshapeImportinfofromtxt()
{
	// TODO: �ڴ���������������
	vgVector::AutoBuildManager::getSingleton().OnReadTxt();
}

void CvgFrameWorkView::OnAutobuildingCloseinfofile()
{
	// TODO: �ڴ���������������
	vgVector::AutoBuildManager::getSingleton().removeAllShpInfo();

}

void CvgFrameWorkView::OnTriggerConfig()
{
	// TODO: �ڴ���������������
	CDlgTriggerConfig	dlg;
	VGMSGBOX2(dlg.DoModal());
}

void CvgFrameWorkView::OnQueryplaninfoQueryplotratio()
{
	// TODO: �ڴ���������������
	vgKernel::InputSystem::getSingleton().setCurrentStatus(
		VG_INPUT_VECTOR_ANALYSIS_GREEN);

	if( !vgShape::ShapeManager::getSingleton().setQueryRatioFlag(vgShape::PLOT_RATIO) )
	{
		vgKernel::InputSystem::getSingleton().setDefaultStatus();
	}
}

void CvgFrameWorkView::OnUpdateQueryplaninfoQueryplotratio(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck( vgShape::PLOT_RATIO ==
		vgShape::ShapeManager::getSingleton().getQueryType());
}

void CvgFrameWorkView::OnVgshapeTranslateall()
{
	// TODO: �ڴ���������������
	CDlgTranslateVector dialog;

	VGMSGBOX2(dialog.DoModal());
}

void CvgFrameWorkView::OnVgplanAddground()
{
	// TODO: �ڴ���������������
	vgPlan::vgPlanScene *pScene = vgPlan::vgPlanManager::getSingleton().getActiveScene();

	if (pScene != NULL)
	{
		if ( pScene->addGroundFromSelectedShp() )
		{
			AfxMessageBox("���Ground�ɹ�.");
		}
		else
		{
			AfxMessageBox("δ�����Ground.");
		}
	}
}

void CvgFrameWorkView::OnVgplanAddsecne()
{
	// TODO: �ڴ���������������
	vgPlan::vgPlanManager::getSingleton().addNewScene();
}

void CvgFrameWorkView::OnDyntextureConvert()
{
	// TODO: �ڴ���������������
	vgApp.appProc(VG_COMMAND_SE, VG_COMMAND_SE_DYNAMICTEX_CONVERT, this);
	SetCurrentDirectoryA(vgApp.scenetree->pathbuffer);

}


void CvgFrameWorkView::OnVgshadowTurnonshadow()
{
	using namespace vgSolar;
	// TODO: Add your command handler code here
	// ���Ѿ�������ʱ�����ʱ����ж�ء�
	if ( SolarManager::getSingleton().getShadowEnabled() )
	{
		SolarManager::getSingleton().turnOffShadowEffect();
	}

	vgKernel::RendererQueue* qu = 
		&vgKernel::SelectManager::getSingleton().getSelectedItems();

	//vgKernel::Vec3 dir( -1 , -1 , 2 );
	//dir.normalise();


	SolarManager::getSingleton().turnOnShadowEffect( qu , 
		NULL , true );
}

void CvgFrameWorkView::OnVgshadowSelectshadowcastobjects()
{
	using namespace vgSolar;
	vgKernel::ShadowCastObjectVec* vec = 
		SolarManager::getSingleton().getShadowCastObjects();

	if ( vec == NULL )
	{
		MessageBox( "û��ͶӰ������" );
		return;
	}

	vgKernel::SelectManager::getSingleton().clearSelection();

	vgKernel::ShadowCastObjectVec::iterator iter = vec->begin();
	vgKernel::ShadowCastObjectVec::iterator iter_end = vec->end();

	for( ; iter != iter_end ; ++ iter )
	{
		vgKernel::SelectManager::getSingleton().addSelection(*iter , false );
	}

	vgKernel::SelectManager::getSingleton().updateBox();

}

void CvgFrameWorkView::OnVgplanSetparameter()
{
	// TODO: Add your command handler code here
}

void CvgFrameWorkView::OnVgshadowChange()
{
	using namespace vgSolar;


	this->SetTimer(1, 150, NULL);
	


	CSolarConfig dlg(
		&SolarManager::getSingleton().getCurrentDateAndTime() );

	if ( dlg.DoModal() == IDOK )
	{
		bool changeok = 
			SolarManager::getSingleton().changeDateTimeAndLoc(&dlg.getTimeAndLoc());

		if ( changeok == false )
		{
			::MessageBox( NULL , "̫����û�г���" , "��ʾ" ,  MB_OK );
		}
	}

	this->SetTimer(1, 15, NULL);
}

void CvgFrameWorkView::OnVgshadowTurnoffshadow()
{
	using namespace vgSolar;
	SolarManager::getSingleton().turnOffShadowEffect();
}

void CvgFrameWorkView::OnVgshadowSolaranalysis()
{
	using namespace vgKernel;
	using namespace vgSolar;

	this->SetTimer(1, 150, NULL);

	if ( SelectManager::getSingleton().getSelectedItems().empty() )
	{
		vgKernel::ShadowCastObjectVec* vec = 
			SolarManager::getSingleton().getShadowCastObjects();

		if ( vec == NULL )
		{
			MessageBox( "û��ͶӰ������" );
			return;
		}

		vgKernel::SelectManager::getSingleton().clearSelection();

		vgKernel::ShadowCastObjectVec::iterator iter = vec->begin();
		vgKernel::ShadowCastObjectVec::iterator iter_end = vec->end();

		for( ; iter != iter_end ; ++ iter )
		{
			vgKernel::SelectManager::getSingleton().addSelection(*iter , false );
		}

		vgKernel::SelectManager::getSingleton().updateBox();
	}

	if ( SelectManager::getSingleton().getSelectedItems().empty() )
	{
		MessageBox( "��ѡ������" );
		this->SetTimer(1, 15, NULL);
		return;
	}

	// TODO: Add your command handler code here
	vgSolar::SolarManager::getSingleton().setSolarAnalysis(
		!SolarManager::getSingleton().getSolarAnalysisEnable() );

	this->SetTimer(1, 15, NULL);
}

void CvgFrameWorkView::OnUpdateVgshadowSolaranalysis(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( 
		vgSolar::SolarManager::getSingleton().getSolarAnalysisEnable() );
}

void CvgFrameWorkView::OnUpdateVgshadowTurnonshadow(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(
		vgSolar::SolarManager::getSingleton().getShadowEnabled() );
}

void CvgFrameWorkView::OnTestModUnopacity()
{
	vgKernel::RendererQueue& selectedBuffer = vgKernel::SelectManager::
		getSingleton().getSelectedItems();
	if (selectedBuffer.empty())
	{
		AfxMessageBox("��ѡ����Ҫ��͸����ģ�ͣ�");
		return;
	}

	vgKernel::RendererQueue::iterator iter = selectedBuffer.begin();
	vgKernel::RendererQueue::iterator end = selectedBuffer.end();
	for (; iter != end; iter++)
	{
		vgKernel::TransparentObject* pNode = 
			dynamic_cast<vgKernel::TransparentObject*>(*iter);

		if ( pNode == NULL )
		{
			continue;
		}
		assert( pNode != NULL );

		pNode->setTransparent( false );
	}
}

void CvgFrameWorkView::OnTesttestSelectchangecolor()
{
	vgKernel::RendererQueue& selectedBuffer = vgKernel::SelectManager::
		getSingleton().getSelectedItems();
	if (selectedBuffer.empty())
	{
		AfxMessageBox("��ѡ����Ҫ��͸����ģ�ͣ�");
		return;
	}

	vgKernel::RendererQueue::iterator iter = selectedBuffer.begin();
	vgKernel::RendererQueue::iterator end = selectedBuffer.end();
	for (; iter != end; iter++)
	{
		vgKernel::ColorableObject* pNode = 
			dynamic_cast<vgKernel::ColorableObject*>(*iter);

		if ( pNode == NULL )
		{
			continue;
		}
		assert( pNode != NULL );

		pNode->setColorableValue( vgKernel::RgbaQuad( 255, 0 , 0 , 128) );
	}
}

void CvgFrameWorkView::OnVgplanAddstreetline()
{
	// TODO: �ڴ���������������
	vgPlan::vgPlanScene *pScene = vgPlan::vgPlanManager::getSingleton().getActiveScene();
	
	if (pScene != NULL)
	{
		if ( pScene->addStreetFromSelectedShp() )
		{
			AfxMessageBox("���Street�ɹ�.");
		}
		else
		{
			AfxMessageBox("δ�����Street.");
		}
	}
	else
	{
		AfxMessageBox("������δ��ӹ滮����.");
	}
}

void CvgFrameWorkView::OnPlanExportscene()
{
	// TODO: �ڴ���������������
	vgPlan::vgPlanManager::getSingleton().exportAllSceneToShpFile("");
}

void CvgFrameWorkView::OnVgplanGenerateplanmap()
{
	// TODO: �ڴ���������������
	vgPlan::vgPlanManager::getSingleton().generateMap();
}

void CvgFrameWorkView::OnVgplanAddregion()
{
	// TODO: �ڴ���������������
	vgPlan::vgPlanManager::getSingleton().getActiveScene()->addExclusiveAreaFromSelectedShp();
}

void CvgFrameWorkView::OnVgplanAddnew()
{
	// TODO: �ڴ���������������
	DlgPlanShpIn dlg;

	this->SetTimer(1, 150, NULL);

	if ( dlg.DoModal() != IDOK )
	{
		this->SetTimer(1, 15, NULL);

		return ;
	}
	this->SetTimer(1, 15, NULL);

	vgPlan::vgPlanManager::getSingleton().addNewScene(dlg.getPlanName());
	
	if (dlg.getAreaFilePath().length() > 0)
	{
		// vgShape::ShapeManager::getSingleton().openShapeFileWithUpdateUI(dlg.getAreaFilePath());

		//if ( !vgPlan::vgPlanManager::getSingleton().getActiveScene()->addGroundFromLayer(dlg.getAreaFileName()) )
		if (!vgPlan::vgPlanManager::getSingleton().getActiveScene()->addGroundFromLayer(dlg.getAreaShpName()))	
		{	
			AfxMessageBox("δ�ܴ���Ground.");
			return ;
		}
	}

	if (dlg.getRoadFilePath().length() > 0)
	{
		// vgShape::ShapeManager::getSingleton().openShapeFileWithUpdateUI(dlg.getRoadFilePath());
	
		// if ( !vgPlan::vgPlanManager::getSingleton().getActiveScene()->addStreetFromLayer(dlg.getRoadFileName()) )
		if ( !vgPlan::vgPlanManager::getSingleton().getActiveScene()->addStreetFromLayer(dlg.getRoadShpName(), dlg.getRoadWidth()) )
		{
			stringstream outString;
			outString << "δ�ܴ���Street - " << dlg.getRoadFileName(); 
			AfxMessageBox(outString.str().c_str());
			return ;
		}
	}

	if (dlg.getExlcFilePath().length() > 0)
	{
		// vgShape::ShapeManager::getSingleton().openShapeFileWithUpdateUI(dlg.getExlcFilePath());

		//if ( !vgPlan::vgPlanManager::getSingleton().getActiveScene()->addExculsiveAreaFromLayer( dlg.getExlcFileName()) )
		if ( !vgPlan::vgPlanManager::getSingleton().getActiveScene()->addExculsiveAreaFromLayer( dlg.getExlcShpName()) )
		{
			AfxMessageBox("δ�ܴ���Exclusive Area.");

			return ;
		}

	} 

	if (dlg.getRegionFilePath().length() > 0)
	{
		// vgShape::ShapeManager::getSingleton().openShapeFileWithUpdateUI(dlg.getRegionFilePath());

		//if ( !vgPlan::vgPlanManager::getSingleton().getActiveScene()->addBuildingAreaFromLayer( dlg.getRegionFileName()) )
		if ( !vgPlan::vgPlanManager::getSingleton().getActiveScene()->addBuildingAreaFromLayer( dlg.getRegionShpName()) )
		{
			AfxMessageBox("δ�ܴ���Region Area. ");
		
			return ;
		}
	}
	AfxMessageBox("�����ɹ�.", MB_OK);

	vgKernel::RendererManager::getSingleton().invalidate();
}

void CvgFrameWorkView::OnVgplanCgrid()
{
	// TODO: �ڴ���������������
	DlgPlanParam dlg;

	this->SetTimer(1, 150, NULL);
	
 	dlg.DoModal();

	this->SetTimer(1, 15, NULL);
}

void CvgFrameWorkView::OnVgplanCalgrid()
{
// 	// TODO: �ڴ���������������
// 	// for (int i=0; i< 100; i++)
// 	{
//  		//if (i % 10 == 0)
//  		//{
//  		//	AfxMessageBox("10-");
//  		//}
// 		vgPlan::vgPlanManager::getSingleton().getLastScenePtr()->updateGrid();
// 	}

	vgGIS3D::vgSunAngle sunangle;
	sunangle.SetDate(2009,12,22);
	sunangle.SetTime(11,0);
	sunangle.SetLat(40);
	float aa=sunangle.GetHeightOfSun();

	float height = (20-0.9)/tan(aa);
	
	float angle = aa * 180 / 3.1415692;

	stringstream outString;

	outString << "̫���߶Ƚ�" << aa <<  ", " << angle << " ǰ����" << height << "\n";

	VGK_SHOW(outString.str());
}

void CvgFrameWorkView::OnVgplanGenplan()
{
	// TODO: �ڴ���������������
	DlgPlanBlock dlg;

	vgGIS3D::vgSunAngle sunangle;

	this->SetTimer(1, 150, NULL);

	if (dlg.DoModal() == IDOK)
	{
		vgPlan::BlockParam blockParam = dlg.getBlockParam();

		// lss add 2009��5��13��10:44:50
		if (blockParam.blockX == 0	||
			blockParam.blockY == 0	||
			blockParam.floorAreaRatio == 0	
			)
		{
			AfxMessageBox("���ĳ�����ݻ��ʲ�����Ϊ�㣬����������!");
			this->SetTimer(1, 15, NULL);
			return ;
		}

		if (blockParam.maxHeight < blockParam.minHeight)
		{
			AfxMessageBox("��߸߶Ȳ�ӦС����͸߶ȣ�����������!");
			this->SetTimer(1, 15, NULL);
			return ;
		}

		vgPlan::SunlightParam sunlightParam = dlg.getSunlightParam();
	
		vgPlan::vgPlanManager::getSingleton().getActiveScene()->generatePlan(blockParam, sunlightParam);
	}

	this->SetTimer(1, 15, NULL);
}

void CvgFrameWorkView::OnVgplanOutput()
{
	// TODO: �ڴ���������������
	vgPlan::vgPlanScene *pScene = vgPlan::vgPlanManager::getSingleton().getActiveScene();

	if (pScene != NULL)
	{
		DlgPlanOutput dlg;

		// 	vgGIS3D::vgSunAngle sunangle;
		// 	sunangle.SetDate(2009,12,22);
		// 	sunangle.SetTime(11,0);
		// 	sunangle.SetLat(40);
		// 	float aa=sunangle.GetHeightOfSun();
		// 
		// 	float height = (30-0.9)/tan(aa);
		// 
		// 	float angle = aa * 180 / 3.1415692;
		// 
		// 	stringstream outString;
		// 
		// 	outString << "̫���߶Ƚ�" << aa <<  ", " << angle << " ǰ����" << height << "\n";
		// 
		// 	VGK_SHOW(outString.str());
		// 
		// 
		// 	sunangle.SetDate(2009,12,22);
		// 	sunangle.SetTime(12,00);
		// 	sunangle.SetLat(32);
		// 
		// 	 aa=sunangle.GetHeightOfSun();
		// 	 height = (43-0.9)/tan(aa);
		// 
		// 	 angle = aa * 180 / 3.1415692;
		// 
		// 	 outString.str();
		// 	 outString << "̫���߶Ƚ�" << aa <<  ", " << angle << " ǰ����" << height << "\n";
		// 	 VGK_SHOW(outString.str());

		// 	dlg.Create(IDD_PLAN_OUTPUT, NULL);	///	����
		// 
		// 	dlg.ShowWindow(SW_SHOW);			///	��ʾ
		this->SetTimer(1, 150, NULL);

		dlg.DoModal();

		this->SetTimer(1, 15, NULL);
	}
	else
	{
		AfxMessageBox("û�з�������ʾ�����½�����!");
		this->SetTimer(1, 15, NULL);
		return ;
	}
}
