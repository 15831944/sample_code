// vgnContour.h: interface for the CContour class.
//
//////////////////////////////////////////////////////////////////////

#include "vgNoise\vgnContourValues.h"
#include "vgNoise\vgnNoiseGeoStruct.h"
#include "vgNoise\vgnNoiseGeoData.h"
#include "vgNoise\vgnContourDrawer.h"
#pragma once
#if !defined(AFX_CONTOUR_H__7ED2ACCB_E944_4543_AF11_2762F561CF69__INCLUDED_)
#define AFX_CONTOUR_H__7ED2ACCB_E944_4543_AF11_2762F561CF69__INCLUDED_

#if _MSC_VER > 1000

#endif // _MSC_VER > 1000
namespace vgNoise
{
	/*===================��ֵ����=============================*/
	//                   CContour
	//�������ڱ����ֵ������,������ֵ��ֵ����ֵ��׷�٣���ֵ�߻��ƣ��ȵ�....
	//
	//�����ͱ༭��ֵ��ֵʱ������CContourValues�������
	//�����ɵ�ֵ��ʱ������CContourTracer��ֵ��׷���������׷��
	//�ڵ�ֵ�߻���ʱ������CContourDrawer������������
	//
	/*==============================================================*/


	typedef CArray<CGeoPoint,CGeoPoint&> CCurve; //һ����ֵ������
	typedef CTypedPtrList<CPtrList, CCurve*> CCurveList; //��ֵ����������(����ĳֵ�ĵ�ֵ�߲�����һ��)
	typedef CTypedPtrArray<CPtrArray,CCurveList*> CContourArray; 

	typedef CTypedPtrArray<CPtrArray,CContourObj*> CContourObjArray;
	typedef CTypedPtrArray<CPtrArray,CContourObj*> CContourFillArray;


	class CContour  
	{
	public:

		CContour(CNoiseGeoData* pOwner=NULL);
		~CContour();

		//----��ֵ��ֵ����ز���-------------
		int GetNumberOfValues();
		void AddContourValue(float newVal);
		void DeleteContourValue(float oldVal);
		void ReplaceContourValue(float oldVal, float newVal);
		void GenerateContourValues(int numContours, float valStart, float valEnd);
		CValueList& GetValueList();
		float GetValue(int i);
		void SetValueList(CValueList& list);
		void GenerateDefaultContourValues();
		void EditContourLevels(); //�༭��ֵ��

		//----------------------------
		void GenerateContours();
		void RemoveAllContours();

		//----------�����������---------
		void SetDataOwner(CNoiseGeoData* pOwner)
		{
			ASSERT(pOwner!=NULL); m_pDataOwner = pOwner;
		}
		CNoiseGeoData* GetDataOwner()
		{
			return m_pDataOwner;
		}

		//-----׷�ٽ��---------
		CContourArray& GetContours()
		{
			return m_Contours;
		}

		//-----�������---------------
		void Draw(CDC* pDC,const CRect& drawRect);
		BOOL IsHavingContours(){ return m_Contours.GetSize()>0;}
		void ModifyColorTable()
		{
			/*ASSERT(m_pContourDrawer!=NULL);*/
			m_pContourDrawer->ModifyColorTable();
		}
		BOOL IsGeneratingAgain(){return m_bReGenerated;}


	public:
		CContourValues m_contourValues; //������ֵ��ֵ
		CContourArray    m_Contours;    //��ֵ��׷�ٽ�����ڴ���
		//CContourTracer*  m_pContourTracer;
		CContourDrawer*  m_pContourDrawer;
		CNoiseGeoData*       m_pDataOwner;

		BOOL m_bReGenerated; //ָʾ�������Ƿ���Ҫ��������Contour���ƶ���ı�־


	private:
		CContour(const CContour& rhs); //not implimented
		CContour& operator=(const CContour& rhs);//not implimented
	};

	inline int CContour::GetNumberOfValues()
	{
		return m_contourValues.GetNumberOfValues();
	}

	inline void CContour::AddContourValue(float newVal)
	{
		m_contourValues.AddValue(newVal);
	}

	inline void CContour::DeleteContourValue(float oldVal)
	{
		m_contourValues.DeleteValue(oldVal);
	}

	inline void CContour::ReplaceContourValue(float oldVal, float newVal)
	{
		m_contourValues.ReplaceValue(oldVal,newVal);
	}

	inline void CContour::GenerateContourValues(int numContours, float valStart, float valEnd)
	{
		m_contourValues.GenerateValues(numContours,valStart,valEnd);
	}

	inline CValueList& CContour::GetValueList()
	{ 
		return m_contourValues.GetValueList(); 
	}

	inline float CContour::GetValue(int i)
	{
		return m_contourValues.GetValue(i);
	}

	inline void CContour::SetValueList(CValueList& list)
	{
		m_contourValues.RemoveAllValues();

		POSITION pos = list.GetHeadPosition();
		while(pos!=NULL)
		{
			m_contourValues.AddValue(list.GetNext(pos));
		}

	}
}
#endif // !defined(AFX_CONTOUR_H__7ED2ACCB_E944_4543_AF11_2762F561CF69__INCLUDED_)