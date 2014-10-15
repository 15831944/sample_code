

#include <vgStableHeaders.h>
#include <vgKernel/vgkRendererManager.h>
#include <vgKernel/vgkRendererHolderManager.h>
#include <vgConf/vgcoProjectConfig.h>
#include <vgConf/vgcoProjectConfigRegProxy.h>
#include <vgKernel/vgkAbstractTree.h>
#include <vgKernel/vgkAbstractTreeFac.h>
#include <vgKernel/vgkAbstractTreeFacMgr.h>

#include <vgKernel/vgkRenderMngCommand.h>
#include <vgKernel/vgkRenderCommandManager.h>

namespace vgKernel {


#define VGK_RENDERERMGR_NAME "RendererManager"

#define VGK_RENDERERMGR_DISTANCE_CULL		"DistanceCull"
#define VGK_RENDERERMGR_DISTANCE_CULL_DEFAULT		-0.1
#define VGK_RENDERERMGR_DISTANCE_CULL_DEFAULT_STR		"-0.1"


	/**
		�����ﴴ��project config
	*/
	vgConf::ProjectConfigRegProxy vgk_rendermgr_reg1( 
		VGK_RENDERERMGR_NAME , VGK_RENDERERMGR_DISTANCE_CULL , 
		VGK_RENDERERMGR_DISTANCE_CULL_DEFAULT_STR );


	//----------------------------------------------------------------
	void RendererManager::clear()
	{
		RendererPackage::iterator ipac = _renderersAll.begin();
		RendererPackage::iterator ipac_end = _renderersAll.end();

		for ( ; ipac != ipac_end ; ++ ipac )
		{
			RendererQueue* curqueue = &ipac->second;

			curqueue->clear();
		}

		_renderersAll.clear();


		RendererPackage::iterator icu = _renderersCulled.begin();
		RendererPackage::iterator icu_end = _renderersCulled.end();

		for ( ; icu != icu_end ; ++ icu )
		{
			RendererQueue* curqueue = &icu->second;

			curqueue->clear();
		}

		_renderersCulled.clear();


		return;
	}
	//---------------------------------------------------------------
	void RendererManager::reset()
	{
		clear();

	}
	//----------------------------------------------------------------
	RendererQueue* RendererManager::addRenderer( Renderer* renderer )
	{
		if ( renderer == NULL )
		{
			return NULL;
		}

		RendererType type = renderer->getType();

		RendererQueue* queque = getRenderQueue( type , true );

		assert( queque != NULL );

		//���ʱ��,�������,�Ƿ�����ͬ��renderer
		RendererQueue::iterator iter = queque->begin();
		RendererQueue::iterator iter_end = queque->end();

		for ( ; iter != iter_end ; ++iter )
		{
			if ( *iter == renderer )
			{
				//assert(0 &&"RendererManager�к�����ͬ����!!");
				std::ostringstream oStr;
#if VGK_LANG_CHS
				oStr << "\n\tRendererManager�к�����ͬ����:"<< renderer->getName() << " \n";
#else
				oStr << "\n\tRendererManager same item: "<< renderer->getName() << " \n";
#endif
				VGK_SHOW( oStr.str() );

				return queque;
			}
		}

		queque->push_back( renderer );
		// ���°�Χ��
		//_renderersAllBox.merge(renderer->getBoundingBox());

		return queque;
	}
	//----------------------------------------------------------------

