


#ifndef __VGCAACTIONHOLDER_H__
#define __VGCAACTIONHOLDER_H__
#include <vgCam/vgcaDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgCam/vgcaAction.h>

	
namespace vgCam {

	/**
		@date 	2009/02/06  15:35	
		@author  leven
	
		@brief 	
	
		@see    
	*/
	class  VGCA_EXPORT ActionHolder
	{
	public:
		ActionHolder()
		{
			
		}
		~ActionHolder()
		{
			
		}
	
	public:


		virtual bool registerAction( ActionPtr act );

		virtual void clearActions()
		{
			_actionMap.clear();
		}

		void deleteActionByActionPtr( ActionPtr act );

		void deleteActionByActionType( const ActionType& act_type );
		

		ActionPtrMap* getActions()
		{
			return &_actionMap;
		}

		//------------------------------------------
		// һ��Ҫע������,action��doaction��ʱ��,
		// ���ܹ���ִ��deleteAction�����,��Ȼ����
		// ���벻���ķ���.
		//------------------------------------------
		bool doActionByActionType( const ActionType& act_type );

		ActionPtr getActionPtrByActionType( const ActionType& act_type )
		{
			ActionPtrMap::iterator iter = _actionMap.find( act_type );

			if ( iter != _actionMap.end() )
			{
				return iter->second;
			}

			return ActionPtr();
		}

	protected:

		ActionPtrMap _actionMap;
	
	};
	
	
}// end of namespace vgCam
	


#endif // end of __VGCAACTIONHOLDER_H__