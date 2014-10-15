
#include <vgStableHeaders.h>
#include <vgLogo/vgloMenuLogo.h>
#include <vgLogo/vgloLogoManager.h>

#include <vgKernel/vgkSystemUtility.h>
#include <vgKernel/vgkStringUtility.h>
#include <vgImage/vgiFreeImageWrapper.h>

#include <vgConf/vgcoExeConfig.h>

#define VGDATA_PATHCONF						"DataPathConfig"
#define VGDATA_PATHCONF_TEXTURE_PATH		"TexturePath"

#define VGDATA_FILECONF						"DataFileConfig"
#define VGDATA_FILECONF_TEXTURE_LOGO		"LogoName"


namespace vgLogo {  


	//----------------------------------------------------------------
	MenuLogo::MenuLogo()
	{
		m_varyType = NONE;
		m_dWidth = 512.0;
		m_dHeight = 477.0;

		int viewport[4];
		glGetIntegerv( GL_VIEWPORT, viewport );
		m_lastViewWidth = viewport[2];
		m_lastViewHeight = viewport[3];

		SetHandleStatus( VG_MENU_LOGO );
		vgKernel::InputSystem::getSingleton().registerHandle(this);
		vgKernel::InputSystem::getSingleton().setCurrentStatus( VG_MENU_LOGO | 
			VG_INPUTMODE_CAMERA | VG_INPUTMODE_SELECT | VG_INPUTMODE_UI );
	}

	//----------------------------------------------------------------
	MenuLogo::~MenuLogo()
	{
		this->uninitMenuLogo( &m_rootMenu );  //ж������
	}

	//-----------------------------------------------------------
	void MenuLogo::initMenuLogoLD()
	{
		MenuItem &menuItemLayer1 = m_rootMenu;

		vgKernel::StringVector fileNames;
		//****************************************************************
		MenuItem* menuItemLayer2;
		menuItemLayer2 = new MenuItem( "M2L11", 1, USER_MENU_LEAF );
		menuItemLayer2->setLogoWidth( (float)0.1 );
		menuItemLayer2->setCenterPos( vgKernel::Vec2( (float)-0.9, (float)-1.0 ), false );
		menuItemLayer2->setWCounter( -1 );
		fileNames.push_back( "MenuLogoLeft6.tga" );
		menuItemLayer2->loadTexture( fileNames );
		menuItemLayer1.appendSubItem( menuItemLayer2 ); 
		//****************************************************************
		menuItemLayer2 = new MenuItem( "M2L12", 2, USER_MENU_LEAF );
		menuItemLayer2->setLogoWidth( (float)0.1 );
		menuItemLayer2->setCenterPos( vgKernel::Vec2( (float)-0.8, (float)-1.0 ), false );
		menuItemLayer2->setWCounter( -3 );
		fileNames.clear();
		fileNames.push_back( "MenuLogoLeft5.tga" );
		menuItemLayer2->loadTexture( fileNames );
		menuItemLayer1.appendSubItem( menuItemLayer2 ); 
		//****************************************************************
		menuItemLayer2 = new MenuItem( "M2L13", 3, USER_MENU_LEAF );
		menuItemLayer2->setLogoWidth( (float)0.1 );
		menuItemLayer2->setCenterPos( vgKernel::Vec2( (float)-0.7, (float)-1.0 ), false );
		menuItemLayer2->setWCounter( -5 );
		fileNames.clear();
		fileNames.push_back( "MenuLogoLeft4.tga" );
		menuItemLayer2->loadTexture( fileNames );
		menuItemLayer1.appendSubItem( menuItemLayer2 ); 
		//****************************************************************
		menuItemLayer2 = new MenuItem( "M2L14", 4, USER_MENU_LEAF );
		menuItemLayer2->setLogoWidth( (float)0.1 );
		menuItemLayer2->setCenterPos( vgKernel::Vec2( (float)-0.6, (float)-1.0 ), false );
		menuItemLayer2->setWCounter( -7 );
		fileNames.clear();
		fileNames.push_back( "MenuLogoLeft3.tga" );
		menuItemLayer2->loadTexture( fileNames );
		menuItemLayer1.appendSubItem( menuItemLayer2 ); 
		//****************************************************************
		menuItemLayer2 = new MenuItem( "M2L15", 5, USER_MENU_LEAF );
		menuItemLayer2->setLogoWidth( (float)0.1 );
		menuItemLayer2->setCenterPos( vgKernel::Vec2( (float)-0.5, (float)-1.0 ), false );
		menuItemLayer2->setWCounter( -9 );
		fileNames.clear();
		fileNames.push_back( "MenuLogoLeft2.tga" );
		menuItemLayer2->loadTexture( fileNames );
		menuItemLayer1.appendSubItem( menuItemLayer2 ); 
		//****************************************************************
		menuItemLayer2 = new MenuItem( "M2L16", 6, USER_MENU_LEAF );
		menuItemLayer2->setLogoWidth( (float)0.1 );
		menuItemLayer2->setCenterPos( vgKernel::Vec2( (float)-0.4, (float)-1.0 ), false );
		menuItemLayer2->setWCounter( -11 );
		fileNames.clear();
		fileNames.push_back( "MenuLogoLeft1.tga" );
		menuItemLayer2->loadTexture( fileNames );
		menuItemLayer1.appendSubItem( menuItemLayer2 ); 

	}

