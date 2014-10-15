#ifndef __VGOBJFRAMENODE_H__
#define __VGOBJFRAMENODE_H__
#include <vgMod/vgObjNode.h>
//#include <vgMod/vgModTerrain.h>
#include <vgKernel/vgkTimer.h>
#include <vgMs3d/math.h>


#include <vgKernel/vgkSharePtr.h>

namespace vgMod{

	//using namespace vgCore;
	/*
	5010 -- 5030֮��ΪQuadTreeSceneMananger�Ĺ�Ͻ��Χ֮��
	*/
	//const vgKernel::RendererType RENDERER_TYPE_KEYFRAME = 5043;

	struct ModKeyFrame
	{
		int		m_fTime;		//	Time at which keyframe is started
		Vector3 key_translate;	//	Translation or Rotation values
		float	key_rotate[4];
		Vector3 key_scale;
	};

	class  VGDEP_EXPORT ObjFrameNode :	public ObjectNode
	{
		VGK_RENDERER_IN_DECLARE( ObjFrameNode )
		//	Part 0 :�������ԣ����class ObjectNode���ޣ�

		
		//	Part 1 :�������ݿ飬���class ObjectNode
	private:

		void	setup();
		void    updateFrameMatrix(int currentFrame);

		unsigned int		m_uiKeyFramesCount;
		//vector<ModKeyFrame> objKeyFrames;
		ModKeyFrame*		m_pKeyFrames;
		
		
		vgKernel::Box		m_mergeBox;
		vgKernel::Box		_staticBox;
		vgKernel::Box		_dynamicBox;
		vgKernel::Mat4		_matrix;

		
		//	Part 2 :����״̬����
		float	m_speed;
		int		m_startframe;
		int		m_endframe;
		bool	b_loop;
		bool	b_play;

		float	fStartTime;
		float	fLastTime;
		float	fEndTime;
		float	fTime;
		vgKernel::Timer m_Timer;
		float	rotangel;

		Vector3 m_pos0;
		
		bool	bFirstTime;
		bool    bFirstRender;

		int		nCurrentFrame;
	public:
		//	Part 3 :��������
		inline	unsigned int	getKeyFrameCount()	{	return m_uiKeyFramesCount;}
		inline bool		getPlayingState()				{	return b_play;		}
		inline float	getStartFrame()				{	return m_startframe;}
		inline float	getEndFrame()				{	return m_endframe;	}
		inline bool		getLoopState()				{	return b_loop;		}
		inline float	getSpeed()					{	return m_speed;		}
		
		inline void		setPlayState(bool play)			{	b_play = play;				}
		inline void		setStartFrame(float startframe)	{	m_startframe = startframe;	bFirstTime  =true;}//chunyongma
		inline void		setEndFrame(float endframe)		{	m_endframe = endframe;	bFirstTime = true;	}
		inline void		setLoopState(bool loop)			{	b_loop = loop;				}
		inline void		setSpeed(float speed)			{	m_speed = speed;			}
			
		inline bool		hasKeyFrame()					{	return getKeyFrameCount()!=0 ;	}
	
		
		//	Part 5 :���ظ�����麯��
		virtual	void	prepareMatrix();
		virtual	long	readExtraData(char	*pBytes);
		virtual	void	writeExtraDataToVG(CFile &fp);
		virtual void    writeOceanProperty(CFile& fp);
		//virtual	void	readExtraDataFromVG(CFile &fp);

		virtual	void	prepareExtra()	;
		virtual	void	resetExtra() ;


		//	Part 6 :�����麯�����ޣ�
		//	Part 7 :������������
		void	reset()	{bFirstTime = true;}
		void	startPlayingByTime(float fAnytime = 0.0f);
		void	startPlayingByFrame(long uiAnyFrame);
		float	getTimeWithFrame( int m_frame );
		void	updateMatrix();		
		
		
		long	readKeyFrames(char	*pBytes);
		void	readKeyFrames(CFile &fp);
		void	writeKeyFramesToVG(CFile &fp);

		long	readUniqueID(char	*pBytes);
		void	readUniqueID(CFile &fp);
		void	writeUniqueID(CFile &fp);

		//RenderDoubleSidesState
		long	readRenderDoubleSidesState(char	*pBytes);
		void	readRenderDoubleSidesState(CFile &fp);
		void	writeRenderDoubleSidesState(CFile &fp);

		//TriggerState
		long	readTriggerState(char	*pBytes);
		void	readTriggerState(CFile &fp);
		void	writeTriggerState(CFile &fp);
		
		long	readTriggerInfo(char	*pBytes);
		void	readTriggerInfo(CFile &fp);
		void	writeTriggerInfo(CFile &fp);

		//ColorProperty
 		long	readColorProperty(char  *pBytes);
 		void	readColorProperty(CFile &fp);
 		void	writeColorProperty(CFile &fp);

		//CollisionProperty
		long    readCollisionProperty(char *pBytes);
		void	writeCollisionProperty(CFile &fp);

		//TimelineProperty
		long    readTimelineProperty(char *pBytes);
		void	readTimelineProperty(CFile &fp);
		void    writeTimelineProperty(CFile &fp);

		void translate( Vector3 offset );


		virtual vgKernel::Box getBoundingBox()
		{
			if ( m_uiKeyFramesCount == 0 )
			{
				return ObjectNode::getBoundingBox();
			}

			return m_mergeBox/*_dynamicBox*/;
		}

	public:
		//	����/����
		ObjFrameNode( int nVersionMod = VERSION_1);
		virtual ~ObjFrameNode(void);

		void pausePlaying();
		void continuePlaying();
		virtual bool	cloneFromObjectNode(ObjectNode* src_node);

		/**
			@action		add
			@date 		2009/03/13  10:50	
			@author  	lss
		
			@brief 	
		
			@see    
			*/
		virtual void	initializeExtra();
		virtual void	initializeTrigger();	//��ʼ��
		virtual void	initializeColor();
	};//class ObjFrameNode

	VGK_RENDERER_OUT_DECLARE( ObjFrameNode )

	typedef vgCore::SharedPtr<ObjFrameNode> ObjFrameNodePtr;

}//namespace vgCore

#endif
