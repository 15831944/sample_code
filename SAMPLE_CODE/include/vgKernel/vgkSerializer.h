



#ifndef __VGKSERIALIZER_H__
#define __VGKSERIALIZER_H__

#include <vgKernel/vgkForward.h>

	
namespace vgKernel {

	/**
		@date 	2009/05/12  9:31	
		@author  xy
	
		@brief 	�ṩͳһ�����л��ͷ����л��Ľӿ�
	
		@see    
	*/
	class Serializer
	{
	public:
		Serializer()
		{
			
		}
		virtual ~Serializer()
		{
			
		}

	public:
	
	
	};
	


	/**
		�ṩ�����Ƶ����л�
	*/
	class SerializerBinary : public Serializer
	{
	public:
		SerializerBinary()
		{
			
		}
		~SerializerBinary()
		{
			
		}
	
	
	protected:
	
		
	
	private:
	
	
	};


	/**
		�ṩXML�����л�
	*/
	//class SerializerXML
	//{
	//public:
	//	SerializerXML()
	//	{
	//		
	//	}
	//	~SerializerXML()
	//	{
	//		
	//	}
	//
	//
	//protected:
	//
	//	
	//
	//private:
	//
	//
	//};
	
}// end of namespace vgKernel
	


#endif // end of __VGKSERIALIZER_H__