#include <vgStableHeaders.h>
#include <vgAutoBuild/vgvAutoBuildManager.h>

#include <vgEntry/vgEntryFactory.h>
#include <vgUIController/vgUIController.h>

#include <vgShape/vgshShapeManager.h>
#include <gdal/ogrsf_frmts.h>
#include <vgShape/vgshSpecialPolygon.h>
#include <vgShape/vgshLayer.h>
#include <vgShape/vgshGeometry.h>
#include <vgKernel/vgkSelectManager.h>
#include <vgEntry/vgAutoBuildEntry.h>
#include <vgEntry/vgSolidBuildEntry.h>
#include <DlgAutoBuildImportTxt.h>

#include <vgKernel/vgkPluginManager.h>

#include <vgMod/vgVersionControl.h>

namespace vgAutoBuild
{
	string AutoBuildManager::scheStr[4] = {"GS", "LS", "NE", "WM"};

	bool AutoBuildManager::onDeleteRenderer(vgKernel::Renderer *render)
	{
#if 1
		vector<AutoBuildNode*>::iterator iStart = m_autoBuildNodeRenderList.begin();
		vector<AutoBuildNode*>::iterator iEnd   = m_autoBuildNodeRenderList.end();

		vector<AutoBuildNode*>::iterator iter = find(iStart, iEnd, render);
		if ( iter != iEnd)
		{
			delete *(iter);
			*iter = NULL;

			return true;
		}

		iStart = m_autoBuildNodeList.begin();
		iEnd   = m_autoBuildNodeList.end();

		iter = find(iStart, iEnd, render);
		if ( iter != iEnd)
		{
			delete *(iter);
			*iter = NULL;

			return true;
		}

#endif

		vector<SolidBuildNode*>::iterator iter_begin = m_solidBuildNodeList.begin();
		vector<SolidBuildNode*>::iterator iter_end   = m_solidBuildNodeList.end();

		vector<SolidBuildNode*>::iterator iter_result = find(iter_begin, iter_end, render);
		
		if (iter_result != iter_end)
		{
			delete *(iter_result);
			*iter_result = NULL;

			m_solidBuildNodeList.erase(iter_result);
			return true;
		}


		vector<MarkBuildNode*>::iterator iter_begin1 = m_markBuildNodeList.begin();
		vector<MarkBuildNode*>::iterator iter_end1   = m_markBuildNodeList.end();

		vector<MarkBuildNode*>::iterator iter_result1 = find(iter_begin1, iter_end1, render);

		if (iter_result1 != iter_end1)
		{
			delete *(iter_result1);
			*iter_result1 = NULL;

			m_markBuildNodeList.erase(iter_result1);
			return true;
		}

		return false;
	}

	bool AutoBuildManager::addAutoBuildNode(AutoBuildNode* pNode)
	{	
		pNode->setName(pNode->m_pMidNode->GetName());
		pNode->SetName(pNode->m_pMidNode->GetName());

		vgKernel::RendererManager::getSingleton().addRenderer(pNode);

#if !defined(VG_ACTIVEX)

		HTREEITEM	hRootGpu = vgUI::UIController::getSingleton().getEntryRootItemByType(EFFECT_MOD_AUTOBUILD);

		//vgBaseEntry* entry = vgEntryFactory::createEntryFromRenderer( pNode );
		vgBaseEntry* entry = new vgCore::vgAutoBuildEntry(pNode);

		vgUI::UIController::getSingleton().AddNode( hRootGpu, entry );

		vgUI::UIController::getSingleton().SelectNode(entry);
		pNode->notifyOberversOnEvent(vgKernel::VG_OBS_PROPCHANGED, NULL);
		// vgKernel::SelectManager::getSingleton().updateSelectedItem();
#endif

		if (find(m_autoBuildNodeList.begin(), m_autoBuildNodeList.end(), pNode) == m_autoBuildNodeList.end())
		{
			m_autoBuildNodeList.push_back(pNode);

			return true;
		}
		else
		{
			return false;
		}
	}

	bool AutoBuildManager::addPlanNode(AutoBuildNode* pNode)
	{
#if 1
		pNode->setName(pNode->m_pMidNode->GetName());
		//pNode->SetName(pNode->m_pMidNode->GetName());

		vgKernel::RendererManager::getSingleton().addRenderer(pNode);

 // �Զ���ģ�ڵ��ڹ滮�в�������ͼ��ӽڵ�.
 #if !defined(VG_ACTIVEX)
 
		HTREEITEM	hRootGpu = vgUI::UIController::getSingleton().getEntryRootItemByType(EFFECT_MOD_AUTOBUILD);

 		vgBaseEntry* entry = vgEntryFactory::createEntryFromRenderer( pNode );
 		vgUI::UIController::getSingleton().AddNode( hRootGpu, entry );
 
 		vgUI::UIController::getSingleton().SelectNode(entry);
 		pNode->notifyOberversOnEvent(vgKernel::VG_OBS_PROPCHANGED, NULL);
 		// vgKernel::SelectManager::getSingleton().updateSelectedItem();
 #endif

		if (find(m_autoBuildNodeRenderList.begin(), m_autoBuildNodeRenderList.end(), pNode) == m_autoBuildNodeRenderList.end())
		{
			m_autoBuildNodeRenderList.push_back(pNode);

			return true;
		}
		else
		{
			return false;
		}
#endif
		return true;
	}

	bool AutoBuildManager::addMarkBuildNode( MarkBuildNode* pNode )
	{
		vgKernel::RendererManager::getSingleton().addRenderer(pNode);

		if ( find(m_markBuildNodeList.begin(), m_markBuildNodeList.end(), pNode) 
			== m_markBuildNodeList.end() )
		{
			m_markBuildNodeList.push_back(pNode);
			return true;
		}
		else
		{
			return false;
		}

	}