	//-----------------------------------------------------------
	void MenuLogo::initMenuLogoRD()
	{
		MenuItem &menuItemLayer1 = m_rootMenu;
		menuItemLayer1.setRenderState( false );  //��������Ⱦ
		menuItemLayer1.setSubRenderState( true );//����������Ⱦ

		MenuItem* menuItemLayer2;
		MenuItem* menuItemLayer3;
		MenuItem* menuItemLayer4;

		vgKernel::StringVector fileNames;
		//*******************������һ����Ŧ*******************************
		menuItemLayer2 = new MenuItem( "return", 1000, USER_MENU_LEAF );   // new ������
		menuItemLayer2->setLogoWidth( (float)0.1 );  // ���ÿ��,ͬʱ������Ļ��߱ȼ���߶�
		menuItemLayer2->setCenterPos( vgKernel::Vec2( (float)1.0, (float)-1.0 ), false );// ��������λ��
		menuItemLayer2->setWCounter( 1 ); 
		fileNames.clear();
		fileNames.push_back( "��͸��.tga" );
		fileNames.push_back( "����2.tga" );
		menuItemLayer2->loadTexture( fileNames );          // ������ͼ
		menuItemLayer2->setRenderState( false );           // ������Ⱦ״̬
		menuItemLayer2->setParentPtr( &menuItemLayer1 );   // ���ø��ڵ� 

		menuItemLayer1.appendSubItem( menuItemLayer2 );

		//****************************************************************
		menuItemLayer2 = new MenuItem( "M1L11", 1, USER_MENU_BRANCH );
		menuItemLayer2->setLogoWidth( (float)0.1 );                  
		menuItemLayer2->setCenterPos( vgKernel::Vec2( 1.0, -1.0 ), false );     
		menuItemLayer2->setWCounter( 1 );
		fileNames.clear();
		fileNames.push_back( "MenuLogo6.tga" );
		fileNames.push_back( "MenuLogo6In.tga" );
		menuItemLayer2->loadTexture( fileNames );          
		menuItemLayer2->setParentPtr( &menuItemLayer1 );

		menuItemLayer3 = new MenuItem( "M1L211", 11, USER_MENU_BRANCH );
		menuItemLayer3->setLogoWidth( (float)0.1 );
		menuItemLayer3->setCenterPos( vgKernel::Vec2( (float)0.9, (float)-1.0 ), false );
		menuItemLayer3->setWCounter( 3 );
		fileNames.clear();
		fileNames.push_back( "MenuLogo5.tga" );
		fileNames.push_back( "MenuLogo5In.tga" );
		menuItemLayer3->loadTexture( fileNames );
		menuItemLayer3->setParentPtr( menuItemLayer2 );

		menuItemLayer4 = new MenuItem( "M1L3111", 111, USER_MENU_LEAF );
		menuItemLayer4->setLogoWidth( (float)0.1 );
		menuItemLayer4->setCenterPos( vgKernel::Vec2( (float)0.9, (float)-1.0 ), false );
		menuItemLayer4->setWCounter( 3 );
		fileNames.clear();
		fileNames.push_back( "MenuLogo2.tga" );
		fileNames.push_back( "MenuLogo2In.tga" );
		menuItemLayer4->loadTexture( fileNames );
		menuItemLayer4->setParentPtr( menuItemLayer3 );
		menuItemLayer3->appendSubItem( menuItemLayer4 );

		menuItemLayer2->appendSubItem( menuItemLayer3 );


		menuItemLayer3 = new MenuItem( "M1L212", 12, USER_MENU_LEAF );
		menuItemLayer3->setLogoWidth( (float)0.1 );
		menuItemLayer3->setCenterPos( vgKernel::Vec2( (float)0.8, (float)-1.0 ), false );
		menuItemLayer3->setWCounter( 5 );
		fileNames.clear();
		fileNames.push_back( "MenuLogo4.tga" );
		fileNames.push_back( "MenuLogo4In.tga" );
		menuItemLayer3->loadTexture( fileNames );
		menuItemLayer3->setParentPtr( menuItemLayer2 );
		menuItemLayer2->appendSubItem( menuItemLayer3 );

		menuItemLayer3 = new MenuItem( "M1L213", 13, USER_MENU_LEAF );
		menuItemLayer3->setLogoWidth( (float)0.1 );
		menuItemLayer3->setCenterPos( vgKernel::Vec2( (float)0.7, (float)-1.0 ), false );
		menuItemLayer3->setWCounter( 7 );
		fileNames.clear();
		fileNames.push_back( "MenuLogo2.tga" );
		fileNames.push_back( "MenuLogo2In.tga" );
		menuItemLayer3->loadTexture( fileNames );
		menuItemLayer3->setParentPtr( menuItemLayer2 );
		menuItemLayer2->appendSubItem( menuItemLayer3 );

		menuItemLayer3 = new MenuItem( "M1L214", 14, USER_MENU_LEAF );
		menuItemLayer3->setLogoWidth( (float)0.1 );
		menuItemLayer3->setCenterPos( vgKernel::Vec2( (float)0.6, (float)-1.0 ), false );
		menuItemLayer3->setWCounter( 9 );
		fileNames.clear();
		fileNames.push_back( "MenuLogo1.tga" );
		fileNames.push_back( "MenuLogo1In.tga" );
		menuItemLayer3->loadTexture( fileNames );
		menuItemLayer3->setParentPtr( menuItemLayer2 );
		menuItemLayer2->appendSubItem( menuItemLayer3 );

		menuItemLayer1.appendSubItem( menuItemLayer2 );

		//****************************************************************
		menuItemLayer2 = new MenuItem( "M1L12", 2, USER_MENU_BRANCH );
		menuItemLayer2->setLogoWidth( (float)0.1 );
		menuItemLayer2->setCenterPos( vgKernel::Vec2( (float)0.9, (float)-1.0 ), false );
		menuItemLayer2->setWCounter( 3 );
		fileNames.clear();
		fileNames.push_back( "MenuLogo5.tga" );
		fileNames.push_back( "MenuLogo5In.tga" );
		menuItemLayer2->loadTexture( fileNames );
		menuItemLayer2->setParentPtr( &menuItemLayer1 );

		menuItemLayer3 = new MenuItem( "M1L221", 21, USER_MENU_LEAF );
		menuItemLayer3->setLogoWidth( (float)0.1 );
		menuItemLayer3->setCenterPos( vgKernel::Vec2( (float)0.9, (float)-1.0 ), false );
		menuItemLayer3->setWCounter( 3 );
		fileNames.clear();
		fileNames.push_back( "MenuLogo6.tga" );
		fileNames.push_back( "MenuLogo6In.tga" );
		menuItemLayer3->loadTexture( fileNames );
		menuItemLayer3->setParentPtr( menuItemLayer2 );
		menuItemLayer2->appendSubItem( menuItemLayer3 );

		menuItemLayer3 = new MenuItem( "M1L222", 22, USER_MENU_LEAF );
		menuItemLayer3->setLogoWidth( (float)0.1 );
		menuItemLayer3->setCenterPos( vgKernel::Vec2( (float)0.8, (float)-1.0 ), false );
		menuItemLayer3->setWCounter( 5 );
		fileNames.clear();
		fileNames.push_back( "MenuLogo4.tga" );
		fileNames.push_back( "MenuLogo4In.tga" );
		menuItemLayer3->loadTexture( fileNames );
		menuItemLayer3->setParentPtr( menuItemLayer2 );
		menuItemLayer2->appendSubItem( menuItemLayer3 );

		menuItemLayer1.appendSubItem( menuItemLayer2 );
		//****************************************************************
		menuItemLayer2 = new MenuItem( "M1L13", 3, USER_MENU_LEAF );
		menuItemLayer2->setLogoWidth( (float)0.1 );
		menuItemLayer2->setCenterPos( vgKernel::Vec2( (float)0.8, (float)-1.0 ), false );
		menuItemLayer2->setWCounter( 5 );
		fileNames.clear();
		fileNames.push_back( "MenuLogo4.tga" );
		fileNames.push_back( "MenuLogo4In.tga" );
		menuItemLayer2->loadTexture( fileNames );
		menuItemLayer2->setParentPtr( &menuItemLayer1 );
		menuItemLayer1.appendSubItem( menuItemLayer2 );
		//****************************************************************
		menuItemLayer2 = new MenuItem( "M1L14", 4, USER_MENU_LEAF );
		menuItemLayer2->setLogoWidth( (float)0.1 );
		menuItemLayer2->setCenterPos( vgKernel::Vec2( (float)0.7, (float)-1.0 ), false );
		menuItemLayer2->setWCounter( 7 );
		fileNames.clear();
		fileNames.push_back( "MenuLogo3.tga" );
		fileNames.push_back( "MenuLogo3In.tga" );
		menuItemLayer2->loadTexture( fileNames );
		menuItemLayer2->setParentPtr( &menuItemLayer1 );
		menuItemLayer1.appendSubItem( menuItemLayer2 ); 
		//****************************************************************
		menuItemLayer2 = new MenuItem( "M1L15", 5, USER_MENU_LEAF );
		menuItemLayer2->setLogoWidth( (float)0.1 );
		menuItemLayer2->setCenterPos( vgKernel::Vec2( (float)0.6, (float)-1.0 ), false );
		menuItemLayer2->setWCounter( 9 );
		fileNames.clear();
		fileNames.push_back( "MenuLogo2.tga" );
		fileNames.push_back( "MenuLogo2In.tga" );
		menuItemLayer2->loadTexture( fileNames );
		menuItemLayer2->setParentPtr( &menuItemLayer1 );
		menuItemLayer1.appendSubItem( menuItemLayer2 ); 
		//****************************************************************
		menuItemLayer2 = new MenuItem( "M1L16", 6, USER_MENU_LEAF );
		menuItemLayer2->setLogoWidth( (float)0.1 );
		menuItemLayer2->setCenterPos( vgKernel::Vec2( (float)0.5, (float)-1.0 ), false );
		menuItemLayer2->setWCounter( 11 );
		fileNames.clear();
		fileNames.push_back( "MenuLogo1.tga" );
		fileNames.push_back( "MenuLogo1In.tga" );
		menuItemLayer2->loadTexture( fileNames );
		menuItemLayer2->setParentPtr( &menuItemLayer1 );
		menuItemLayer1.appendSubItem( menuItemLayer2 ); 

		//////////////////////////////////////////////////////////////////////////
		clearCurrentItemVec();
		findCurrentItems( &m_rootMenu );
	}

