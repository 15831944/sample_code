// dlgCoordTransform.cpp : ʵ���ļ�
//


#include "vgStableHeaders.h"
#include "vgGIS/dlgCoordTransform.h"
/*#include "gdal/ogr_avc.h"*/
#include <vgKernel/vgkVec3.h>
#include <vgKernel/vgkCoordSystem.h>

// CdlgCoordTransform �Ի���

IMPLEMENT_DYNAMIC(CdlgCoordTransform, CDialog)



CdlgCoordTransform::CdlgCoordTransform(CWnd* pParent /*=NULL*/)
	: CDialog(CdlgCoordTransform::IDD, pParent)
	, strSourceCoordPath(_T(""))
	, strTargetCoordPath(_T(""))
	, strSourceParam(_T(""))
	, strTargetParam(_T(""))
{

}

CdlgCoordTransform::~CdlgCoordTransform()
{

}

void CdlgCoordTransform::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SOURPATH, strSourceCoordPath);
	DDX_Text(pDX, IDC_EDIT_TARGETPATH, strTargetCoordPath);
	DDX_Text(pDX, IDC_EDIT_SOURCE_PARAM, strSourceParam);
	DDX_Text(pDX, IDC_EDIT_TARGET_PARAM, strTargetParam);
}


BEGIN_MESSAGE_MAP(CdlgCoordTransform, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SOURCE_PATH, &CdlgCoordTransform::OnBnClickedBtnSourcePath)
	ON_BN_CLICKED(IDC_BTN_TARGET_PATH, &CdlgCoordTransform::OnBnClickedBtnTargetPath)
	ON_BN_CLICKED(IDOK, &CdlgCoordTransform::OnBnClickedOk)
END_MESSAGE_MAP()


// CdlgCoordTransform ��Ϣ�������

void CdlgCoordTransform::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	delete this;
}

void CdlgCoordTransform::OnBnClickedBtnSourcePath()
{
#if 0
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char szFilters[]=
		"project files(*.prj)|*.prj|";

	CFileDialog dlg (TRUE, "prj", NULL,
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters);	

	std::string fileName;

	if( dlg.DoModal ()==IDOK )
	{
		fileName = dlg.GetPathName();
		strSourceCoordPath = dlg.GetPathName();
		UpdateData(false);

		CString strFileName = dlg.GetFileName();

		//��ȡprj�ļ������ݶ���
		const char  *pszPrjFile = fileName.c_str();

		FILE	*fp;
		fp = VSIFOpen( pszPrjFile, "r" );

		if( fp != NULL )
		{
			char **papszLines;

			VSIFClose( fp );

			papszLines = CSLLoad( pszPrjFile );

			if( vg_SourceSPF.importFromESRI( papszLines ) == OGRERR_NONE )
			{
				double dsemiMaj = vg_SourceSPF.GetSemiMajor();//������
				double dsemiMor = vg_SourceSPF.GetSemiMinor();//�̰���
				double dFlat = vg_SourceSPF.GetInvFlattening();//ƫ����

				const char *pszProjection = vg_SourceSPF.GetAttrValue("PROJECTION");
				const char *pDatum = vg_SourceSPF.GetAttrValue("DATUM");			//��ػ�׼��
				const char *pGeoCS = vg_SourceSPF.GetAttrValue( "GEOGCS" );			//��õ�������ϵͳ
				const char *pSpheroid = vg_SourceSPF.GetAttrValue("SPHEROID");		//������

				char **unitname = new (char* [32]);
				char **primeMerdian = new (char* [64]);
				char **angularUnit = new (char* [32]);

				double dUnits = vg_SourceSPF.GetLinearUnits(unitname);
				double dPrimeMedia = vg_SourceSPF.GetPrimeMeridian(primeMerdian);
				double dAngularUnit = vg_SourceSPF.GetAngularUnits(angularUnit);

				double dfalseEasting = vg_SourceSPF.GetProjParm(SRS_PP_FALSE_EASTING, 0.00000, NULL);
				double dfalseNorthing = vg_SourceSPF.GetProjParm(SRS_PP_FALSE_NORTHING, 0.00000, NULL);
				double dcentral_Meridian = vg_SourceSPF.GetProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.00000, NULL);
				double dScale_Factor = vg_SourceSPF.GetProjParm(SRS_PP_SCALE_FACTOR, 0.00000, NULL);
				double dLatit_Origin = vg_SourceSPF.GetProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.00000, NULL);
				double dStand_parallel_1 = vg_SourceSPF.GetProjParm(SRS_PP_STANDARD_PARALLEL_1, 0.00000, NULL);	//��׼γ��1
				double dStand_parallel_2 = vg_SourceSPF.GetProjParm(SRS_PP_STANDARD_PARALLEL_2, 0.00000, NULL);	//��׼γ��2
				double dlongitude_center = vg_SourceSPF.GetProjParm(SRS_PP_LONGITUDE_OF_CENTER, 0.00000, NULL);	//��������
				double dlatitude_center = vg_SourceSPF.GetProjParm(SRS_PP_LATITUDE_OF_CENTER, 0.00000, NULL);	//γ������

				strSourceParam.Format("�ļ�����:%s,\r\n\r\nͶӰϵ����:%s,\r\n��ƫ��ֵ:%f,\r\n��ƫ��ֵ:%f,\r\n���뾭��:%f,\r\n���ű���:%f,\r\nγ��ԭ��:%f,\r\n��׼γ��1:%f,\r\n��׼γ��2:%f,\r\nγ������:%f,\r\n��������:%f,\r\n��λ:%s(%.9f),\r\n\r\n��������ϵͳ:%s,\r\n�Ƕȵ�λ:%s(%.9f),\r\n����������:%s(%.7f),\r\n��ػ�׼��:%s,\r\n������:%s,\r\n������:%f,\r\n�̰���:%f,\r\nƫ����:%f",
					strFileName,
					pszProjection, 
					dfalseEasting, 
					dfalseNorthing,
					dcentral_Meridian,
					dScale_Factor,
					dLatit_Origin,
					dStand_parallel_1,
					dStand_parallel_2,
					dlongitude_center,
					dlatitude_center,
					*unitname,
					dUnits,
					pGeoCS,
					*angularUnit,
					dAngularUnit,
					*primeMerdian,
					dPrimeMedia,
					pDatum,
					pSpheroid,
					dsemiMaj,
					dsemiMor,
					dFlat);
				UpdateData(false);

				delete unitname;
				delete primeMerdian;
				delete angularUnit;
			}
		}
	}
