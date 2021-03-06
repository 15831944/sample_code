


#ifndef __VGKETERNITYINTERFACE_H__
#define __VGKETERNITYINTERFACE_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkStreamReader.h>
#include <vgKernel/vgkStreamWriter.h>

//#include <eternity/eternity.hpp>

	
namespace vgKernel {

#if 0
	/**
		@date 	2009/02/13  14:46	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class StreamArchive: public eternity::bin_archive
	{
	public:
		StreamArchive( StreamReaderPtr preader )  
		{
			setStreamReader( preader );
		}

		StreamArchive( StreamWriterPtr pwriter )  
		{
			setStreamWriter( pwriter );
		}

		~StreamArchive() 
		{
		}

	public:

		void setStreamReader( StreamReaderPtr preader )
		{
			// 注意,需要初始化
			archive::init();

			assert( preader.isNull() == false );
			_reader = preader;

			set_loading( true );
		}

		void setStreamWriter( StreamWriterPtr pwriter )
		{
			archive::init();

			assert( pwriter.isNull() == false );
			_writer = pwriter;

			set_loading( false );
		}

		virtual size_t write( const void *buffer, size_t size, size_t count)
		{
			assert( is_storing() );
			assert( _writer.isNull() == false );

			size_t written = 0;
			for ( size_t i=0; i< count; ++i)
				_writer->write( (void*)buffer, size);

			return written;
		}

		virtual size_t read ( void	*buffer, size_t size, size_t count)
		{
			assert( is_loading() );
			assert( _reader.isNull() == false );

			size_t ridden=0;
			for ( size_t i=0; i< count; ++i)
				_reader->read( (char*) buffer, (int) size);

			return ridden;
		}


	private:

		StreamReaderPtr _reader;
		StreamWriterPtr _writer;
	};
	

#endif
	
}// end of namespace vgKernel
	


#endif // end of __VGKETERNITYINTERFACE_H__