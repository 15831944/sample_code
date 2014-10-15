

#include <vgStableHeaders.h>
#include <vgMod/vgFileDefinitionVG.h>

#include <vgMod/vgModelStruct.h>
#include <vgMod/vgVersionControl.h>
#include <string>

namespace vgMod{

	//----------------------------------------------------------------
	VGFileHeader::VGFileHeader()
		:m_nodenum(0)
		,abstractNodePos(0)
	{
		strcpy_s(vgfileheader, sizeof(vgfileheader), "");
	}

	//----------------------------------------------------------------
	bool	VGFileHeader::read(vgKernel::StreamReaderPtr pfile)
	{
		// Part 1: ��ʶ�ļ��汾���ַ���
		pfile->read(vgfileheader, 20);
		String strheader( vgfileheader );
		//vgGlobal::GlobalFacade::getSingleton().setVgVersionRead( strheader );
		//if( !vgGlobal::GlobalFacade::getSingleton().isVersionCompatible() )
		VersionControl::setVgVersionRead(strheader);
		if( !VersionControl::isVersionCompatible() )
		{
			ostringstream ErrorMes;

			ErrorMes<<"�ɰ汾ƽ̨�޷������°汾ƽ̨���ɵ�vg�ļ���";

			AfxMessageBox(ErrorMes.str().c_str());
			return	false;
		}


		// Part 2: �ڵ���������ļ�λ��
		pfile->read(&abstractNodePos , sizeof(unsigned long));


		// Part 3: �ڵ���Ŀ
		pfile->read(&m_nodenum , sizeof(int));

		return true;
	}

	void	VGFileHeader::write(vgKernel::StreamWriterPtr pfile)
	{
		// Part 1: ��ʶ�ļ��汾���ַ���
		String	pStrVgVersion  = VersionControl::getVgVersionWrite();
		//  vgGlobal::GlobalFacade::getSingleton().getVgVersionString();
		strcpy_s(vgfileheader, sizeof(vgfileheader), pStrVgVersion.c_str());
		pfile->write(vgfileheader, sizeof(vgfileheader) );


		// Part 2: �ڵ���������ļ�λ��
		pfile->write(&abstractNodePos , sizeof(unsigned long));


		// Part 3: �ڵ���Ŀ
		pfile->write(&m_nodenum , sizeof(int));
	}
	bool	VGFileHeader::read(CFile*  pfile)
	{
		// Part 1: ��ʶ�ļ��汾���ַ���
		pfile->Read(vgfileheader, 20);
		String strheader( vgfileheader );
		//vgGlobal::GlobalFacade::getSingleton().setVgVersionRead( strheader );
		//if( !vgGlobal::GlobalFacade::getSingleton().isVersionCompatible() )
		VersionControl::setVgVersionRead(strheader);
		if( !VersionControl::isVersionCompatible() )
		{
			ostringstream ErrorMes;

			ErrorMes<<"�ɰ汾ƽ̨�޷������°汾ƽ̨���ɵ�vg�ļ���";

			AfxMessageBox(ErrorMes.str().c_str());
			return	false;
		}


		// Part 2: �ڵ���������ļ�λ��
		pfile->Read(&abstractNodePos , sizeof(unsigned long));


		// Part 3: �ڵ���Ŀ
		pfile->Read(&m_nodenum , sizeof(int));

		return true;
	}

	bool VGFileHeader::read( String filename )
	{
		vgKernel::StreamReaderPtr m_pfile = 
			vgKernel::StreamReaderFactory::createFromFile( filename );

		return read(m_pfile);

	}
	void	VGFileHeader::write(CFile* pfile)
	{
		// Part 1: ��ʶ�ļ��汾���ַ���
		String	pStrVgVersion  = VersionControl::getVgVersionWrite();
		//  vgGlobal::GlobalFacade::getSingleton().getVgVersionString();
		strcpy_s(vgfileheader, sizeof(vgfileheader), pStrVgVersion.c_str());
		pfile->Write(vgfileheader, sizeof(vgfileheader) );


		// Part 2: �ڵ���������ļ�λ��
		pfile->Write(&abstractNodePos , sizeof(unsigned long));


		// Part 3: �ڵ���Ŀ
		pfile->Write(&m_nodenum , sizeof(int));
	}

	//----------------------------------------------------------------
	MODFileHeader::MODFileHeader()
	{

	}

	//----------------------------------------------------------------
	bool MODFileHeader::read( vgKernel::StreamReaderPtr pfile )
	{
		// Part 1: ��ʶ�ļ���ʽ���ַ���
		pfile->read(strFileFormat, sizeof(strFileFormat));

		char	fmt[6]="";

		for (int i=0; i<6; i++)
		{
			fmt[i] = '\0';
		}

		strncpy_s( fmt, sizeof(fmt), strFileFormat, 5);
		if (strcmp( fmt, "VRGIS") )
		{
			ostringstream ErrorMes;

			std::string outHeader = fmt;
#if VGK_LANG_CHS
			ErrorMes<<"VR-GISƽ̨��֧�ָ��ļ����ͣ�";
			ErrorMes<<outHeader;
#else
			ErrorMes<<"VR-GIS Platform do not support such file type��";
#endif

			AfxMessageBox(ErrorMes.str().c_str());
			pfile->close();
			return	false;
		}


		// Part 2: ��ʶ�ļ���ʽ������
		pfile->read(&uiFileVersion , sizeof(unsigned int));

		unsigned int nByteLow  = uiFileVersion & ((1<<16)-1);
		unsigned int nByteHigh = uiFileVersion >> 16;
		std::ostringstream oVersion;

#if VGK_LANG_CHS
		oVersion << endl << "\tMOD�汾:\t" ;
#else
		oVersion << endl << "\tMOD Version:\t" ;
#endif		
		oVersion << nByteHigh<< "." << nByteLow << endl;
		
		VGK_SHOW( oVersion.str() );

		if( uiFileVersion < VERSION_MOD(2008,5) )
		{
#if VGK_LANG_CHS
			MessageBox(NULL, "MOD�İ汾���ɲ��ټ��ݣ�����ʧ��", "MOD����ʧ��", MB_OK);
#else
			MessageBox(NULL, "MOD file version is out of date, failed to import", 
				"Faied to import MOD file", MB_OK);
#endif			
			return false;
		}


		// Part 3: ����modģ�͵���ƫ����
		pfile->read( &vSceneCenter, sizeof(Vector3) );


		// Part 4: modģ����Ŀ
		pfile->read( &uiObjectCount, sizeof(unsigned int) );


		// Part 5: modģ�����ļ��е�����λ��
		arrModPosIndex.assign(uiObjectCount, 0);
		for (int indexMod = 0; indexMod < uiObjectCount; indexMod++)
		{
			unsigned int uiTemp;
			pfile->read( &uiTemp, sizeof(unsigned int) );
			arrModPosIndex[indexMod] = uiTemp;
		}


		// Part 6: modģ�͵���������
		arrModType.assign(uiObjectCount, /*(unsigned int)*/ModType());
		for (int indexMod = 0; indexMod < uiObjectCount; indexMod++)
		{
			if( uiFileVersion > VERSION_MOD(2008,5) )
			{
				unsigned int uiTemp;
				pfile->read( &uiTemp, sizeof(unsigned int) );
				arrModType[indexMod].nType = uiTemp;
			}
			else
			{
				bool bTemp;
				pfile->read( &bTemp, sizeof(bool) );
				arrModType[indexMod].bType = bTemp;
			}
		}		

		return true;
	}

	void MODFileHeader::write( vgKernel::StreamWriterPtr pfile )
	{

	}

}//namespace vgMod