	bool AutoBuildManager::addSolidBuildNode( SolidBuildNode* pNode)
	{
		vgKernel::RendererManager::getSingleton().addRenderer(pNode);

#if !defined(VG_ACTIVEX)

		HTREEITEM	hRootGpu = vgUI::UIController::getSingleton().getEntryRootItemByType(EFFECT_MOD_AUTOBUILD);

		vgBaseEntry* entry = new vgCore::vgSolidBuildEntry(pNode);

		vgUI::UIController::getSingleton().AddNode( hRootGpu, entry );

		vgUI::UIController::getSingleton().SelectNode(entry);

		pNode->notifyOberversOnEvent(vgKernel::VG_OBS_PROPCHANGED, NULL);
#endif

		if ( find(m_solidBuildNodeList.begin(), m_solidBuildNodeList.end(), pNode) 
			== m_solidBuildNodeList.end() )
		{
			m_solidBuildNodeList.push_back(pNode);
			return true;
		}
		else
		{
			return false;
		}

	}

	bool AutoBuildManager::clearPlan()
	{
		vgKernel::SelectManager::getSingleton().clearSelection();

		for(int i=0; i<m_autoBuildNodeRenderList.size(); i++)
		{
			vgKernel::RendererManager::getSingleton().deleteRenderer(m_autoBuildNodeRenderList[i]);
		}

		m_autoBuildNodeRenderList.clear();

		return true;
	}


	void AutoBuildManager::writeAutoBuildToFile( CFile* pCfile , String savePath )
	{
		stringstream outString;

		//pCfile->Write( &VG_FILE_ID_AUTOBUILDMANAGER, sizeof(unsigned int) );

		pCfile->Write( &VG_FILE_ID_AUTOBUILDMANAGER_V2, sizeof(unsigned int) );

		int nodeCnt = m_autoBuildNodeList.size();
		pCfile->Write( &nodeCnt, sizeof(unsigned int) );

		for (int i=0; i<m_autoBuildNodeList.size(); i++)
		{
			m_autoBuildNodeList[i]->WriteNodeToVG(*pCfile);
		}

		int v2_nodeCnt = 0;
		v2_nodeCnt = m_autoBuildNodeRenderList.size();

		pCfile->Write(&v2_nodeCnt, sizeof(int));

		for (int i=0; i<v2_nodeCnt; i++)
		{
			m_autoBuildNodeRenderList[i]->WriteNodeToVG(*pCfile);
		}

		pCfile->Write( &VG_FILE_ID_AUTOBUILDMANAGER_END, sizeof(int));

	}

	bool AutoBuildManager::readAutoBuildFromFile( CFile* pCfile , String readPath )
	{
		int id;
		stringstream outString;

		vgUI::UIController::getSingleton().GetPropertiesViewBar()->ShowControlBar(FALSE, FALSE, FALSE);

		pCfile->Read(&id, sizeof(unsigned int));

		if (id != VG_FILE_ID_AUTOBUILDMANAGER && id != VG_FILE_ID_AUTOBUILDMANAGER_V2)
		{
			int seekPos = sizeof(unsigned int);

			pCfile->Seek(-seekPos, CFile::current);

			//AfxMessageBox("���棺�ϵ͵�VG�汾�ļ�");

			return false;
		}

		int nodeCnt;

		pCfile->Read( &nodeCnt, sizeof(unsigned int) );

		// #if VGK_LANG_CHS
		// 		outString << "����Autobuild��Ŀ: ";
		// #else
		// 		outString << "\tNumber of Autobuild: ";
		// 		outString << nodeCnt << "\n";
		// #endif
		// 		VGK_SHOW(outString.str());
		// 		outString.str("");
		for (int i=0; i<nodeCnt; i++)
		{
			AutoBuildNode* pNode = new AutoBuildNode;
			pNode->ReadNodeFromVG(*pCfile);
			addAutoBuildNode(pNode);
		}



		int v2_nodeCnt = 0;

		if (id == VG_FILE_ID_AUTOBUILDMANAGER_V2)
		{
			pCfile->Read( &v2_nodeCnt, sizeof(int));

			for (int i=0; i<v2_nodeCnt; i++)
			{
				AutoBuildNode *pNode = new AutoBuildNode;
				pNode->ReadNodeFromVG(*pCfile);
				addPlanNode(pNode);
			}

			pCfile->Read( &v2_nodeCnt, sizeof(int) );
			if ( v2_nodeCnt != VG_FILE_ID_AUTOBUILDMANAGER_END )
			{
#if VGK_LANG_CHS
				VGK_SHOW("���棺��ȡAutoNodeʱ��������.");
#else
				VGK_SHOW("Warning: Err in AutoNode reading.");
#endif

			}
		}
		// Endit  ByFengYK @2010-03-18 19:53 �޸����ڶ�ȡVG���Զ���ʾ������������
		//vgUI::UIController::getSingleton().GetPropertiesViewBar()->ShowControlBar(TRUE, TRUE, TRUE);

		return true;

	}

	void AutoBuildManager::writeSolidBuildToFile( CFile* pCfile , String savePath )
	{
		int ss = m_solidBuildNodeList.size();

		pCfile->Write( &ss, sizeof(int) );
		vector<SolidBuildNode*>::iterator iter = m_solidBuildNodeList.begin();
		vector<SolidBuildNode*>::iterator iterEnd = m_solidBuildNodeList.end();
		for( ; iter != iterEnd; ++iter )
		{
			(*iter)->writeToFile( *pCfile );
		}

	}

	bool AutoBuildManager::readSolidBuildFromFile( CFile* pCfile , String readPath )
	{
		int ss;

		pCfile->Read( &ss, sizeof(unsigned int) );

		for (int i=0; i<ss; i++)
		{
			SolidBuildNode* pNode = new SolidBuildNode;

			if (!pNode->readFromFile(*pCfile))
			{
				return false;
			}

			pNode->initialise();
			addSolidBuildNode(pNode);
		}

		return true;
	}

