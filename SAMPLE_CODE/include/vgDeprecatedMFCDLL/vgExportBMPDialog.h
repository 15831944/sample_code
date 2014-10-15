
#pragma once
#include "resource.h"

// BMPExportDialog �Ի���

class  VGDEP_EXPORT BMPExportDialog : public CDialog
{
	DECLARE_DYNAMIC(BMPExportDialog)

public:
	BMPExportDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~BMPExportDialog();

	// �Ի�������
	enum { IDD = IDD_BMPEXPORT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditBmppath();
	afx_msg void OnEnChangeEditBmpwidth();
	CString m_bmpFilePath;
	int m_bmpWidth;
	int m_bmpHeight;
	afx_msg void OnBnClickedExpbmpPath();

	//add by ZhouZY 2009-11-4 22:31 
	//����ѭ������Ƿ����ͬ���ļ�����������򷵻������ļ�������
	void saveBmp( CFileDialog &fileDlg );
};
