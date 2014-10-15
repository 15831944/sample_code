#ifndef   _VGMOD_DEFS_H_INCLUDED_
#define   _VGMOD_DEFS_H_INCLUDED_	



#define		YEAR_LAST		2008
#define		YEAR		2009

//#define		UPDATA_TIME	4// 2008��6��23��15:59:28	add		by lss
//#ifdef	_MOD
#define		UPDATA_TIME_LAST 5// 2008��6��27��10:19:43	update	by lss
//#else
//	#define		UPDATA_TIME 6// 2008��8��1��11:13:01		update	by yx	
//#endif

#define		UPDATA_TIME 1// 2009��2��5��14:20:22		update	by lss	

#define	VERSION_MOD(year, updateTime)		(	( (year)<<16 )	+	(updateTime) )
#define VERSION_MOD_CURRENT	VERSION_MOD(YEAR, UPDATA_TIME)
#define VERSION_MOD_LAST	VERSION_MOD(YEAR_LAST, UPDATA_TIME_LAST)

/* �����С���ID���� */
#define OBJECTS		0x1518
#define OBJECT_HEAD	0x1519
#define OBJECT_BODY	0x1520
#define OBJECT_MATINFO 0x4130

// 2008��3��5��16:39:57 ���
#define MAP_CHANNEL_ONE			1
#define MAP_CHANNEL_THREE		3
//#define MAP_CHANNEL_DEFAULT 0

#define MSG_BLOCK_LENGTH	24


// ��������
#define 	MTL_STANDARD	(1<<0)
#define 	MTL_SHELL		(MTL_STANDARD<<1)

#define 	OBJ_BAKED		(MTL_STANDARD<<2)
#define 	OBJ_KEYFRAME	(MTL_STANDARD<<3)

#if	!IMPORT_MAX_LIB
#define SCENE_EXPORT_SELECTED 1
#endif

// ����ѡ��
enum	E_Export_Option
{
	FILTER_SELECTED		= SCENE_EXPORT_SELECTED,
	PROMPT_SETTING		= SCENE_EXPORT_SELECTED << 1,
	PROMPT_RESULT		= SCENE_EXPORT_SELECTED << 2,
	PROMPT_WARNING		= SCENE_EXPORT_SELECTED << 3,
	EXPORT_OPTION_COUNT
};

#endif	//_VGMOD_DEFS_H_INCLUDED_	
