


#include <vgStableHeaders.h>
#include <vgMesh/vgmMeshManager.h>
#include <vgMesh/vgmIoRequestMeshData.h>
#include <vgMesh/vgmMeshData.h>
//#include <vgConfig/vgcConfigManager.h>
#include <vgKernel/vgkOpenGLSupport.h>

#include <vgMesh/vgmMeshDataPlus.h>



namespace vgMesh {
	
	
	
	
	//----------------------------------------------------------------
	bool IoRequestMeshData::anotherThreadHandler()
	{
		//Sleep(50);

		//assert( _meshData == NULL );
		//assert( _buffer != NULL );


		assert( _buffer != NULL );

		assert( _meshData == NULL );

		_meshData = 
			new MeshData();

		bool initok = _meshData->initialise(  _buffer , _fileInfo->_fileSize  );

		assert( initok == true );

		assert( _meshData != NULL );

		if ( _loadShadowData )
		{
			assert( _meshDataPlus == NULL );
			_meshDataPlus = new MeshDataPlus;
			bool loadok = _meshDataPlus->importFromMeshData( _meshData );

			assert( loadok == true );
		}
		

		dispatchMainThreadFunc();

		return true;
	}
	//----------------------------------------------------------------
	bool IoRequestMeshData::mainThreadHandler()
	{


		MeshItem* item = MeshManager::getSingleton().getMeshItem( _meshID );

		if ( item == NULL )
		{
			// ��ʱitem�Ѿ�������,���Ѿ�ɾ������
			// ��ô�ӵ������ݼ���.
			//assert(0);
			delete this;
			return true;
		}

		Mesh* mesh = item->getMeshByLod( _lod );

		assert( mesh != NULL );

		const int& refcount = mesh->getReferenceCount();

		assert( refcount > -1 );

		//------------------------------------------
		// ֻ�м���ֵ����0��ʱ�����Ҫ����
		// ��Ҫ���ǵ��������:
		//  1.������0��1,������;�����������س�,��ʱ
		//		����Ϊ0,����Ҫ����
		//------------------------------------------
		if ( refcount != 0 && mesh->getMeshData() == NULL )
		{
			_meshData->initInMainThread();


 		//	vgConfig::ConfigObject* renderwaymode = //chunyongma
			//	vgConfig::ConfigManager::getSingleton().getObject( "renderwaymode" );

			//int m_renderwaymode;

			//if ( renderwaymode != NULL )
			//{
			//	m_renderwaymode = renderwaymode->getInt();

			//	if (m_renderwaymode == 0)
			//	{
					VGK_CHECK_OPENGL_ERROR();
					_meshData->convertToVboRenderWay();
					VGK_CHECK_OPENGL_ERROR();
			//	}
			//	else if (m_renderwaymode == 1)
			//	{
			//		_meshData->convertToDisplayListRenderWay();
			//	}
			//	else
			//		goto MESHDATASET;
			//}

//MESHDATASET:

			if ( _loadShadowData && mesh->hasMeshPlusData() == false )
			{
				assert( _meshDataPlus != NULL );
				// ע����һ��
				_meshDataPlus->setFatherMesh( mesh );
				mesh->setMeshDataAndPlus( _meshData, _meshDataPlus );
			}
			else
			{
				assert( _meshDataPlus == NULL );
				mesh->setMeshData( _meshData );
			}
			

			//------------------------------------------
			// �����ﴴ��vbo
			//------------------------------------------

			setDeleteBufferFlag( false );
		}

		delete this;
		return true;
	}
	//----------------------------------------------------------------
	bool IoRequestMeshData::isReallyNeedToSend()
	{
		MeshItem* item = MeshManager::getSingleton().getMeshItem( _meshID );

		if ( item == NULL )
		{
			return false;
		}

		Mesh* mesh = item->getMeshByLod( _lod );
		assert( mesh != NULL );


		const int count = mesh->getReferenceCount();

		/**
			�����ü���Ϊ0�����Ѿ�������ʱ,�Ͳ���Ҫ������
		*/
		if ( count == 0 || mesh->getMeshData() != NULL)
		{
			return false;
		}

		return true;
	}

	IoRequestMeshData::IoRequestMeshData( UniqueID unique_id , int lod , 
		vgAsyn::FileInfo* fileinfo,bool load_shadow_data  ) : vgAsyn::IoRequest( fileinfo ) ,
		_lod( lod ),
		_meshID( unique_id ),
		_loadShadowData( load_shadow_data ),
		_meshDataPlus( NULL )
	{
		_meshData = NULL;

	}
	//----------------------------------------------------------------



}// end of namespace vgMesh
