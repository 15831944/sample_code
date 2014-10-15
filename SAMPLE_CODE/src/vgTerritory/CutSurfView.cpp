// CutSurfView.cpp : ʵ���ļ�
//

#include "vgStableHeaders.h"
#include "vgTerritory/CutSurfDoc.h"
#include "vgTerritory/CutSurfView.h"
#include <math.h>

#pragma comment(lib, "gdiplus.lib")

IMPLEMENT_DYNCREATE(CutSurfView, CView)

CutSurfView::CutSurfView()
{
	fLeft = 0.0f;
	fRight = 0.0f;
	fButtom = 0.0f;
	fTop = 0.0f;
	drawMouseLine = FALSE;
	mousPosX = 0;
	mousPosY = 0;

	//��ʼ��GDI+�ӿ�
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_nGdiplusToken, &gdiplusStartupInput,NULL);
	
	m_font.CreatePointFont(10,"����");

	heightPoint = NULL;

	lineLength = 0.0f;
}

CutSurfView::~CutSurfView()
{
	if ( heightPoint != NULL )
	{
		delete heightPoint;
		heightPoint = NULL;
	}
	std::vector <vgKernel::Vec3*> ::iterator iterBegin = vec_htPoint.begin();
	std::vector <vgKernel::Vec3*> ::iterator iterEnd = vec_htPoint.end();
	for (; iterBegin != iterEnd; iterBegin++)
	{
		delete (*iterBegin);
		(*iterBegin) = NULL;
	}
	//ע��GDI+��Դ
	GdiplusShutdown(m_nGdiplusToken);
}


BEGIN_MESSAGE_MAP(CutSurfView, CView)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_TIMER()
	ON_WM_MBUTTONDBLCLK()
END_MESSAGE_MAP()

// CutSurfView ��ͼ

#ifdef _DEBUG
void CutSurfView::AssertValid() const
{
	CView::AssertValid();
}



#ifndef _WIN32_WCE
void CutSurfView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CutSurfView ��Ϣ�������

void CutSurfView::OnDestroy()
{
	CView::OnDestroy();
	
	// TODO: �ڴ˴������Ϣ����������
	
}

int CutSurfView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	SetTimer(1, 50, NULL);
	
	return 0;
}

void CutSurfView::OnDraw(CDC* pDC)
{
	// TODO: �ڴ����ר�ô����/����û���

	Render();
	CClientDC dc(this);
}

void CutSurfView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	
}

void CutSurfView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!drawMouseLine)
	{
		drawMouseLine = TRUE;
	}
	else
	{
		drawMouseLine = FALSE;
	}
	CView::OnLButtonDown(nFlags, point);
}

void CutSurfView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	mousPosX = point.x;
	mousPosY = point.y;
	CView::OnMouseMove(nFlags, point);
}

void CutSurfView::Render()
{
	int temp = 13;						//�ָ��߼��
	
	//X��
	Point point1(40, 160);
	Point point2(428, 160);
	//Y��
	Point point3(40,160);
	Point point4(40,10);

	CString str;						//ԭ��
	CString strButtom;					//�ָ��߱�ʶ
	CDC *pDC = GetDC();

	SolidBrush  brush(Color(255, 0, 0, 255));
	FontFamily  fontFamily(L"����");
	Gdiplus::Font font(&fontFamily, 13, FontStyleRegular, UnitPixel);

	Pen penCoord(Color( 255, 0, 0), 2.0f);		//��ɫ2.0��������X��Y
	Pen penCoord2(Color( 50, 200, 200), 0.1f);	//��ɫ0.1�ķָ���
	Pen penHorizon(Color(255,0,0),0.1f);		//��ƽ����ɫ

	Pen penHtLine(Color(0,0,255),2.0f);

	CRect rect;

	Bitmap CacheImage(460, 185); 
	PointF pointY(0.0f,5.0f);					//�߳�txt
	
 	GetClientRect(&rect);

	Graphics vg_graphics(pDC->m_hDC);			//
	Graphics CacheGraphics( &CacheImage );		//ʹ��˫����	
	
	CacheGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	
	CacheGraphics.Clear(Color(255,255,255));	//���
	
	penCoord.SetStartCap(LineCapRound) ;		//�޸���ʼ��ͷ���ͣ�������Բͷ

	penCoord.SetEndCap(LineCapDiamondAnchor);	//�޸Ľ�����ͷ���ͣ������Ǽ�ͷ
	
	float m_top = 0.0f;
	float m_buttom = 0.0f;
	float fdeltaY = 0.0f;
	float fdeltaX = 0.0f;

	if (fTop < 100 )//���߳� < 100
	{
		if (((int)fTop % 10) != 0 )
		{
			 m_top = (((int)fTop - (int)fTop % 10)/ 10 + 1) * 10; 
		}
		else
		{
			m_top = fTop;
		}		
	}
	else if (fTop >= 100 && fTop <1000)
	{
		if (((int)fTop % 100) != 0)
		{
			m_top = (((int)fTop - (int)fTop % 100) / 100 + 1) * 100;
		}
		else
		{
			m_top = fTop;
		}
	}
	else if (fTop >=1000 && fTop < 10000)
	{
		if (((int)fTop % 1000) != 0)
		{
			m_top = (((int)fTop - (int)fTop % 1000) / 1000 + 1) * 1000;
		}
		else
		{
			m_top = fTop;
		}
	}

	if (fButtom > -10000 && fButtom <= -1000)
	{
		if (((int)fButtom % 1000) != 0)
		{
			m_buttom = (((int)fButtom - (int)fButtom % 1000)/ 1000 - 1) * 1000;
		}
		else
		{
			m_buttom = fButtom;
		}
	}
	else if (fButtom > -1000 && fButtom <= -100)
	{
		if (((int)fButtom % 100) != 0)
		{
			m_buttom = (((int)fButtom - (int)fButtom % 100)/ 100 - 1) * 100;
		}
		else
		{
			m_buttom = fButtom;
		}
	}
	else if ( fButtom > -100)
	{
		if (((int)fButtom % 10) != 0)
		{
			m_buttom = (((int)fButtom - (int)fButtom % 10)/ 10 - 1) * 10;
		}
		else
		{
			m_buttom = fButtom;
		}
	}

	int temdelta = m_top - m_buttom;

	if (temdelta > 10000)							//10000 < temdelta
	{
		fdeltaY = 1000.0;
	}
	if (temdelta > 1000 && fdeltaY <= 10000)		//1000 < temdelta<=10000
	{
		if (temdelta <= 2000)
		{
			fdeltaY = 200.0f;
		}
		else if (temdelta > 2000 && temdelta <= 3000)
		{
			fdeltaY = 300.0f;
		}
		else if (temdelta > 3000 && temdelta <= 4000)
		{
			fdeltaY = 400.0f;
		}
		else if (temdelta > 4000 && temdelta <= 5000)
		{
			fdeltaY = 500.0f;
		}
		else if (temdelta > 5000 && temdelta <= 6000)
		{
			fdeltaY = 600.0f;
		}
		else if (temdelta > 6000 && temdelta <= 7000)
		{
			fdeltaY = 700.0f;
		}
		else if (temdelta > 7000 && temdelta <= 8000)
		{
			fdeltaY = 800.0f;
		}
		else if (temdelta > 8000 && temdelta <= 9000)
		{
			fdeltaY = 900.0f;
		}
		else
		{
			fdeltaY = 1000.0f;
		}
	}
	else if (temdelta > 100 && temdelta <= 1000)		// 100<temdelta<=1000
	{
		if (temdelta <= 200)
		{
			fdeltaY = 20.0f;
		}
		else if (temdelta > 200 && temdelta <= 300)
		{
			fdeltaY = 30.0f;
		}
		else if (temdelta > 300 && temdelta <= 400)
		{
			fdeltaY = 40.0f;
		}
		else if (temdelta > 400 && temdelta <= 500)
		{
			fdeltaY = 50.0f;
		}
		else if (temdelta > 500 && temdelta <= 600)
		{
			fdeltaY = 60.0f;
		}
		else if (temdelta > 600 && temdelta <= 700)
		{
			fdeltaY = 70.0f;
		}
		else if (temdelta > 700 && temdelta <= 800)
		{
			fdeltaY = 80.0f;
		}
		else if (temdelta > 800 && temdelta <= 900)
		{
			fdeltaY = 90.0f;
		}
		else
		{
			fdeltaY = 100.0f;
		}
	}
	else					//temdelta <= 100ʱ
	{
		fdeltaY = 10.0f;
	}
	
	fdeltaX = (fRight - fLeft) / 4;

	CacheGraphics.DrawLine(&penCoord,point1,point2);//X��
	Point ptx1(428,165);	//rect.Right - 32, rect.Buttom - 20	//��ͷ��
	Point ptx2(428,155);	//rect.Right - 32, rect.Buttom - 30
	Point ptx3(443,160);	//rect.Right - 17, rect.Buttom - 25
	Point ptx[] = {ptx1, ptx2, ptx3};
	SolidBrush  coorbrush(Color(255, 255, 0, 0));
	PointF pt_YBtom_txt(0.0f,153.0f);				//Y�����±� m_Buttom

	CacheGraphics.FillPolygon(&coorbrush,ptx,3);	//X���ͷ
	
	strButtom.Format("%f",(float)m_buttom);	
	CacheGraphics.DrawString(strButtom.AllocSysString(),5,&font, pt_YBtom_txt,&brush);
	
	//���Ʊ��
	for (int i = 0; i < 5; i++)
	{
		int temp1 = 40 + 95 * i;		//X����Ʊ�ߵ�λ��
		float tempNumX = 0.0f;			
		tempNumX = fLeft + fdeltaX * i;	//X���ߵ���ֵ
		Point ptCorX1(temp1, 160);
		Point ptCorX2(temp1, 155);
		PointF ptX_txt(temp1 - 20, 165);

		CacheGraphics.DrawLine(&penCoord, ptCorX1, ptCorX2);

		str.Format("%f", tempNumX);
		CacheGraphics.DrawString(str.AllocSysString(), 8, &font, ptX_txt, &brush);

	}
	for (int i =1; i <=10; i++)
	{
		int temp2 = rect.bottom - (temp * i +25);	//Y���ߵ���ֵ
		Point pt1(40,temp2);
		Point pt2(420,temp2);
		PointF ptY_txt(0.0f, (float)temp2 - 8);
		CacheGraphics.DrawLine(&penCoord2,pt1, pt2);

		str.Format("%f",(m_buttom + fdeltaY * i));
		CacheGraphics.DrawString(str.AllocSysString(),5,&font, ptY_txt,&brush);
		
	}

	float fDeviation = fdeltaY * 10;	//��������ֵ�����ֵ֮��
	float rateY = 130 / fDeviation;		//��������ֵ�����ֵ���ز�130
	float rateX = 380 / (fRight - fLeft);//���������ֵ���������ز�130
	CacheGraphics.DrawString(L"�߳�",2, &font, pointY, &brush); //Y���ʶ
	
	//����Y��
	CacheGraphics.DrawLine(&penCoord,point3,point4);	
	//����������
	Point pty1(35,20);	//rect.Lift + 35, 20
	Point pty2(45,20);	//rect.Lift + 45, 20
	Point pty3(40,5);	//rect.Lift + 40, 5
	Point pty[] = {pty1, pty2, pty3};
	CacheGraphics.FillPolygon(&coorbrush,pty,3);

	if (m_buttom < 0)								//���Ƶ�ƽ��
	{
		float m_horizon = 160 - ( - m_buttom * 130) / fDeviation;
		PointF ptHoriz(420.0f,(m_horizon-5));
		str.Format("��ƽ��0.00");
		PointF ptHoriz1(40.0f, m_horizon);
		PointF ptHoriz2(420.0f, m_horizon);
		CacheGraphics.DrawLine(&penHorizon,ptHoriz1,ptHoriz2);
		CacheGraphics.DrawString(str.AllocSysString(),7,&font, ptHoriz, &brush);

	}
	//��������ƶ�����
	if (drawMouseLine)
	{
		if (mousPosX > 38 && mousPosY <162)
		{
			
			Point pointMousX1(40, mousPosY);
			Point pointMousX2(rect.right,mousPosY);
			Point pointMousY1(mousPosX,0);
			Point pointMousY2(mousPosX,160);	//Y 160 = rect.Buttom - 25
			Pen penMous(Color( 0, 0, 0), 1.0f);
			float temMousY = (float)(160 - mousPosY) / rateY + m_buttom;
			float temMousX = (float)(mousPosX - 40) / rateX + fLeft;
			int mouspos = 0;
			if (mousPosX > 370)
			{
				mouspos = mousPosX - 55;
			}
			else
			{
				mouspos = mousPosX;
			}
			PointF ptMous_txt1((float)(mouspos), (float)mousPosY - 16);
			PointF ptMous_txt2((float)(mouspos), (float)mousPosY - 30);

			penMous.SetEndCap(LineCapRound);			
		
			str.Format("�߳�ֵ:%f", temMousY);
			strButtom.Format("ˮƽֵ:%f",temMousX);

			CacheGraphics.DrawString(str.AllocSysString(),12,&font, ptMous_txt1, &brush);
			CacheGraphics.DrawString(strButtom.AllocSysString(),12, &font, ptMous_txt2, &brush);

			CacheGraphics.DrawLine(&penMous,pointMousX1, pointMousX2);
			CacheGraphics.DrawLine(&penMous,pointMousY1, pointMousY2);
		}
	}
	
	//���Ƹ߳���
	std::vector <vgKernel::Vec3*> ::iterator iterBegin = vec_htPoint.begin();
	std::vector <vgKernel::Vec3*> ::iterator iterEnd = vec_htPoint.end();
	for (; iterBegin != iterEnd; iterBegin++)
	{
		PointF htpt1((float)(((*iterBegin)->x - fLeft) * rateX + 40),(float)(160 - ((*iterBegin)->y - m_buttom) * rateY));
		if (m_tempPoint_stck.empty())
		{
			m_tempPoint_stck.push((*iterBegin));
		}
		else
		{
			vgKernel::Vec3 m_pt = *(m_tempPoint_stck.top());
			m_pt.x =(float)((m_pt.x - fLeft) * rateX + 40);
			m_pt.y = (float)(160 - (m_pt.y - m_buttom) * rateY);
			PointF m_pt1(m_pt.x, m_pt.y);
			CacheGraphics.DrawLine(&penHtLine,m_pt1,htpt1);
			m_tempPoint_stck.push((*iterBegin));
		}
	}
	
	while (!m_tempPoint_stck.empty())//���ջ
	{
		m_tempPoint_stck.pop();
	}
	vg_graphics.DrawImage( &CacheImage, 0,0);

	ReleaseDC(pDC);
}

void CutSurfView::setCoordinate()//	��ȡ�̵߳��X Y���ֵ����Сֵ
{
	render_Height();
	std::vector <vgKernel::Vec3*> ::iterator iterBegin = vec_htPoint.begin();
	std::vector <vgKernel::Vec3*> ::iterator iterEnd = vec_htPoint.end();
	float x_maxNum = 0.0f;
	float y_maxNum = 0.0f;
	float x_minNum = 32767.0f;
	float y_minNum = 32767.0f;

	for (; iterBegin != iterEnd; iterBegin++)
	{
		if (m_tempPoint_stck.empty())
		{
			m_tempPoint_stck.push((*iterBegin));
		}
		else
		{
			vgKernel::Vec3 m_pt = *(m_tempPoint_stck.top());
			lineLength += m_pt.distance(*(*iterBegin));
			m_tempPoint_stck.push((*iterBegin));
		}

		if ((*iterBegin)->x > x_maxNum)
		{
			x_maxNum = (*iterBegin)->x;
		}
		if ((*iterBegin)->x < x_minNum)
		{
			x_minNum = (*iterBegin)->x;
		}

		if ((*iterBegin)->y > y_maxNum)
		{
			y_maxNum = (*iterBegin)->y;
		}
		if ((*iterBegin)->y < y_minNum)
		{
			y_minNum = (*iterBegin)->y;
		}
	}

	while (!m_tempPoint_stck.empty())//���ջ
	{
		m_tempPoint_stck.pop();
	}

	fLeft = x_minNum;
	fRight = x_maxNum;
	fTop = y_maxNum;
	fButtom = y_minNum;
}

void CutSurfView::render_Height()
{
	if (heightPoint != NULL)
	{
		Vec3Vector::iterator iter = heightPoint->begin();
		Vec3Vector::iterator iter_end = heightPoint->end();

		if (!vec_htPoint.empty())
		{
			vec_htPoint.clear();
		}
		for ( ; iter != iter_end ; ++ iter )
		{
			vgKernel::Vec3 *temppt1 = new vgKernel::Vec3(iter->x, iter->y, iter->z);
		 	vec_htPoint.push_back(temppt1);

		}
	}	

}

int CutSurfView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return MA_ACTIVATE;
	//return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CutSurfView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	Render();
	CView::OnTimer(nIDEvent);
}


void CutSurfView::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CView::OnMButtonDblClk(nFlags, point);
}
