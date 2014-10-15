#include <vgStableHeaders.h>
#include <vgGIS3D/vggiContourCreateFacade.h>

#include <vgGIS3D/vggiContour.h>
#include <vgGIS3D/vggiContourManager.h>

#include <vgkernel/vgkBox.h>
#include <vgKernel/vgkPluginManager.h>
#include <vgTerritory/vgtDefinition.h>
#include <limits>
#include <vgGIS3D/vggiDlgContourVtr.h>
#include <vgGIS/vgGisManager.h>
#include <vgGIS3D/vggiProcessingDlg.h>

namespace vgGIS3D
{

	bool ContourCreateFacade::createFromGrdFile(string name, const vector<vgKernel::Vec3>& boundPoints)
	{
		vgKernel::Box area = vgKernel::Box::NULL_BOX;

		// ���ݵ����ɱ߽�
		for (int i=0; i<boundPoints.size(); i++)
		{
			area.merge(boundPoints.at(i));
		}

		CString szFilterFDlg = "�������� (*.grd)|*.grd||";
		CFileDialog fileDlg(TRUE, NULL, NULL, NULL,szFilterFDlg, NULL); 
		
		
		if(fileDlg.DoModal() == IDOK) 
		{
			ContourGeoData *pGeoData = new ContourGeoData;
			GridDataInfo   dataInfo;
			Contour *pContour = new Contour(NULL, name);

			CString  filepath = fileDlg.GetPathName();//����ļ�  MessageBox(filepath);//·����û�а�����չ��
			if( pGeoData->ReadData(filepath) == FALSE )
				return FALSE;
			
			dataInfo = pGeoData->GetDataInfo();

			dataInfo.xMin = area.getMinimum().x;
			dataInfo.xMax = area.getMaximum().x;
			dataInfo.yMin = area.getMinimum().z;
			dataInfo.yMax = area.getMaximum().z;
			pGeoData->setGridDataInfo(dataInfo);

			pContour->setDataDirectly(pGeoData);
			
			pContour->GenerateDefaultContourValues();
			pContour->GenerateContours();
			pContour->setBoundingBox(area);

			ContourManager::getSingleton().addContour(pContour);
			ContourManager::getSingleton().traceAllContour();
		} 

		return TRUE;
	}

