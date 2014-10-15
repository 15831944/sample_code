


#ifndef __VGENTRYFACTORY_H__
#define __VGENTRYFACTORY_H__

#include <vgKernel/vgkForward.h>
#include <vgEntry/vgBaseEntry.h>


#include <vgUIController/vgRendererToEntryRegister.h>

	
class  VGUI_EXPORT vgEntryFactory
{
private:
	vgEntryFactory()
	{
		
	}
	~vgEntryFactory()
	{
		
	}


public:

	//------------------------------------------
	// ����дת����Ϣ
	// ��renderer��vgbaseentry��ת��
	//------------------------------------------

	static vgBaseEntry* createEntryFromRenderer( vgKernel::Renderer* renderer );


	static bool registerRendererToEntry( 
		vgUI::RendererToEntryRegister* reg );

private:

	static vgUI::RendererToEntryRegisterPtrVec _regs;
};

#endif // end of __VGENTRYFACTORY_H__