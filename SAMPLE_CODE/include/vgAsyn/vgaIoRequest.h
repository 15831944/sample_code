



#ifndef __VGAIOREQUEST_H__
#define __VGAIOREQUEST_H__
#include <vgAsyn/vgaDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgAsyn/vgaIoRequestStruct.h>
#include <vgAsyn/vgaFileInfo.h>


namespace vgAsyn {




	/**
		@date 	2008/06/20  15:39	
		@author  leven
	
		@brief 	
			ע������:
				1.�����Լ�������ioRequstHandlerFunc����
				mainThreadHandlerFunc��ɾ������,��Ҫִ��:
					delete request;
				2.����2���߳��еĲ�ͬ�Ĳ�������.
				 ����ִ��ioRequstHandlerFunc,��������������
				 ִ����dispatchMainThreadFunc,��_mainThreadFunc��ִ��,
				 ��֮����ִ��,����IoRequestExtractVgimg��.
				3.���ڶ�ȡ����buffer,��ʱ��ϣ���Զ�ɾ��(����texture������ɺ�)
				��ʱ��ϣ�����Զ�ɾ��(���綥��������ʽ��Ⱦ),ֻҪ���ñ�־λ����:
					void setDeleteBufferFlag( const bool& flag )
	
				4.����ʵ����,����ע��û���Զ�ɾ��.
				��Ҫ�Զ�ɾ������,����anotherThreadHandler����mainThreadHandler
				�����delete this���.
				
				//08-11-8���.

		@see    
	*/
	class  VGA_EXPORT IoRequest
	{
	public:
		// ע��,���ฺ��delete
		IoRequest( FileInfo* fileinfo ); 

		virtual ~IoRequest();

		/**
			��run_immediately = true,��ʱ��������,������
			�����Ŷ�
		*/
		void sendRequestAndDoAsynHandlerFunc( 
			const bool& run_immediately = false );

		/**
			����ÿ֡���͵����ݰ��ܶ�,����Ҫ������
			�����ݰ��ѻ��ܶ�ʱ,��Щio����Ͳ���Ҫ�ٴη���.
			�����������sendRequestAndDoAsynHandlerFunc֮��
			���,������Ҫ,��ɾ������(��MainThreadMessageManager��).
			Ĭ��Ϊ����
			for MainThreadMessageManager only
		*/
		bool isReallyNeedToSend()
		{
			return true;
		}

		/**
			for MainThreadMessageManager only
		*/
		void sendRequestAndDoAsynHandlerFuncImpl();

		void handleRequest()
		{
			anotherThreadHandler();
		}

		void handleMainThreadRequest()
		{
			mainThreadHandler();
		}


		virtual bool anotherThreadHandler()
		{
			return true;
		}

		virtual bool mainThreadHandler()
		{
			return true;
		}

		/**
			�Ƿ�Ϊ�첽ģʽ.Ĭ��Ϊ�첽ģʽ
			true --> �첽ģʽ
		*/
		void setAsynchronousMode( const bool& async_mode )
		{
			_requestStruct->setAsynMode( async_mode );
		}

		/**
			�õ��첽ģʽ��־  
			true --> �첽ģʽ
		*/
		bool getAsynChronousMode() const
		{
			return _requestStruct->isAsynMode();
		}


		void dispatchMainThreadFunc();

		void setDeleteBufferFlag( const bool& flag )
		{
			_deleteBufferWhenDestruction = flag;
		}
	
		char* getBuffer() const
		{
			return _buffer;
		}

		inline long getBufferSize() const
		{
			return _fileInfo->_fileSize;
		}

		inline FileInfo* getFileInfoClone()
		{
			return _fileInfo->clone();
		}

		inline FileInfo* getFileInfoRef() 
		{
			return _fileInfo;
		}

		inline bool getPriority() const
		{
			return _priority;
		}

		inline void setPriority( const bool& priority )
		{
			_priority = priority;
		}

	protected:

		// ���ȼ�
		// trueΪ�����ȼ�
		// false�ǵ����ȼ�
		volatile bool _priority;

		bool _deleteBufferWhenDestruction;

		FileInfo* _fileInfo;

		char* _buffer;

		IoRequestStruct* _requestStruct;
	
	};
	

	typedef std::vector<IoRequest> IoRequestVector;



	
}// end of namespace vgAsyn

#endif // end of __VGAIOREQUEST_H__