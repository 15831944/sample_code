#pragma once

#include "vgTerritoryResource.h"
// CdlgSetCellSize �Ի���

class CdlgSetCellSize : public CDialog
{
	DECLARE_DYNAMIC(CdlgSetCellSize)

public:
	CdlgSetCellSize(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CdlgSetCellSize();

// �Ի�������
	enum { IDD = IDD_DLG_SET_CELLSIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	float cellSize;
	afx_msg void OnEnChangeEditcellsize();
};
