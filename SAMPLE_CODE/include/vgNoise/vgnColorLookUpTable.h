// ColorLookUpTable.h: interface for the CColorLookUpTable class.
//
//////////////////////////////////////////////////////////////////////
namespace vgNoise
{
#if !defined(AFX_COLORLOOKUPTABLE_H__0285AF4A_55D5_4F93_9773_EDADC6E67CC1__INCLUDED_)
#define AFX_COLORLOOKUPTABLE_H__0285AF4A_55D5_4F93_9773_EDADC6E67CC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*==============================================*
//
//        『颜色查找表』类
//
*===============================================*/
struct CColorNode
{//颜色表中的颜色节点
	float pos;        // 0~1 ,归一化坐标位置
	COLORREF color;
	CColorNode(float p=0.f,COLORREF clr=0):pos(p),color(clr){}
};

class CColorLookUpTable  
{
public:
	CColorLookUpTable();
	~CColorLookUpTable();

	void SetValueRange(float min, float max);
	void GetValueRange(float &min, float &max);

	COLORREF GetColor(float pos)const;
	void SetColor(float pos, COLORREF newClr);

	COLORREF LookUpColor(float v) const; //在颜色表中查询v值的颜色

	int InsertColorIntoTable(float pos, COLORREF newClr); //向颜色表中加入颜色节点(插入后保证pos从小到大排序)
	void RemoveColorFromTable(float pos);//删除颜色表中的颜色节点
	void DrawColorSpectrum(CDC* pDC, const CRect& drawRect);//绘制颜色表的颜色谱

	BOOL LoadColorTable(const CString& filePath );
	BOOL SaveColorTable(const CString& filePath );
private:
	void GradientRectangle(CDC* pDC, const CRect& drawRect,COLORREF clrStart,COLORREF clrEnd);

private:
	CArray<CColorNode,CColorNode&> m_ColorTable; //颜色表(0~1内的位置--->RGB颜色)
	float m_ValueMin, m_ValueMax;       //数值的范围,该范围内的数值和颜色表内的颜色相互对应


private:
	CColorLookUpTable& operator=(const CColorLookUpTable&); 	
	CColorLookUpTable(const CColorLookUpTable&);

};

inline void CColorLookUpTable::SetValueRange(float min, float max)
{
	ASSERT( min < max );
	m_ValueMin = min;
	m_ValueMax = max;
}

inline void CColorLookUpTable::GetValueRange(float &min, float &max)
{
	min = m_ValueMin;
	max = m_ValueMax;
}
#endif // !defined(AFX_COLORLOOKUPTABLE_H__0285AF4A_55D5_4F93_9773_EDADC6E67CC1__INCLUDED_)
}