



#ifndef __VGTERRITORY_TIOREQUESTELEVATION_H__
#define __VGTERRITORY_TIOREQUESTELEVATION_H__


#include <vgTerritory/vgtDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgAsyn/vgaIoRequest.h>
#include <vgTerritory/vgtElevationManager.h>
	
namespace vgTerritory {

	/**
		@date 	2008/09/08  9:24	
		@author  leven
	
		@brief 	�õ��̵߳�io����.ע��!����Ϊͬ��ģʽ.
		���캯�����Ѿ��趨��ͬ��ģʽ.
	
		@see    
	*/
	class  VGT_EXPORT IoRequestElevation : public vgAsyn::IoRequest
	{
	public:
		IoRequestElevation( Elevation* elev );
		virtual ~IoRequestElevation()
		{

		}



		virtual bool anotherThreadHandler();

		virtual bool mainThreadHandler();

		virtual bool isReallyNeedToSend();

	private:


		int _lod;

		String _elevItemName;

		Elevation* _oldElev;
	
	};
	
	
}// end of namespace vgTerritory
	


#endif // end of __VGTERRITORY_TIOREQUESTELEVATION_H__