	void AutoBuildManager::writeMarkBuildToFile( CFile* pCfile , String savePath )
	{
		int mm = m_markBuildNodeList.size();

		pCfile->Write( &mm, sizeof(int) );
		vector<MarkBuildNode*>::iterator iter = m_markBuildNodeList.begin();
		vector<MarkBuildNode*>::iterator iterEnd = m_markBuildNodeList.end();
		for( ; iter != iterEnd; ++iter )
		{
			(*iter)->writeToFile( *pCfile );
		}

	}

	bool AutoBuildManager::readMarkBuildFromFile( CFile* pCfile , String readPath )
	{
		int mm;

		pCfile->Read( &mm, sizeof(unsigned int) );

		for (int i=0; i<mm; i++)
		{
			MarkBuildNode* pNode = new MarkBuildNode;

			if (!pNode->readFromFile(*pCfile))
			{
				return false;
			}

			pNode->initialise();
			addMarkBuildNode(pNode);
		}

		return true;
	}

	bool AutoBuildManager::writeToVGFile(CFile* pCfile , String savePath)
	{
		writeAutoBuildToFile(pCfile, savePath);

		String	vgVersion  = vgMod::VersionControl::getVgVersionWrite();

		if( vgVersion >= String("VRGIS.VERSION 2.21") )
		{
			writeSolidBuildToFile(pCfile, savePath);

			writeMarkBuildToFile(pCfile, savePath);
		}
		
		return true;
	}

	bool AutoBuildManager::readFromVGFile(CFile* pCfile , String readPath)
	{
		readAutoBuildFromFile(pCfile, readPath);

		String	vgVersion  = vgMod::VersionControl::getVgVersionWrite();
		
		if ( vgVersion >= String("VRGIS.VERSION 2.21") )
		{
			readSolidBuildFromFile(pCfile, readPath);

			readMarkBuildFromFile(pCfile, readPath);
		}

		return true;
	}

	bool AutoBuildManager::importShpInfo()
	{
		CString DefExt;

		DefExt.Format("%s","shp�ļ�(*.shp)|*.shp|");

		CFileDialog dlgmodelFile(true,"shp",
			NULL,OFN_HIDEREADONLY|
			OFN_CREATEPROMPT|
			OFN_NONETWORKBUTTON|
			OFN_ALLOWMULTISELECT|   
			OFN_FILEMUSTEXIST,
			DefExt);

		DWORD   MAXFILE = 0xFFFF;   
		dlgmodelFile.m_ofn.nMaxFile=MAXFILE; 

		char*  pc = new char[MAXFILE];   
		
		dlgmodelFile.m_ofn.lpstrFile = pc;   
		dlgmodelFile.m_ofn.lpstrFile[0] = NULL;   
		dlgmodelFile.m_ofn.lpstrTitle="shp�ļ�����";
		
		String filename;

		if( dlgmodelFile.DoModal() == IDOK )
		{
			filename = dlgmodelFile.GetPathName();   
		}
		else
		{
			return false;
		}

		return importSpecialShpInfo(filename);


		// ��ʩ����Դ���ȴ��û�����Ҫʱ���ͷ�.
		// OGRFeature::DestoryFeature(poFeature);
		// OGRDataSource::DestroyDataSource( poDS );

		return true;
	}

	bool AutoBuildManager::addAutoShpInfo(int sid, OGRFeature* pInfoFeature, OGRGeometry *pGemo)
	{
#if 0
		// ȡ�����е�Grid
		vgShape::Layer *pLayer = vgShape::ShapeManager::getSingleton().getLayerByShortName("Grid");

		if (pLayer == NULL)
		{
			AfxMessageBox("�����ڵ�Layer");

			return false; 
		}

		vgShape::GeometryPointerVec* geomVec = pLayer->getGeometries();

		for ( int i=0; i<(*geomVec).size(); i++ )
		{
			vgShape::Geometry *pPoly = (*geomVec)[i];


			vgShape::SpecialPolygon *pSpoly = dynamic_cast<vgShape::SpecialPolygon*>(pPoly);
		
			OGRFeature *pFeature = pSpoly->getOGRFeature();
			int id = pFeature->GetFieldAsInteger("ID");

			if (id == sid)
			{
				pSpoly->setAutoInfo(pInfoFeature, pGemo->clone());

// 				std::stringstream outString;
// 
// 				outString << "����Grid "<< id <<"\n";
// 				VGK_SHOW(outString.str());
// 
// 				outString.str("");
			}
		}	
#endif

		return true;
	}

	bool AutoBuildManager::removeAllShpInfo()
	{
#if 0
		// ȡ�����е�Grid
		if (! m_shpInfoIsOpen)
		{
			return false;
		}

		vgShape::Layer *pLayer = vgShape::ShapeManager::getSingleton().getLayerByShortName("Grid");

		if (pLayer == NULL)   
		{
			AfxMessageBox("�����ڵ�Layer");

			return false; 
		}

		vgShape::GeometryPointerVec* geomVec = pLayer->getGeometries();

		for ( int i=0; i<(*geomVec).size(); i++ )
		{
			vgShape::Geometry *pPoly = (*geomVec)[i];

			vgShape::SpecialPolygon *pSpoly = dynamic_cast<vgShape::SpecialPolygon*>(pPoly);

			OGRFeature *pFeature = pSpoly->getOGRFeature();
			int id = pFeature->GetFieldAsInteger("ID");

			pSpoly->unsetAutoInfo();
	
		}	

		OGRDataSource::DestroyDataSource( m_pDS );
		m_shpInfoIsOpen = false;


#endif


		return true;
	}

