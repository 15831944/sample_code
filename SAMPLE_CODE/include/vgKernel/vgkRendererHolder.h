


#ifndef __VGKRENDERERHOLDER_H__
#define __VGKRENDERERHOLDER_H__

#include <vgKernel/vgkForward.h>


	
namespace vgKernel {

	class Renderer;

	/**
		@date 	2009/01/04  15:35	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class  VGK_EXPORT RendererHolder
	{
	public:
		RendererHolder( bool auto_register = true );
		virtual ~RendererHolder();

	public:



		virtual bool onDeleteRenderer( Renderer* renderer ) = 0;

	protected:

		void registerToHolderMananger();

	private:

	};

	typedef std::vector<RendererHolder*> RendererHolderPtrVec;
	
	
}// end of namespace vgKernel
	


#endif // end of __VGKRENDERERHOLDER_H__

 