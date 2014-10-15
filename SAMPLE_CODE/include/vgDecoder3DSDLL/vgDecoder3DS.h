

#ifndef _VG_3DS_DECODER_H_INCLUDED_
#define _VG_3DS_DECODER_H_INCLUDED_
/**********************************************************************
*<
vg3DSDecoder.h:	.3DS��ʽ���� ͷ�ļ�
*>	
**********************************************************************/

#include "vgStableHeaders.h"

#include <vgMod/vgObjectSet.h>


//////////////////////////////////////////////////////////////////////////
class Decoder3DS
{
public:
	Decoder3DS();
	~Decoder3DS();
	

public:
	string	DoImport(const TCHAR *name);	// Import file

	// ����mod�ļ�
	string	writeModFile( const TCHAR * filename);
	int		checkFileState( const TCHAR * filename );

protected:
	Dirlight	studioLt;
	Camera3DS	studioCam;

public:
	BOOL autoConv /*= 1*/;

	FILE* stream;//import 3ds file

	int		m_nImportType;
	int		m_nIndexShapeNumber;


	int merging/* = 0*/;
	int lastchunk;

	char obname[32];
	int nodeLoadNumber /*= -1*/;
	short cur_node_id,nodeflags,nodeflags2;
	int nodetag;
	int skipNode /*= 0*/;
	
#if SWITCH_DECODE_OUTPUT_OBJWORKER		
	ObjWorker* m_pTheWorker;
	ImpNode *obnode;
	vector<INode*>	tmpNodes;
#endif

	short readVers;
	int skipped_nodes /*= 0*/;
	int got_mat_chunk;
	BGdata BG;
	
	BOOL importShapes /*= TRUE*/;

	float msc_wk;

	WorkFile*	m_WorkFile;

	std::ostringstream os;


	MeshMtlList*	m_pTheMtls ;			//	ȫ�ֲ��ʽӿ�
	t3DModel*		m_pTheModels ;		//	ȫ��ģ��ָ��
	
	unsigned int	m_nVersionMod;			

private:	//	������ʱ����
	t3DObject*		newobj ;			//	��ǰ����ָ��

public:
	int		get_mchunk(void *data);
	void	setDefault();

};

//////////////////////////////////////////////////////////////////////////


#endif	//_VG_3DS_DECODER_H_INCLUDED_