	bool AutoBuildManager::saveInfoToShpFile()
	{
#if 0
		stringstream outString;

		const char *pszDriverName = "ESRI Shapefile";
		OGRSFDriver *poDriver;
		OGRRegisterAll();

		poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(
			pszDriverName );

		if( poDriver == NULL )
		{
			AfxMessageBox("driver not available.");
			return false;
		}

		OGRDataSource *poDS;
		poDS = poDriver->CreateDataSource( "f:\\point_out.shp", NULL );
		if ( poDS == NULL )
		{
			poDS = poDriver->CreateDataSource( "f:\\point_out.shp", NULL );

			AfxMessageBox( "Creation of output file failed.");
			return false;
		}

		OGRLayer *poLayer;
		poLayer = poDS->CreateLayer( "shpinfo", NULL, wkbPolygon25D, NULL );
		if( poLayer == NULL )
		{
			AfxMessageBox( "Layer creation failed." );
			return false;
		}

		OGRFeatureDefn* def = poLayer->GetLayerDefn();

		OGRFieldDefn oField( "ID", OFTString );

		oField.SetWidth(32);
		if( poLayer->CreateField( &oField ) != OGRERR_NONE )
		{
			AfxMessageBox( "Creating Name field failed." );
			return false;
		}

		//------------------------------------------
		// leven add
		//------------------------------------------
		// �����˿��ܶ�
		//OGRFieldDefn gridNowBuildN( "NowBuildN", OFTInteger );

		//if( poLayer->CreateField( &gridNowBuildN ) != OGRERR_NONE )
		//{
		//	AfxMessageBox( "Creating gridNowBuildN failed." );
		//	return false;
		//}

		// ������λField
		OGRFieldDefn gridNumFiled( "AvailGridN", OFTInteger );

		if( poLayer->CreateField( &gridNumFiled ) != OGRERR_NONE )
		{
			AfxMessageBox( "Creating Name field failed." );
			return false;
		}
		
		OGRFieldDefn gridSizeXFiled("GridSizeX", OFTReal);
		if( poLayer->CreateField( &gridSizeXFiled ) != OGRERR_NONE )
		{
			AfxMessageBox( "Creating Name field failed." );
			return false;
		}

		OGRFieldDefn gridSizeYFiled("GridSizeY", OFTReal);
		if( poLayer->CreateField( &gridSizeYFiled ) != OGRERR_NONE )
		{
			AfxMessageBox( "Creating Name field failed." );
			return false;
		}

		for (int i=0; i<ShpGridNum; i++)
		{
			outString.str("");
			outString<<"Grid_"<<i<<"_x";

			OGRFieldDefn gridPosFieldX( outString.str().c_str(), OFTReal);

			if( poLayer->CreateField( &gridPosFieldX ) != OGRERR_NONE )
			{
				AfxMessageBox( "Creating Grid Y field failed." );
				return false;
			}

			outString.str("");
			outString<<"Grid_"<<i<<"_y";

			OGRFieldDefn gridPosFieldY( outString.str().c_str(), OFTReal);

			if( poLayer->CreateField( &gridPosFieldY ) != OGRERR_NONE )
			{
				AfxMessageBox( "Creating Grid X field failed." );
				return false;
			}
		}

		// ������е�years����
		int yearIndex;

		// modify ����shp��txt����field
		for (int i=0; i<4; i++)
		{
			for (int j=0; j<101; j++)
			{
				outString.str("");
				outString.clear();

				yearIndex = 2000 + j;
				outString<<scheStr[i]<<"_"<< yearIndex;

				OGRFieldDefn oField( outString.str().c_str(), OFTInteger );

				if( poLayer->CreateField( &oField ) != OGRERR_NONE )
				{
					AfxMessageBox( "Creating Name field failed." );
					return false;
				}
			}
		}

		// ȡ�����е�Grid
		vgShape::Layer *pLayer = vgShape::ShapeManager::getSingleton().getLayerByShortName("Grid");

		if (pLayer == NULL)
		{
			AfxMessageBox("�����ڵ�Layer");
		}

		vgShape::GeometryPointerVec* geomVec = pLayer->getGeometries();

		outString << "����Grid "<< (*geomVec).size()<<"\n";
		VGK_SHOW(outString.str());

		outString.str("");

		for ( int i=0; i<(*geomVec).size(); i++ )
		{
			vgShape::Geometry *pPoly = (*geomVec)[i];

			vgShape::SpecialPolygon *pSpoly = dynamic_cast<vgShape::SpecialPolygon*>(pPoly);

			if (pSpoly == NULL)
			{
				VGK_SHOW("ת��ʧ��!\n");		
				continue ;
			}
			else
			{
				OGRGeometry *pGeometry = pSpoly->getResultGemo();

				if (pGeometry == NULL)
				{
					continue;
				}

				//pSpoly->constructInsectVec();

				OGRFeature *pFeature = pSpoly->getOGRFeature();
				string id = pFeature->GetFieldAsString("ID");

				OGRFeature *poFeature;
				poFeature = new OGRFeature( poLayer->GetLayerDefn() );

				poFeature->SetField( "ID", id.c_str() );

				// д���λ��Ϣ

				vector<Vector2> gridVec = pSpoly->getAvailGridVec();

				//------------------------------------------
				// leven add
				//------------------------------------------
				//poFeature->SetField("NowBuildN", pSpoly->getNowBuildingNum() );

				poFeature->SetField("AvailGridN", (int)gridVec.size());
				poFeature->SetField("GridSizeX", pSpoly->getGridSizeX());
				poFeature->SetField("GridSizeY", pSpoly->getGridSizeY());
				
				for (int gi=0; gi<gridVec.size(); gi++)
				{
					outString.str("");
					outString<<"Grid_"<<gi<<"_x";
					poFeature->SetField(outString.str().c_str(), gridVec[gi].x);

					outString.str("");
					outString<<"Grid_"<<gi<<"_y";
					poFeature->SetField(outString.str().c_str(), gridVec[gi].y);
				}
//  // ���� ��ʱ����txt��Ϣд��
//  #if 0
				// modify д��txt��Ϣ
				for (int i2=0; i2<4; i2++)
				{
					int *pYearData = m_yearsParam[i2][id];

					if (pYearData == NULL)
					{
						VGK_SHOW("No data in Txt File.\n");
						break;
					}
					for (int j2=0; j2<101; j2++)
					{
						outString.str("");
						outString.clear();

						yearIndex = 2000 + j2;
						outString<<scheStr[i2]<<"_"<< yearIndex;

						poFeature->SetField( outString.str().c_str(), pYearData[j2]);	
					}
				}
/*#endif*/
				//OGRGeometry *pGeometry = pFeature->GetGeometryRef();

				if (poFeature->SetGeometry( pGeometry) != OGRERR_NONE)
				{
					AfxMessageBox( "Failed to set feature in shapefile.\n" );
				}


				int returnValue = poLayer->CreateFeature( poFeature );

				if( returnValue != OGRERR_NONE )
				{
					OGRwkbGeometryType gtype = pGeometry->getGeometryType();
					AfxMessageBox( "Failed to create feature in shapefile.\n" );
					return false;
				}

				// OGRFeature::DestroyFeature(pFeature);
				OGRFeature::DestroyFeature(poFeature);

				outString.str("");
				outString << "�Ѿ�д��"<< i << "/" << (*geomVec).size() << "\n";
				VGK_SHOW(outString.str());
			}
		}


		OGRDataSource::DestroyDataSource( poDS );

#endif

		return true;
	}