#endif
}

void CdlgCoordTransform::OnBnClickedBtnTargetPath()
{
#if 0
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char szFilters[]=
		"project files(*.prj)|*.prj|";

	CFileDialog dlg (TRUE, "prj", NULL,
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters);

	std::string fileName;

	if( dlg.DoModal ()==IDOK )
	{
		fileName = dlg.GetPathName();
		strTargetCoordPath = dlg.GetPathName();
		UpdateData(false);

		CString strFileName = dlg.GetFileName();

		//AfxMessageBox(fileName.c_str());
		//��ȡprj�ļ������ݶ���

		const char  *pszPrjFile = fileName.c_str();

		FILE	*fp;
		fp = VSIFOpen( pszPrjFile, "r" );

		if( fp != NULL )
		{
			char	**papszLines;
			VSIFClose( fp );

			papszLines = CSLLoad( pszPrjFile );

			if( vg_TargetSPF.importFromESRI( papszLines ) == OGRERR_NONE )
			{
				//char    *pszWKT = NULL;
				//vg_TargetSPF.exportToWkt( &pszWKT );
				//printf( "%s", pszWKT );
				//strTargetParam.Format("%s",pszWKT);

				double dsemiMaj = vg_TargetSPF.GetSemiMajor();//������
				double dsemiMor = vg_TargetSPF.GetSemiMinor();//�̰���
				double dFlat = vg_TargetSPF.GetInvFlattening();//ƫ����

				const char *pszProjection = vg_TargetSPF.GetAttrValue("PROJECTION");//���ͶӰϵͳ����
				const char *pDatum = vg_TargetSPF.GetAttrValue("DATUM");			//��ػ�׼��
				const char *pGeoCS = vg_TargetSPF.GetAttrValue( "GEOGCS" );			//��õ�������ϵͳ
				const char *pSpheroid = vg_TargetSPF.GetAttrValue("SPHEROID");		//������

				char **unitname = new (char* [32]);									//��λ
				char **primeMerdian = new (char* [64]);								//����������
				char **angularUnit = new (char* [32]);								//�Ƕȵ�λ

				double dUnits = vg_TargetSPF.GetLinearUnits(unitname);				//��õ�λ
				double dPrimeMedia = vg_TargetSPF.GetPrimeMeridian(primeMerdian);	//��ñ���������
				double dAngularUnit = vg_TargetSPF.GetAngularUnits(angularUnit);	//��ýǶȵ�λ

				double dfalseEasting = vg_TargetSPF.GetProjParm(SRS_PP_FALSE_EASTING, 0.00000, NULL);			//��ƫ��ֵ
				double dfalseNorthing = vg_TargetSPF.GetProjParm(SRS_PP_FALSE_NORTHING, 0.00000, NULL);			//��ƫ��ֵ
				double dcentral_Meridian = vg_TargetSPF.GetProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.00000, NULL);	//���뾭��ֵ
				double dScale_Factor = vg_TargetSPF.GetProjParm(SRS_PP_SCALE_FACTOR, 0.00000, NULL);			//���ű���
				double dLatit_Origin = vg_TargetSPF.GetProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.00000, NULL);		//γ��ԭ��
				double dStand_parallel_1 = vg_TargetSPF.GetProjParm(SRS_PP_STANDARD_PARALLEL_1, 0.00000, NULL);	//��׼γ��1
				double dStand_parallel_2 = vg_TargetSPF.GetProjParm(SRS_PP_STANDARD_PARALLEL_2, 0.00000, NULL);	//��׼γ��2
				double dlongitude_center = vg_TargetSPF.GetProjParm(SRS_PP_LONGITUDE_OF_CENTER, 0.00000, NULL);	//��������
				double dlatitude_center = vg_TargetSPF.GetProjParm(SRS_PP_LATITUDE_OF_CENTER, 0.00000, NULL);	//γ������


				strTargetParam.Format("�ļ�����:%s,\r\n\r\nͶӰϵ����:%s,\r\n��ƫ��ֵ:%f,\r\n��ƫ��ֵ:%f,\r\n���뾭��:%f,\r\n���ű���:%f,\r\nγ��ԭ��:%f,\r\n��׼γ��1:%f,\r\n��׼γ��2:%f,\r\nγ������:%f,\r\n��������:%f,\r\n��λ:%s(%.9f),\r\n\r\n��������ϵͳ:%s,\r\n�Ƕȵ�λ:%s(%.9f),\r\n����������:%s(%.7f),\r\n��ػ�׼��:%s,\r\n������:%s,\r\n������:%f,\r\n�̰���:%f,\r\nƫ����:%f",
					strFileName,
					pszProjection, 
					dfalseEasting, 
					dfalseNorthing,
					dcentral_Meridian,
					dScale_Factor,
					dLatit_Origin,
					dStand_parallel_1,
					dStand_parallel_2,
					dlongitude_center,
					dlatitude_center,
					*unitname,
					dUnits,
					pGeoCS,
					*angularUnit,
					dAngularUnit,
					*primeMerdian,
					dPrimeMedia,
					pDatum,
					pSpheroid,
					dsemiMaj,
					dsemiMor,
					dFlat);

				UpdateData(false);

				delete unitname;
				delete primeMerdian;
				delete angularUnit;
			}
		}
	}
