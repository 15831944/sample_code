#pragma once

#include <string>
#include <vgTerritory/vgtGeoGlobalData.h>

using std::string;

// TifView ��ͼ

struct Dataset_t
{
	string dataset_name;

	int startx;
	int starty;

	int sizex;
	int sizey;
};

class TifView : public CScrollView
{
	DECLARE_DYNCREATE(TifView)

protected:
	TifView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~TifView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	void SetFileVector(std::vector<string> &fileList);
	void ClearDataSet();

protected:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	virtual void OnInitialUpdate();     // �����ĵ�һ��

	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	//std::map<GDALDataset*, BYTE*> m_vdataSet;
	//std::vector<string> m_fileList;
	std::map<struct Dataset_t*, BYTE*> m_vdataSet;

	double m_orgiX;
	double m_orgiY;

	DOUBLE	m_lowRightX;
	DOUBLE  m_lowRightY;

	vgTerritory::GeoGlobalData* _globalData;

public:

	void setGlobalData( vgTerritory::GeoGlobalData* globaldata )
	{
		_globalData = globaldata;
	}

	double _interval;

public:
	afx_msg void OnDestroy();
};