	//-----------------------------------------------------------
	void MenuLogo::uninitMenuLogo( MenuItem* pRootMenuItem )
	{
		if ( !pRootMenuItem->hasSubItem() )
		{
			return;
		}

		MenuItemVecL1* pMenuItemVec = pRootMenuItem->getSubItems();
		MenuItemVecL1Itr itr = pMenuItemVec->begin();
		MenuItemVecL1Itr itr_end = pMenuItemVec->end();

		for ( ; itr != itr_end; itr++ )
		{
			if ( (*itr)->hasSubItem() )
			{
				this->uninitMenuLogo( (*itr) );  //������ӽڵ���ݹ�ж��

				(*itr)->unloadData();

				//�ͷ�vector ��new�������ڴ�
				delete (*itr);
				*itr = NULL;
			}
			else
			{
				(*itr)->unloadData();   //�����Ҷ�ڵ���ֱ��ж��

				//�ͷ�vector ��new�������ڴ�
				delete (*itr);
				*itr = NULL;
			}
		}
	}

	//-----------------------------------------------------------
	void MenuLogo::reset()
	{/*
		m_bRender = true;
		m_centerPos = vgKernel::Vec2();
		m_logoHeight = 0.1f;                             
		m_logoWidth = 0.1f;*/
	
	}