	RendererQueue* RendererManager::getRendererQueueByType(
		const RendererType& renderertype ,
		const bool& add_if_needed )
	{
		return getRenderQueue( renderertype , add_if_needed );
	}
	//----------------------------------------------------------------
	RendererQueue* RendererManager::getCulledRenderQueueByType(
		const RendererType& rendertype )
	{
		RendererPackage::iterator iter = _renderersCulled.find( rendertype );

		if ( iter != _renderersCulled.end() )
		{
			return &iter->second;
		}

		return NULL;
	}
	//----------------------------------------------------------------
	RendererQueue* RendererManager::getRenderQueue( const RendererType& type,
		const bool& add_if_needed)
	{
		RendererPackage::iterator iter = _renderersAll.find( type );

		if ( iter != _renderersAll.end() )
		{
			// ��ʱ������Ӧ�Ķ���,ֻҪ����������м���.
			return &iter->second;
		}

		if ( add_if_needed == false )
		{
			return NULL;
		}

		// û��������͵Ķ���,���ʱ��Ҫ����.
		// ������_renderersAll��_renderersCulled��
		//RendererQueue* queueall = new RendererQueue;
		_renderersAll.insert( std::make_pair( type , RendererQueue() ));

		//RendererQueue* queueculled = new RendererQueue;
		_renderersCulled.insert( std::make_pair( type , RendererQueue() ));


		return &_renderersAll[type];
	}
	//----------------------------------------------------------------
	bool RendererManager::deleteRenderer( Renderer* renderer )
	{
		assert( renderer != NULL );
		if ( renderer == NULL )
		{
			return false;
		}

		RendererQueue* que = getRenderQueue( renderer->getType() , false );

		if ( que == NULL )
		{
			return RendererHolderManager::getSingleton().deleteRenderer( renderer );
		}

		// ���ʱ����Ҫ�ֶ�ɾ�����renderer
		RendererQueue::iterator iter = que->begin();
		RendererQueue::iterator iter_end = que->end();

		for ( ; iter != iter_end ; ++ iter )
		{
			if ( *iter == renderer )
			{
				que->erase( iter );

				if ( RendererHolderManager::getSingleton().deleteRenderer( renderer ) )
				{
					return true;
				}
				
				assert(0 && "��������");

				return false;
			}
		}

		//assert(0 && "deleteRenderer��Ӧ�õ�����");

		return false;
	}

	bool RendererManager::initialise()
	{
		return true;
	}
	//----------------------------------------------------------------
	void RendererManager::caluateVisiblePackage()
	{
		//------------------------------------------
		// 0.���culled����
		//------------------------------------------
		{
			RendererPackage::iterator iter = _renderersCulled.begin();
			RendererPackage::iterator iter_end = _renderersCulled.end();

			for ( ; iter != iter_end ; ++iter )
			{
				iter->second.clear();
			}
		}


		assert( _renderersCulled.size() == _renderersAll.size() );

		if ( _sceneTree == NULL )
		{
			assert(0);
		}
		assert( _sceneTree != NULL );
		_sceneTree->getVisibleSet( &_renderersCulled );


	}
	//----------------------------------------------------------------
	void RendererManager::renderBegin()
	{
		//------------------------------------------
		// ÿһ֡�Ŀ�ʼ������Ӽ�
		//------------------------------------------
		caluateVisiblePackage();

	}

	void RendererManager::render()
	{

	}

