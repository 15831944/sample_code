#pragma once

#include <ximage.h>



// DEMView ��ͼ

class DEMView : public CScrollView
{
	DECLARE_DYNCREATE(DEMView)

protected:
	DEMView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~DEMView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	virtual void OnInitialUpdate();     // �����ĵ�һ��

	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

};