	bool AutoBuildManager::generateBuildingAll(int schem, int years)
	{
#if 0
		// �����plan
		vgUI::UIController::getSingleton().GetPropertiesViewBar()->ShowControlBar(FALSE, FALSE, FALSE);

		clearPlan();

		vgShape::Layer *pLayer = vgShape::ShapeManager::getSingleton().getLayerByShortName("Grid");

		if (pLayer == NULL)
		{
			AfxMessageBox("�����ڵ�Layer");

			return false; 
		}

		vgShape::GeometryPointerVec* geomVec = pLayer->getGeometries();

		for ( int i=0; i<(*geomVec).size(); i++ )
		{
			vgShape::Geometry *pPoly = (*geomVec)[i];

			vgShape::SpecialPolygon *pSpoly = dynamic_cast<vgShape::SpecialPolygon*>(pPoly);

			if (pSpoly == NULL)
			{
				continue ;
			}
			
			OGRGeometry *pGeom = pSpoly->getResultGemo();
			
			if (pGeom == NULL)
			{
				continue ;
			}
			if (pSpoly->generateAutobuilding(m_autoBuildNodeList, schem, years))
			{
				TRACE("Success on %d\n", i);
			}
			else
			{
				TRACE("Failed on %d \n", i);
			}
			// ���㷿��λ��.
		}

		// leven add
		//------------------------------------------
		// ѡ����ӵ���������i
		//------------------------------------------
		vgKernel::SelectManager::getSingleton().clearSelection();

		for(int i=0; i<m_autoBuildNodeRenderList.size(); i++)
		{
			vgKernel::SelectManager::getSingleton().addSelection(
				m_autoBuildNodeRenderList[i]);
		}


#endif
		return true;
	}
	
	bool AutoBuildManager::generateBuildingSelected(int schem, int years)
	{
#if 0
		//------------------------------------------
		// leven add begin
		//------------------------------------------
		vgKernel::SelectManager::getSingleton().clearSelection();

		// �������ѡ�е�special�����е�����.
		using namespace vgShape;
		GeometryPointerVec* selectgeos = getSelectedSpecialPolygons();
		if ( selectgeos == NULL )
		{
			AfxMessageBox("Please select a region.");
			return false;
		}

		GeometryPointerVec::iterator iter = selectgeos->begin();
		GeometryPointerVec::iterator iter_end = selectgeos->end();

		for ( ; iter != iter_end ; ++ iter )
		{
			vgShape::SpecialPolygon* speical = 
				dynamic_cast<vgShape::SpecialPolygon *>( *iter );

			assert( speical != NULL );

			vector<vgVector::AutoBuildNode*>* autos = 
				speical->getAutoBuildNodes();
			assert( autos != NULL );

			vector<vgVector::AutoBuildNode*>::iterator i = autos->begin();
			vector<vgVector::AutoBuildNode*>::iterator i_end = autos->end();

			for ( ; i != i_end ; ++ i )
			{
				vgVector::AutoBuildNode* pNode = *i;

				vector<vgVector::AutoBuildNode*>::iterator findre = 
					find(m_autoBuildNodeRenderList.begin(), 
					m_autoBuildNodeRenderList.end(), pNode);

				if ( findre != m_autoBuildNodeRenderList.end())
				{
					RendererManager::getSingleton().deleteRenderer( *findre );
				}
			}

			speical->clearAutoBuildNodes();

		}


		//------------------------------------------
		// leven add end
		//------------------------------------------

		vgUI::UIController::getSingleton().GetPropertiesViewBar()->ShowControlBar(FALSE, FALSE, FALSE);

		// clearPlan();

		vgShape::Selector* selector = 
			vgShape::ShapeManager::getSingleton().getSelector();

		if ( selector == NULL )
		{
			return false;
		}

		vgShape::Layer* layer = selector->getLayerPtr();




			vgShape::Layer *pSelectedLayer = layer;

			if (pSelectedLayer == NULL)
			{
				return false;
			}

			vgShape::GeometryPointerVec *pGemoVec = pSelectedLayer->getSelectedGeometry();

			for (int i=0; i<(*pGemoVec).size(); i++)
			{
				vgShape::SpecialPolygon *pSPoly = dynamic_cast<vgShape::SpecialPolygon*>((*pGemoVec)[i]);

				if (pSPoly != NULL)
				{
					pSPoly->generateAutobuilding(m_autoBuildNodeList, schem, years);
				}
			}

			//------------------------------------------
			// leven add
			//------------------------------------------
			{
				vgKernel::SelectManager::getSingleton().clearSelection();

				// �������ѡ�е�special�����е�����.
				using namespace vgShape;
				GeometryPointerVec* selectgeos = getSelectedSpecialPolygons();
				if ( selectgeos == NULL )
				{
					return false;
				}

				GeometryPointerVec::iterator iter = selectgeos->begin();
				GeometryPointerVec::iterator iter_end = selectgeos->end();

				for ( ; iter != iter_end ; ++ iter )
				{
					vgShape::SpecialPolygon* speical = 
						dynamic_cast<vgShape::SpecialPolygon *>( *iter );

					assert( speical != NULL );

					vector<vgVector::AutoBuildNode*>* autos = 
						speical->getAutoBuildNodes();
					assert( autos != NULL );

					vector<vgVector::AutoBuildNode*>::iterator i = autos->begin();
					vector<vgVector::AutoBuildNode*>::iterator i_end = autos->end();

					for ( ; i != i_end ; ++ i )
					{
						SelectManager::getSingleton().addSelection( *i );
					}

				}
			}


#endif

		return true;
	}


