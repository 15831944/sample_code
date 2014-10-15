#pragma once

namespace vgChannel
{
	enum PIXEL_BUFFER_TYPE
	{
		PIXEL_PACK,
		PIXEL_UNPACK
	};

	class PixelBuffer
	{
	public:
		PixelBuffer(int width, int height, PIXEL_BUFFER_TYPE type=PIXEL_PACK, GLenum format=GL_BGR);
		~PixelBuffer();

		bool reSize(int width, int height);
		int getWidth() { return m_width; }
		int getHeight() { return m_height; }

		/*
			Pack �����ػ����������ݵ��ڴ�
			1���ȵ���Pack����DMA����
			2������mapBuffer��ȡ���ݵ��ڴ�
			3���Է��ص��ڴ���в�����ע���޸Ĳ���Ӱ�쵽��ɫ������
			4������unmapBuffer�ͷŻ������ڴ�
		*/

		// DMA���ͣ�����
		bool pack();
		// ӳ����Ļ���ص����ݵ���ʱ������ 
		GLubyte* mapBuffer();
		// �ͷ���ʱ����������
		bool unmapBuffer();

		/*
		UnPack ���ڴ洫�����ݵ����ػ�����
		1���ȵ���getDrawBuffer�õ��ڴ滺������ַ
		2�����ڴ滺�������в���
		3������pack������������ɫ������
		*/

		// DMA���ͣ�д��
		bool unpack();
		// �õ�д�����������ݿɽ����޸�
		GLubyte* getDrawBuffer();
	
	private:

		GLuint					m_bufferId;
		GLubyte*				m_pDrawBuffer;

		int						m_width;
		int						m_height;
	
		int						m_pixelLen;
		GLenum					m_format;

		PIXEL_BUFFER_TYPE		m_type;
	};
}