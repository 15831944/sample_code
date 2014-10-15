#pragma once

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkRenderer.h>
#include <vgKernel/vgkRendererObserver.h>
#include "vgEntryType.h"

/**
* vgBaseEntry ������node�ĸ��࣬�Զ����Node��
* ����ʵ�����е��麯��
*
* @see		   vgTestNode1
*
* @author      ZSC
* @version     VRGIS FrameWork RC
* @since       VRGIS 1.0+
*/
  

class vgBaseEntry : public vgKernel::RendererObserver
{
public:
	vgBaseEntry( vgEntryType type ,vgKernel::Renderer* renderer )
	{
		m_Renderer = renderer;
		m_typeFlag = type;

		if ( renderer != NULL )
		{
			renderer->registerObserver( this );
		}
	}

	virtual ~vgBaseEntry(void)
	{

	}
	
	virtual String GetName()
	{
		assert( m_Renderer != NULL );
		return m_Renderer->getName();
	}
	

	virtual bool onDelete();


	virtual void AddNodeTabs() = 0;

	virtual int GetUnSelectedImage() = 0;
	
	virtual int GetSelectedImage() = 0;

	// Node ����ʱ֪ͨObject
	virtual void OnPropertyChanged(string paramName) = 0;

	virtual void OnDatabasePropertyChanged(string paraName, _variant_t dataValue) {}

	vgEntryType  GetType() { return m_typeFlag; }


	//------------------------------------------
	// �Ƿ���Ա�del����ֱ��ɾ��.
	// ������rootentryһ���entry����, ����
	// true�Ǻ��ʵ�
	//  �ο�CEditTreeCtrlEx::DeleteSelectedNode()
	//------------------------------------------
	virtual bool canBeDeletedDirectly()
	{
		return false;
	}

	//------------------------------------------
	// �趨������del��ʱ
	// entry�Լ����õ�ɾ������.
	// ������Ϊtrueʱ, ��ʾ�ɹ�ɾ��.
	//------------------------------------------
	virtual bool onUserDefinedDeletion()
	{
		return true;
	}

	vgKernel::Renderer* getRender()
	{
		return m_Renderer;
	}

	virtual CMenu* GetContextMenu() 
	{
		return NULL;
	}

public:

	HTREEITEM hTreeItem;

protected:

	vgEntryType    m_typeFlag;

	vgKernel::Renderer* m_Renderer;
};


// Observer �Ķ�������vgKernel