	void AutoBuildManager::readTxtFile(String shpfilepath, String txtfilepath, YearsParmMap& inforMap)
	{		
		vector<string> strVec;
		vector<string> typeVec;

		//�������������¼��Id
		OGRDataSource	*poDS = OGRSFDriverRegistrar::Open( shpfilepath.c_str(), FALSE );

		if( poDS == NULL ) 
		{
			MessageBox(NULL, "shape file does not exist", shpfilepath.c_str(), MB_OK);
			return;
		}

		OGRLayer  *poLayer;

		poLayer = poDS->GetLayer(0);

		OGRFeature *poFeature;

		poLayer->ResetReading();

		while( (poFeature = poLayer->GetNextFeature()) != NULL )
		{
			String shpId;

			OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
			int iField;
			String strFieldName;
			//���������Ϣ
			for( iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
			{
				OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );

				strFieldName = poFieldDefn->GetNameRef();

				if (strFieldName == "ID")
				{
					shpId = poFeature->GetFieldAsString(iField); 
					strVec.push_back(shpId);
				}

			}

		}

		OGRDataSource::DestroyDataSource( poDS );


		//��txt�ļ�
		ifstream file;  
		//  	filename2 = "D:\\ad.txt";
		file.open(txtfilepath.c_str());

		if ( file.fail() )
		{
			assert(0);
			return;
		}

		//��ȡtxt�ļ������ݶ���c�����ڣ�169817*102
		string (*c)[102] = new string[169817][102];

		for (long i = 0; i< 169817; i++)
		{
			for (int j=0; j< 102;j++)
			{
				file >>  c[i][j];
			}
		}

		file.close();

		if (inforMap.size() != 0)
		{
			map<string, int*>::iterator iter	 = inforMap.begin();
			map<string, int*>::iterator iter_end = inforMap.end();

			while (iter != iter_end)
			{
				delete (*iter).second;
				iter ++;
			}
		}

		inforMap.clear();

		//��c����ȡ������Ҫ��ļ�¼������d���飬229*102,
		//���ڱ����������飬�˴�Ч�ʽ���
		string d[229][102];
		long row = 0;
		vector<String>::iterator iter;

		for (long k = 1; k < 169817; k++)
		{
			iter = find(strVec.begin(),strVec.end(),c[k][0]);
			
			if (iter != strVec.end())
			{
				int *cntArray = new int[101];

 				for (long column = 0; column < 101; column++)
 				{
					stringstream ss;
					int count;
					ss << c[k][column+1];
					ss >> count;

					cntArray[column] = count;
 					// d[row][column] = c[k][column];
 				}
				inforMap.insert(make_pair((*iter), cntArray));

				// row++;
			}
		}

		//����洢���ͣ�����ݷ����¼���еؿ�grid��Ϣ
// 		for (int j = 1; j < 102; j++)
// 		{
// 			YearsInfo inf;
// 
// 			inf.year = d[0][j];
// 
// 			for (int i = 1; i < 229; i++ )
// 			{
// 				stringstream ss;
// 				int count;
// 				ss << d[i][j];
// 				ss >> count;
// 
// 				inf.id_count.insert(make_pair(d[i][0], count));
// 			}
// 
// 			inforvec.push_back(inf);
// 		}


		delete []c; 
		return;

	}

