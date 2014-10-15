



#include <vgStableHeaders.h>
#include <vgAsyn/vgaIoServer.h>

#include <vgAsyn/vgaIoRequestStruct.h>
#include <vgAsyn/vgaIoRequest.h>
#include <vgAsyn/vgaFileHandle.h>


#include <vgConf/vgcoExeConfig.h>
#include <vgConf/vgcoExeConfigRegProxy.h>



namespace vgAsyn {
	


#define VGA_NAME "Asyn"

#define VGA_USEMULTITHREAD		"UseMultithread"
#define VGA_USEMULTITHREAD_DEFAULT		1
#define VGA_USEMULTITHREAD_DEFAULT_STR		"1"



	/**
	�����ﴴ��project config
	*/
	vgConf::ExeConfigRegProxy vga_reg1( 
		VGA_NAME , VGA_USEMULTITHREAD , 
		VGA_USEMULTITHREAD_DEFAULT_STR );




	//----------------------------------------------------------------
	bool IoServer::initialise()
	{
		//------------------------------------------
		// 0.�������ļ��ж�ȡ�߳�ģʽ�ı��
		//------------------------------------------
		bool usemulti = VG_EXECONF_GET_BOOL( 
			VGA_NAME,
			VGA_USEMULTITHREAD );

		//vgConfig::ConfigObject* usemultithreadobj = 
		//	vgConfig::ConfigManager::getSingleton().getObject( "useMultithread" );

		//if ( usemultithreadobj != NULL )
		//{
		//	_useMultithreadMode = usemultithreadobj->getBool();
		//}

		_useMultithreadMode = usemulti;

		//------------------------------------------
		// ��ʼ����
		//------------------------------------------
		if ( !_useMultithreadMode )
		{
			_initialisedOK = true;
			return true;
		}

		if ( _initialisedOK == true )
		{
			return true;
		}

		//------------------------------------------
		// 1.����completion port
		//------------------------------------------
		_completionPort = CreateIoCompletionPort(
			INVALID_HANDLE_VALUE,
			NULL,   // No prior port
			0,      // No key
			0       // Use default  # of threads
			);

		if (_completionPort == NULL)
		{
			fatalError("CreateIoCompletionPort() failed");
			
			return false;
		}

		//------------------------------------------
		// 2.����worker thread
		//------------------------------------------
		SYSTEM_INFO  sysinfo;
		DWORD        dwThreadId;
		DWORD        dwThreads;
		DWORD        i;

		GetSystemInfo(&sysinfo);
		dwThreads = sysinfo.dwNumberOfProcessors * 2+2;
		for (i=0; i<dwThreads; i++)
		{
			HANDLE hThread;
			hThread = CreateThread(
				NULL, 0, &workerThreadFunc, NULL, 0, &dwThreadId
				);

			SetThreadPriority( hThread , THREAD_PRIORITY_IDLE );
			CloseHandle(hThread);
		}


		_initialisedOK = true;

		return _initialisedOK;
	}
	//----------------------------------------------------------------
	DWORD WINAPI IoServer::workerThreadFunc( LPVOID pVoid )
	{
		HANDLE completion_port = 
			IoServer::getSingleton().getCompletionPort();
		assert( completion_port != INVALID_HANDLE_VALUE );

		BOOL    bResult;
		DWORD   dwNumRead;
		DWORD completionKey;

		LPOVERLAPPED lpOverlapped;


		UNREFERENCED_PARAMETER(pVoid);

		//------------------------------------------
		// ����ѭ���Խ���package
		// !!!!!!!!!!!!������Լ��ж�event!!!!!!!!!!!!!!!!!!!!
		//------------------------------------------
		for (;;)
		{
			bResult = GetQueuedCompletionStatus(
				completion_port,
				&dwNumRead,
				&completionKey,
				&lpOverlapped,
				INFINITE );

			if (bResult == FALSE
				&& lpOverlapped == NULL)
			{
				fatalError(
					"ThreadFunc - Illegal call to GetQueuedCompletionStatus");

				return false;
			}

			else if (bResult == FALSE
				&& lpOverlapped != NULL)
			{
				// This happens occasionally instead of
				// end-of-file. Not sure why.
				fatalError(
					"ThreadFunc - I/O operation failed\n" );

				return false;
			}

			else if (dwNumRead == 0)
			{
				fatalError(
					"ThreadFunc - End of file.\n");

				return false;
			}

			//------------------------------------------
			// ���ʱ��õ���ȷ������
			//------------------------------------------
			else
			{
				IoRequestStruct* request_struct = 
					(IoRequestStruct*) lpOverlapped;

				IoRequest* request = request_struct->getIoRequest();

				// ������ɾ��
				request->handleRequest();
			}
		}

	}
	//----------------------------------------------------------------
	bool IoServer::associateWithFileHandle( FileHandle* handle )
	{
		if( _useMultithreadMode == false )
		{
			return true;
		}

		HANDLE ret = CreateIoCompletionPort(
			handle->getHandle(),
			_completionPort,
			0,   // No key
			0  // Use default # of threads
			);

		VG_ASYN_VERIFY( ret );

		return ret != INVALID_HANDLE_VALUE;
	}
	//----------------------------------------------------------------

	
	
}// end of namespace vgAsyn
