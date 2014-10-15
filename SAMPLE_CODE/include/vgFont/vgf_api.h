/*! \file vglo_api.h
\brief ���� ģ��ӿں���
*/

#ifdef __cplusplus
extern "C" {
#endif

	/** \fn void FontManager_reset(��
	\brief ��λ
	*/
	typedef void (*pfnFontManager_reset)();
	extern VGF_EXPORT void FontManager_reset();
	/** \fn void FontManager_initialiseFonts(HDC hDC)
	\brief ��ʼ������
	\param hDC �豸�����ľ��
	*/
	typedef void (*pfnFontManager_initialiseFonts)(HDC hDC);
	extern VGF_EXPORT void FontManager_initialiseFonts(HDC hDC);
	/** \fn void FontManager_uninitBeforeOpenGLDestroy()
	\brief ��OpenGL����֮ǰע������
	\param hDC �豸�����ľ��
	*/
	typedef void (*pfnFontManager_uninitBeforeOpenGLDestroy)();
	extern VGF_EXPORT void FontManager_uninitBeforeOpenGLDestroy();
	/** \fn void FontManager_getOriginX()
	\brief �õ������x����ԭ��
	*/
	typedef float (*pfnFontManager_getOriginX)();
	extern VGF_EXPORT float FontManager_getOriginX();
	/** \fn void pfnFontManager_setOriginX(float x)
	\brief ����x����ԭ��
	\param x x����ֵ
	*/
	typedef void (*pfnFontManager_setOriginX)(float x);
	extern VGF_EXPORT void FontManager_setOriginX(float x);
	/** \fn void FontManager_getOriginY()
	\brief �õ�����y����ԭ��
	*/
	typedef float (*pfnFontManager_getOriginY)();
	extern VGF_EXPORT float FontManager_getOriginY();
	/** \fn void FontManager_setOriginY(float y)
	\brief ����y����ԭ��
	\param y y����ԭ��
	*/
	typedef void (*pfnFontManager_setOriginY)(float y);
	extern VGF_EXPORT void FontManager_setOriginY(float y);
	/** \fn void FontManager_getFontSize()
	\brief �õ������С
	*/
	typedef float (*pfnFontManager_getFontSize)();
	extern VGF_EXPORT float FontManager_getFontSize();
	/** \fn void FontManager_setFontSize(float fontSize)
	\brief ���������С
	\param fontSize �����С
	*/
	typedef void (*pfnFontManager_setFontSize)(float fontSize);
	extern VGF_EXPORT void FontManager_setFontSize(float fontSize);
	/** \fn void FontManager_setDefaultDiaplay(String& str)
	\brief ����Ҫ��ʾ������
	\param str Ҫ��ʾ���ַ���
	*/
	typedef void (*pfnFontManager_setDefaultDisplay)(String& str );
	extern VGF_EXPORT void FontManager_setDefaultDiaplay(String& str);
	/** \fn void FontManager_rebuildFontSize(int fontSize)
	\brief ���������С
	\param fontSize �����С
	*/
	typedef void (*pfnFontManager_rebuildFontSize)(int fontSize);
	extern VGF_EXPORT void FontManager_rebuildFontSize(int fontSize);
	/** \fn void FontManager_drawNumber(float h)
	\brief ����Ҫ��Ⱦ������
	\param h ��Ҫ��ʾ������
	*/
	typedef void (*pfnFontManager_drawNumber)(float h);
	extern VGF_EXPORT void FontManager_drawNumber(float h);
	/** \fn void FontManager_drawString(const std::string& str)
	\brief ����Ҫ��Ⱦ������
	\param str ��Ҫ��ʾ������
	*/
	typedef void(*pfnFontManager_drawString)(const std::string& str);
	extern VGF_EXPORT void FontManager_drawString(const std::string& str);
	/** \fn void FontManager_splitToMultiLine(const std::string &str)
	\brief ������ʾ����
	\param str ��Ҫ��ʾ������
	*/
	typedef vgKernel::StringVector* (*pfnFontManager_splitToMultiLine)(const std::string &str);
	extern VGF_EXPORT vgKernel::StringVector* FontManager_splitToMultiLine(const std::string &str);


#ifdef __cplusplus
}
#endif
