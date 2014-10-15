


#ifndef __VGKRENDERERMANAGER_H__
#define __VGKRENDERERMANAGER_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkSingleton.h>
#include <vgKernel/vgkRenderer.h>
#include <vgKernel/vgkRenderCommand.h>



namespace vgKernel {



	class AbstractTree;


	/**
	@date 	2008/09/05  13:41	
	@author  xy

	@brief 	

	@see    
	*/
	class  VGK_EXPORT RendererManager : public vgKernel::Singleton<RendererManager>
	{
		friend class vgKernel::Singleton<RendererManager>;
	private:
		RendererManager();
	public:
		virtual~RendererManager();

	protected:

		virtual bool initialise();

		virtual bool shutdown();
		
		vgKernel::RenderCommand *m_pRenderBegin;
		vgKernel::RenderCommand *m_pRenderEnd;

	public:

		void reset();


		void initAfterOpenGLSetup();

		void clear();

		void invalidate();

		//void switchTreeType(vgTree::TREE_TYPE treeType)
		//{
		//	_treeType = treeType;
		//	initialise();
		//	invalidate();
		//}


		void renderBegin();
		void render();
		void renderEnd();


		void readProject(const String& projpath , const String& projname);

		void saveProject();

		/**
			��ӳɹ��Ļ�,���ص���һ���ܵĶ��� 
			���_renderersAll��û�����render typeʱ, ���Զ����һ��RendererQueue
		*/
		RendererQueue* addRenderer( Renderer* renderer );

		/**
			ɾ���ɹ��Ļ�,����ɾ���������Ӧ���͵��ܶ���
		*/

		bool deleteRenderer( Renderer* renderer );

		/**
			ɾ�����е�renderer, ����resetScene
		*/
		bool deleteAllRenderers();

		void setAllRenderersVisiable(bool visiable);

		RendererQueue* getRendererQueueByType(
			const RendererType& renderertype ,
			const bool& add_if_needed );

		/**
			�õ����������һ������
		*/
		RendererPackage* getAllRendererPackage()
		{
			return &_renderersAll;
		}

		RendererPackage* getCulledRendererPackage()
		{
			return &_renderersCulled;
		}

		RendererQueue* getCulledRenderQueueByType( 
			const RendererType& rendertype );

		Renderer* getRendererbyName(const String& name,
			bool bCaseSensitive = true);



		/**
			����Renderer����ƽ��
		*/
		void	translateAllRenderers( const Vec3& vec);

		vgKernel::Box	getAllRendererBox();

		String getDetails();

	private:

		void caluateVisiblePackage();

	public:
		/**
			���������Ӧ������,�򷵻����Ӧ����������Ķ���
			���û����Ӧ������,������ӹ��������Ķ���.
		*/
		RendererQueue* getRenderQueue(
			const RendererType& type ,
			const bool& add_if_needed );
		
		Box getAllRendererBoxWithoutshp();

	private:

		// ���ڲü���tree
		AbstractTree* _sceneTree;


		RendererPackage _renderersAll;

		RendererPackage _renderersCulled;

	};


}// end of namespace vgKernel



#endif // end of __VGKRENDERERMANAGER_H__