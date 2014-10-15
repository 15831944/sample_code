
#ifndef _VGCHANNEL_H_MANAGER_
#define _VGCHANNEL_H_MANAGER_

#include <vgChannel/vgcDefinition.h>

#include <vgKernel/vgkSingleton.h>

#include <vgKernel/vgkVec3.h>
#include <vgChannel/vgchBlendPixelImpl.h>
#include <vgKernel/vgkRendererObserver.h>
#include <vgKernel/vgkInputSystem.h>
#include <vgKernel/vgkRenderCommand.h>
#include <vgKernel/vgkPropertyObserver.h>

namespace vgChannel
{

	static const int MAX_OF_CLIENT = 10;

	typedef struct ViewPackage_t
	{
		float			x;
		float			y;
		float			z;
		float			pitchDegree;
		float			headDegree;
	}ViewPackage;

	typedef struct ControlPackage_t
	{
		int channelNum;

		int cameraOffset;

		int leftEdge;
		int rightEdge;

		float hOffset;
		float vOffset;
		
		float eyePosOffset;
	}ControlPackage;

	typedef struct ClientInfo_t
	{
		ControlPackage	  controlParam;
		SOCKADDR_IN		  clientSocketAddr;
		bool			  clientIsInCome;
	}ClientInfo;

	typedef struct NetPackage_t
	{
		char type;
		union Data_t
		{
			ControlPackage    control;
			ViewPackage   view;
		}Data;
	}NetPackage;

	class ChannelManager : public vgKernel::Singleton<ChannelManager>,
		public vgKernel::InputHandler, public vgKernel::ObserverContainer, 
		public vgKernel::PropertyObserver
	{
		friend class vgKernel::Singleton<ChannelManager>;
	private:
		ChannelManager();

	public:

		~ChannelManager();
		virtual void onEvent(unsigned int eventId, void *param);

	protected:
		virtual bool initialise();//

		virtual bool shutdown();

		vgKernel::RenderCommand *m_pChannelRenderBegin;//��Ϊ��vgchChannelRenderCommand.h��������ͷ�ļ� <vgChannel/vgcChannelManager.h>
		vgKernel::RenderCommand *m_pClientRenderBegin; //Ϊ�˷�ֹͷ�ļ��Ļ������ã���������Ϊ������ĸ����ָ��
		vgKernel::RenderCommand *m_pServerRenderBegin; //��cpp������ͷ�ļ�vgchChannelRenderCommand.h
		vgKernel::RenderCommand *m_pChannelRenderEnd;  //��cpp�е���֮ǰ��ʼ��ʱ��new һ��������
		vgKernel::RenderCommand *m_pBlendAndAdjust;

	public:

		bool startChannel(int channelNum);
		bool cancelChannel();

		bool serverRenderBegin();
		bool clientRenderBegin();

		bool renderEnd();


		// �ṩ�����ķ�װ
		int  getChannelNum(); 
		bool getChannelEnable(); 
		ViewPackage getCurrentViewPackage();

		float getOffsetRatio();
		float getHOffset() { return m_hOffset; }
		float getVOffset() { return m_vOffset; }

		float getEyePosOffset() { return m_eyeOffset; }
		void  setEyePosOffset(float eyeOffset) { m_eyeOffset = eyeOffset; }

		void SetChannelFromFile();//���ļ�����ͨ������
		void SaveChannelSetToFile();//�������ò������ļ�
		void setServerIP(DWORD dwIP);//���ÿͻ��˶�Ҫ���ӵķ�������IP
		CString getConfigFilePth();//��������ļ���·��
		DWORD GetServerIP();//��ÿͻ��˶�Ҫ���ӵķ�������IP

		void setClientInfoPath(CString fullPathName);
		CString getClientInfoPath();

		void saveClientInfotoXML(CString clientIp, CString chanNum, 
			int chCameraOffset, float hOffset, float vOffset, float eyePosOffset);
		void readClientInfofromXML();

		CString floatToCstr(float floatValue);

		// ��ͨ���㲥
		int sendToServer();
		int sendToClient(ViewPackage& vp);

		int sendControlMsgToClient(SOCKADDR_IN addr, ControlPackage controlPackage);

		float getLeftOffset();
		float getRightOffset();

		bool processUnkownClient(SOCKADDR_IN inAddr);

		int getInComeClientNum();
		ClientInfo getClientInfo(int index);

		SOCKADDR_IN getClientSockAddr(int index);

		// �������ϵĸ����ͻ���������Ϣ
		ControlPackage* getControlInfoByIp(string ipAddress);
		int setControlInfoByIp(string ipAddress, ControlPackage controlInfo);

		void updateControlInfo(ControlPackage controlInfo);

	public:

		virtual void OnChar(int keyCode);

		virtual void OnMouseMove(UINT nFlags, CPoint position);

		virtual void OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

		virtual void OnLButtonDown(UINT nFlags, CPoint position);

		virtual void OnLButtonUp(UINT nFlags, CPoint position);

	public:		
		bool	renderBegin(); 

		void    setNurbsLittleCorrectEnable(bool enable) { m_nurbsAdjustImpl.setLittleCorrectEnable(enable); }

		bool    getNurbsLittleCorrectEnable() { return m_nurbsAdjustImpl.getLittleCorrectEnable(); }		

		void    setBlendAndAdjustEnable(bool b);

		bool    getBlendAndAdjustEnable() { return m_blendAndAdjust; }

		void    setEdgeBlendEnable(bool b) { m_blendAdjustImpl.setEdgeBlendEnable(b); }

		bool    getEdgeBlendEnable() { return m_blendAdjustImpl.getEdgeBlendEnable(); }

		void    savePramaToXml() {m_nurbsAdjustImpl.SavePramaToXml();}	//@FengYK 2009/10/31 10:57

		void    ReturnToOriPrama() {m_nurbsAdjustImpl.ReturnToOriPrama();}
	public:
		// ��Ե�ں��뼸��У��

		void blendAndAdjust();

		void setScreenSize(int x, int y) { m_blendAdjustImpl.setScreenSize(x, y); }

		NurbsCorrectImpl* getNurbsPtr() { return &m_nurbsAdjustImpl;/*m_blendAdjustImpl.getNurbsPtr();*/ }

		BlendPixelImpl*   getBlenderPtr() { return &m_blendAdjustImpl; }

	private:
		bool createServerSocket(int serverRecvPort);

		bool createClientSocket(int clientRecvPort);

	private:
		// float m_overlapLen;
		// for server only;

		string m_broadCastAddress;
		bool Lflag;

		int m_servePort;
		int m_clientPort;

		int m_leftEdge;
		int m_rightEdge;

		float m_hOffset;
		float m_vOffset;

		float m_eyeOffset;

		int m_cameraOffset;

		// 		ControlPackage	  m_controlParam[MAX_OF_CLIENT];
		// 		SOCKADDR_IN		  m_clientSocketAddr[MAX_OF_CLIENT];
		// 		bool			  m_clientInCome[MAX_OF_CLIENT];
		ClientInfo		  m_clientInfo[MAX_OF_CLIENT];

		BlendPixelImpl	  m_blendAdjustImpl;
		NurbsCorrectImpl  m_nurbsAdjustImpl;

		bool m_blendAndAdjust;


	};

}//vgChannelManager�����ռ�


#endif//_VGMGIS_H_INCLUDED_