	//-----------------------------------------------------------
	void MenuLogo::render()
	{
		//����Ⱦ���ڴ�С�仯ʱ���¼���Logo�Ĳ���
		int viewport[4];
		glGetIntegerv( GL_VIEWPORT, viewport );

		//ֻ��ȱ仯
		if ( viewport[2] != m_lastViewWidth && viewport[3] == m_lastViewHeight )
		{
			m_lastViewWidth = viewport[2];
			this->setVaryType( WIDTH );
		}
		//ֻ�߶ȱ仯 
		if ( viewport[3] != m_lastViewHeight && viewport[2] == m_lastViewWidth )
		{
			m_lastViewHeight = viewport[3];
			this->setVaryType( HEIGHT );
		}
		//�߶ȿ�ȶ��仯
		if ( viewport[2] != m_lastViewWidth && viewport[3] != m_lastViewHeight )
		{
			m_lastViewWidth = viewport[2];
			m_lastViewHeight = viewport[3];
			this->setVaryType( BOTH );
		}

		//���ݴ��ڱ仯���ͣ����¼���MenuLogo������
		if ( m_varyType != NONE )
		{
			this->recalculate( &m_rootMenu );
			this->setVaryType( NONE );
		}

		//��ȾLogo
		this->render( &m_rootMenu );
	}

