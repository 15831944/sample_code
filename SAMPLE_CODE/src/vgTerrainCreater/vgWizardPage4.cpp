// G:\#NewWorkingFolders\vrgis_2.0_terrain\RealityFramework_BCG8\src\vgTerrainCreater\vgWizardPage4.cpp : implementation file
//

#include <vgStableHeaders.h>
#include "vgWizardPage3.h"
#include "vgWizardPage4.h"
#include "vgWizardPropertySheet.h"
#include <vgTerritory/vgTerrainHugeImageProcessor.h>
#include <vgImage/vgiFileWriterDdsToVgi.h>
#include <vgKernel/vgkSystemUtility.h>
#include <vgTerritory/vgtFileWriterVgtrToVtr.h>

// WizardPage4 dialog

IMPLEMENT_DYNAMIC(WizardPage4, CPropertyPage)

WizardPage4::WizardPage4()
	: CPropertyPage(WizardPage4::IDD)
	, _processMsg(_T(""))
	, _showMsgResize(_T("δ���"))
	, _showMsgCut(_T("δ���"))
	, _showMsgVgtr(_T("δ���"))
	, _showMsgClean(_T("δ���"))
{
	m_psp.dwFlags |= PSP_DEFAULT|PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;

	m_psp.pszHeaderTitle = _T("��3��");
	m_psp.pszHeaderSubTitle = _T("��������");

	_currentProcessBar = NULL;

	_currentMsg = NULL;
}

WizardPage4::~WizardPage4()
{
}

void WizardPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX ,IDC_PROCESS_MESSAGE,_processMsg );
	DDX_Text(pDX ,IDC__PROCESS_MSG_RESIZE ,_showMsgResize );
	DDX_Text(pDX ,IDC__PROCESS_MSG_CUT ,_showMsgCut );
	DDX_Text(pDX ,IDC__PROCESS_MSG_VGTR ,_showMsgVgtr );
	DDX_Text(pDX ,IDC__PROCESS_MSG_CLEAN ,_showMsgClean );

	DDX_Control(pDX, IDC_PROGRESS_IMG_RESIZE, _processBarOfResizing);
	DDX_Control(pDX, IDC_PROGRESS_IMG_CUT, _processBarOfCutting);
	DDX_Control(pDX, IDC_PROGRESS_VGTR, _processBarOfVgtr);
	DDX_Control(pDX, IDC_PROGRESS_CLEAN, _processBarOfClean);

	DDX_Control(pDX, IDC_PROCESS_START, _startBtn);
}


BEGIN_MESSAGE_MAP(WizardPage4, CPropertyPage)
	ON_BN_CLICKED(IDC_PROCESS_START, &WizardPage4::OnBnClickedProcessStart)
END_MESSAGE_MAP()


// WizardPage4 message handlers

BOOL WizardPage4::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL WizardPage4::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

	WizardPropertySheet* psheet = (WizardPropertySheet*)GetParent();         

	WizardPage3* page3 = ( WizardPage3* )(psheet->GetPage( 2 ));



	//if ( psheet->_demBag->getScaleOfDemToImg() != page3->_scaleDemToImg )
	//{
	//	_startBtn.EnableWindow(TRUE);
	//	psheet->SetWizardButtons(PSWIZB_BACK);  
	//}
	//else
	//{
	//	_startBtn.EnableWindow(FALSE);
	//	psheet->SetWizardButtons(PSWIZB_NEXT);  
	//}

	return CPropertyPage::OnSetActive();
}