	bool ContourCreateFacade::createFromVtrArea(const vector<vgKernel::Vec3>& boundPoints)
	{
//		vgUserInterface::GuiManager::getSingleton().showProcessingDlg(true);
		
// 		vgProcessingDlg dlg1;
// 		dlg1.DoModal();
		vgKernel::Box area = vgKernel::Box::NULL_BOX;
		
		// ���ݵ����ɱ߽�
		for (int i=0; i<boundPoints.size(); i++)
		{
			area.merge(boundPoints.at(i));
		}

		vgDlgContourVtr dlg(area.getSize());
		
		// Edit By FengYK @2010-03-26 22:13
		// �޸��˴󳡾��¶Ի�����˫�����һ��ʱ��ŵ���������
		CWnd* pCWnd = vgKernel::SystemUtility::getCurrentViewWnd();
		pCWnd->SetTimer( 1, 150, NULL );
		
		if (dlg.DoModal() != IDOK)
		{
			pCWnd->SetTimer( 1, 15, NULL );
			return false;
		}
		pCWnd->SetTimer( 1, 15, NULL );

		GisManager::getSingleton().showProcessingDlg(true);
		GisManager::getSingleton().setProcessingInfo("���ڽ��е����������...");

		vgGIS3D::GisManager::getSingleton().showProcessingDlg(true);

		string name = dlg.getContourName();

		float slice = dlg.getStep();

		int numOfLine = dlg.getNumOfLine();

		// ��������
	
		vgKernel::Plugin *pTerrainPlugin 
			= vgKernel::PluginManager::getSingleton().getPluginRef(VGT_DLL_NAME);

		if ( pTerrainPlugin == NULL )
		{
			AfxMessageBox("û�м��ص���ģ��.��������ֹ");
			vgGIS3D::GisManager::getSingleton().showProcessingDlg(false);
			return false;
		}

		// ��Ϊʹ�ò��ʽ���ã�GISģ�鲻����������ģ��
		pfnTerrainManager_hasTerrainData pfHasTData 
			= (pfnTerrainManager_hasTerrainData)GetProcAddress(pTerrainPlugin->getPluginHandle(), "TerrainManager_hasTerrainData");

		bool haveterrain = (*pfHasTData)();
		if ( haveterrain == false )
		{
			AfxMessageBox("û�м��ص�������.��������ֹ");
			vgGIS3D::GisManager::getSingleton().showProcessingDlg(false);
			return false;
		}

		pfnTerrainManager_getElevationByPosition pfGetElevation 
			= (pfnTerrainManager_getElevationByPosition)GetProcAddress(pTerrainPlugin->getPluginHandle(), "TerrainManager_getElevationByPosition");

		GridDataInfo   dataInfo;

		int rows, cols;
			
		rows = area.getSize().z / slice;
		cols = area.getSize().x / slice;
		
		dataInfo.cols = cols;
		dataInfo.rows = rows;
		dataInfo.xMin = area.getMinimum().x;
		dataInfo.xMax = area.getMaximum().x;
		dataInfo.yMin = area.getMinimum().z;
		dataInfo.yMax = area.getMaximum().z;
		dataInfo.maxVal = -std::numeric_limits<float>::infinity();
		dataInfo.minVal = std::numeric_limits<float>::infinity();

		float **ppRawData = new float*[rows];

		TRACE("%d %d \n", rows, cols);

		for (int i=0; i<rows; i++)
		{
			ppRawData[i] = new float[cols];
			float elva;
		
			GisManager::getSingleton().setProcessingProgress((int)(((float)i/rows) * 100));

			for (int j=0; j<cols; j++)
			{
				vgKernel::Vec3 pos(area.getMinimum().x + slice*j, 0, area.getMinimum().z +slice*i);

				bool getok = pfGetElevation(pos, elva , true );

				ppRawData[i][j] = elva;
				TRACE("%f ", elva);
				if (elva < dataInfo.minVal)
				{
					dataInfo.minVal = elva;
				}
				else if (elva > dataInfo.maxVal)
				{
					dataInfo.maxVal = elva;
				}
			}
			TRACE("\n");

		}

		GisManager::getSingleton().setProcessingInfo("��ʼ��ֵ��׷��...");

		createContour(name, area, numOfLine, dataInfo, ppRawData, NULL, true);
	
//		vgUserInterface::GuiManager::getSingleton().showProcessingDlg(false);
		GisManager::getSingleton().showProcessingDlg(false);

		return true;
	}

	bool ContourCreateFacade::createContour(string name, vgKernel::Box area, int numOfLine, GridDataInfo info, float **ppRawData, float* specialVal, bool use3dCoord)
	{
		// Raw����
		ContourGeoData *pGeoData = new ContourGeoData;
		GridDataInfo   dataInfo = info;
		Contour *pContour = new Contour(NULL, name);
		
		pGeoData->setGridData(ppRawData);
		pGeoData->setGridDataInfo(dataInfo);

		// pGeoData->setGridData(ppRawData);
		pContour->setDataDirectly(pGeoData);


//		pContour->GenerateDefaultContourValues();

		pContour->GenerateContourValues(dataInfo.minVal, dataInfo.maxVal, numOfLine);

		pContour->GenerateContours(true);

		pContour->setBoundingBox(area);

		if (use3dCoord)
		{
			pContour->attachToTerrain();
		}

		ContourManager::getSingleton().addContour(pContour);
		// ContourManager::getSingleton().traceAllContour();

		// vgGIS3D::GisManager::getSingleton().showProcessingDlg(false);

		return true;
	}
}