	//----------------------------------------------------------------
	void RendererManager::renderEnd()
	{
	}
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	void RendererManager::translateAllRenderers( const Vec3& vec)
	{
		RendererPackage::iterator	iter_pack = _renderersAll.begin();
		RendererPackage::iterator	iter_packEnd = _renderersAll.end();
		for (; iter_pack != iter_packEnd; iter_pack++)
		{
			RendererQueue*	pQuene = &iter_pack->second;

			RendererQueue::iterator	iter_quene = pQuene->begin();
			RendererQueue::iterator	iter_queneEnd = pQuene->end();
			for (; iter_quene != iter_queneEnd; iter_quene++)
			{
				Renderer*	pRenderer = *iter_quene;
				if (pRenderer)
				{
					pRenderer->translate(vec.x, vec.y, vec.z);
				}
			}
		}

	}
	//----------------------------------------------------------------
	Renderer* RendererManager::getRendererbyName(const String& name,
		bool bCaseSensitive/* = true*/)
	{
		Vec3 vec;

		RendererPackage::iterator	iter_pack = _renderersAll.begin();
		RendererPackage::iterator	iter_packEnd = _renderersAll.end();

		if ( bCaseSensitive )
		{
			for (; iter_pack != iter_packEnd; iter_pack++)
			{
				vgKernel::RendererQueue*	pQuene = &iter_pack->second;

				vgKernel::RendererQueue::iterator	iter_quene = pQuene->begin();
				vgKernel::RendererQueue::iterator	iter_queneEnd = pQuene->end();

				for (; iter_quene != iter_queneEnd; iter_quene++)
				{
					Renderer*	pRenderer = *iter_quene;

					if (pRenderer->getName()==name)
					{
						return pRenderer;
					}
				}
			}
		} 
		else
		{
			String strTempDes(name), strTempSrc;
			StringUtility::toLowerCase(strTempDes);

			for (; iter_pack != iter_packEnd; iter_pack++)
			{
				vgKernel::RendererQueue*	pQuene = &iter_pack->second;

				vgKernel::RendererQueue::iterator	iter_quene = pQuene->begin();
				vgKernel::RendererQueue::iterator	iter_queneEnd = pQuene->end();

				for (; iter_quene != iter_queneEnd; iter_quene++)
				{
					Renderer*	pRenderer = *iter_quene;

					String strTempSrc(pRenderer->getName());
					StringUtility::toLowerCase(strTempSrc);

					if (strTempDes == strTempSrc)
					{
						return pRenderer;
					}
				}
			}
		}
		
		return NULL;
	}

	void RendererManager::setAllRenderersVisiable(bool visiable)
	{
		//Ҫдoperator delete all

		RendererPackage::iterator 	iter_pack = _renderersAll.begin();
		RendererPackage::iterator 	iter_packEnd = _renderersAll.end();

		for (; iter_pack != iter_packEnd; iter_pack++)
		{
			RendererQueue*	pQuene = &iter_pack->second;

			RendererQueue::iterator iter = pQuene->begin();

			while ( iter != pQuene->end() )
			{
				Renderer* prenderer = *iter;

				prenderer->setVisible(visiable);

				iter ++;
			}

		}
	}

	//----------------------------------------------------------------
	bool RendererManager::deleteAllRenderers()
	{
		//Ҫдoperator delete all


		//------------------------------------------
		// ���������Ⱦ��
		//------------------------------------------
		RendererPackage::iterator icu = _renderersCulled.begin();
		RendererPackage::iterator icu_end = _renderersCulled.end();

		for ( ; icu != icu_end ; ++ icu )
		{
			RendererQueue* curqueue = &icu->second;

			curqueue->clear();
		}

		_renderersCulled.clear();


		RendererPackage::iterator 	iter_pack = _renderersAll.begin();
		RendererPackage::iterator 	iter_packEnd = _renderersAll.end();

		for (; iter_pack != iter_packEnd; iter_pack++)
		{
			RendererQueue*	pQuene = &iter_pack->second;

			RendererQueue::iterator iter = pQuene->begin();

			while ( iter != pQuene->end() )
			{
				Renderer* prenderer = *iter;
				bool deleteok = deleteRenderer( prenderer );

				//------------------------------------------
				// �����һ��rendererɾ��ʧ��,����������
				//------------------------------------------
				assert( deleteok == true );

				iter = pQuene->begin();
			}

			pQuene->clear();
		}

		_renderersAll.clear();


		return true;
	}
	//----------------------------------------------------------------
	void RendererManager::readProject( const String& projpath , const String& projname )
	{
		float distancecull = VG_PROJCONF_GET_FLOAT( 
			VGK_RENDERERMGR_NAME,
			VGK_RENDERERMGR_DISTANCE_CULL );

		assert( _sceneTree != NULL );

		_sceneTree->setDistanceCull( distancecull );
	}
	//----------------------------------------------------------------
	void RendererManager::saveProject()
	{
		VG_PROJCONF_SET( VGK_RENDERERMGR_NAME , 
			VGK_RENDERERMGR_DISTANCE_CULL , 
			_sceneTree->getDistanceCull() );
	}

