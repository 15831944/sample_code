#ifndef __VGPLANSHPGROUND__
#define __VGPLANSHPGROUND__

#include <vgShape/vgshGeometry.h>
#include <vgShape/vgshSelector.h>
#include <vgPlan/vgPlanground.h>
#include <vgShape/vgshLayer.h>

#include <vgKernel/vgkForward.h>
#include <vgShape/vgshDefinition.h>
#include <vgKernel/vgkSharePtr.h>
#include <vgPlan/vgpShpArea.h>

namespace  vgPlan
{
	/*
		��ʼ��ͼ�࣬���ڴ����Ϲ������ķ��õ�ͼ
	*/

	class vgShpPlanground : public vgShpArea
	{
	public:

		vgShpPlanground()
		{
		}
		
		vgShpPlanground(OGRGeometry* pGemo) 
		{
			__super::setGeometry(pGemo);
		}

		~vgShpPlanground()
		{
		}



		bool diffGemotry(OGRGeometry *pGemo);
		

	private:
// 		// ʹ�õ�GeometryӦ����clone�����ģ�������ܳ�ΪҰָ��
//  		OGRGeometry *m_pGeometry;
//  
//  		string m_groundName;

	};

	// ����ָ������.
	typedef vgKernel::SharePtr<vgPlan::vgShpPlanground> vgShpPlangroundPtr;
}

#endif