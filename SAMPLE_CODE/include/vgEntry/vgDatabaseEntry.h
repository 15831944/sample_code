#pragma once


#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkRenderer.h>
#include <vgKernel/vgkRendererObserver.h>
#include <vgEntry/vgEntryType.h>
#include <vgUIController/vgDatabasePage.h>
#include <vgDatabase/vgdDatabaseManager.h>
#include <vgDatabase/vgdProfileManager.h>
#include <vgEntry/vgGroupBasedEntry.h>
#include <vgKernel/vgkRendererData.h>

/**
* vgDatabaseEntry 
* ����ʵ�����е��麯��
*
* @see		   vgBasedEntry
*
* @author      ZSC
* @version     VRGIS SuperPlatform SP
* @since       VRGIS 2.0+
*/

class  VGUI_EXPORT vgDatabaseEntry : public vgGroupBasedEntry
{
public:
	vgDatabaseEntry( vgKernel::Renderer* renderer );
	
	virtual String getSearchName()
	{
		return GetName();
	}//YXDB

/*	vgDatabaseEntry()*/
	// ������AddSingleTabPage������profileѡ�������� �ο�vgMeshEntry.cpp line:182
	// ���าдAddSingleTabPage��Ӧ�ں����������� vgDatabaseEntry::AddSingleTabPage ʵ�����ݿ�Tab����ʾ�͸���
	virtual void AddSingleTabPage();

	virtual void AddGroupTabPage(){ return; }

	virtual void OnDatabasePropertyChanged(string paraName, _variant_t dataValue);
	 
	virtual void OnSinglePropertyChanged(string paramName);

	virtual void OnGroupPropertyChanged(string paramName);

	virtual void onChanged(int eventId, void *param){ return; }

	virtual int GetSelectedImage() { return 1; }

	virtual	int GetUnSelectedImage() { return 2; }

	virtual void UpDataProp() { return; }

protected:
	// ���ݿ���غ��������ݳ�Ա
	void initDatabaseParam();

	static vgDatabasePage s_databasePage; 

	static vector<DatabaseProperties> s_databaseParam; 

	static string s_groupProfileName;

	string m_profileName;

	string m_queryString;
};