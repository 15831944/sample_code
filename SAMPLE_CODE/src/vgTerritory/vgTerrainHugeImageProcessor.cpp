




#include <vgStableHeaders.h>
#include <vgTerritory/vgTerrainHugeImageProcessor.h>
#include <vgTerritory/vgtDefinition.h>
#include <vgImage/vgiFreeImageWrapper.h>


namespace vgTerritory {
	

	//----------------------------------------------------------------
	HugeImagesProcessor::~HugeImagesProcessor()
	{
		this->clear();
	}
	//----------------------------------------------------------------
	void HugeImagesProcessor::clear()
	{
		m_pFileNamesInfo.setNull();

		m_pAimsblocksOption.setNull();

		m_pfblockVector.setNull();

		m_pSmallblockLayout.setNull();
	}
	//----------------------------------------------------------------
	bool HugeImagesProcessor::checkFileExist( const String& filename )
	{
		std::fstream f( filename.c_str() ,std::fstream::in);
		if ( f.fail() )
		{
			return false;
		}
		f.close();
		return true;
	}
	//----------------------------------------------------------------
	bool HugeImagesProcessor::removeFile( const String& filename , const bool& iflog /*= true */)
	{
		/// use stdio.h 
		int ret_remove;

#ifdef UNICODE
		assert(0);
		ret_remove = ::remove( (StringUtil::convertWStringToString(filename)).c_str() );
#else
		ret_remove = ::remove( filename.c_str() );
#endif

		if ( ret_remove == 0 )
		{
			/// ��ʱ�ɹ�����
			if( iflog )
			{
				LogManager::getSingleton().logMessage( String( _T("File: ")) + filename + _T(" has been deleted."));
			}
			return true;
		}			
		else  
		{
			/// δ�ɹ����ص����.
			if ( iflog )
			{
				LogManager::getSingleton().logMessage( String( _T("When deleting ")) 
					+ filename + _T(" , failed. The file may be not exist or just in use"));
			}
			return false;
		}
	}

