
#include <vgStableHeaders.h>
#include <vgGIS3D\vggiContourGeoData.h>

#include <vgGIS3D\vggi2DMemAllocator.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace vgGIS3D
{
	ContourGeoData::ContourGeoData()
	{
		m_ppGridData = NULL;
		memset(&m_dataInfo,0,sizeof(m_dataInfo));
	}

	ContourGeoData::~ContourGeoData()
	{
		destoryData();
	}

	bool ContourGeoData::destoryData()
	{
		if( m_ppGridData!=NULL)
		{
			// C2DMemAllocator::FreeMemory2D(m_ppGridData);

			for (int i=0; i<m_dataInfo.rows; i++)
			{
				delete [] m_ppGridData[i];
			}

			delete[] m_ppGridData;

			m_ppGridData = NULL;
		}

		return true;
	}

	//-----------------------------------------
	//�������ݴ�ȡ(����д)
	//-----------------------------------------

	BOOL ContourGeoData::ReadData(CString pathName)
	{

		FILE* fp=fopen(pathName,"rt");
		if(!fp)
		{
			AfxMessageBox("Open" + pathName + "error!!");
			return FALSE;
		}
		/////////////////////////////////////////////////

		char head[5];
		//ASCII SURF grid file
		fread( head, sizeof( char ), 4, fp );
		if(!(head[0]=='D' && head[1]=='S' && head[2]=='A' && head[3]=='A'))
		{
			AfxMessageBox("���ܴ��ļ���");
			fclose(fp);
			return FALSE;
		}

		//���ļ�ͷ����
		float txmin,txmax,tymin,tymax,tcmin,tcmax;
		fscanf(fp,"%d%d%f%f",&m_dataInfo.cols,&m_dataInfo.rows,
			&tcmin,&tcmax);
		
		m_dataInfo.minVal = tcmin;
		m_dataInfo.maxVal = tcmax;

		if( m_ppGridData!=NULL)
		{
			C2DMemAllocator::FreeMemory2D(m_ppGridData);
			m_ppGridData = NULL;
		}

		///////////////////////////////////////
		//����������
		ASSERT( m_ppGridData==NULL );
		if( !C2DMemAllocator::AllocMemory2D(m_ppGridData, m_dataInfo.rows, m_dataInfo.cols) )
		{
			AfxMessageBox("�ڴ����ʧ�ܣ����ݶ�ȡû�гɹ�!");
			return FALSE;
		}

		ASSERT( m_ppGridData );
		for(int j=0; j<m_dataInfo.rows;j++)
		{
			//making the map looks like in "SURF", so changing the order of data reading
			for(int i=0;i<m_dataInfo.cols;i++)
			{
				fscanf(fp,"%f",&m_ppGridData[j][i]);
				/*TRACE("m_ppGridData[%j][%i]=%v",j,i,&m_ppGridData[j][i]);*/
			}
		}

		fclose(fp);

		return TRUE;

	}
	BOOL ContourGeoData::SaveData(CString pathName)
	{
		ASSERT(m_ppGridData!=NULL);

		int i, j;
		float tmp;
		FILE* fpout = NULL;


		fpout = fopen(pathName, "wt");
		if(fpout == 0){
			printf("   open file error!!!!!!!!!!!!\n");
			return FALSE;
		}
		float gridNoiseMin= 1.e38f;
		float gridNoiseMax=-1.e38f;
		for(i= 0; i<m_dataInfo.rows; i++)
		{
			for(j=0; j<m_dataInfo.cols; j++)
			{
				tmp=m_ppGridData[i][j];
				if(tmp > gridNoiseMax) gridNoiseMax=tmp;
				if(tmp < gridNoiseMin) gridNoiseMin=tmp;
			}
		}

		m_dataInfo.maxVal = gridNoiseMax;
		m_dataInfo.minVal = gridNoiseMin;

		fprintf(fpout,"DSAA\n");
		//��ȡ����50������39
		fprintf(fpout,"%d %d\n",m_dataInfo.cols,m_dataInfo.rows);
		//��ȡx��Сֵ0�����ֵ9
		fprintf(fpout,"%f %f\n",m_dataInfo.xMin,m_dataInfo.xMax);
		//��ȡy��Сֵ0�����ֵ7
		fprintf(fpout,"%f %f\n",m_dataInfo.yMin,m_dataInfo.yMax);
		//��ȡ������Сֵ�����ֵ24.997953414917 104.25473022461
		fprintf(fpout,"%f %f\n",m_dataInfo.minVal,m_dataInfo.maxVal);

		for(i= 0; i<m_dataInfo.rows; i++)
		{
			for(j=0; j<m_dataInfo.cols; j++)
			{
				if( (i*m_dataInfo.cols+j)%8 == 0 ) 
					fprintf(fpout,"\n");
				fprintf(fpout,"%11.5f ",m_ppGridData[i][j]);
			}
		}
		fclose(fpout);
		return TRUE;
	}
}