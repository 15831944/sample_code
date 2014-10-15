#pragma once
#include <vgShape/vgshDefinition.h>

#include "vgShapeResource.h"
#include <vgShape/vgshLayer.h>
// CDlgShpToDB dialog

class  VGSH_EXPORT CDlgShpToDBEx : public CDialog
{
	DECLARE_DYNAMIC(CDlgShpToDBEx)

public:
	CDlgShpToDBEx(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgShpToDBEx();

// Dialog Data
	enum { IDD = IDD_DLG_SHP2DB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	vgShape::LayerPtrVector	m_LayersReadyToSet;//�����˳����Ĵ����õĲ����
	vgShape::LayerPtrVector	m_LayersReadyToDB;//�����Ĳ����
	vgShape::LayerPtr			m_pCurrentLayer;


	vgKernel::StringVector		m_strFieldArray;

	int			m_nCurrentLayerType;
	int			m_nCurrentLayer;
	int			m_nCurrentKeyField;

public:
	CComboBox m_layerTypeList;
	CComboBox m_layerInfoList;
	CComboBox m_fieldInfoList;
	CListBox m_resultList;
	afx_msg void OnCbnSelchangeListLayer();
	afx_msg void OnBnClickedBtnSubmit();
	afx_msg void OnCbnSelchangeListField();

	afx_msg void OnBnClickedBtnShp2db2();
	afx_msg void OnCbnSelchangeListBoolInDb();

public:
	int		IsAllreadyInsert(vgShape::LayerPtr pCurrentLayer);
	bool	writeLayerRecordsToDatabaseTable(vgShape::LayerPtr pCurrentLayer, 
		bool bCreateNew = true);
	bool	renamaeLayerShapes(vgShape::LayerPtr pCurrentLayer, int nKeyField = -1);
	bool CreateShapeTable(vgShape::Layer*	pCurrentLayer);

	bool AppendFile(vgShape::LayerPtr pCurrentLayer,String filename,int PrimaryId);//YX

};
