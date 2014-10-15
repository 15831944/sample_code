#pragma once

#include <string>
#include "vgBaseEntry.h"

using std::string;

/**
* vgBasedTabPage �������Զ�������ҳ�ĸ��࣬ͨ��
* ConnectNode������ĳ���ض��ڵ�����ݽ��а���
* ʵ��������ʾ�͸���
*
* @see		   vgTestNode1
*
* @author      ZSC
* @version     VRGIS FrameWork RC
* @since       VRGIS 1.0+
*/

class vgBasedTabPage
{
public:
	vgBasedTabPage(void);

	virtual ~vgBasedTabPage(void);

	virtual string GetTitle() = 0;

	virtual CWnd* GetWnd() = 0;
	
	virtual void ConnectNode(vgBaseEntry* node) = 0;

	// virtual int GetIconIndex() = 0;
protected:
	string m_sTitle;
	int m_nIconIndex;
};
