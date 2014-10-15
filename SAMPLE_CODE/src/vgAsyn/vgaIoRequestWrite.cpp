


#include <vgStableHeaders.h>
#include <vgAsyn/vgaIoRequestWrite.h>


namespace vgAsyn {
	
	
	
	
	//----------------------------------------------------------------
	void IoRequestWrite::sendIoRequestWrite()
	{
		assert( _fileInfo->_fileHandle != NULL );

		assert( _buffer != NULL );

		if ( _buffer == NULL )
		{
			return;
		}

		//------------------------------------------
		// ��ʼ��ʽд������
		//------------------------------------------
		bool readret = _fileInfo->_fileHandle->doWrite( _buffer , _requestStruct );

		assert( readret == true );

		if ( readret == false )
		{
			MessageBox( NULL , "��������" , "Error!!" , MB_OK );
			return;
		}
	}
	//----------------------------------------------------------------


}// end of namespace vgAsyn
