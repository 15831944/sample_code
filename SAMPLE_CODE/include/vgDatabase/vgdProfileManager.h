#pragma once
#include <vgDatabase/vgdDefinition.h>


#include <vgKernel/vgkForward.h>
#include <vgkernel/vgkSingleton.h>
#include <vgkernel/vgkSingletonLifeTimeDefine.h>


#include <vgKernel/vgkRenderer.h>
#include <vgKernel/vgkRendererData.h>
#include <vgKernel/vgkRendererManager.h>

#include <vgDatabase/tinyxml.h>

namespace vgDatabase
{
	class  VGD_EXPORT QueryProfile
	{
	public:
		String CreateSingleQueryStirng(const std::vector<String>& primaryList);
		
		void SetName(String name) { m_profileName = name; }
		String GetName() { return m_profileName; }
		
		CString GetQueryNameFromShowName(CString shownName);
		CString GetShowNameFromQueryName(CString queryName);

		//��ȡ��¼������Ϣ
		CString GetPrimaryId(); 
   
		void PrintInfo();

	protected:
		String m_profileName;

	public:
		// ����ѯ�ı���
		std::vector<String> m_tableList;
		// ����ѯ���ֶ�
		std::vector<String> m_recordList;
		// ��ʾ���ֶ���
		std::vector<String> m_shownNameList;
		// ���Ӳ�ѯ����
		std::vector<String> m_connectList;
		// �������� һ��Ϊһ��
		std::vector<String> m_primaryRecordList;
	};

	class  VGD_EXPORT ProfileManager : public vgKernel::Singleton<ProfileManager>
	{
		friend class vgKernel::Singleton<ProfileManager>;

	public:
		ProfileManager();

		~ProfileManager();

		bool LoadFromFile(String filepath);
		
		bool IsProfileLoaded();

		String CreateQuery(String profileName, bool isSingleQuery = true);

		String CreateQueryWithWhere(String profileName, bool isSingleQuery  = true );

		std::vector<QueryProfile>& GetProfiles() { return m_profileList; }
		
		QueryProfile* GetProfile(String profileName);

		QueryProfile* GetProfile(int index);

		/// add for import/export
		long GetProfileIdByName(String profileName);

		String GetProfileNameByIndex(int index);

		int GetProfileListSize();

		long SaveRendererProfile(String filepath);

		long loadRenderProfile(String filepath);

		bool InitRendererProfile(vgKernel::Renderer* pRenderer);

		//bool	AppendFile( String filename,int PrimaryId );
		
		//��¼��ǰѡ���profile
		void SetCurProfile(CString profileName);

		//��õ�ǰѡ���profile
		QueryProfile GetCurProfile(){ return m_curentprofile; }

		TiXmlNode* AppendXmlProfile(const String &fileName, const String &profileName);

		void InsertTable(const TiXmlNode *profileNode, const String &tableName);

		void InsertFeild(const TiXmlNode *profileNode, const String &feildName, const String &aliasName);

		void InsertConnect(const TiXmlNode *profileNode, const String &connectName);

		void InsertPrimaryId(const TiXmlNode *profileNode, const String &primaryIdName);

		void SaveXmlProfile() { doc->SaveFile(); }

		void IterateNode(String &profileName);

		virtual void reset(){};


	protected:
		bool initialise() { return true; }

		bool shutdown() { return true; }

	private:
		std::vector<QueryProfile> m_profileList;

		QueryProfile m_curentprofile;

		std::map<vgKernel::UniqueID, long> m_rendererProfileMap;

		TiXmlDocument *doc;

	};
}
