


#include <vgStableHeaders.h>
#include <vgAsyn/vgaIoRequest.h>
#include <vgAsyn/vgaMainThreadMessageManager.h>

#include <vgAsyn/vgaMemoryFileManager.h>



namespace vgAsyn {
	
	
	
	
	//----------------------------------------------------------------
	IoRequest::IoRequest( FileInfo* fileinfo )
		: _fileInfo( fileinfo ),
		_deleteBufferWhenDestruction( true ),
		_buffer( NULL ),
		_priority( false ),
		_requestStruct( NULL )
	{
		assert( fileinfo != NULL );
		assert( _fileInfo->_fileSize > 0 );

		_requestStruct = new IoRequestStruct( this , 
			fileinfo->_fileOffset , fileinfo->_fileSize );
	}
	//----------------------------------------------------------------
	IoRequest::~IoRequest()
	{
		if ( _fileInfo != NULL )
		{
			delete _fileInfo;
			_fileInfo = NULL;
		}

		if ( _deleteBufferWhenDestruction == true )
		{
			if ( _buffer != NULL )
			{
				delete[] _buffer;
				_buffer = NULL;
			}
		}

		if ( _requestStruct != NULL )
		{
			delete _requestStruct;
			_requestStruct = NULL;
		}
	}
	//----------------------------------------------------------------
	void IoRequest::dispatchMainThreadFunc()
	{
		if ( _requestStruct->isAsynMode() == false )
		{
			// ͬ��ģʽʱֻҪֱ��ִ�м���.
			handleMainThreadRequest();
		}
		else
		{
			MainThreadMessageManager::getSingleton().pushMainHandlerOfIoRequest( this );
		}
	}
	//----------------------------------------------------------------
	void IoRequest::sendRequestAndDoAsynHandlerFunc(const bool& run_immediately /*= false*/)
	{
		if ( _requestStruct->isAsynMode() == false ||
			run_immediately == true )
		{
			//------------------------------------------
			// ��Ϊͬ��ģʽ����run_immediatelyģʽ,ֱ��ִ�м���
			//------------------------------------------
			sendRequestAndDoAsynHandlerFuncImpl();
		}
		else
		{
			//------------------------------------------
			// ����ÿһ֡���͵����ݰ�������
			//------------------------------------------
			MainThreadMessageManager::getSingleton().pushSenderOfIoRequest( this );
		}
	}
	//----------------------------------------------------------------
	void IoRequest::sendRequestAndDoAsynHandlerFuncImpl()
	{
		assert( _fileInfo->_fileHandle != NULL );
		//------------------------------------------
		// ���ȴ��ڴ��ļ��н��в���
		//------------------------------------------
		char* bufread = 
			MemoryFileManager::getSingleton().readFromMemory( _fileInfo );

		if ( bufread != NULL )
		{
			// ���ʱ��ֱ�Ӷ����ˡ�
			_buffer = bufread;
			handleRequest();
			return;
		}


		//------------------------------------------
		// buffer����������.
		// ����ڹ��캯��������,�п��ܻᱨ�ڴ�ERROR_NOACCESS����.
		//------------------------------------------
		if( _buffer != NULL )
		{
			assert(0);
		}

		if ( _buffer == NULL )
		{
			_buffer = new char[ _requestStruct->_readSize ];
		}
		assert( _buffer != NULL );

		//------------------------------------------
		// ��ʼ��ʽ��ȡ����
		//------------------------------------------
		bool readret = _fileInfo->_fileHandle->doRead( _buffer , _requestStruct );

		assert( readret == true );

		if ( readret == false )
		{
			MessageBox( NULL , "��������" , "Error!!" , MB_OK );
			return;
		}
	}
	//----------------------------------------------------------------



}// end of namespace vgAsyn
