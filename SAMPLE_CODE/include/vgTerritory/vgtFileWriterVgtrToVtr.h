

#ifndef __VGTERRITORY_TFILEWRITERVGTRTOVTR_H__
#define __VGTERRITORY_TFILEWRITERVGTRTOVTR_H__



#include <vgTerritory/vgtDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgTerritory/vgtFileDefinitionVtr.h>
#include <vgTerritory/vgVgtrFile.h>
#include <vgKernel/vgkProcessObserver.h>


namespace vgTerritory {

	/**
		@date 	2008/09/02  9:33	
		@author  leven
	
		@brief 	
	
		@see    
	*/
	class  VGT_EXPORT FileWriterVgtrToVtr : public vgKernel::ProcessNotifier
	{
	public:
		FileWriterVgtrToVtr()
		{
			
		}
		virtual ~FileWriterVgtrToVtr()
		{
			
		}
	public:


		/**
			���ڴ�vgtr name list���浽vtr�ļ�
		*/
		bool writeToVtrFileFromVgtrNames( const VgtrHeaderPtr vgtrheader,
			const vgKernel::StringVectorPtr input_vgtrs , 
			const String& output_filename , uchar* filledColor );

	private:

		// ȫд��һ��vtr�ļ�
		bool writeToVtrFile( const VgtrHeaderPtr vgtrheader,
			const vgKernel::StringVectorPtr input_vgtrs , 
			const String& output_filename , const long& nextFileID ,
			uchar* filledColor );


	};
	
	
}// end of namespace vgTerritory
	


#endif // end of __VGTERRITORY_TFILEWRITERVGTRTOVTR_H__