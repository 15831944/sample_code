/*! \file vglo_api.h
\brief ���� ģ��ӿں���
*/

#ifdef __cplusplus
extern "C" {
#endif

	/** \fn bool renderBegin()
	\brief ��ʼ��Ⱦ
	*/
	typedef bool (*pfnrenderBegin)();
	extern VGF_EXPORT bool renderBegin();

	/** \fn bool clientRenderBegin()
	\brief �ͻ��˿�ʼ��Ⱦ
	*/
	typedef bool (*pfnclientRenderBegin)();
	extern VGF_EXPORT bool clientRenderBegin();

	/** \fn bool serverRenderBegin()
	\brief �������˿�ʼ��Ⱦ
	*/
	typedef bool (*pfnserverRenderBegin)();
	extern VGF_EXPORT bool serverRenderBegin();

	/** \fn bool renderEnd()
	\brief ������Ⱦ
	*/
	typedef bool (*pfnrenderEnd)();
	extern VGF_EXPORT bool renderEnd();

	/** \fn void blendAndAdujst()
	\brief ����У��
	*/
	typedef void (*pfnblendAndAdjust)();
	extern VGF_EXPORT void blendAndAdjust();

	/** \fn void setScreenSize()
	\brief ������Ļ��С
	\x y ��Ļ��С��������
	*/
	typedef void (*pfnsetScreenSize)();
	extern VGF_EXPORT void setScreenSize(int x, int y);

	/** \fn void FontManager_setOriginY(float y)
	\brief ����y����ԭ��
	\param y y����ԭ��
	*/
	//typedef void (*pfnFontManager_setOriginY)(float y);
	//extern VGF_EXPORT void FontManager_setOriginY(float y);
	/** \fn void FontManager_getFontSize()
	\brief �õ������С
	*/
	//typedef float (*pfnFontManager_getFontSize)();
	//extern VGF_EXPORT float FontManager_getFontSize();
	/** \fn void FontManager_setFontSize(float fontSize)
	\brief ���������С
	\param fontSize �����С
	*/
	//typedef void (*pfnFontManager_setFontSize)(float fontSize);
	//extern VGF_EXPORT void FontManager_setFontSize(float fontSize);
	/** \fn void FontManager_rebuildFontSize(int fontSize)
	\brief ���������С
	\param fontSize �����С
	*/
	//typedef void (*pfnFontManager_rebuildFontSize)(int fontSize);
	//extern VGF_EXPORT void FontManager_rebuildFontSize(int fontSize);
	/** \fn void FontManager_drawNumber(float h)
	\brief ����Ҫ��Ⱦ������
	\param h ��Ҫ��ʾ������
	*/
	//typedef void (*pfnFontManager_drawNumber)(float h);
	//extern VGF_EXPORT void FontManager_drawNumber(float h);
	/** \fn void FontManager_drawString(const std::string& str)
	\brief ����Ҫ��Ⱦ������
	\param str ��Ҫ��ʾ������
	*/
	//typedef void(*pfnFontManager_drawString)(const std::string& str);
	//extern VGF_EXPORT void FontManager_drawString(const std::string& str);
	/** \fn void FontManager_splitToMultiLine(const std::string &str)
	\brief ������ʾ����
	\param str ��Ҫ��ʾ������
	*/
	//typedef vgKernel::StringVector* (*pfnFontManager_splitToMultiLine)(const std::string &str);
	//extern VGF_EXPORT vgKernel::StringVector* FontManager_splitToMultiLine(const std::string &str);


#ifdef __cplusplus
}
#endif
