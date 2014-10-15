

#ifndef __TVGFILESTREAMREADER_H__
#define __TVGFILESTREAMREADER_H__

#include <vgCore/vgPrerequisites.h>
#include <vgTex/tvgStreamReader.h>
#include <vgTex/tvgStringUtil.h>

namespace vgCore {

	class  VGDEP_EXPORT FileStreamReader : public StreamReader
	{
	protected:

		bool _isValid;
		HANDLE _filehandle;


	public:

		///���캯��.ע��freeOnClose����;:��������ʱ������ɾ��ifstream
		///������һ��ifstream��ʵ��,��Ҫע�ⲻҪ����2����������.

		FileStreamReader()
		{
			_isValid = false;
			_filehandle = INVALID_HANDLE_VALUE;
			mSize = 0;
			mName = "";
		}
		FileStreamReader(const String& filename)
		{
			_isValid = false;
			_filehandle = INVALID_HANDLE_VALUE;

			create( filename );
		}

		~FileStreamReader()
		{
			if( _isValid == true )
			{
				close();
			}
		}

		virtual bool create( const String& filename )
		{
			// Open the file
			_filehandle = CreateFile(filename.c_str(),
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN,
				NULL);

			if( _filehandle == INVALID_HANDLE_VALUE)
			{
				return false;
			}

			DWORD err = GetLastError();
			assert( err == ERROR_SUCCESS );

			assert( _filehandle != INVALID_HANDLE_VALUE );

			//------------------------------------------
			// �õ��ļ��Ĵ�С
			//------------------------------------------
			DWORD filesize = GetFileSize( _filehandle , NULL);
			assert( filesize > 0 );
			assert( filesize != INVALID_FILE_SIZE );

			if ( filesize == INVALID_FILE_SIZE )
			{
				assert(0&&"������");
				return false;
			}

			//------------------------------------------
			// ���ֵ
			//------------------------------------------
			mSize = filesize;
			mName = filename;

			_isValid = true;

			return true;
		}

		virtual bool isValid() 
		{
			return _isValid;
		}

		virtual size_t read(void* buf, size_t count);

		void skip(long count)
		{
			assert( _isValid == true );

			assert( count > 0 );

			DWORD dwCurrentFilePosition; 

			dwCurrentFilePosition = SetFilePointer( 
				_filehandle, // must have GENERIC_READ and/or GENERIC_WRITE 
				count,     // do not move pointer 
				NULL,  // hFile is not large enough to need this pointer 
				FILE_CURRENT);  // provides offset from current position 
		}

		void seek( size_t pos )
		{
			assert( _isValid == true );

			assert( pos >= 0 );

			DWORD dwCurrentFilePosition; 

			dwCurrentFilePosition = SetFilePointer( 
				_filehandle, // must have GENERIC_READ and/or GENERIC_WRITE 
				pos,     // do not move pointer 
				NULL,  // hFile is not large enough to need this pointer 
				FILE_BEGIN);  // provides offset from current position 

			assert( dwCurrentFilePosition == pos );
		}

		size_t tell(void) const
		{
			if ( _isValid == false )
			{
				assert(0);
			}

			assert( _isValid == true );

			DWORD dwCurrentFilePosition; 

			dwCurrentFilePosition = SetFilePointer( 
				_filehandle, // must have GENERIC_READ and/or GENERIC_WRITE 
				0,     // do not move pointer 
				NULL,  // hFile is not large enough to need this pointer 
				FILE_CURRENT);  // provides offset from current position 

			return dwCurrentFilePosition;
		}

		bool eof(void) const
		{
			if ( tell() == mSize )
			{
				return true;
			}

			return false;
		}

		void close(void)
		{
			assert( _isValid == true );
			CloseHandle( _filehandle );
			_filehandle = INVALID_HANDLE_VALUE ;
			_isValid = false;
			mSize = 0;
			mName = "";
		}

	};


#if 0 
	class FileStreamReader : public StreamReader
	{
	protected:

#define VGES_FILESTREAM_LINE_TEMP_SIZE 300

		static vgThreads::CriticalSection mCriticalSect;

		/// Reference to source stream
		std::ifstream* mpStream;
		bool mFreeOnClose;		

	public:

		///���캯��.ע��freeOnClose����;:��������ʱ������ɾ��ifstream
		///������һ��ifstream��ʵ��,��Ҫע�ⲻҪ����2����������.

		FileStreamReader();
		FileStreamReader(const String& filename);

		FileStreamReader(std::ifstream* s, 
			bool freeOnClose = true);

		FileStreamReader(const String& name, 
			std::ifstream* s, 
			bool freeOnClose = true);

		FileStreamReader(const String& name, 
			std::ifstream* s, 
			size_t size, 
			bool freeOnClose = true);

		~FileStreamReader();



		virtual bool create( const String& filename )
		{
			std::ifstream *filestream = 
				new std::ifstream( filename.c_str() , std::ifstream::binary);

#if _MSC_VER == 1400	
			if ( filestream->fail() )
			{
				filestream->clear();

				delete filestream;

				return false;
				/*
				// ��ʱʹ��WString���г���
				filestream->open( 
					StringUtil::convertStringToWString(filename).c_str() );

				if ( filestream->fail() )
				{
					delete filestream;
					return false;
				}
				*/
			}
#else
	#if _MSC_VER == 1200
				if ( filestream->fail() )
				{
					delete filestream;
					return false;
				}
	#else
		#error IDE is not VC6 or VC8!
	#endif
#endif

			this->create( filestream );
			return true;
		}

		inline void create(std::ifstream* s, bool freeOnClose = true )
		{
			mpStream = s;
			mFreeOnClose = freeOnClose;

			// calculate the size
			mpStream->seekg(0, std::ios_base::end);
			mSize = mpStream->tellg();
			mpStream->seekg(0, std::ios_base::beg);
		}

		inline bool fail()
		{
			return mpStream->fail();
		}

		template<typename T> 
		FileStreamReader& operator >> (T& val)
		{
			/// ע�����ﲻ����String����.��Ȼ��׼��std::fstream����.
			/// template���������ص�.
			(*this->mpStream) >> val;
			return *this;
		}





		virtual size_t read(void* buf, size_t count);

		virtual size_t readLine(char* buf, size_t maxCount, const String& delim = "\n");

		inline String readLine(const String& delim = "\n" )
		{
			char *buf = new char[VGES_FILESTREAM_LINE_TEMP_SIZE];
			memset(buf,0,VGES_FILESTREAM_LINE_TEMP_SIZE);
			readLine( buf , VGES_FILESTREAM_LINE_TEMP_SIZE , delim );

			String res(buf);
			return res;
		}


		void skip(long count);

		void seek( size_t pos );

		size_t tell(void) const;

		bool eof(void) const;

		void close(void);

	};
#endif

	
	typedef SharedPtr<FileStreamReader> FileStreamReaderPtr;
	
	
}// end of namespace

#endif // end of __TVGFILESTREAMREADER_H__