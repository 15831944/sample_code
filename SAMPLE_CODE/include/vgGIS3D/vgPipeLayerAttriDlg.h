#pragma once

#include <vgGis/Resource.h>
#include <vgGIS/vggisDefinition.h>
#include <vgGis3d/vggiOGRPipeLineMap.h>
#include <vgGIS3D/vggiOgrUtility.h>
#include <vgDatabase/vgdAccessConnector.h>
#include <vgDatabase/vgdDatabaseManager.h>

#include <string>

// vgPipeAttriDlg �Ի���
namespace vgGIS3D
{
	class VGG_EXPORT vgPipeLayerAttriDlg : public CDialog
	{
		DECLARE_DYNAMIC(vgPipeLayerAttriDlg)

	public:
		vgPipeLayerAttriDlg(CWnd* pParent = NULL);   // ��׼���캯��
		virtual ~vgPipeLayerAttriDlg();

		// �Ի�������
		enum { IDD = IDD_PIPELAYER_ATTRILIST };

	public:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);		
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

		BOOL OnInitDialog();
		void OnTimer(UINT_PTR nIDEvent);
		virtual void OnOK();
		void OnDestroy();
		void OnAppendOption();
		void OnAddAttriSection();
		void getValue();
		void getValueFromDatabase();
		void OnPrintPipeAttri();
		void OnExportPipeAttri();
	public:
		void updateList(CString sectionName, CString type, CString initValue);
		void addSortToDatabase(CString sectionName, CString type);

	private:
		CStatic m_wndGridLocation;
		CBCGPGridRow *sLastRow;
		CBCGPGridCtrl	m_wndGrid;

		OGRPipeLineMap *m_ogrPipeLineMap;

		CADORecordset* m_pRecordsetLand;	

		CMenu m_menu;

	public:
		String m_tableName;

		static std::vector<CString> m_fieldNameVec;
		

		DECLARE_MESSAGE_MAP()			

	};

}//end namespace


