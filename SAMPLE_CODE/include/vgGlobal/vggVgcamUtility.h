

#ifndef __VGGCAMUTILITY_H__
#define __VGGCAMUTILITY_H__

#include <vgGlobal/vgglDefinition.h>
#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkStreamReader.h>
#include <vgKernel/vgkStreamWriter.h>

namespace  vgGlobal{

	/**
		@date     2009/10/21  20:45
		@author   zzy

		@brief    ��������Cam��ز������м��

		@see
	*/

	class	VGGL_EXPORT CamUtility
	{
	public:
		CamUtility();
		~CamUtility();

	public:
		static  bool importVcrFile();              //�������ζ���
		//static  bool exportVcrFile();            //�������ζ���

		static  bool exportBmp();                  //����Bmp��̬ͼ
		static  bool exportAvi();                  //����Avi��Ƶ�ļ�

		static	bool createViewCam();              //�������
		static	bool switchViewCam();              //�л����

		static  bool switchToTopView();            // �л�������ͼ
		static  bool switchToPerspectiveView();    // �л���͸��ͼ

		static  bool switchToAxonoViewNorthWest(); //�л����������ͼ
		static  bool switchToAxonoViewSouthWest(); //�л����������ͼ
		static  bool switchToAxonoViewNorthEast(); //�л����������ͼ
		static  bool switchToAxonoViewSouthEast(); //�л����������ͼ

		static	bool positionViewCamToRenderer();  //�л�������

		static	bool beginRecordVcr();             //��ʼ��¼Vcr���ζ���
		static  bool endRecordVcr();               //����¼��Vcr���ζ���

		static	bool runVcr();
		static	bool continueRunVcr();
		static	bool stopRunVcr();

		static  bool accelerateVcrSpeed();          //�ӿ����ζ��������ٶ�
		static  bool decelerateVcrSpeed();          //�������ζ��������ٶ�

		static  bool setVcrCameraHeight();          //���ö����߶�λ��--SetTimer��ʱδ��ȡ     

		static  bool onOrOffCollicition();          //��/����ײ���
		static  bool updateOnOrOffCollication();

		//���������ѱ��滻����������
		/*static	bool importVCRAnimation( const String& file_name);	
		static	bool exportVCRAnimation( const String& file_name);*/

		//���εĶ���ӿ�
		static  bool importVcrAni( const String& strFilePath );  
		static  bool exportVcrAni( const String& strFilePath );

		static  bool setCenterOfScreen( const uint& centerx , 
			const uint& centery );

		static  bool setAviRecordCnt(int cnt);

		static  bool setChannelNum(int par);

	private:
		static  bool importVcrBegin( const vgKernel::StringVector& strFilePaths );   
		static  bool importVcrAni( const vgKernel::StringVector& strFilePaths );
		static  bool importVcrEnd( const String& strFilePath );

	};

}//namespace vgGlobal

#endif//__VGGCAMUTILITY_H__