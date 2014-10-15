


#ifndef __VGKLOGMANAGER_H__
#define __VGKLOGMANAGER_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSingleton.h>
#include <vgKernel/vgkLog.h>

	
namespace vgKernel {

	/**
		@date 	2008/08/01  21:14	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class  VGK_EXPORT LogManager : public vgKernel::Singleton<LogManager>
    {
		friend class vgKernel::Singleton<LogManager>;
	private:
		LogManager() 
			: vgKernel::Singleton<LogManager>( VGK_SINGLETON_LEFE_LOGMANAGER ) ,
			mDefaultLog( NULL )
		{
		}

		virtual~LogManager()
		{
			LogList::iterator i;
			for (i = mLogs.begin(); i != mLogs.end(); ++i)
			{
				delete i->second;
			}
		}

	protected:

		virtual bool initialise()
		{
			Log* log = createLog();
			return log != NULL;
		}
		virtual bool shutdown() 
		{
			destroyAllLogs();
			return true;
		}
	protected:

        typedef std::map<String, Log*, std::less<String> >	LogList;
        LogList mLogs;
        Log* mDefaultLog;

    public:

		virtual void reset(){};

        Log* createLog( const String& name = (""),
			bool defaultLog = false, bool debuggerOutput = true, 
			bool suppressFileOutput = false);

		Log* getLog( const String& name);

        Log* getDefaultLog();

		void destroyLog(const String& name);
		void destroyLog(Log* log);

		void destroyAllLogs();


        Log* setDefaultLog(Log* newLog);

 
        void logMessage( const String& message, LogMessageLevel lml = LML_NORMAL, 
            bool maskDebug = false);


        void logMessage( LogMessageLevel lml, const String& message,  
			bool maskDebug = false) { logMessage(message, lml, maskDebug); }


        void setLogDetail(LoggingLevel ll);
    };

	
	
}// end of namespace vgKernel
	


#endif // end of __VGKLOGMANAGER_H__