// VgimgReaderDoc.cpp : CVgimgReaderDoc ���ʵ��
//

#include <vgStableHeaders.h>
#include "VgimgReader.h"

#include "VgimgReaderDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVgimgReaderDoc

IMPLEMENT_DYNCREATE(CVgimgReaderDoc, CDocument)

BEGIN_MESSAGE_MAP(CVgimgReaderDoc, CDocument)
END_MESSAGE_MAP()


// CVgimgReaderDoc ����/����

CVgimgReaderDoc::CVgimgReaderDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CVgimgReaderDoc::~CVgimgReaderDoc()
{
}

BOOL CVgimgReaderDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CVgimgReaderDoc ���л�

void CVgimgReaderDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CVgimgReaderDoc ���

#ifdef _DEBUG
void CVgimgReaderDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVgimgReaderDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CVgimgReaderDoc ����
