


#ifndef __VGEDITUTILITY_H__
#define __VGEDITUTILITY_H__

#include <vgGlobal/vgglDefinition.h>
#include <vgKernel/vgkForward.h>

#include <vgKernel/vgkSelectManager.h>
#include <vgKernel/vgkRendererManager.h>

using namespace vgKernel;

namespace vgGlobal{

	/**
		@date    2009/10/21  14:08
		@author  zzy

		@brief   

		@see
	*/	

	class	VGGL_EXPORT EditUtility
	{
	public:
		EditUtility()	{}
		~EditUtility()	{}


		static bool     translate();                 //ƽ��
		static bool     rotate();                    //��ת
		static bool     scale();                     //����
		static bool     updateTranslate();
		static bool     updateRotate();
		static bool     updateScale();

		static bool     undo();                      //����
		static bool     redo();                      //����

		static bool     selectionAll();              //ȫ��ѡ��
		static bool     clearSelection();            //ȡ��ѡ��
		static bool     selectionReverse();          //����ѡ��
		static bool     FindObject();                //��������
		static bool     selectShapeRect();           //���ο�ѡ��
		static bool     selectShapePolygon();        //�����ѡ��
		static bool     selectRegionNew();           //�½�ѡ��
		static bool     selectRegionAdd();           //��ӵ�ѡ��
		static bool     selectRegionSubtract();      //��ѡ����ȥ
		static bool     selectRegionIntersect();     //��ѡ������
		static bool     selectModeInclude();         //ȫ��Χģʽ
		static bool     selectModeIntersect();       //���Χģʽ  

		static bool     translateAll();              //ƽ���������� 
		static bool     translateSeleted();          //ƽ��ѡ������

		static bool     setSelTransparent();         //ѡ�������͸��
		static bool     setSelUnTransparent();       //ѡ������ȡ����͸��
		static bool     updateSetSelTransprant();
		static bool     updateSetSelUnTransprant();

		static bool     SingleFaceRenderSelected();  //ѡ�����嵥����Ⱦ  
		static bool     SingleFaceRenderAll();       //��������˫����Ⱦ
		static bool     updateSingleFaceRenderSelect();
		static bool     updateSingleFaceRenderAll();

		//���νӿ�
		static bool     translateRenderers( RendererQueue* queue, const Vec3& trans_vec );

	private:
		static bool     setSelectNodesToTransparent( bool bSet );
		
		
	private:
		static bool m_bSetSelTransprent;      //��־ѡ�нڵ��͸��
		static bool m_bSetSelUnTransprent;    //��־ѡ�нڵ㲻��͸��

		static bool m_bSingleFaceRenderSel;   //��־ѡ�нڵ��Ƿ�����Ⱦ
		static bool m_bSingleFaceRenderAll;   //��־���нڵ��Ƿ�����Ⱦ

		static bool m_bTranslateMode;
		static bool m_bRotateMode;
		static bool m_bScaleMode;

		static bool m_bSelectModeInclude;
		static bool m_bSelectModeIntersect;

	};
}//namespace vgGlobal

#endif//__VGEDITUTILITY_H__
