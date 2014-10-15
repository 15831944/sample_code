


#ifndef __VGAMAINTHREADMESSAGEMANAGER_H__
#define __VGAMAINTHREADMESSAGEMANAGER_H__
#include <vgAsyn/vgaDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSingleton.h>
#include <vgKernel/vgkTrace.h>
#include <vgThreads/CriticalSection.h>
#include <vgAsyn/vgaIoRequest.h>

#include <vgAsyn/LockFreeQ.h>
#include <vgKernel/vgkRenderCommand.h>

namespace vgAsyn {



	/**
		@date 	2008/06/25  9:58	
		@author  leven
	
		@brief 	��������:
			1.IoRequest��dispatchMainThreadFunc()ִ��֮�󷵻����߳�
			ִ��bool IoRequestTextureLod::mainThreadHandler();����.
			2.����ÿһ֡���͵�IoRequest����.
	
		@see    
	*/
	class  VGA_EXPORT MainThreadMessageManager : 
		public vgKernel::Singleton<MainThreadMessageManager>
	{
		typedef std::deque<IoRequest*> RequestPipe;

		friend class vgKernel::Singleton<MainThreadMessageManager>;
	private:
		MainThreadMessageManager();
		~MainThreadMessageManager();

	protected:
		virtual bool initialise();
		virtual bool shutdown();

	public:

		virtual void reset(){};//need to write

		void clearAllData();

		/**
			(��������1)������������̷߳������̵߳ĺ�������.
		*/
		void pushMainHandlerOfIoRequest( IoRequest* request );


		/**
			��2����:
			1.����ÿ֡��io����(������������Ʒ��͵�����)
			2.���������̷߳��ص�IoRequest������mainHandler
		*/
		void doRequests();
	

		/**
			(��������2)�������IO����
		*/
		void pushSenderOfIoRequest( IoRequest* request );


		int getNumOfRequestToSend() const
		{ 
			return _numOfRequestToSend; 
		}

		void setNumOfRequestToSend( const int& val ) 
		{ 
			_numOfRequestToSend = val; 
		}
		int getNumOfNullRequestToSkip() const
		{ 
			return _numOfNullRequestToSkip; 
		}

		void setNumOfNullRequestToSkip( const int& val ) 
		{ 
			_numOfNullRequestToSkip = val; 
		}

		int getNumOfMainHandlerToProcess() const
		{ 
			return _numOfMainHandlerToProcess; 
		}

		void setNumOfMainHandlerToProcess( const int& val ) 
		{ 
			_numOfMainHandlerToProcess = val; 
		}

		void renderBegin();

		void forceToSynModeAndDealWithAllRequests();

	private:

		/**
			(��������1)���������̺߳���
		*/
		void doMainHandlerOfRequests();

		/**
			(��������2)������IO����
		*/
		void doSenderOfRequests();

		/**
			�����ռ�mainhandler������.
		*/
		RequestPipe _requestsOfMainHandler;
		RequestPipe _requestBufOfMainHandler;
		vgThreads::CriticalSection _requestsSectOfMainHandler;

		/**
			����ÿ֡����IO�����Pipe
		*/
		RequestPipe _requestsOfSender;

		int _numOfRequestToSend;
		int _numOfNullRequestToSkip;
		int _numOfMainHandlerToProcess;

		bool _forceToSynMode;

		vgKernel::RenderCommand *m_renderBeginCommand;

	};
	

	
	
}// end of namespace vgAsyn

#endif // end of __VGAMAINTHREADMESSAGEMANAGER_H__