 	void AutoBuildManager::OnReadTxt()
 	{
 		String shppath;
 		String txtpath;

 
 		std::ostringstream o;
 		o	<< "-----------------------------------------------------------\n" 
 			<< "\t\t����ִ�� ����shp����:\n"
 			<< "-----------------------------------------------------------\n" ;
 		VGK_SHOW( o.str() );
 
 		CString DefExt;
 
 		DefExt.Format("%s","shp�ļ�(*.shp)|*.shp|");
 
 		CFileDialog dlgmodelFile(true,"shp",
 			NULL,OFN_HIDEREADONLY|
 			OFN_CREATEPROMPT|
 			OFN_NONETWORKBUTTON|
 			OFN_ALLOWMULTISELECT|   
 			OFN_FILEMUSTEXIST,
 			DefExt);
 
 		DWORD   MAXFILE = 0xFFFF;   
 		dlgmodelFile.m_ofn.nMaxFile=MAXFILE; 
 
 		char*  pc = new char[MAXFILE];   
 
 		dlgmodelFile.m_ofn.lpstrFile = pc;   
 		dlgmodelFile.m_ofn.lpstrFile[0] = NULL;   
 		dlgmodelFile.m_ofn.lpstrTitle="shp�ļ�����";
 
 		if( dlgmodelFile.DoModal() == IDOK )
 		{
 			POSITION posi;   
 			vgKernel::StringVector strVec;
 
 			posi = dlgmodelFile.GetStartPosition();   
 
 			while( posi != NULL )
 			{   
 				strVec.push_back( dlgmodelFile.GetNextPathName(posi).GetBuffer(0));
 			}   
 
 			delete[] pc;
 
 			//------------------------------------------
 			// ��ʼ����
 			//------------------------------------------
 
 			vgKernel::StringVector::iterator iter = strVec.begin();
 			vgKernel::StringVector::iterator iter_end = strVec.end();
 
 			shppath = *iter;
 
 		}		
 
 		std::ostringstream oEnd;
 		oEnd<< "-----------------------------------------------------------\n" 
 			<< "\t\t���ִ�� ����shp����\n"
 			<< "-----------------------------------------------------------\n" ;
 		VGK_SHOW( oEnd.str() );
 
 		//////////////////////////////////////////////////
 
 		std::ostringstream o2;
 		o	<< "-----------------------------------------------------------\n" 
 			<< "\t\t����ִ�� ����txt����:\n"
 			<< "-----------------------------------------------------------\n" ;
 		VGK_SHOW( o2.str() );
 
 		CString DefExt2;
 
 		DefExt2.Format("%s","txt�ļ�(*.txt)|*.txt|");
 
 		CFileDialog dlgmodelFile2(true,"txt",
 			NULL,OFN_HIDEREADONLY|
 			OFN_CREATEPROMPT|
 			OFN_NONETWORKBUTTON|
 			OFN_ALLOWMULTISELECT|   
 			OFN_FILEMUSTEXIST,
 			DefExt2);
 
 		DWORD   MAXFILE2 = 0xFFFF;   
 		dlgmodelFile2.m_ofn.nMaxFile=MAXFILE2; 
 
 		char*  pc2 = new char[MAXFILE2];   
 
 		dlgmodelFile2.m_ofn.lpstrFile = pc2;   
 		dlgmodelFile2.m_ofn.lpstrFile[0] = NULL;   
 		dlgmodelFile2.m_ofn.lpstrTitle="txt�ļ�����";
 
 		if( dlgmodelFile2.DoModal() == IDOK )
 		{
 			POSITION posi;   
 			vgKernel::StringVector strVec;
 
 			posi = dlgmodelFile2.GetStartPosition();   
 
 			while( posi != NULL )
 			{   
 				strVec.push_back( dlgmodelFile2.GetNextPathName(posi).GetBuffer(0));
 			}   
 
 			delete[] pc2;
 
 			//------------------------------------------
 			// ��ʼ����
 			//------------------------------------------
 
 			vgKernel::StringVector::iterator iter = strVec.begin();
 			vgKernel::StringVector::iterator iter_end = strVec.end();
 
 			txtpath = *iter;

 			DlgAutobuildImportTxt dlgSchem;
			if (dlgSchem.DoModal() == IDOK)
			{
				//��ʼ��ȡ�ļ�
				if (dlgSchem.getSchemName() == "GS")
				{
					readTxtFile(shppath, txtpath, m_yearsParam[0]);
				}
				else if (dlgSchem.getSchemName() == "LS")
				{
					readTxtFile(shppath, txtpath, m_yearsParam[1]);
				}
				else if (dlgSchem.getSchemName() == "NE")
				{
					readTxtFile(shppath, txtpath, m_yearsParam[2]);
				}
				else
				{
					readTxtFile(shppath, txtpath, m_yearsParam[3]);
				}
			} 
 		}		
 
 
 		std::ostringstream oEnd2;
 		oEnd2<< "-----------------------------------------------------------\n" 
 			<< "\t\t���ִ�� ����txt����\n"
 			<< "-----------------------------------------------------------\n" ;
 		VGK_SHOW( oEnd2.str() );
 
 		return;
 	}

	bool AutoBuildManager::importSpecialShpInfo( const String &filename )
	{
		m_pDS = OGRSFDriverRegistrar::Open( filename.c_str(), FALSE );

		if( m_pDS == NULL ) 
		{
			AfxMessageBox("shape�ļ������ڻ��޷���!");
			return false;
		}
		else
		{
			m_shpInfoIsOpen = true;
		}

		OGRLayer  *poLayer;

		poLayer = m_pDS->GetLayer(0);

		OGRFeature *poFeature;

		poLayer->ResetReading();

		while( (poFeature = poLayer->GetNextFeature()) != NULL )
		{
			int id = poFeature->GetFieldAsInteger("ID");

			//��ü�����Ϣ
			OGRGeometry *poGeometry;

			poGeometry = poFeature->GetGeometryRef();

			vgAutoBuild::AutoBuildManager::getSingleton().addAutoShpInfo(id, poFeature, poGeometry);

		}

		return true;
	}


	vgShape::GeometryPointerVec* AutoBuildManager::getSpecialPolygons()
	{
		//vgShape::Layer* layer = 
		//	vgShape::ShapeManager::getSingleton().getLayerByShortName("Grid");

		//assert( layer != NULL );

		//vgShape::GeometryPointerVec* ret = layer->getGeometries();

		//assert( ret != NULL );
		//assert( ret->size() != 0 );

		//vgShape::SpecialPolygon* speical = 
		//	dynamic_cast<vgShape::SpecialPolygon *>( (*ret)[0] );
		//
		//if( speical == NULL)
		//{
		//	assert(0);
		//}

		//return ret;

		return NULL;
	}

