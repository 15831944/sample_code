


#ifndef __VGKRENDERERDATAINTERFACE_H__
#define __VGKRENDERERDATAINTERFACE_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkRendererData.h>

	
namespace vgKernel {

	/**
		@date 	2009/01/05  10:28	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class RendererDataInterface
	{
	public:
		RendererDataInterface()
		{
			
		}
		virtual ~RendererDataInterface()
		{
			for (int i=0; i<_rendererDataList.size(); i++)
			{
				if (_rendererDataList[i] != NULL)
				{
					delete _rendererDataList[i];
				}
			}

			_rendererDataList.clear();
		}
	
	public:

		void addRendererData(RendererData* newData)
		{
			if ( std::find(_rendererDataList.begin(), _rendererDataList.end(), newData) == _rendererDataList.end())
			{
				_rendererDataList.push_back(newData);
			}
		}

#if 0
		void removeRendereData(RendererData* pData)
		{
			for (int i=0; i<_rendererDataList.size(); i++)
			{
				if (_rendererDataList[i] == pData)
				{
					_rendererDataList.erase ( _rendererDataList.begin() + i );
				}
			}
		}
#endif

		RendererData* getRendererDataByType( const RendererDataType& type)
		{
			for (int i=0; i<_rendererDataList.size(); i++)
			{
				if (_rendererDataList[i]->dataType == type)
				{
					return _rendererDataList[i];
				}
			}	

			return NULL;
		}

	private:
	
		RendererDataPtrVec _rendererDataList;

	};
	
	
}// end of namespace vgKernel
	


#endif // end of __VGKRENDERERDATAINTERFACE_H__