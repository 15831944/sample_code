

#ifndef __TVGBILLBOARD_H__
#define __TVGBILLBOARD_H__

#include <vgCore/vgPrerequisites.h>
#include <vgTex/tvgString.h>
#include <vgTex/tvgStringUtil.h>
#include <vgTex/tvgTexture.h>
#include <vgCore/vgSharedPtr.h>


namespace vgCore {


	/**
		@date 	2007/04/28  21:12	
		@author  Leven
			
		@brief 	��Ƭ��,���������Ĳ��������κ�ϣ������ʹ���ߵ�����.
			
		@see    BillboardSet
	*/
	class  VGDEP_EXPORT Billboard
	{
	public:

		/**
			billboard����Ⱦ��ʽ.

			RENDERTYPE_EYE_ORIENTED
				ʼ�ճ���۲���,������ʲô�Ƕ�.

			RENDERTYPE_ROTATE_Y
				����۲���,����y�᷽��̶�.

			RENDERTYPE_HOLD_STILL
				������billboard,��ʱ������ѡ��.

		*/
		enum  RenderTypes
		{
			RENDERTYPE_EYE_ORIENTED,
			RENDERTYPE_ROTATE_Y ,
			RENDERTYPE_HOLD_STILL
		};


		

		Billboard();

		Billboard( String uniname , String texUniName , Vector3 pos )
		{
			/// this must be less efficient ,but more clear.
			this->setParametersDefault();
			this->create( uniname , texUniName , pos );
		}

		Billboard( String uniname , TexturePtr ptex , Vector3 pos )
		{
			/// this must be less efficient ,but more clear.
			this->setParametersDefault();
			this->create( uniname , ptex , pos );
		}

		~Billboard();
	
		
		/**
			ת��billboard����Ⱦ��ʽ.
		*/
		void	reverseRenderType();

		/**
			ͨ������������һ��billboard.
				
			@param	uniname Ϊ���ⶨΪΨһ������,����ʹ�õ���ָ�����ʽ.
			@param	ptex ��Ƭ�ϵ�����ָ��.
			@param	pos λ��.Ĭ��Ϊԭ��.
		*/
		void create( const StringPtr& uniname ,
			const TexturePtr& ptex , const Vector3& pos = Vector3::ZERO );

		/**
			ͨ������������һ��billboard.
				
			@param	uniname Ϊ���ⶨΪΨһ������.
			@param	ptex ��Ƭ�ϵ�����ָ��.
			@param	pos λ��.Ĭ��Ϊԭ��.
		*/
		void create( const String& uniname , const TexturePtr& ptex ,
			const Vector3& pos = Vector3::ZERO );

		/**
			ͨ������������һ��billboard.
				
			@param	uniname Ϊ���ⶨΪΨһ������
			@param	texUniName ��Ƭ�ϵ������Ψһ����.�����л��Զ���ȡ����.
			@param	pos λ��.Ĭ��Ϊԭ��.
				
			@returns	void 
		*/
		void create( const String& uniname ,
			const String& texUniName , const Vector3& pos = Vector3::ZERO );

		/**
			ͨ��һϵ�еĲ���������billboard.
				
			@note	����б�Ҫ,�����ṩĬ��ֵ.
				
			@param	uniname ����Ψһ������,�������.
			@param	ptex ����ָ��.
			@param	pos billboard��λ��.
			@param	selected �Ƿ�ѡ��.
			@param	visible �Ƿ�ɼ�.
			@param	rendertype ��Ⱦ�ķ�ʽ.
			@param	width ���.
			@param	height �߶�.
			@param	upVector ���ϵ�����,����Rotate Y ��ʽ.
				
			@returns	void 
		*/
		void setParameters( const StringPtr& uniname ,
			const TexturePtr& ptex , const Vector3& pos ,
			const bool& selected , const bool& visible ,
			const RenderTypes& rendertype ,
			const Real& width , const Real& height ,
			const Vector3& upVector );

		/**
			��Ⱦ����,��billboard��Ⱦ����Ļ��.
		*/
		void	draw();

		/**
			������Ƭ������ֵ.
		*/
		void	setScale( const Real& scale );

		
		friend istream& operator >> ( istream& in , Billboard& bbd );

		friend ostream& operator << ( ostream& out , Billboard& bbd );



	private:
	public:

		/**
			ͨ��texturePtr�Զ�����billboard�Ĵ�С.
		*/
		void  autoUpdateRect();

		/**
			����Ĭ�ϵĲ���.
		*/
		void setParametersDefault();

		/**
			ʹ��RENDERTYPE_EYE_ORIENTED��ʽ��Ⱦʵ��.
		*/
		void	drawUsingEyeOriented() const;

		/**
			ʹ��RENDERTYPE_ROTATE_Y��ʽ��Ⱦʵ��.
		*/
		void	drawUsingRotatedY() const;

		/**
			ʹ��RENDERTYPE_HOLD_STILL��ʽ��Ⱦʵ��.
		*/
		void	drawUsingHoldStill() const;