	vgShape::GeometryPointerVec* AutoBuildManager::getSelectedSpecialPolygons()
	{
		//vgShape::Layer* layer = 
		//	vgShape::ShapeManager::getSingleton().getLayerByShortName("Grid");
		using namespace vgKernel;
		vgKernel::Plugin* plug = PluginManager::getSingleton().getPluginRef( VGSH_DLL_NAME );

		if ( plug == NULL )
		{
			return NULL;
		}

		VGK_DLL_DECLARE_CLASSFUNC( plug, ShapeManager, getLayerByShortName, pfn1 );

		assert( pfn1 != NULL);

		vgShape::Layer* pLayer = (vgShape::Layer*)pfn1("Grid");
		
		assert( pLayer != NULL );

		//vgShape::GeometryPointerVec* ret = pLayer->getSelectedGeometry();
		VGK_DLL_DECLARE_CLASSFUNC( plug, ShapeManager, getSelectGeometriesFromLayer, pfn2 );
		vgShape::GeometryPointerVec* ret = (vgShape::GeometryPointerVec*)
			(*pfn2)( (vgShape::Layer *)pLayer );
		
		return ret;
	}

	void AutoBuildManager::generateSolidBuild()
	{
		bool bDelete = false, bCreate = false;

		if( m_solidBuildHandler != NULL )
		{
			bDelete = true;
		}
		else
		{
			bCreate = true;
		}

		if (bDelete)
		{
			delete m_solidBuildHandler;
			m_solidBuildHandler = NULL;
		}

		if (bCreate)
		{
			m_solidBuildHandler = new SolidBuildHandler();
		}


	}

	void AutoBuildManager::generateMarkBuild()
	{
		if (m_markBuildHandler != NULL)
		{
			delete m_markBuildHandler;
			m_markBuildHandler = NULL;
		}
		m_markBuildHandler = new MarkBuildHandler();

	}

	bool AutoBuildManager::getGenerateBuildFlag()
	{
		if ( m_solidBuildHandler == NULL )
		{
			return false;
		}

		return true;
	}

	void AutoBuildManager::renderElements()
	{
		if (m_solidBuildHandler != NULL)
		{
			m_solidBuildHandler->render();
		}

		if (m_markBuildHandler != NULL)
		{
			m_markBuildHandler->render();
		}
	}

	bool AutoBuildManager::initialise()
	{
		m_pAutoBuildRenderCommand = new AutoBuildRenderCommand();

		vgKernel::RenderCommandFacade::AddCommand(m_pAutoBuildRenderCommand);

		return true;
	}

	bool AutoBuildManager::shutdown()
	{
		if (m_pAutoBuildRenderCommand != NULL)
		{
			vgKernel::RenderCommandFacade::RemoveCommand(m_pAutoBuildRenderCommand);

			m_pAutoBuildRenderCommand = NULL;
		}

		return true;
	}

	void AutoBuildManager::OnLButtonDbClick( UINT nFlags, CPoint position )
	{
		if (!m_markEnable)
		{
			return;
		}

		MarkBuildNode *pClickedMark = testClick(position);

		if (pClickedMark != NULL && pClickedMark->getMarkEnable())
		{
			pClickedMark->fireClicked();
		}
	}

	MarkBuildNode* AutoBuildManager::testClick( CPoint position )
	{
		using namespace vgKernel;

		Renderer* tmpMesh = NULL;
		Vec3 click_pos = vgKernel::Math::trans2DPointTo3DVec(position.x, position.y);
		Vec3 camera_pos = vgCam::CamManager::getSingleton().getCurrentPosition();
		vgKernel::Ray translatedRay;


		Vec3 click_dir = click_pos - camera_pos;
		click_dir.normalise();

		vgKernel::Ray click_ray(camera_pos , click_dir);
		std::pair<bool, float> result;

		vgKernel::RendererPackage* _culledRenderers = 
			RendererManager::getSingletonPtr()->getCulledRendererPackage();
		RendererPackage::iterator ipac = _culledRenderers->begin();
		RendererPackage::iterator ipac_end = _culledRenderers->end();

		RendererQueue resultQueue;

		//m_bufferMap.clear();
		float distance = 1e10f;

		for ( ; ipac != ipac_end; ++ ipac )
		{
			RendererQueue* que = &ipac->second;
			assert( que != NULL );

			RendererQueue::iterator iter  = que->begin();
			RendererQueue::iterator iter_end = que->end();

			for ( ; iter != iter_end ; ++ iter )
			{
				Renderer* cr = *iter;

				if ( (cr->isSelectable() == false || cr->getVisible() == false))
				{
					continue;
				}

				result = vgKernel::Math::intersects( click_ray, cr->getBoundingBox() );

				if (result.first && result.second < distance)
				{
					std::pair<bool, float> testres = 
						cr->testIntersectWithTriangles( click_ray );

					if ( testres.first == true && testres.second < distance )
					{

						if (find(resultQueue.begin(), resultQueue.end(), cr) != resultQueue.end())
						{
							//VGK_SHOW("Fixed \n");
							continue;
						}

						if (!resultQueue.empty() && tmpMesh != NULL)
						{
							resultQueue.pop_back();
						}

						resultQueue.push_back( cr );
						tmpMesh = cr;
						distance = testres.second;

						TRACE("Select %s Distance : %f \n", cr->getName().c_str(), distance);
					}
				}

			}
		}// end ipac

		if (resultQueue.size() == 1 && resultQueue[0]->getType() == RENDERER_TYPE_LANDMARKBUILD)
		{	
			return dynamic_cast<MarkBuildNode*>(resultQueue[0]);
		}

		return NULL;

	}

	void AutoBuildManager::setAllMarkEnable(bool enable) 
	{
		m_markEnable = enable; 

		for (int i = 0; i< m_markBuildNodeList.size(); i++)
		{
			m_markBuildNodeList.at(i)->setMarkEnable(m_markEnable);
		}
	}
}