#endif
}

void CdlgCoordTransform::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ( strSourceCoordPath.GetLength() == 0 )
	{
		AfxMessageBox("��ѡ��ԭͶӰ����ϵ��");
		return;
	}
	if ( strTargetCoordPath.GetLength() == 0 )
	{
		AfxMessageBox("��ѡ��Ŀ��ͶӰ����ϵ��");
		return;
	}
	
	if (&vg_SourceSPF == NULL)
	{
		AfxMessageBox("ԭͶӰ����ϵΪ�գ�");
	}

	if (&vg_TargetSPF == NULL)
	{
		AfxMessageBox("Ŀ��ͶӰ����ϵΪ�գ�");
	}
	
 	vgKernel::Vec3 vgCoord = vgKernel::CoordSystem::getSingleton().getProjectionCoord();
 
 	double originX = (double)vgCoord.x;
 	double originY = (double)vgCoord.y;
 	double originZ = (double)vgCoord.z;

	OGRCoordinateTransformation *pvg_coordTrans = NULL;

	pvg_coordTrans = OGRCreateCoordinateTransformation(&vg_SourceSPF, &vg_TargetSPF);
	
	//CString strResult;

	if (pvg_coordTrans != NULL)
	{
		pvg_coordTrans->Transform( 1, &originX, &originY, &originZ) ;
		//strResult.Format("ת�����:(x:%f, y:%f, z:%f)", originX, originY, originZ);
		//AfxMessageBox(strResult);
		vgKernel::Vec3 resCoord;
		resCoord.x = originX;
		resCoord.y = originY;
		resCoord.z = originZ;
		//��ת�����ֵ��Ϊ����ԭ������
		vgKernel::CoordSystem::getSingleton().setProjectionCoord(resCoord);
		AfxMessageBox("ת����ɣ�");
		
	}
	else
	{
		AfxMessageBox("ת��ʧ�ܣ�");
	}

	OnOK();
}
