


#ifndef __VGTERRITORY_VGTRHEADER_H__
#define __VGTERRITORY_VGTRHEADER_H__


#include <vgTerritory/vgtDefinition.h>
#include <vgKernel/vgkStreamReader.h>
#include <vgKernel/vgkStreamWriter.h>
#include <vgKernel/vgkForward.h>



namespace vgTerritory {


#define VG_VGTR_NAME_LENGTH	64

	class  VGT_EXPORT VgtrHeader
	{
	public:
		VgtrHeader();
		~VgtrHeader()
		{
			
		}
	
	public:
	
		char fourCC[4];		// Ϊ"vgtr",ע����Сд��

		int version;		// 1

		/**
			���ļ���,���Ϊ64���ȵ�����
			ע��,��vtr�ļ���,ÿһ��vgtr�ļ���name������֤������һ��
			��vtr brief��һ�µ�.�п��ܱ����������ԭʼ��vgtr�ļ�����.
			�����Ĺ����н���Դ���Ϣ������������.
		*/
		char name[ VG_VGTR_NAME_LENGTH ];

		/**
			��Ӧ��������ļ���
		*/
		char texname[ VG_VGTR_NAME_LENGTH ];	


		/**
			Nֵ,Ĭ��Ϊ8,��ÿ��256��������
		*/
		long N;	

		/**
			ÿ��cell��Ӧ��texture��С,ע�����ϸ���.	
		*/
		long cellTexSize;

		/**
			x��z��������
			ע��!�������һ������!
		*/
		uint xNum;			
		uint zNum;

		/**
			�߼�ԭ��,ע��!!����vtr֮�����ֵһ���ò���!
		*/
		double originX;		
		double originY;
		double originZ;

		/**
			��Ҫ����GPU�Ĳ���
		*/
		float displaceScaleY;	// Y��ķŴ�ֵ
		float displaceChunkScale;	// ��С������ڴ�ͼ�ķŴ�ֵ
		float displaceChunkBiasX;	// ��С������ڴ�ͼ��ƫ��, x ����
		float displaceChunkBiasZ;	// ��С������ڴ�ͼ��ƫ��, z ����

		/**
			���
		*/
		float cellsize;	// ���

		/**
			��������ʼ�ʹ�С
			ע��!�����length�����ֽ�Ϊ��λ!!����������ڱ��ļ�!!
			����,VGLOD_CLEARָ����������һ��
		*/
		long dataPosition[ VGLOD_N ];
		long dataLen[ VGLOD_N ];	


	public:
		String getDetails();

	};


	typedef vgKernel::SharePtr<VgtrHeader> VgtrHeaderPtr;

	class  VGT_EXPORT VgtrFile
	{
	public:
		VgtrFile();
		~VgtrFile();

		long writeToStream( const vgKernel::StreamWriterPtr pwriter );

		bool readFromStream( const vgKernel::StreamReaderPtr preader );


		String getDetails();

#if 0
		void render();
#endif

	public:

		VgtrHeaderPtr _headerPtr;

		/**
			û��lod���ݵ�data
		*/
		float* _dataLod[ VGLOD_N ];
	};
	
	typedef vgKernel::SharePtr<VgtrFile> VgtrFilePtr;

	typedef std::vector<VgtrFilePtr> VgtrFilePtrVec;

	
	
}// end of namespace vgTerritory

#endif // end of __VGTERRITORY_VGTRHEADER_H__