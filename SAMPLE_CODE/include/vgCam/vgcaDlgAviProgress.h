#pragma once

#include <vgLibraryMFCDLL/vgLibraryResource.h>
#include "afxcmn.h"
#include "afxwin.h"

// vgcaDlgAviProgress �Ի���
namespace vgCam
{
	class vgcaDlgAviProgress : public CDialog
	{
		DECLARE_DYNAMIC(vgcaDlgAviProgress)

	public:
		vgcaDlgAviProgress(CWnd* pParent = NULL);   // ��׼���캯��
		virtual ~vgcaDlgAviProgress();

		// �Ի�������
		enum { IDD = IDD_AVI_PROGRESS };

	public:
		void setProgress(int progress);

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

		DECLARE_MESSAGE_MAP()
	public:
		virtual BOOL DestroyWindow();
	protected:
		virtual void OnOK();
		virtual void OnCancel();
	public:
		virtual BOOL OnInitDialog();
	};

}