	//-----------------------------------------------------------
	void MenuLogo::render( MenuItem* pRootMenuItem )
	{	
		if ( !pRootMenuItem->getRenderState() && !pRootMenuItem->getSubRenderState() )
		{
			//�����ӽڵ㶼������Ⱦʱ����
			return;
		}
		
		MenuItemVecL1* pMenuItemVec = pRootMenuItem->getSubItems();

		MenuItemVecL1Itr itr = pMenuItemVec->begin();
		MenuItemVecL1Itr itr_end = pMenuItemVec->end();

		for ( ; itr != itr_end; itr++ )
		{
			if ( (*itr)->hasSubItem() )
			{
				//������ӽڵ����ӽڵ����Ⱦ
				if ( (*itr)->getSubRenderState() )
				{
					//����ӽڵ����Ⱦ
					render( (*itr) );

					(*itr)->render();
				}
				if ( (*itr)->getRenderState() )
				{
					//����������Ⱦ
					(*itr)->render();
				}
			}
			else
			{
				(*itr)->render();
			}
		}
	}
	//-----------------------------------------------------------
	void MenuLogo::recalculate( MenuItem* pRootMenuItem )
	{
		MenuItemVecL1* pMenuItemVec = pRootMenuItem->getSubItems();

		MenuItemVecL1Itr itr = pMenuItemVec->begin();
		MenuItemVecL1Itr itr_end = pMenuItemVec->end();

		for ( ; itr != itr_end; itr++ )
		{
			if ( (*itr)->hasSubItem() )    //������ӽڵ���ѭ������
			{
				this->recalculate( (*itr) );

				this->recalculateData( (*itr) );
			}
			else  //�����Ҷ�ڵ���ֱ�Ӵ���
			{
				this->recalculateData( (*itr) );
			}
		}
	}