		/**
			ʹ��RENDERTYPE_EYE_ORIENTED��ʽ��Ⱦ�߿�.
		*/
		void	drawEdgeUsingEyeOriented() const;

		/**
			ʹ��RENDERTYPE_HOLD_STILL��ʽ��Ⱦ�߿�.
		*/
		void	drawEdgeUsingHoldStill() const;

		/**
			ʹ��RENDERTYPE_ROTATE_Y��ʽ��Ⱦ�߿�.
		*/
		void	drawEdgeUsingRotateY() const;

		/**
			������Ⱦ�Ŀ�ʼʱ��.
		*/
		void	drawingBegin() const;

		/**
			������Ⱦ��ĩβʱ��.
		*/
		void	drawingEnd() const;

		/**
			ͨ�����뵱ǰ�ľ��������µ�ǰ���ӵ���Ϣ.
		*/
		void	updateCurrentViewVectors();

		/**
			ͨ��ModelViewMatrix����ȡ��������Ϣ.
				
			@param	right ��ǰ�ӵ����������ֵ
			@param	up ��ǰ�ӵ�ĳ���
			@param	direction ��ǰ�ӵ����������

			@returns	void 
		*/
		static void	extractVectorFromModelViewMatrix(
			Vector3& right , Vector3& up , Vector3& direction );

	public:

		/**
			�õ��������Ϣ.
		*/
		String getDetails() const;

		// ������ת���set��get����.
		const Vector3		getRotatedAxis() const;
		Vector3&		getRotatedAxis();
		void		setRotatedAxis( const Vector3& val );


		/**
			�õ�Ψһ����.
		*/
		const StringPtr		getUniqueNamePtr() const;

		// ����visible��set��get����.
		const bool		getIfVisible() const;
		bool&		getIfVisible();
		void		setIfVisible( const bool& val );


		// ����position��set��get����.
		const Vector3		getPosition() const;
		Vector3&		getPosition();
		void		setPosition( const Vector3& val );


		// ����IfSelected��set��get����.
		const bool		getIfSelected() const;
		bool&		getIfSelected();
		void		setIfSelected( const bool& val );


		// ����TexturePtr��set��get����.
		const TexturePtr		getTexturePtr() const;
		TexturePtr&		getTexturePtr();
		void		setTexturePtr( const TexturePtr& val );


		// ����RenderType��set��get����.
		const RenderTypes getRenderType() const;
		RenderTypes&	getRenderType();
		void		setRenderType( 
			const RenderTypes& val );
	

		/**
			�õ���Ƭ�ĸ߶�.
		*/
		const Real		getHeight() const;

		/**
			�õ���Ƭ�߶ȵ�����.
		*/
		Real&		getHeight();


		/**
			������Ƭ�ĸ߶�.
		*/
		void		setHeight( const Real& val );

		/**
			�õ���Ƭ�Ŀ��.
		*/
		const Real		getWidth() const;

		/**
			�õ���Ƭ��ȵ�����.
		*/
		Real&		getWidth();

		/**
			������Ƭ�Ŀ��.
		*/
		void		setWidth( const Real& val );

	public:

		/// ���ص�3D��λ��ת������
		static  Real	 PIXCEL_TO_FLOAT_FACTOR; 

		/// Ĭ�ϵ�����billboard�Ŀ��(�����������ʱ��)
		static  Real	 DEFAULT_RECT_WIDTH;

		/// Ĭ�ϵ�����billboard�ĸ߶�(�����������ʱ��)
		static  Real	 DEFAULT_RECT_HEIGHT;

	private:
			
		/// ��Ӧ��Guid�̳���ϵ�����.
		//uint		mUniqueID;

		StringPtr	mUniqueNamePtr;
		Vector3		mPosition;	

		/**
			��ת��,������RENDERTYPE_ROTATE_Y��ʽ.
			used for rotated Y drawing
		*/
		Vector3		mRotatedAxis;

		/// �Ƿ�ѡ����.
		bool		mIfSelected;

		/// �Ƿ���ʾ.
		bool		mIfVisible;	

		/// ����Ƭ������ָ��.
		TexturePtr	mTexturePtr;


		Real		mScale;



		/// �߶�
		Real		mWidth;

		/// �߶�
		Real		mHeight;

		/// ��Ⱦ�����ı߿���ɫ.
		static vgMath::ColorValue	msEdgeColor;

		/// billboard����Ⱦ��ʽ.
		RenderTypes	mRenderType;

		/// ��ǰ�ӵ����������ֵ.
		mutable  Vector3	mCurrentViewRight;

		/// ��ǰ�ӵ�ĳ���.
		mutable  Vector3	mCurrentViewDirection;

		/// ��ǰ�ӵ����������.
		mutable  Vector3	mCurrentViewUp;
	};
	

	/**
		@date 	2007/04/28  21:29	
		@author  Leven
			
		@brief 	������Ƭ���ָ��.
			
		@see    SharedPtr
	*/
	typedef  SharedPtr<Billboard>  BillboardPtr;
	
	
}// end of namespace vgCore

#endif // end of __TVGBILLBOARD_H__