void WizardPage4::OnBnClickedProcessStart()
{
	WizardPropertySheet* psheet = (WizardPropertySheet*)GetParent();      

	WizardPage3* page3 = ( WizardPage3* )(psheet->GetPage( 2 ));

	_startBtn.EnableWindow(FALSE);


	psheet->_demBag->registerProcessObs( this );

	//------------------------------------------
	// ��ò���
	//------------------------------------------
	long N = page3->_terrainN;

	long imgsize = (long)pow( 2.0f, page3->_terrainN ) * page3->_scaleDemToImg;
	assert( imgsize > 0 );

	String terrain_name = page3->_terrainName.GetBuffer(0);


	String aimfileprefix = page3->_selectOuputFolder.GetBuffer(0);
	aimfileprefix += "\\";
	aimfileprefix += page3->_terrainName.GetBuffer(0);


	String saveVgtrPath = page3->_selectOuputFolder.GetBuffer(0);
	saveVgtrPath += "\\";

	String saveVgiPath = page3->_selectOuputFolder.GetBuffer(0);
	saveVgiPath += "\\";
	saveVgiPath += page3->_terrainName + ".vgi";

	String saveVtrPath = page3->_selectOuputFolder.GetBuffer(0);
	saveVtrPath += "\\";
	saveVtrPath += page3->_terrainName + ".vtr";

	String tempPath = page3->_selectedFolder.GetBuffer(0);

	StringVectorPtr outddsnames( new StringVector );


	//------------------------------------------
	// 1.���ȿ�ʼ��ͼ�����resize����
	//------------------------------------------
	_currentProcessBar = &_processBarOfResizing;
	_currentMsg = &_showMsgResize;
	assert( psheet->_demBag != NULL );

	if ( psheet->_demBag->getScaleOfDemToImg() != page3->_scaleDemToImg )
	{
		//------------------------------------------
		// ���ʱ����Ҫ����resizeһ��
		//------------------------------------------
		bool resizeres = 
			psheet->_demBag->resizeImages( page3->_scaleDemToImg , tempPath);

		if ( resizeres == false )
		{
			MessageBox("����Ӱ������ʱ��������." ,"error");
			return;
		}
	}
	else
	{
		onFinish("�������");
	}

	_showMsgResize = "�����";
	UpdateData( FALSE );
	//------------------------------------------
	// 2.��ʼ��ͼ������и����
	//------------------------------------------
	_currentProcessBar = &_processBarOfCutting;
	_currentMsg = &_showMsgCut;

	// ����㼶
	long width = psheet->_demBag->getDemWidth();
	assert( width != 0 );
	assert( width % 2 == 0 );

	assert( width % (int)pow( 2.0, N )  == 0 );

	float tmpnum =  log( (float) (width / (int)pow( 2.0, N )  ) );

	long layernum = (int)( tmpnum / log( 2.0f ) );

	for ( int i = 0 ; i < layernum ; ++ i )
	{
		std::ostringstream o;
		o << aimfileprefix << "_level" << i;

		vgTerritory::HugeImagesProcessor imgprocessor;
		vgTerritory::HugeImageBlockInfoVectorPtr vgtrinfovec;

		imgprocessor.registerProcessObs( this );

		try 
		{
			imgprocessor.initialiseFileBlocks( psheet->_demBag->getFileBlockVec() );

			imgprocessor.initialiseParameters( o.str() , 
				imgsize , imgsize ,"bmp","dds",false,true,false,true
				,false,"",false,"",
				false, //�Ƿ�д�ϵ�����Ϣ
				vgKernel::ImagePos::ZERO ,0, -1);

			//imgprocessor.makeProcessing();
			imgprocessor.makeProcessingAndSaving();

			vgtrinfovec = imgprocessor.getHugeImageBlockInfos();

			//------------------------------------------
			// resize��һ��ĳ���. ���һ�β���Ҫ
			//------------------------------------------
			if ( i != layernum - 1)
			{
				psheet->_demBag->resizeImageForLod( tempPath );
			}

			//------------------------------------------
			// ���뵽output����,����ɾ��
			//------------------------------------------
			vgTerritory::HugeImageBlockInfoVector::iterator iter = vgtrinfovec->begin();
			vgTerritory::HugeImageBlockInfoVector::iterator iter_end = vgtrinfovec->end();

			vgKernel::StringVectorPtr ddsnames( new vgKernel::StringVector );

			for ( ; iter != iter_end ; ++ iter )
			{
				if ( vgKernel::SystemUtility::checkFileExist( iter->m_BlockFileName) )
				{
					outddsnames->push_back( iter->m_BlockFileName );
				}
			}
		}
		catch ( vgKernel::Exception *e ) 
		{
			::MessageBox( NULL , e->getFullDescription().c_str() , "error!" , MB_OK );
			return;
		}
		catch (...)
		{
			assert(0);
			return;
		}
	}

	_showMsgCut = "�����";
	UpdateData( FALSE );
	//------------------------------------------
	// 3.�������vgtr
	//------------------------------------------
	_currentProcessBar = &_processBarOfVgtr;
	_currentMsg = &_showMsgVgtr;

	bool savevtrok = psheet->_demBag->saveToVtr( outddsnames , 
		terrain_name , 
		saveVgtrPath , N );
	assert( savevtrok == true );
	if ( savevtrok == false )
	{
		MessageBox("����vgtrʱ���ִ���","error");
		return;
	}
    

	_showMsgVgtr = "�����";
	UpdateData( FALSE );
	//------------------------------------------
	// 4.�������.��������vgi��ɾ����ʱ�ļ� 
	//------------------------------------------
	_currentProcessBar = &_processBarOfClean;
	_currentMsg = &_showMsgClean;

	// ����vgi
	assert( outddsnames.isNull() == false );


	vgImage::FileWriterDdsToVgi vgiwriter;

	vgiwriter.registerProcessObs( this );

	bool writeok = vgiwriter.writeToVgiFileFromDdsNames(
		outddsnames , saveVgiPath );

	if ( writeok == false )
	{
		MessageBox("����, ����DDS�ļ�.");
		return;
	}

	// �������
	// ����м���ļ�
	psheet->_demBag->clearTempFiles();

	// ���dds�ļ�
	{
		StringVector::iterator iter = outddsnames->begin();
		StringVector::iterator iter_end = outddsnames->end();
		for ( ; iter != iter_end ; ++ iter )
		{
			vgKernel::SystemUtility::removeFile( *iter );
		}
	}


	_showMsgClean = "�����";
	UpdateData( FALSE );

	//------------------------------------------
	// ������ʾ
	//------------------------------------------
	_processMsg = "�����,������һ���鿴����.";
	psheet->SetWizardButtons(PSWIZB_NEXT);   

	UpdateData( FALSE );
}

void WizardPage4::onStart( const String& text , const int& range_lower , const int& range_upper )
{
	_currentProcessBar->SetRange32( range_lower , range_upper );

	_processMsg = text.c_str();

	UpdateData( FALSE );
	Invalidate();
}

void WizardPage4::onNotify( const String& msg , const int& process_pos )
{
	_currentProcessBar->SetPos( process_pos );

	_processMsg = msg.c_str();

	UpdateData( FALSE );	
	Invalidate();
}

void WizardPage4::onFinish( const String& msg )
{
	int lower = 0;
	int upper = 0;


	_processMsg = msg.c_str();

	_currentProcessBar->GetRange( lower , upper );
	_currentProcessBar->SetPos( upper );

	//*_currentMsg = "�����";
	UpdateData( FALSE );
	Invalidate();

}