	//-----------------------------------------------------------
	void MenuLogo::recalculateData( MenuItem* pMenuItem )
	{
		int viewport[4];
		glGetIntegerv( GL_VIEWPORT, viewport );

		// 1.�ָ�ԭʼ����
		pMenuItem->setLogoWidth( pMenuItem->getOriginalWidth() );
		pMenuItem->setLogoHeight( pMenuItem->getOriginalHeight() );
		pMenuItem->setCenterPos( pMenuItem->getOriginalCenterPos(), true );
		// 2.�������
		float radio = viewport[2] / (float)viewport[3];

		float originalHeight = pMenuItem->getOriginalHeight();
		float originalWidth = pMenuItem->getOriginalWidth();

		if ( m_varyType == WIDTH )  //���ֿ�ȱ������ø߶ȱ仯
		{
#if 0  
			// 1.�ָ�ԭʼ����
			pMenuItem->setLogoWidth( pMenuItem->getOriginalWidth() );
			pMenuItem->setLogoHeight( pMenuItem->getOriginalHeight() );
			pMenuItem->setCenterPos( (*pMenuItem->getOriginalCenterPos()), true );

			// 2.�������
			float ratio = viewport[2] / (float)viewport[3];
			float originalHeight = pMenuItem->getOriginalHeight();
#endif
			// 3.�����µĲ���
			pMenuItem->setLogoHeight( originalHeight * radio );
			pMenuItem->getCenterPos()->y += 
				pMenuItem->getHCounter() * ( pMenuItem->getLogoHeight() - originalHeight ) / 2.0;
		}
		else if ( m_varyType == HEIGHT  )  //���ֿ�ȱ������ÿ�ȱ仯
		{
			//�˱��������¼��㣬���������������ͬ
			radio = ( viewport[3] / m_dHeight ) * ( m_dWidth / (float)viewport[2] );

			// 3.�����µĲ���
			pMenuItem->setLogoWidth( originalWidth * radio );
			pMenuItem->getCenterPos()->x -= 
				pMenuItem->getWCounter() * ( pMenuItem->getLogoWidth() - originalWidth ) / 2.0;
		}
		else if ( m_varyType == BOTH )   //���ֿ�ȱ������ø߶ȱ仯
		{
			// 3.�����µĲ���
			pMenuItem->setLogoHeight( originalHeight * radio );
			pMenuItem->getCenterPos()->y += 
				pMenuItem->getHCounter() * ( pMenuItem->getLogoHeight() - originalHeight ) / 2.0;
		}
	}

	//-----------------------------------------------------------
	void MenuLogo::OnLButtonDown(UINT nFlags, CPoint position)
	{
		//////////////////////////////////////////////////////////////////////////
		//Ϊ���ų����½�Logo�������Ϣ����Ӧ�����䲻�����¼�
		MenuItemVecL1* pVec = m_rootMenu.getSubItems();
		if ( pVec->size() > 0 )
		{
			MenuItemVecL1Itr itr = pVec->begin();
			if ( (*itr)->getID() != 1000 )
			{
				return;
			}
		}
		else
		{
			return;
		}
		//////////////////////////////////////////////////////////////////////////

		int viewport[4];
		glGetIntegerv( GL_VIEWPORT, viewport );

		float x = ( position.x - viewport[2] / 2.0 ) / ( viewport[2] / 2.0 );
		float y = ( viewport[3] / 2.0 - position.y ) / ( viewport[3] / 2.0 );

		MenuItemVecL1Itr itr = getCurrentItems()->begin();
		MenuItemVecL1Itr itr_end = getCurrentItems()->end();

		for ( ; itr != itr_end; itr++ )
		{
			float width = (*itr)->getLogoWidth();
			float height = (*itr)->getLogoHeight();

			vgKernel::Vec2* centerPos = (*itr)->getCenterPos();

			float xMin = centerPos->x - width / 2.0;
			float xMax = centerPos->x + width / 2.0;
			float yMin = centerPos->y - height / 2.0;
			float yMax = centerPos->y + height / 2.0;

			if ( x >= xMin && x <= xMax && y >= yMin && y <= yMax )
			{
				int id = (*itr)->getID();

				if( id == 1000 )  //������һ����Ŧ
				{
					setItemState( m_currentItemVec[m_currentItemVec.size()-1], true );
					clearCurrentItemVec();
					findCurrentItems( &m_rootMenu );
				} 
				else
				{
					if ( (*itr)->hasSubItem() )  //��Ⱦ�ӽڵ�Item
					{
						setItemState( (*itr) );
						clearCurrentItemVec();
						findCurrentItems( &m_rootMenu );
					}
					else  //ֱ�ӽ�����Ϣ��Ӧ
					{
						//���ʹ����¼�����Ϣ
						static int i = 1;
						if ( i >= 6 )
						{
							i = 1;
						}
						CorpLogo* pTrriger = vgLogo::LogoManager::getSingleton().getCorpLogoPtr();
						CString texFileName;
						texFileName.Format( "%s%d%s", "MenuLogo", i, ".tga" );
						pTrriger->setCurrentTexID( (string)texFileName );
						i++;
					}
					
				}
				break;
			}//end if
		} //end for
	}

