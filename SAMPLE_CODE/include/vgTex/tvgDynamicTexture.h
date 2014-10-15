// DynamicTexture.h: interface for the DynamicTexture class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __TVGDYNAYMICTEXTURE_H__
#define __TVGDYNAYMICTEXTURE_H__

#include <vgTex/vgDtxDeclaration.h>
#include <vgKernel/vgkStreamReader.h>
#include <vgkernel/vgkStringUtility.h>
#include <vgKernel/vgkStringVector.h>

namespace vgCore
{

	/**
	@date 	2009/03/15  8:34	
	@author  YUXIN

	@brief 	

	@see    
	*/


class VGDEP_EXPORT DynamicTexture  
{
public:
	DynamicTexture(String FileName);
	virtual ~DynamicTexture();


	int Update ();	// Perform Motion Updates
	bool ReadFile();
	bool init();
	void GetFrameTexture(int frameId);
	vgKernel::StringVector getTexfilePaths();
	FrameVector getFrames();

public:

	//**********************************
	//����֡�����ñ���
	int next;
	int frameId;
	int lastTickCount;
	int newTickCount;

	long sum;				//���ڼ���ʱ��
	//**********************************

	DtxHeader header;
	FrameVector framevec;
	//**********************************
	String szDtxFile;		//DTX�ļ�·��
	StringVector texfilepathvec;	
	string texfilefolderpath;		//������ͼ�ļ���·��

	TextureManager &texmgr;
	ArchiveManager &archmgr;
};

#endif // !defined(AFX_AVITEXTURE_H__7943A9A3_15D7_4FD7_8DC2_F245DA84E41A__INCLUDED_)

}