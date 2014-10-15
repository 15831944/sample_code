// vgnContourObj.h: interface for the CContourObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTOUROBJ_H__AC6A2306_787D_4F9D_9CDD_21E0B7EC14E3__INCLUDED_)
#define AFX_CONTOUROBJ_H__AC6A2306_787D_4F9D_9CDD_21E0B7EC14E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*===================��ֵ�߻��ƶ�����===========================*

һ��CContourObj����һ����ֵ�����ߵĻ��ƣ�

���Ƶ����ݰ���: ������ �� �������

|-->����
������------|
|-->��ɫ

|-->ǰ��ɫ
�������----|
|-->����ɫ
|
|-->�������

jeny_man@163.com
*==============================================================*/

#include "vgNoise\vgnNoiseGeoStruct.h"
#include "vgNoise\vgnColorLookUpTable.h"
namespace vgGIS3D
{
typedef CArray<CGeoPoint,CGeoPoint&> CCurve; //һ����ֵ�����ߵĵ㼯������


class CContourObj  
{
public:
	CContourObj(CCurve*  pCoord, float value);
	~CContourObj();



public:

	void Draw(CDC* pDC,
		const CColorLookUpTable& clrTbl,
		float scaleRatio, float moveDeltX, float moveDeltY);
	void DrawContours(CDC* pDC, float scaleRatio, float moveDeltX, float moveDeltY);


	void LookUpColor(const CColorLookUpTable& clrTbl); //������ɫ��,�趨������ɫ 
	void LookUpColor1( const CColorLookUpTable& clrTbl,float m_Value); // 

	void FillContours(CDC* pDC, int j,float scaleRatio, float moveDeltX, float moveDeltY,const CColorLookUpTable& clrTbl);
	//private:
public:
	//	float** m_ppGridData;

	//------������Ϣ------------------
	CCurve*  m_pCoord; //�õ�ֵ�ߵ�������,ָ���ⲿ������

	float m_Value; //�õ�ֵ������Ӧ��ֵ
	COLORREF   m_LineColor;
	COLORREF   m_FillColor;

	//--�������Թ������----------
	//
	// CContourLine       m_Line;  //��û��ʵ��
	// CContourRegion     m_Region;   //��û��ʵ�� 
private:
	CContourObj& operator=(const CContourObj&); //not implemented
	CContourObj(const CContourObj&);            //not implemented


};

#endif // !defined(AFX_CONTOUROBJ_H__AC6A2306_787D_4F9D_9CDD_21E0B7EC14E3__INCLUDED_)
}