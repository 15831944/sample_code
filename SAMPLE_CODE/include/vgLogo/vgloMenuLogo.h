

#ifndef __VGMENULOGO_H__
#define __VGMENULOGO_H__

#include <vgLogo/vgloDefinition.h>
#include <vgLogo/vgloLogoHandle.h>
#include <vgLogo/vgloMenuItem.h>

#include <vgKernel/vgkInputSystem.h>
namespace vgLogo {  
	

	/**
	@date 	2009/11/27  9:53	
	@author  ZhouZY

	@brief 	

	@see    
	*/
	

	class VGL_EXPORT  MenuLogo : public LogoHandle, public vgKernel::InputHandler
	{

	public:

		MenuLogo();

		virtual ~MenuLogo();

	public:

		//�Ӹ���LogoHandle�̳ж���
		virtual void reset();
		virtual void render();

		//InputHandler �̳ж���
		virtual void OnLButtonDown(UINT nFlags, CPoint position);
		virtual void OnMouseMove(UINT nFlags, CPoint position);

		//�Լ�������
		void initMenuLogoLD();   //��ʼ�����½�Logo
		void initMenuLogoRD();   //��ʼ�����½�Logo


		//����Items��״̬--(�����������Ⱦ״̬��������Ⱦ״̬)
		void setItemState( MenuItem* pMenuItem, bool bUpLevel = false );      

		vector<MenuItem*>* getCurrentItems();

		void clearCurrentItemVec();     

		void findCurrentItems( MenuItem* pMenuItem );    //���ҵ�ǰ��Ⱦ��Items

		MenuItem* getRootMenuItemPtr()
		{
			return &m_rootMenu;
		}

	private:

		//���ڲ�ʹ�ã��ǹ����Ľӿ�

		//���¼���Item�Ŀ�ȣ��߶ȼ���������
		//�������ߵݹ����
		void recalculate( MenuItem* pRootMenuItem );   
		void recalculateData( MenuItem* pMenuItem );

		void uninitMenuLogo( MenuItem* pRootMenuItem );  //ж����ͼ����

		//ͨ������ĸ��ڵ㼰��ǰ�ӽڵ����øø��ڵ����ӽڵ����Ⱦ״̬
		void setSubItemsState( MenuItem* pParentItem, MenuItem* pSubItem );

		void render( MenuItem* pRootMenuItem );

		void setVaryType( VaryType varyType )
		{
			m_varyType = varyType;
		}
		
#if 0   //��չΪ�����ָ��ʱLogo���Ч��
		void largenLogo( MenuItem* pMenuItem, vector<MenuItem*>* pMenuItemVec );
		
		void reInitAllItem( MenuItem* pMenuItem );
#endif

	private:
 
		MenuItem            m_rootMenu;        //���˵�Logo

		VaryType            m_varyType;        //����ӿڱ仯

		float               m_dWidth;          //Ĭ��ƽ̨��Ⱦ���ڿ��
		float               m_dHeight;         //Ĭ��ƽ̨��Ⱦ���ڸ߶�

		vector<MenuItem*>   m_currentItemVec;  //��ǰ��Ⱦ��Items

		//ÿ��MenuLogo������������������
		int                 m_lastViewWidth;
		int                 m_lastViewHeight;
		
	};

}

#endif