	//-----------------------------------------------------------
	void MenuLogo::OnMouseMove(UINT nFlags, CPoint position)
	{
		int viewport[4];
		glGetIntegerv( GL_VIEWPORT, viewport );

		float x = ( position.x - viewport[2] / 2.0 ) / ( viewport[2] / 2.0 );
		float y = ( viewport[3] / 2.0 - position.y ) / ( viewport[3] / 2.0 );

		MenuItemVecL1Itr itr = getCurrentItems()->begin();
		MenuItemVecL1Itr itr_end = getCurrentItems()->end();

		for ( ; itr != itr_end; itr++ )
		{
			float width = (*itr)->getLogoWidth();
			float height = (*itr)->getLogoHeight();

			vgKernel::Vec2* centerPos = (*itr)->getCenterPos();

			float xMin = centerPos->x - width / 2.0;
			float xMax = centerPos->x + width / 2.0;
			float yMin = centerPos->y - height / 2.0;
			float yMax = centerPos->y + height / 2.0;

			if ( x >= xMin && x <= xMax && y >= yMin && y <= yMax )
			{

				(*itr)->setCurrentTexID( true );
			}
			else
			{
				(*itr)->setCurrentTexID( false );
			}
		}
	}

	//-----------------------------------------------------------
	void MenuLogo::findCurrentItems( MenuItem* pMenuItem ) 
	{
		MenuItemVecL1* pRootMenuVec = pMenuItem->getSubItems();

		MenuItemVecL1Itr itr = pRootMenuVec->begin();
		MenuItemVecL1Itr itr_end = pRootMenuVec->end();

		for ( ; itr != itr_end; itr++ )
		{
			if ( (*itr)->getName() == "return" )  //������һ��
			{
				if ( (*itr)->getRenderState() ) //���������һ���������������
				{
					m_currentItemVec.push_back( (*itr) );
				}
				continue;
			}

			if ( (*itr)->getRenderState() ) //�˽ڵ����Ⱦ
			{
				m_currentItemVec.push_back( (*itr) );
			}
			else  //�˽ڵ㲻����Ⱦ
			{
				if ( (*itr)->hasSubItem() && (*itr)->getSubRenderState() ) 
				{
					// ���ӽڵ����ӽڵ����Ⱦ
					findCurrentItems( (*itr) );
				}
			}//end esle
		}//end for
	}

	//-----------------------------------------------------------
	void MenuLogo::setItemState( MenuItem* pMenuItem, bool bUpLevel /* = false */ ) 
	{
		if ( bUpLevel )   //���������һ��ʱ����Items״̬
		{
			MenuItem* pParent = pMenuItem->getParentPtr(); //��ȡ��ǰ�ڵ�ĸ��ڵ�
			MenuItem* pParentParent = pParent->getParentPtr();//��ȡ���ڵ�ĸ��ڵ�

			MenuItemVecL1* pParentParentVec = pParentParent->getSubItems();

			MenuItemVecL1Itr itr2 = pParentParentVec->begin();
			MenuItemVecL1Itr itr_end2 = pParentParentVec->end();

			for ( ; itr2 != itr_end2; itr2++ )
			{
				if ( pParentParent->getID() == 0 && (*itr2)->getName() == "return" )
				{
					(*itr2)->setRenderState( false );
				}
				else
				{
					(*itr2)->setRenderState( true );
					(*itr2)->setSubRenderState( false );
				}
			}
		}//end if
		else  //������ڵ㽫�����ӽڵ���Ⱦ״̬��ͬʱ�رո��ڵ�ͬһ���������ڵ���Ⱦ״̬
		{

			MenuItem* pParent = pMenuItem->getParentPtr();
			if ( pParent->getID() != 0 )
			{
				setItemState( pParent );
				
				setSubItemsState( pParent, pMenuItem ); //�����ӽڵ���Ⱦ״̬
			}
			else
			{
				setSubItemsState( pParent, pMenuItem );//�����ӽڵ���Ⱦ״̬
			}
		}//end else
	}

