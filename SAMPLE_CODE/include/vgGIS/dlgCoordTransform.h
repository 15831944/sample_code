#pragma once


#include "resource.h"
#include "gdal/ogr_spatialref.h"
// CdlgCoordTransform �Ի���

class CdlgCoordTransform : public CDialog
{
	DECLARE_DYNAMIC(CdlgCoordTransform)

public:
	CdlgCoordTransform(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CdlgCoordTransform();

// �Ի�������
	enum { IDD = IDD_DLG_COORD_TRANS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString strSourceCoordPath;
	CString strTargetCoordPath;

	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnSourcePath();
	afx_msg void OnBnClickedBtnTargetPath();
	afx_msg void OnBnClickedOk();
public:
	OGRSpatialReference vg_SourceSPF;
	OGRSpatialReference vg_TargetSPF;

	CString strSourceParam;
	CString strTargetParam;
};
