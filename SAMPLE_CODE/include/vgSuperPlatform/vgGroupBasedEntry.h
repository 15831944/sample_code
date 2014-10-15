#pragma once

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkRenderer.h>
#include <vgKernel/vgkRendererObserver.h>
#include "vgEntryType.h"


#include "vgPropertyPage.h"

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


class vgGroupBasedEntry : public vgBaseEntry
{
public:
	vgGroupBasedEntry( vgEntryType type ,vgKernel::Renderer* renderer ) : vgBaseEntry(type, renderer) {}
	
	// ���಻��Ҫ��д�˺��� 
	// ���Group����Ӧ��дAddGroupTabPage
	// ���Single����Ӧ��дAddSingleTabPage
	virtual void AddNodeTabs();

	// ���಻��Ҫ��д�˺��� 
	// ���Group����Ӧ��дOnGroupPropertyChanged
	// ���Single����Ӧ��дOnSinglePropertyChanged
	virtual void OnPropertyChanged(string paramName);

protected:
	virtual void AddGroupTabPage() = 0;

	virtual void AddSingleTabPage() = 0;

	virtual void OnSinglePropertyChanged(string paramName) = 0;

	virtual void OnGroupPropertyChanged(string paramName) = 0;

	static vector<vgGroupBasedEntry*> s_group_list;

	// group ����
	static vector<PropertiesParam> s_GroupParamArray;
	
	// ����Node����
	static vector<PropertiesParam> s_ParamArray;
};