	RendererManager::RendererManager() : Singleton<RendererManager>( VGK_SINGLETON_LEFE_RENDERERMGR )
	{
		_sceneTree  = NULL;

		m_pRenderBegin = new RenderMngBegin;
		m_pRenderEnd   = new RenderMngEnd;

		vgKernel::RenderCommandFacade::AddCommand(m_pRenderBegin);
		vgKernel::RenderCommandFacade::AddCommand(m_pRenderEnd);

		VGK_TRACE(_T("RendererManager created."));
	}

	RendererManager::~RendererManager()
	{
		vgKernel::RenderCommandFacade::RemoveCommand(m_pRenderBegin);
		vgKernel::RenderCommandFacade::RemoveCommand(m_pRenderEnd);

		clear();

		if ( _sceneTree != NULL )
		{
			delete _sceneTree;
			_sceneTree = NULL;
		}

		VGK_TRACE(_T("RendererManager destroyed."));
	}

	bool RendererManager::shutdown()
	{
		return true;
	}
	//----------------------------------------------------------------
	String RendererManager::getDetails()
	{
		std::ostringstream o;

		o << "RendererManager::getDetails()\n";

		RendererPackage::iterator 	iter_pack = _renderersAll.begin();
		RendererPackage::iterator 	iter_packEnd = _renderersAll.end();

		for (; iter_pack != iter_packEnd; iter_pack++)
		{
			RendererQueue*	pQuene = &iter_pack->second;

			if ( pQuene->empty() == false )
			{
				Renderer* ren = *pQuene->begin();
				o << "<" << ren->getType() << ", " << ren->getTypeDesc() << " >\n";
			}
		}

		return o.str();
	}

	void RendererManager::invalidate()
	{
		_sceneTree->invalidate();
	}

	void RendererManager::initAfterOpenGLSetup()
	{
		assert( _sceneTree == NULL );
		_sceneTree = AbstractTreeFacMgr::getSingleton().getAbstractTreeFac()->create();
		assert( _sceneTree != NULL );

		_sceneTree->setRendererPackage( &_renderersAll );
	}
	//----------------------------------------------------------------
	
	Box RendererManager::getAllRendererBox()
	{

		Box reBox(INT_MAX);

		RendererPackage::iterator 	iter_pack = _renderersAll.begin();
		RendererPackage::iterator 	iter_packEnd = _renderersAll.end();

		for (; iter_pack != iter_packEnd; iter_pack++)
		{
			RendererQueue*	pQuene = &iter_pack->second;

			RendererQueue::iterator iter = pQuene->begin();

			while ( iter != pQuene->end() )
			{
				Renderer* prenderer = *iter;
				
				reBox.merge(prenderer->getBoundingBox());
			
				iter ++;
			}

		}


		return reBox;
	}
	//----------------------------------------------------------------

	Box RendererManager::getAllRendererBoxWithoutshp()
	{

		Box reBox(INT_MAX);

		RendererPackage::iterator 	iter_pack = _renderersAll.begin();
		RendererPackage::iterator 	iter_packEnd = _renderersAll.end();

		for (; iter_pack != iter_packEnd; iter_pack++)
		{
			RendererQueue*	pQuene = &iter_pack->second;

			RendererQueue::iterator iter = pQuene->begin();

			while ( iter != pQuene->end() )
			{
				Renderer* prenderer = *iter;

				if ( prenderer->getType() != 16152 )//shape type ID
				{
					reBox.merge(prenderer->getBoundingBox());
				}

				iter ++;
			}

		}


		return reBox;
	}
}// end of namespace vgKernel
