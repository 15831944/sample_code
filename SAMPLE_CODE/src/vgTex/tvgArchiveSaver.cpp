
#include <vgStableHeaders.h>

#include "vgtex/tvgArchiveSaver.h"
#include <vgTex/tvgImageFactory.h>


namespace vgCore {
	
	
	
	
	//----------------------------------------------------------------
	bool ArchiveSaver::saveToStream( const MetaSaverListPtr& saver_list )
	{
		if ( saver_list.isNull() )
		{
			return false;
		}

		MetaSaverList::iterator iter = saver_list->begin();
		MetaSaverList::iterator iter_end = saver_list->end();

		for ( ; iter != iter_end ; ++iter )
		{
			assert( !iter->first.isNull() );
			assert( !iter->second.isNull() );


			//////����Ҫ�ģ�����������������ֱ��loadFromFileInfo���ɣ�����

			ImagePtr pimg = ImageFactory::createFromFileFormat( iter->first->filename );

			if( !pimg->loadFromFileInfo( iter->first ) )
			{
				VG_WARN(Warning::WARN_FILE_NOT_FOUND,
					Warning::BUTTON_NONE,
					"��ȡԭ�ļ�ʱʧ��." ,
					"ArchiveSaver::saveToStream");

				continue;
			}

			if ( !pimg->saveToWriter( iter->second ) )
			{
				VG_WARN(Warning::WARN_CANNOT_WRITE_TO_FILE,
					Warning::BUTTON_NONE,
					"��ȡ�ļ�ʱʧ��.ԭ�ļ�:" + iter->first->filename 
					+ " . Ŀ���ļ�: " + iter->second->aimFilename ,
					"ArchiveSaver::saveToStream");

				continue;
			}

			/*
			ImagePtr pimg = ImageFactory::createFromFileFormat( iter->first->filename );

			if( !pimg->loadFromStream( iter->first->streamReaderPtr ,
				iter->first->filename ) )
			{
				VG_WARN(Warning::WARN_FILE_NOT_FOUND,
					Warning::BUTTON_NONE,
					"��ȡԭ�ļ�ʱʧ��." ,
					"ArchiveSaver::saveToStream");

				continue;
			}

			if ( !pimg->saveToWriter( iter->second ) )
			{
				VG_WARN(Warning::WARN_CANNOT_WRITE_TO_FILE,
					Warning::BUTTON_NONE,
					"��ȡ�ļ�ʱʧ��.ԭ�ļ�:" + iter->first->filename 
					+ " . Ŀ���ļ�: " + iter->second->aimFilename ,
					"ArchiveSaver::saveToStream");

				continue;
			}
			*/
		}

		return true;
	}

	bool ArchiveSaver::saveToStream( pair<FileInfoPtr, WriterInfoPtr>& input_param )
	{
		assert( !input_param.first.isNull() );
		assert( !input_param.second.isNull() );

		ImagePtr pimg = ImageFactory::createFromFileFormat( input_param.first->basename );

		if( !pimg->loadFromFileInfo( input_param.first ) )
		{
			VG_WARN(Warning::WARN_FILE_NOT_FOUND,
				Warning::BUTTON_NONE,
				"��ȡԭ�ļ�ʱʧ��." ,
				"ArchiveSaver::saveToStream");

			return false;
		}

		if ( !pimg->saveToWriter( input_param.second ) )
		{
			VG_WARN(Warning::WARN_CANNOT_WRITE_TO_FILE,
				Warning::BUTTON_NONE,
				"��ȡ�ļ�ʱʧ��.ԭ�ļ�:" + input_param.first->filename 
				+ " . Ŀ���ļ�: " + input_param.second->aimFilename ,
				"ArchiveSaver::saveToStream");

			return false;
		}

		return true;
	}

	bool ArchiveSaver::saveHeader( const StreamWriterPtr& pwriterstream , const StringPairListPtr& pstrpairlist )
	{
		if ( pstrpairlist.isNull() )
		{
			return false;
		}
		if ( pwriterstream.isNull() )
		{
			return false;
		}

		return true;
	}
	//----------------------------------------------------------------


}// end of namespace vgCore
