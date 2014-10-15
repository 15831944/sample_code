
#ifndef __VGMVGFILEDEFINITIONVG_H__
#define __VGMVGFILEDEFINITIONVG_H__

#include <vgKernel/vgkStreamWriter.h>

namespace vgMod{

	/**
	@date 	2009/03/02  15:12	
	@author  lss

	@brief 	

	@see    
	*/
	class   VGDEP_EXPORT VGFileHeader 
	{
	public:
		char	vgfileheader[20];	// Part 1: ��ʶ�ļ��汾���ַ���

		int		m_nodenum;			// Part 2: �ڵ���������ļ�λ��

		int		abstractNodePos ;	// Part 3: �ڵ���Ŀ

		VGFileHeader();

		//��д�ļ�_vgKernel::StreamReaderPtr
		bool	read(vgKernel::StreamReaderPtr pfile);
		void	write(vgKernel::StreamWriterPtr pfile);

		//��д�ļ�_CFile*
		bool	read(CFile*  pfile);
		void	write(CFile* pfile);

		//��д�ļ�_String filename
		bool	read(String filename);
	};

	union	ModType
	{
		unsigned int nType;
		bool		 bType;
	};
	class   VGDEP_EXPORT MODFileHeader 
	{
	public:
		char			strFileFormat[32];	// Part 1: ��ʶ�ļ���ʽ���ַ���

		unsigned int	uiFileVersion;		// Part 2: ��ʶ�ļ���ʽ������

		Vector3			vSceneCenter;		// Part 3: ����modģ�͵���ƫ����
		
		unsigned int	uiObjectCount;		// Part 4: modģ����Ŀ
	
		vector<unsigned int>	arrModPosIndex;	// Part 5: modģ�����ļ��е�����λ��

		vector<ModType>		arrModType;			// Part 6: modģ�͵���������


		MODFileHeader();

		//��д�ļ�_vgKernel::StreamReaderPtr
		bool	read(vgKernel::StreamReaderPtr pfile);
		void	write(vgKernel::StreamWriterPtr pfile);
	};

}//namespace vgMod

#endif//__VGMVGFILEWRITER_H__
