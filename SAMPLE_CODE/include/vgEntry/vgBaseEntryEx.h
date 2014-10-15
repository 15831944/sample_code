


#ifndef __VGBASEENTRYEX_H__
#define __VGBASEENTRYEX_H__

#include <vgKernel/vgkForward.h>
#include <vgEntry/vgBaseEntry.h>
#include <vgUIController/vgPropertyPage.h>
#include <vgUIController/vgTabDef.h>

	

	/**
		@date 	2008/09/11  20:49	
		@author  leven

		@brief 	
		BaseEntry��չ��

		�̳д����ֻҪ��д

			virtual String GetName(); // ��ѡ

			virtual PropertiesParamVector& pushPropertiesParam( 
			PropertiesParamVector& params ); // ����

			virtual void updateUIFromInternal(); // ��ѡ

			virtual void updateInternalFromUI();// ��ѡ

		�⼸����������.

		@see    
	*/
	class  VGUI_EXPORT vgBaseEntryEx :	public vgBaseEntry 
	{
	public:

		vgBaseEntryEx(vgKernel::Renderer* renderer );
		virtual ~vgBaseEntryEx(void);

		virtual void AddNodeTabs();

		virtual int GetSelectedImage() { return 0; }
		virtual int GetUnSelectedImage() { return 1; }

		virtual void OnPropertyChanged(string paramName);

		// �̳���Changed������Object�޸ĺ����Node
		virtual void onChanged(int eventId, void *param){}


		PropertiesParamVector& getPropertiesParamVector()
		{
			return _params;
		}

	protected:

		virtual PropertiesParamVector& pushPropertiesParam( 
			PropertiesParamVector& params ) = 0;

		virtual void updateUIFromInternal() {}

		virtual void updateInternalFromUI( const String& changed_param_name ) {}

	protected:

		void* getConnectPtrByName( const String& param_name );

	private:

		static PropertiesParamVector _params;
	};


#endif // end of __VGBASEENTRYEX_H__