	//----------------------------------------------------------------
	bool HugeImagesProcessor::initSavingEnvironment( const fileBlockVector& fblockvec ,
		const AimSmallBlocksOption& aiminfo,
		const smallBlockVector& sblockLayout  )
	{
		const bool& deleteAllExistFiles = aiminfo.ifOverrideExist;
		const bool& iflog = aiminfo.ifLogInfo;

		int delcount = 0;

		/// ��ѡ�������е��Ѵ����ļ�,�򷵻�
		if ( deleteAllExistFiles == false )
		{
			return true;
		}

		/// ���ò�˵����ĳЩ�ļ����ܼ����2��.���ǿ��ǵ�override��ʱ�򲢲��Ǻܶ�,
		/// �����Ч����΢����һ�㻹�ǿ��Խ���.

		if ( iflog )
		{
			LogManager::getSingleton().logMessage(
				 _T("============================== now begin deleting the exist files========================"));
		}

		/// ����fileblockvector�ļ�Ŀ¼,ɾ�����ڵ��ļ�(��������tmp���ɵ������ļ�).
		for ( fileBlockVector::const_iterator pfblock = fblockvec.begin() ;
			pfblock != fblockvec.end() ;
			++ pfblock )
		{
			for ( smallBlockVector::const_iterator psblock = pfblock->taskList.begin() ;
				psblock != pfblock->taskList.end() ;
				++ psblock )
			{
				if ( checkFileExist( psblock->blockFileName ) )
				{
					if( !removeFile( psblock->blockFileName , iflog ) )
					{
						return false;
					}
					delcount ++;
				}
			}// end of psblock
		}// end of pfblock

		/// ����smallBlockLayout�ļ�Ŀ¼,ɾ�����ڵ��ļ�(������tmp���ɵ������ļ�).
		for ( smallBlockVector::const_iterator i = sblockLayout.begin() ;
			i != sblockLayout.end() ;
			++ i )
		{
			if ( checkFileExist( i->blockFileName ) )
			{
				if( !removeFile( i->blockFileName , iflog ) )
				{
					return false;
				}
				delcount ++;
			}
		}

		/// ɾ��CombinedFileBlocks
		if ( aiminfo.ifOutputCombinedFileBlocks )
		{
			if ( checkFileExist( aiminfo.aimCombinedFBlockName ) )
			{
				if ( !removeFile( aiminfo.aimCombinedFBlockName , iflog ) )
				{
					return false;
				}
				delcount ++;
			}
		}

		/// ɾ��CombinedSmallBlocks
		if ( aiminfo.ifOutputCombinedSmallBlocks )
		{
			if ( checkFileExist( aiminfo.aimCombinedSBlockName ))
			{
				if ( !removeFile( aiminfo.aimCombinedSBlockName , iflog ))
				{
					return false;
				}
				delcount ++;
			}
		}

		/// ������־.
		if ( iflog )
		{
			LogManager::getSingleton().logMessage(
				 _T("============================== now end deleting the exist files : ")
				+ StringUtility::textCast(delcount) +  _T(" files deleted ") +
				 _T("========================"));
		}

		return true;
	}
	//----------------------------------------------------------------
	fileBlockVectorPtr HugeImagesProcessor::createFileblockLayout(
		const FileNamesInfo& fileinfo ,
		const AimSmallBlocksOption& aiminfo )
	{
		const std::vector<String> &stringVec = fileinfo.filenameVector;

		const long& col = fileinfo.columnNum;
		const long& row = fileinfo.rowNum;
		//const bool& ifkownsize = fileinfo.ifKnownSize;

		fileBlockVectorPtr  resultVector( new fileBlockVector );

		fileBlock		 fblock;
		ImagePos		 nowPos( 0 );
		ImageRect		 nowRect;

		for ( int y = 0 ; y < row ; ++ y )
		{
			for ( int x = 0 ; x < col ; ++ x )
			{
				/// �����Խ�����Ƿ����׳��쳣???????
				const String &file = stringVec.at( y * col + x );

WARN_YES_AGAIN:
				/// �����ļ�.
				if ( !readImageFileRect(file , nowRect))
				{
					int ret = VGK_WARN(Warning::WARN_FILE_NOT_FOUND ,
						Warning::BUTTON_ABORT_RETRY_IGNORE,
						 _T("��ȡ�ļ�") + file + _T("��Ϣʱ���ִ���,�ļ������ڻ�������,�Ƿ�����?"),
						 _T("HugeImagesProcessor::createFileblockLayout()"));

					if ( ret == Warning::RETURN_RETRY )
					{
						goto WARN_YES_AGAIN;
					}
					if ( ret == Warning::RETURN_ABORT )
					{
						VGK_EXCEPT(Exception::ERR_FILE_NOT_FOUND,
							 _T("��ȡ�ļ���С��Ϣʱ���ִ���!�ļ������ڻ�������!"),
							 _T("HugeImagesProcessor::createFileblockLayout()"));
					}
					if ( ret == Warning::RETURN_IGNORE )
					{
						nowRect = ImageRect::ZERO;
					}
				}


				/// ��ֵfblock.
				fblock.fileSrcName = file;
				fblock.fileAbsoluteRect = nowRect.translate( nowPos );

				/// �Ƿ�д����־.
				if ( aiminfo.ifLogInfo )
				{
					LogManager &logmgr = LogManager::getSingleton();
					logmgr.logMessage( String( _T("handing File:")) + file );
				}

				resultVector->push_back( fblock );

				/// ����λ��.
				nowPos.x += nowRect.getWidth() ;

			}// end of x

			/// ����λ��.
			nowPos.x = 0;
			nowPos.y += nowRect.getHeight() ;

		}// end of y

		/// �Ƿ�д����־.
		if ( aiminfo.ifLogInfo )
		{
			LogManager &logmgr = LogManager::getSingleton();
			logmgr.logMessage(
				 _T("================= now ready to print fileblockVector information ===================") );
			for ( fileBlockVector::const_iterator i = resultVector->begin() ;
				i != resultVector->end() ;
				++ i )
			{
				logmgr.logMessage( i->getDetails() );
			}
			logmgr.logMessage(
				 _T("================= end to print fileblockVector information ===================") );
		}

		return resultVector;
	}
	//----------------------------------------------------------------
	smallBlockVectorPtr HugeImagesProcessor::createDefaultSmallblockLayout( 
		const fileBlockVector& fblockvec , 
		const AimSmallBlocksOption& aiminfo )
	{
		LogManager &logmgr = LogManager::getSingleton();

		const long& widthAndHeightAddOne = aiminfo.widthAndHeightAddOne;

		/// ��Ҫ�õ���block����Ϣ.
		const ImagePos&  aim_offset			 = aiminfo.aimOffset;
		const long&	 aim_block_height	 =
			aiminfo.aimHeight;
		const long&	 aim_block_width	 = 
			aiminfo.aimWidth;

		const long&	 overlap			 = aiminfo.aimPixcelOverlap;
		const String&	 aim_block_filename	 = aiminfo.aimFilePrefix;
		const String&	 aim_ext			 = aiminfo.aimFilePostfix;
		const bool&      aim_log			 = aiminfo.ifLogInfo;

		
		

		const bool&		 aim_reversSeq		= aiminfo.ifReverseSequence;

		const long&   aim_sblock_layout_num = aiminfo.aimSmallBlocksNum;

		const ImageRect aim_block_rect ( 0 , 0 , aim_block_width , aim_block_height );

		/// �õ�������ͼ�ĳ���
		const ImageRect worldRect = fblockvec.getWorldImageRect();

		if ( aim_log )
		{
			logmgr.logMessage( String( _T("worldRect : ")) + worldRect.getDetails() );
		}

		/// ���������blocks��.ע��,����������ص���.

#if 0
		assert( aim_block_width  == aim_block_height );

		long blocks_num_x =  
			(worldRect.getWidth() + aim_block_width - 2 * overlap - 1) / (aim_block_width  - overlap);

		long blocks_num_y =  
			(worldRect.getHeight() + aim_block_height - 2 * overlap - 1) / (aim_block_height - overlap);

		blocks_num_x = blocks_num_y = VGT_MAX( blocks_num_x , blocks_num_y );

#else

		//------------------------------------------
		// �����޸�
		//------------------------------------------
		assert( aim_block_width  == aim_block_height );

		long blocks_num_x =  
			(worldRect.maxPos.x + aim_block_width - 2 * overlap - 1) / (aim_block_width  - overlap);

		long blocks_num_y =  
			(worldRect.maxPos.y + aim_block_height - 2 * overlap - 1) / (aim_block_height - overlap);

		blocks_num_x = blocks_num_y = VGT_MAX( blocks_num_x , blocks_num_y );


#endif

		if ( aim_log )
		{
			logmgr.logMessage( String( _T("blocks_num_x : ")) + StringUtility::textCast(blocks_num_x) );
			logmgr.logMessage( String( _T("blocks_num_y : ")) + StringUtility::textCast(blocks_num_y) );
		}

		smallBlock  sblock;
		ImagePos	nowPos;
		ImageRect   nowRect;
		ImageRect	nowIntersectRect;
		String		sblockPostfix;
		long		sblocknum = 0;

		/// �����ڴ�ռ�.
		smallBlockVectorPtr psblockvector( new smallBlockVector );

		// ��ʼ���е�smallBlockѭ��
		for ( long sblock_y = 0 ; sblock_y < blocks_num_y ; ++ sblock_y )
		{
			for ( long sblock_x = 0 ; sblock_x < blocks_num_x ; ++ sblock_x )
			{
				/// ���¼�����.
				/// �ж��Ƿ�ﵽ��Ҫ����Ŀ
				if ( aim_sblock_layout_num != -1 && sblocknum ++ == aim_sblock_layout_num )
				{
					goto ENDFUNC;
				}

				nowRect = aim_block_rect;
				nowRect.translate( nowPos + aim_offset );

				/// �������.
				sblock.worldSequenceNumber.x = sblock_x;
				sblock.worldSequenceNumber.y = sblock_y;

				/// ���þ���λ��
				sblock.absoluteRect = nowRect;



				//////////////////////////////////////////////////////////////////////////
				//------------------------------------------
				// ע������!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				// !!!!!!!!!!!!!!!!!!!
				//------------------------------------------
				sblock.absoluteRect.maxPos.x += widthAndHeightAddOne;
				sblock.absoluteRect.maxPos.y += widthAndHeightAddOne;
				//////////////////////////////////////////////////////////////////////////




				/// ���ñ��.��ʱ������intersect�ĺ���,ֻ��Ϊ������õ���Ҫ���ַ���׺.
				/// ����Ĭ�Ϲ��캯���Ѿ�����false��,���һ���Ҫ������?
				/// �ǵ�,���Ƿ��б�Ҫ�������������һ�����Ч��,��ʱ�ſ���ȥ��.
				sblock.hasIntersect = false;

				/// �����ļ���.
				sblockPostfix = sblock.getPostfixNumFlag(aim_reversSeq);
				sblock.blockFileName = aim_block_filename + sblockPostfix + aim_ext;

				/// ����vector.
				psblockvector->push_back( sblock );

				/// ����λ��.
				/// Ϊʲô��ֱ����aim_block_width?
				/// ��Ϊ���������ּ�һ��,�Ժ�nowRect�����Ǳ��,������ȷ����ȷ��,�Ժ�����ٸ�.
				nowPos.x += nowRect.getWidth() - overlap;
			} // end of sblock_x

			/// ����λ��.
			nowPos.x = 0;
			nowPos.y += nowRect.getHeight() - overlap;
		} // end of sblock_y

ENDFUNC:
		if ( aim_log )
		{
			logmgr.logMessage(
				 _T("================= now ready to print SmallblockLayout information ===================") );
			/// ��ӡfileBlockVector��Ϣ.
			for ( smallBlockVector::iterator i = psblockvector->begin() ;
				i != psblockvector->end() ;
				++ i )
			{
				logmgr.logMessage( i->getDetails() );
			}
			logmgr.logMessage( 
				 _T("================= end to print SmallblockLayout information ===================") );
		}
		return psblockvector;
	}
	//----------------------------------------------------------------
	bool HugeImagesProcessor::coreFunction( fileBlockVector& fblockvec ,
		const smallBlockVector& sblockvec , 
		const AimSmallBlocksOption& aimblockinfo )
	{
		LogManager &logmgr = LogManager::getSingleton();

		const String& sfileprefix = aimblockinfo.aimFilePrefix;
		const String& sfilepostfix = aimblockinfo.aimFilePostfix;
		const String& sfiletmppostfix = aimblockinfo.aimTempFilePostfix;
		const bool&   iflog = aimblockinfo.ifLogInfo;

		const bool&	  reverseSq = aimblockinfo.ifReverseSequence;

		smallBlock tmpsblock;

		for ( fileBlockVector::iterator pfblock = fblockvec.begin() ;
			pfblock != fblockvec.end() ;
			++ pfblock )
		{

			for ( smallBlockVector::const_iterator psblock = sblockvec.begin() ;
				psblock != sblockvec.end() ;
				++ psblock )
			{
				if ( psblock->absoluteRect.testIntersection( pfblock->fileAbsoluteRect ) )
				{
					tmpsblock = *psblock;

					/// �������Rect
					tmpsblock.relativeRect = psblock->absoluteRect;
					tmpsblock.relativeRect.translate( -pfblock->fileAbsoluteRect.minPos );

					/// �鿴�Ƿ����ļ��ڲ�.
					if ( pfblock->fileAbsoluteRect.testInside( tmpsblock.absoluteRect ) )
					{
						tmpsblock.hasIntersect = false;

						/// �����ļ�����
						tmpsblock.blockFileName = sfileprefix 
							+ tmpsblock.getPostfixNumFlag(reverseSq) + sfilepostfix;
					}
					else
					{
						tmpsblock.hasIntersect = true;

						/// �����ļ�����
						tmpsblock.blockFileName = sfileprefix 
							+ tmpsblock.getPostfixNumFlag(reverseSq) + sfiletmppostfix;
					}

					pfblock->taskList.push_back( tmpsblock );
				}
			}// end for psblock

		} // end of pfblock

		if ( iflog )
		{
			logmgr.logMessage(
				 _T("================= now ready to print the calculated fileBlockVector information ===================") );
			/// ��ӡfileBlockVector��Ϣ.
			for ( fileBlockVector::const_iterator i = fblockvec.begin() ;
				i != fblockvec.end() ;
				++ i )
			{
				logmgr.logMessage( i->getDetails() );
			}
			logmgr.logMessage( 
				 _T("================= end to print the calculated fileBlockVector information ===================") );
		}
		return true;
	}
	//----------------------------------------------------------------
	bool HugeImagesProcessor::processSaving( const fileBlockVector& fblockvec ,
		const AimSmallBlocksOption& aiminfo )
	{
		LogManager &logmgr = LogManager::getSingleton();

#define CximageWrapperPtr FreeImageWrapperPtr
#define CximageWrapper FreeImageWrapper

 		CximageWrapperPtr fileNimg( new CximageWrapper );
		CximageWrapperPtr tempNimg( new CximageWrapper );
		CximageWrapperPtr tmpNimg( new CximageWrapper );

		const bool& iflog = aiminfo.ifLogInfo;
		const bool& ifoverride = aiminfo.ifOverrideExist;
		const bool& ifdrawinfo = aiminfo.ifDrawInfo;

		const bool&		 aim_axis_left_top	 = aiminfo.ifAxisLeftTop;

		if ( iflog )
		{
			logmgr.logMessage(
				 _T("================= now processing saving begin ===================") );
		}

		//------------------------------------------
		// �����С
		//------------------------------------------
		
		int allsize = fblockvec.size();
		for ( fileBlockVector::const_iterator pfblock = fblockvec.begin() ;
			pfblock != fblockvec.end() ;
			++ pfblock )
		{
			allsize +=  pfblock->taskList.size();
		}

		this->notifyProcessObserversStart( "��ʼ�����п�ͼ��" , 0 , allsize );
		int counter = 0;

		/// �����const_interator������.
		for ( fileBlockVector::const_iterator pfblock = fblockvec.begin() ;
			pfblock != fblockvec.end() ;
			++ pfblock )
		{

			this->notifyProcessObserversPos( pfblock->fileSrcName , counter ++ );


			if ( fileNimg->loadFromFile(pfblock->fileSrcName) == false )
			{
				/// !!!!!!!!!!warn!
				continue;
			}

			for ( smallBlockVector::const_iterator psblock = pfblock->taskList.begin() ;
				psblock != pfblock->taskList.end() ;
				++ psblock )
			{

				this->notifyProcessObserversPos( psblock->blockFileName, counter ++ );


				if ( psblock->hasIntersect == false )  /// û���ص������.
				{
					/// ע��if�������������е�˳��.
					if ( !ifoverride && checkFileExist( psblock->blockFileName ) )
					{
						/// !!!!!!!!!!warn!
						continue;
					}

					if( fileNimg->crop( psblock->relativeRect , tmpNimg.getPointer() ,aim_axis_left_top) == false )
					{
						/// !!!!!!!!!!warn!
						tmpNimg->release();
						continue;
					}

					if ( ifdrawinfo )
					{
						String info( _T("FileName : "));
						info += psblock->blockFileName
							+  _T("\n Width : ") + StringUtility::textCast( psblock->absoluteRect.getWidth() )
							+  _T("\n Height : ") + StringUtility::textCast( psblock->absoluteRect.getHeight() );
						//tmpNimg->drawString( 10 , 50 , info ,  _T("") ,16);
						assert(0);
					}

					if ( ImageUtility::findImageFileExtension(psblock->blockFileName)
						== "dds" )
					{
						if( !tmpNimg->saveToDdsFile( psblock->blockFileName ,
							CximageWrapper::DDS_FORMAT_DXT1 ) )
						{
							assert( 0 &&  _T("��ȡʧ��!"));
							//warn!!!!!!!!!!!!!!!!!!
							tmpNimg->release();
							continue;
						}
					}
					else
					{
						if( !tmpNimg->saveToFile( psblock->blockFileName ) )
						{
							assert( 0 &&  _T("��ȡʧ��!"));
							//warn!!!!!!!!!!!!!!!!!!
							tmpNimg->release();
							continue;
						}
					}

					tmpNimg->release();
				}
				else ///���ص������.
				{
					/// ע��if�������������е�˳��.
					if ( !ifoverride && checkFileExist( psblock->blockFileName ) )
					{
						continue;
					}

					//���ļ�,���ʧ��,�򴴽�.
					if( !tmpNimg->loadFromFile( psblock->blockFileName ) )
					{
						tmpNimg->createFromSize( psblock->absoluteRect.getWidth() ,
							psblock->absoluteRect.getHeight() );
					}

					tmpNimg->overlap( *fileNimg , psblock->relativeRect ,aim_axis_left_top);

					/// tmp�ļ�����д��info����Ϣ.
					/// ��������ʱ��д����Ϣ.

					if ( ImageUtility::findImageFileExtension(psblock->blockFileName)
						== "dds" )
					{
						assert(0&&"tmp�ļ���Ӧ��Ϊdds�ļ�");
						if( !tmpNimg->saveToDdsFile( psblock->blockFileName ,
							CximageWrapper::DDS_FORMAT_DXT1 ) )
						{
							assert( 0 && "��ȡʧ��22!");
							tmpNimg->release();
							continue;
						}
					}
					else
					{
						if( !tmpNimg->saveToFile( psblock->blockFileName ) )
						{
							assert( 0 && "��ȡʧ��22!");
							tmpNimg->release();
							continue;
						}
					}


					tmpNimg->release();
				}
			}// end of psblock

			fileNimg->release();
		}// end of pfblock


		this->notfiyProcessObserversFinish("�и�ͼ��������");



		if ( iflog )
		{
			logmgr.logMessage(
				 _T("================= now processing saving end ===================") );
		}
		return true;
	}
	//----------------------------------------------------------------
	bool HugeImagesProcessor::processTempFiles( fileBlockVector& fblockvec ,
		const AimSmallBlocksOption& aiminfo )
	{
		LogManager &logmgr = LogManager::getSingleton();

		const bool& iflog = aiminfo.ifLogInfo;
		const bool& ifoutCombinefb = aiminfo.ifOutputCombinedFileBlocks;
		const bool& ifoutCombinesb = aiminfo.ifOutputCombinedSmallBlocks;
		const String& aimfileprefix = aiminfo.aimFilePrefix;
		const String& aimfilepostfix = aiminfo.aimFilePostfix;
		const bool& ifdrawinfo = aiminfo.ifDrawInfo;

		const bool& ifReverse = aiminfo.ifReverseSequence;

		const String&	 aim_block_filename	 = aiminfo.aimFilePrefix;
		const String&	 aim_ext			 = aiminfo.aimFilePostfix;

		//UniformImageFile  tmpimg;
		CximageWrapperPtr tmpNimg( new CximageWrapper );

		if ( iflog )
		{
			logmgr.logMessage(
				 _T("================= now processing temp file begin ===================") );
		}

		for ( fileBlockVector::iterator pfblock = fblockvec.begin() ;
			pfblock != fblockvec.end() ;
			++ pfblock )
		{
			for ( smallBlockVector::iterator psblock = pfblock->taskList.begin() ;
				psblock != pfblock->taskList.end() ;
				++ psblock )
			{
				if ( psblock->hasIntersect && checkFileExist( psblock->blockFileName ) )
				{
					psblock->hasIntersect = false;

					String oldfile = psblock->blockFileName;
					String newfile = aim_block_filename + psblock->getPostfixNumFlag(ifReverse) + aim_ext;

					/// �����ļ���.
					psblock->blockFileName = newfile;

					tmpNimg->loadFromFile( oldfile );

					if ( ifdrawinfo )
					{
						String info( _T("FileName : "));
						info += newfile
							+  _T("\n Width : ") + StringUtility::textCast( psblock->absoluteRect.getWidth() )
							+  _T("\n Height : ") + StringUtility::textCast( psblock->absoluteRect.getHeight() );
						//tmpNimg->drawString( 10 , 50 , info,  _T("") ,16);
						assert(0);
					}

					if ( ImageUtility::findImageFileExtension( psblock->blockFileName )
						== "dds" )
					{
						tmpNimg->saveToDdsFile(psblock->blockFileName,
							CximageWrapper::DDS_FORMAT_DXT1 );
					}
					else
					{
						tmpNimg->saveToFile(psblock->blockFileName);
					}


					removeFile( oldfile , iflog );

					tmpNimg->release();
				}
			}// end of psblock
		}// end of pfblock

		if ( iflog )
		{
			logmgr.logMessage(
				 _T("================= now processing temp file end ===================") );
		}

		return true;
	}
	//----------------------------------------------------------------
	bool HugeImagesProcessor::processCombining( 
		const fileBlockVector& fblockvec , 
		const AimSmallBlocksOption& aiminfo ,
		const smallBlockVector& sblockLayout)
	{
		/*
		LogManager &logmgr = LogManager::getSingleton();

		const bool& iflog = aiminfo.ifLogInfo;
		const bool& ifoutCombinefb = aiminfo.ifOutputCombinedFileBlocks;
		const bool& ifoutCombinesb = aiminfo.ifOutputCombinedSmallBlocks;
		const String& aimfileprefix = aiminfo.aimFilePrefix;
		const String& aimfilepostfix = aiminfo.aimFilePostfix;
		const bool& ifdrawinfo = aiminfo.ifDrawInfo;
		const bool& ifoverride = aiminfo.ifOverrideExist;

		const String&	 aim_block_filename	 = aiminfo.aimFilePrefix;
		const String&	 aim_ext			 = aiminfo.aimFilePostfix;

		if ( iflog )
		{
			logmgr.logMessage(
				"================= now processing combining begin ===================" );
		}

		/// ���ȼ�鷵�ص����.���������������.
		if ( ifoutCombinefb == false && ifoutCombinesb == false )
		{
			return true;
		}

		/// �洢���ļ�ƴ��ͼ,�����ڴ�ʹ���൱��,�������
		/// AimSmallBlocksOption���������˲���
		if ( ifoutCombinefb )
		{
			/// �õ�������ͼ�ĳ���
			const ImageRect worldRect = fblockvec.getWorldImageRect();

			UniformImageFile bigimage;
			UniformImageFile tmpimg;

			String finalname = aimfileprefix + '.' + aimfilepostfix;

			bigimage.Create( worldRect.getWidth() , worldRect.getHeight() );

			/// ��ʼ����.
			for ( fileBlockVector::const_iterator pfblock = fblockvec.begin() ;
				pfblock != fblockvec.end() ;
				++ pfblock )
			{
				tmpimg.LoadFromFile( pfblock->fileSrcName );

				bigimage.Overlap( tmpimg , -pfblock->fileAbsoluteRect );

				tmpimg.Release();
			}// end of pfblock

			bigimage.SaveToFile( finalname );
			bigimage.Release();

		}// end if ifoutCombinefb


		/// �洢���ļ�ƴ��ͼ,�����ڴ�ʹ���൱��,�������
		/// AimSmallBlocksOption���������˲���
		if ( ifoutCombinesb )
		{
			/// �õ�������ͼ�ĳ���
			const ImageRect worldRect = sblockLayout.getWorldImageRect();

			UniformImageFile bigimage;
			UniformImageFile tmpimg;

			/// ��ʱ����Ϊ***2.dds����ʽ.
			String finalname = aimfileprefix + "2." + aimfilepostfix;

			bigimage.Create( worldRect.getWidth() , worldRect.getHeight() );

			/// ��ʼ����.
			for ( smallBlockVector::const_iterator psblock = sblockLayout.begin() ;
				psblock != sblockLayout.end() ;
				++ psblock )
			{
				tmpimg.LoadFromFile( psblock->blockFileName );

				bigimage.Overlap( tmpimg , -psblock->absoluteRect );

				tmpimg.Release();
			}// end of pfblock

			bigimage.SaveToFile( finalname );
			bigimage.Release();

		}// end if ifoutCombinefb

		if ( iflog )
		{
			logmgr.logMessage(
				"================= now processing combining end ===================" );
		}

		*/
		return true;
	}
	//----------------------------------------------------------------
	HugeImageBlockInfoVectorPtr HugeImagesProcessor::getHugeImageBlockInfoVector(
		const smallBlockVector& sblockLayout ,
		const AimSmallBlocksOption& aiminfo )
	{
		/// �����Ժ�Ҫ����!~~~~~~~~~~~~~~~~~~~~~
		const float& ratio = 1.0f;

		HugeImageBlockInfo tempinfo(ratio);

		HugeImageBlockInfoVectorPtr pinfovec(new HugeImageBlockInfoVector);

		/// ��ʼ����.
		for ( smallBlockVector::const_iterator psblock = sblockLayout.begin() ;
			psblock != sblockLayout.end() ;
			++ psblock )
		{
			const ImagePos& pos = psblock->worldSequenceNumber;


			tempinfo.m_Vertex[3] =
				ImageUtility::getVector2FromImagePos( pos , tempinfo.m_Ratio ); 

			tempinfo.m_Vertex[2] =
				ImageUtility::getVector2FromImagePos( ImagePos( pos.x + 1 , pos.y ) , tempinfo.m_Ratio ); 

			tempinfo.m_Vertex[1] =
				ImageUtility::getVector2FromImagePos( ImagePos( pos.x + 1 , pos.y + 1 ) , tempinfo.m_Ratio ); 

			tempinfo.m_Vertex[0] =
				ImageUtility::getVector2FromImagePos( ImagePos( pos.x , pos.y + 1 ) , tempinfo.m_Ratio ); 


			tempinfo.m_BlockFileName = psblock->blockFileName;
			tempinfo.m_WorldNumber = pos;

			pinfovec->push_back( tempinfo );
		}// end of psblock

		return pinfovec;
	}
	//----------------------------------------------------------------
	bool HugeImagesProcessor::makeProcessingAndSaving()
	{


		_hugeProcessor.process();

		return true;


		/// �鿴�Ƿ��д���.
		assert(isSafe());
		if ( !isSafe() )
		{
			return false;
		}

		/// �����ļ���Ĳ���.
		if ( m_pfblockVector.isNull() == true )
		{
			this->m_pfblockVector = 
				createFileblockLayout( *m_pFileNamesInfo , *m_pAimsblocksOption );
		}

		if ( m_pfblockVector.isNull() )
		{
			return false;
		}

		/// ���������(С��)�Ĳ���.
		this->m_pSmallblockLayout =
			createDefaultSmallblockLayout( *m_pfblockVector , *m_pAimsblocksOption );

		if ( m_pSmallblockLayout.isNull() )
		{
			return false;
		}

		/// �������㴦������.
		if( !coreFunction( *m_pfblockVector , *m_pSmallblockLayout , *m_pAimsblocksOption ))
		{
			return false;
		}

		/// ��ʼ�����滷��:ɾ��ԭ�е��ļ��ȵ�.
		if(!initSavingEnvironment( *m_pfblockVector , *m_pAimsblocksOption , 
			*m_pSmallblockLayout ))
		{
			return false;
		}

		/// ��ʼ�����ļ�.
		if( !processSaving( *m_pfblockVector , *m_pAimsblocksOption ))
		{
			return false;
		}

		/// ������ʱ�ļ�(��ƴ����ʱ�ļ�,������Ҫ�����).
		if( !processTempFiles( *m_pfblockVector , *m_pAimsblocksOption ))
		{
			return false;
		}

		/// �����ƴ��ͼѡ��Ϊ��,��ʼ����ƴ��ͼ.
		if( !processCombining( *m_pfblockVector , *m_pAimsblocksOption , *m_pSmallblockLayout))
		{
			return false;
		}

		return true;
	}
	//----------------------------------------------------------------
	bool HugeImagesProcessor::makeProcessing()
	{
		/// �鿴�Ƿ��д���.
		assert(isSafe());
		if ( !isSafe() )
		{
			return false;
		}

		/// �����ļ���Ĳ���.

		if ( m_pfblockVector.isNull() == true )
		{
			this->m_pfblockVector = 
				createFileblockLayout( *m_pFileNamesInfo , *m_pAimsblocksOption );
		}

		if ( m_pfblockVector.isNull() )
		{
			return false;
		}

		/// ���������(С��)�Ĳ���.
		this->m_pSmallblockLayout =
			createDefaultSmallblockLayout( *m_pfblockVector , *m_pAimsblocksOption );

		if ( m_pSmallblockLayout.isNull() )
		{
			return false;
		}

		/// �������㴦������.
		if( !coreFunction( *m_pfblockVector , *m_pSmallblockLayout , *m_pAimsblocksOption ))
		{
			return false;
		}
		return true;
	}

