


#ifndef __VGTERRAINENTRY_H__
#define __VGTERRAINENTRY_H__

#include <vgKernel/vgkForward.h>
#include "vgBaseEntry.h"
#include <vgTerritory/vgtTerrainManager.h>
#include <vgTerritory/vgtUpdateObserver.h>

#include "vgPropertyPage.h"


	/**
		@date 	2008/09/11  20:49	
		@author  leven

		@brief 	

		@see    
	*/
	class vgTerrainEntry :	public vgBaseEntry ,
		public vgTerritory::UpdateObserver
	{
	public:
		//static int s_NumOfParam = 0;
		//static PropertiesParam _params[s_NumOfParam];

		static std::vector<PropertiesParam> _params;

	public:
		vgTerrainEntry();


		virtual ~vgTerrainEntry(void);

		/**
			�̳�UpdateObserver�Ľӿ�
		*/
		virtual void onUpdate()
		{
			updateData();
		}


		virtual void AddNodeTabs();

		virtual String GetName()
		{
			return "Terrain";
		}

		int GetSelectedImage() { return 7; }

		int GetUnSelectedImage() { return 8; }


		void OnPropertyChanged(string paramName);

		// �̳���Changed������Object�޸ĺ����Node
		void onChanged(int eventId, void *param);

	private:

		void updateData();

	private:

		String n;

		String xTotalNum;			
		String zTotalNum;

		String cellTexSize;


		 
		String _originX;
		String _originY;
		String _originZ;

		String m_sCaption;

		String _vginame;

		vgTerritory::TerrainManager* _terrMgr;
	};






#endif // end of __VGTERRAINENTRY_H__