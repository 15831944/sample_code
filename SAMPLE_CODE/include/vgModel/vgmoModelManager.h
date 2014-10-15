


#ifndef __VGMO_MESHMANAGER_H__
#define __VGMO_MESHMANAGER_H__




#include <vgModel/vgmoDefinition.h>

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSingleton.h>
#include <vgKernel/vgkUniqueID.h>
#include <vgModel/vgmoModelItem.h>
#include <vgKernel/vgkTrace.h>
#include <vgKernel/vgkRenderer.h>

#include <vgKernel/vgkRendererObserver.h>
#include<vgKernel/vgkRendererHolder.h>

#include <vgModel/vgmoDefinition.h>

#include <vgKernel/vgkRenderCommand.h>


namespace vgModel {

	using namespace vgKernel;

	class ModelObserver;
	class FileHandleVmd;

	/**
		@date 	2008/07/01  19:09	
		@author  leven
	
		@brief 	��������ModelItem��ɾ��.
	
		@see    
	*/

	class  VGMO_EXPORT ModelManager 
		: public vgKernel::Singleton<ModelManager>, 
		public vgKernel::ObserverContainer, 
		public vgKernel::RendererHolder
	{
		friend class vgKernel::Singleton<ModelManager>;
	public:
		typedef std::map<String,FileHandleVmd*> FileHandleVmdMap;
		typedef std::pair<FileHandleVmdMap::iterator , bool > FileHandleVmdMapInsertRes;

	private:
		ModelManager();
	public:
		~ModelManager();

	protected:

		virtual bool initialise();
		virtual bool shutdown();
	
		vgKernel::RenderCommand *m_pModelRenderCmd;

	public:
		virtual void reset(){};
		virtual bool onDeleteRenderer( Renderer *render);

		/**
			��Ҫ����,��OpenGL���û�������֮ǰ,Ҫɾ�����е�data
		*/
		void unloadAllModelItems();


		// only for testing
		void dumpAllModelItemInfo();

		virtual void render();

		/**
			��������������б�,���ڱ�����������
			ע��,�����������𷵻��������Ƶ��ظ����޳�.
		*/
		vgKernel::StringVectorPtr  saveItemsToVmd( const String& absolute_path );

		/**
			�õ���ModelItem�ͻ��˲��ܹ�ɾ��
		*/
		ModelItem* getModelItem( const UniqueID& uuid );

		/**
			ͨ������FileHandleVmd�����ModelItems
		*/
		//FileHandleVmd* addItemsByFileHandle( const String& absolute_file_path );


		

		// ���ص�ָ��ͻ��˲��ܹ�ɾ��
		ModelItemMap* getModelItemMap()
		{
			return &_items;
		}

		FileHandleVmdMap* getFileHandleVmdMap()
		{
			return &_filehandlers;
		}

		bool addItemsAndRegisterFileHandlerVmd( FileHandleVmd* handle );


		void invalidateItemTextures();

		void readProject(const String& projpath , const String& projname);

		void saveProject();

	private:

		bool deleteModelItem( ModelItem* item );

		bool addModelItem( ModelItem* item );

		/**
			register������IoServer��Associate
		*/
		void renderAllModelItems();

		bool registerAndAssociateFileHandleVmd( FileHandleVmd* handle ); 
		bool exist( const UniqueID& uuid );
		bool exist( vgAsyn::FileHandle* handle );

		struct TraverseMapForRender
		{
			inline void operator() (std::pair<UniqueID, ModelItem*> item) 
			{
				item.second->render();
			}
		};

		struct TraverseVecForRender
		{
			inline void operator() ( vgKernel::Renderer* item) 
			{
				((ModelItem*)item)->render();
			}
		};

	private:

		/**
			��Renderermanager�еõ�
		*/
		RendererQueue* _culledModelRenderQueue;

		FileHandleVmdMap _filehandlers;
	
		ModelItemMap _items;
	};
	
	
	
}// end of namespace vgModel

#endif // end of __VGMO_MESHMANAGER_H__