	bool HugeImagesProcessor::makeSaving()
	{
		/// �鿴�Ƿ��д���.
		assert(isSafe());
		if ( !isSafe() )
		{
			return false;
		}
		if ( m_pfblockVector.isNull() )
		{
			return false;
		}
		if ( m_pSmallblockLayout.isNull() )
		{
			return false;
		}

		/// ��ʼ�����滷��:ɾ��ԭ�е��ļ��ȵ�.
		if(!initSavingEnvironment( *m_pfblockVector , *m_pAimsblocksOption , 
			*m_pSmallblockLayout ))
		{
			return false;
		}

		/// ��ʼ�����ļ�.
		if( !processSaving( *m_pfblockVector , *m_pAimsblocksOption ))
		{
			return false;
		}

		/// ������ʱ�ļ�(��ƴ����ʱ�ļ�,������Ҫ�����).
		if( !processTempFiles( *m_pfblockVector , *m_pAimsblocksOption ))
		{
			return false;
		}

		/// �����ƴ��ͼѡ��Ϊ��,��ʼ����ƴ��ͼ.
		if( !processCombining( *m_pfblockVector , *m_pAimsblocksOption , *m_pSmallblockLayout))
		{
			return false;
		}

		return true;
	}
	//----------------------------------------------------------------
	bool HugeImagesProcessor::initialiseParameters(
		uchar* filledColor,
		const bool& remove_input_files,
		const String& temp_file_ext , 
		const String& tmp_path , 
		const bool& remove_tmp_file,
		const String& fileprefix,  const long& width,  
		const long& height,  
		const String& tmpfilepostfix /*= "bmp"*/, 
		const String& fileposfix /*= "dds"*/,  
		const bool& reverseSequence			/*= false*/,
		const bool& axisLeftTop				/*= false*/,
		const bool& loginfo  /*= true*/, 
		const bool& overrid  /*= false*/, 
		const bool& outputcombinesblock /*= false*/,
		const String& outcombinesblockname /*= ""*/,
		const bool& outputcombinefblock /*= false*/, 
		const String& outcombinefblockname /*= ""*/, 
		const bool& drawinfo /*= false*/, 
		const ImagePos& offset /*= IMAGEPOS_ZERO*/, 
		const long& pixceloverlap /*= 1*/,  
		const long& sblocknum /*= -1 */ ,
		const long& width_and_height_add_one/* = 0*/)
	{

		assert( width == height );

		_hugeProcessor.initParams( filledColor , 
			tmp_path , fileprefix , fileposfix , temp_file_ext ,width ,
			width_and_height_add_one , remove_tmp_file , getProcessObs() ,
			remove_input_files );

		if ( !m_pAimsblocksOption.isNull() )
		{
			return false;
		}
		this->m_pAimsblocksOption.bind(
			new AimSmallBlocksOption(fileprefix,
			width,
			height,
			tmpfilepostfix,
			fileposfix,
			reverseSequence,
			axisLeftTop,
			loginfo,
			overrid	,
			outputcombinesblock,
			outcombinesblockname,
			outputcombinefblock,
			outcombinefblockname,
			drawinfo,
			offset,
			pixceloverlap,
			sblocknum ,
			width_and_height_add_one ) );

		return true;
	}
	//----------------------------------------------------------------
	bool HugeImagesProcessor::initDefaultParameters( const String& fileprefix )
	{
		if ( !m_pAimsblocksOption.isNull())
		{
			return false;
		}
		this->m_pAimsblocksOption.bind( new AimSmallBlocksOption(fileprefix) );

		return true;
	}
	//----------------------------------------------------------------
	bool HugeImagesProcessor::initialiseFileNames( const TCHAR** const &chararray , 
		const long& col ,  const long& row , const bool& loginfo /*= true */ )
	{
		if ( !m_pFileNamesInfo.isNull() )
		{
			VGK_EXCEPT(Exception::ERR_INVALID_STATE ,
				 _T("����Ϊ��,�ѱ���ʼ��. The self data struct must be inited.") ,
				 _T("HugeImagesProcessor::initialiseFileNames"));
			return false;
		}
		this->m_pFileNamesInfo = HugeImageUtil::createFileNamesInfoFormArray(
			chararray , col , row , false );
		if ( m_pFileNamesInfo.isNull() )
		{
			return false;
		}

		if ( loginfo )
		{
			LogManager::getSingleton().logMessage(m_pFileNamesInfo->getDetails());
		}
		return true;
	}
	//----------------------------------------------------------------
	void HugeImagesProcessor::initialiseFileBlocks( const fileBlockVectorPtr inputfileblocks )
	{
		assert( inputfileblocks.isNull() == false );
		m_pfblockVector = inputfileblocks;

		_hugeProcessor.initFileBlocks( inputfileblocks);
	}
	//----------------------------------------------------------------
	vgTerritory::HugeImageBlockInfoVectorPtr HugeImagesProcessor::getHugeImageBlockInfos() 
	{
		return _hugeProcessor.getHugeImageBlockInfoVectorPtr();


		//return getHugeImageBlockInfoVector( *m_pSmallblockLayout ,
		//	*m_pAimsblocksOption);
	}
	//----------------------------------------------------------------

	

}// end of namespace vgTerritory
