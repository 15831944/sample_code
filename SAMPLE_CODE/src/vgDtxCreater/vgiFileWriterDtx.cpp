


#include <vgStableHeaders.h>
#include <vgDtxCreater/vgiFileWriterDtx.h>
#include <vgImage/vgiImageUtility.h>
#include <vgImage/vgiFileDefinitionVgi.h>
#include <vgKernel/vgkStreamWriter.h>
#include <vgKernel/vgkStreamReader.h>
#include <vgKernel/vgkStringUtility.h>

	//�����ļ��ĺ���
	BOOL CommonCopyFile(CString SourceFileName, CString DestFileName)
	{
		CFile sourceFile ;
		CFile destFile ;
		CFileException ex;
		if (!sourceFile.Open((LPCTSTR)SourceFileName,CFile::modeRead | CFile::shareDenyWrite, &ex))
		{
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
			CString ErrorMsg = "���ļ���" ;
			ErrorMsg += SourceFileName ;
			ErrorMsg += "ʧ�ܡ�\n������ϢΪ��\n" ;
			ErrorMsg += szError ;
			AfxMessageBox(ErrorMsg);
			return FALSE ;
		}
		else
		{
			if (!destFile.Open((LPCTSTR)DestFileName, CFile::modeWrite | CFile::shareExclusive | CFile::modeCreate, &ex))
			{
				TCHAR szError[1024];
				ex.GetErrorMessage(szError, 1024);
				CString ErrorMsg = "�����ļ���" ;
				ErrorMsg += DestFileName ;
				ErrorMsg += "ʧ�ܡ�\n������ϢΪ��\n" ;
				ErrorMsg += szError ;
				AfxMessageBox(ErrorMsg);
				sourceFile.Close();
				return FALSE ;
			}

			BYTE buffer[4096];
			DWORD dwRead;
			do
			{
				dwRead = sourceFile.Read(buffer, 4096);
				destFile.Write(buffer, dwRead);
			}
			while (dwRead > 0);   
			destFile.Close();
			sourceFile.Close();
		}
		return TRUE ;
	}


	//----------------------------------------------------------------
	bool FileWriterDtx::writeToDtxFile( 
		const vgKernel::StringVectorPtr  input_image,
		const FrameVectorPtr input_frame , 
		const String& output_filename ,
		const int& ratio ,
		const bool& empty_not_insert /*= false */)
	{
		//------------------------------------------
		// ����Ҫ�ļ��
		//------------------------------------------
		if ( vgKernel::StringUtility::getFileExtension( output_filename ) != "dtx" )
		{
			return false;
		}

		if ( input_image.isNull() == true )
		{
			MessageBox( NULL , "����image����Ϊ��" , "Error" , MB_OK );
			return false;
		}

		if ( input_image->empty() == true )
		{
			MessageBox( NULL , "����image����Ϊ��" , "Error" , MB_OK );
			return false;
		}


		//------------------------------------------
		// ȷ��process�Ľ�����
		// ����2�ֱ����ļ���ʼ�Ĺ������ļ���β�Ĺ���
		//------------------------------------------
		int processCounter = 0;
		this->notifyProcessObserversStart( "��ʼ����vgi�ļ�..." ,0,
			(input_image->size()) * 2 - 1 + 2 );

		//------------------------------------------
		// ���ȼ���ļ����Ƿ����,�Ƿ�������
		//------------------------------------------
		{
			std::set<String> names;
			std::pair<std::set<String>::iterator , bool > result;
			vgKernel::StringVector::iterator iter = input_image->begin();
			vgKernel::StringVector::iterator iter_end  =input_image->end();

			for (  ;iter != iter_end ; ++ iter )
			{

				String filename =
					vgKernel::StringUtility::getFilenameFromAbsolutePath( *iter );
				vgKernel::StringUtility::toLowerCase( filename );

				if ( filename.size() >= DTX_FILENAME_LENGTH )
				{
					std::ostringstream o;
					o << "�����ļ���������dds�ļ�:" << filename ;
					MessageBox( NULL , o.str().c_str() , "Error" , MB_OK );
					return false;
				}

				result = names.insert( filename );

				if ( result.second == false )
				{
					std::ostringstream o;
					o << "��������image�ļ�(ע��,vgi�ļ����ִ�Сд): " <<  filename;
					MessageBox( NULL , o.str().c_str() , "Error" , MB_OK );
					return false;
				}
			}
		}


		//------------------------------------------
		// Ȼ�����Ƿ񸲸��ļ�
		//------------------------------------------
		vgKernel::StreamWriterPtr pwriter = 
			vgKernel::StreamWriterFactory::createFromFile( output_filename ,true);

		if ( pwriter.isNull() == true )
		{
			return false;
		}

		if ( pwriter->isValid() == false )
		{
			return false;
		}

		this->notifyProcessObserversPos( "��ʼдdtx�ļ�" , processCounter ++ );

		//------------------------------------------
		// ��ο�ʼд�ļ�,д��dtx
		//------------------------------------------
		std::vector<String> validList;
		DtxHeader header;
		vgKernel::StringVector errList;

		//------------------------------------------
		// 1.��Ԥдһ��header
		// briefд����󼴿�.
		//------------------------------------------
		long filebegin = pwriter->tell();
		assert( filebegin == 0 );
		pwriter->write( &header , sizeof( DtxHeader ) );

		// дdata������
		{
			header.ratio = ratio;

			FrameVector::iterator iter = input_frame->begin();
			FrameVector::iterator iter_end = input_frame->end();


			for (  ;iter != iter_end;  iter++ )
			{

				this->notifyProcessObserversPos( "����Ԥ����" , processCounter ++ );

				long writenum = pwriter->write( &(*iter), sizeof(SingleFrame) );
				assert( writenum == sizeof(SingleFrame) );

				if ( writenum != sizeof(SingleFrame) )
				{
					MessageBox( NULL , "��������" , "Error" , MB_OK );
					return false;
				}

				header.totaltime += (*iter).timelength;
				header.framecount++;

			}

			vgKernel::StringVector::iterator iter_image     = input_image->begin();
			vgKernel::StringVector::iterator iter_image_end = input_image->end();

			for ( ; iter_image != iter_image_end; iter_image++ )
			{
				this->notifyProcessObserversPos( "���ڸ���" + (*iter_image), processCounter ++ );

				//�����ļ�
				String destfilepath = vgKernel::StringUtility::getPathFromAbsoluteFilename(output_filename);
				String destfilename = vgKernel::StringUtility::getFilenameFromAbsolutePath(*iter_image);
				destfilename = destfilepath + "\\" + destfilename;

				BOOL save_ret = CommonCopyFile( (*iter_image).c_str() , destfilename.c_str() );
				//assert( save_ret == TRUE );
				if (save_ret == FALSE)
				{
					break;
				}

				if (!save_ret)
				{
					errList.push_back( *iter_image );
					continue;
				}

				// �������
				validList.push_back( *iter_image );
			}


		}

		if ( validList.empty() == true )
		{
			MessageBox( NULL , "����Ч��image�ļ�����." , "Error" , MB_OK );
			return false;
		}

		//------------------------------------------
		// ��дheader
		//------------------------------------------
		pwriter->seek( filebegin );
		pwriter->write( &header , sizeof( DtxHeader ) );

		pwriter->close();

		//------------------------------------------
		// ������������report
		//------------------------------------------
		String outputmsg;

		if ( errList.empty() == false )
		{
			std::ostringstream o;

			o << "����ʧ�ܵ��б�:\n";

			vgKernel::StringVector::iterator iter = errList.begin();
			vgKernel::StringVector::iterator iter_end  = errList.end();

			for (  ;iter != iter_end ; ++ iter )
			{
				o << *iter << "\n";
			}

			outputmsg	+= o.str();
			outputmsg	+= "\n\n";
		}


		// ���ɱ���
		std::ostringstream o;
		o << "�������,�ɹ�" << validList.size() <<
			"���ļ�,ʧ��:" << errList.size() << "���ļ�.";

		this->notfiyProcessObserversFinish( o.str() );

		MessageBox(NULL, o.str().c_str(), "���ɱ���", MB_OK);

		return true;
	}
	//----------------------------------------------------------------