	//-----------------------------------------------------------
	void MenuLogo::setSubItemsState( MenuItem* pParentItem, MenuItem* pSubItem )
	{
		MenuItemVecL1* pMenuItemVec = pParentItem->getSubItems(); //��ȡ��Items

		MenuItemVecL1Itr itr = pMenuItemVec->begin();
		MenuItemVecL1Itr itr_end = pMenuItemVec->end();

		for ( ; itr != itr_end; itr++ )  //ѭ������
		{
			if ( (*itr)->getName() == "return" )
			{
				(*itr)->setRenderState( true );
				continue;
			}

			if ( (*itr)->getID() == pSubItem->getID() )  
			{
				(*itr)->setRenderState( false );  //�ر�������Ⱦ
				(*itr)->setSubRenderState( true );//���ӽڵ���Ⱦ
			}
			else
			{
				(*itr)->setRenderState( false ); //�ر�ͬ���������ڵ�������Ⱦ
				(*itr)->setSubRenderState( false );//�ر�ͬ���������ڵ��ӽڵ���Ⱦ
			}
		} //end for
	}

	//-----------------------------------------------------------
	vector<MenuItem*>* MenuLogo::getCurrentItems()
	{
		return &m_currentItemVec;
	}

	//-----------------------------------------------------------
	void MenuLogo::clearCurrentItemVec()
	{
		m_currentItemVec.clear();
	}

	//-----------------------------------------------------------
#if 0
	void MenuLogo::largenLogo( MenuItem* pMenuItem, vector<MenuItem*>* pMenuItemVec )
	{
		vector<MenuItem*>::iterator itr = pMenuItemVec->begin();
		vector<MenuItem*>::iterator itr_end = pMenuItemVec->end();

		for ( ; itr != itr_end; itr++ )
		{
			float width = (*itr)->getLogoWidth();
			float height = (*itr)->getLogoHeight();
			vgKernel::Vec2* centerPos = (*itr)->getCenterPos();
			vgKernel::Vec2 cornerPos = (*itr)->getCornerPos();

			if ( pMenuItem->getID() == (*itr)->getID() )
			{
				//IDΨһ
				(*itr)->setLogoWidth( width * 1.5 );
				(*itr)->setLogoHeight( height * 1.5 );
				(*itr)->setCenterPos( cornerPos, false );
				continue;
			}

			if ( pMenuItem->getCenterPos()->x < (*itr)->getCenterPos()->x )
			{
				//�ڱ��ͼ�������
				continue;
			}
			else
			{
				float xVary = (*itr)->getCenterPos()->x - pMenuItem->getLogoWidth() + pMenuItem->getOriginalWidth();
				(*itr)->getCenterPos()->x = xVary;
				//�ڱ��ͼ�������
				//(*itr)->setCenterPos( *(*itr)->getCenterPos(),true );
			}
		}
	}

	//-----------------------------------------------------------
	void MenuLogo::reInitAllItem( MenuItem* pMenuItem )
	{
		MenuItemVecL1* pVec = pMenuItem->getSubItems();

		MenuItemVecL1Itr itr = pVec->begin();
		MenuItemVecL1Itr itr_end = pVec->end();
		
		for ( ; itr != itr_end; itr++ )
		{
			if ( (*itr)->hasSubItem() )
			{
				reInitAllItem( (*itr) );

				(*itr)->setLogoWidth( (*itr)->getOriginalWidth() );
				(*itr)->setLogoHeight( (*itr)->getOriginalHeight() );
				(*itr)->setCenterPos( *(*itr)->getOriginalCenterPos(), true );
			}
			else
			{
				(*itr)->setLogoWidth( (*itr)->getOriginalWidth() );
				(*itr)->setLogoHeight( (*itr)->getOriginalHeight() );
				(*itr)->setCenterPos( *(*itr)->getOriginalCenterPos(), true );
			}
		}

	}
#endif

	//-----------------------------------------------------------

}//namespace vgLogo