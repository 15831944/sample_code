#ifndef	_VGCOMMON_IMPORT_LIBS_H_INCLUDED_
#define	_VGCOMMON_IMPORT_LIBS_H_INCLUDED_

//!  ���볣�ÿ�. 
/*!
OpenGL��CxImage�ȳ��ÿ�ĵ���.
*/



//////////////////////////////////////////////////////////////////////////
//
//	���������
//
#pragma message("\t now input Basic	Libs...")

//	OpenGL��.
#	pragma message("\t\t\t\t now input OpenGL standard lib...")
//#define GLEW_STATIC 
#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/glu.h>
#include <gl/glaux.h>

#		pragma   comment(lib,"opengl32.lib")
#		pragma	  comment(lib,"glu32.lib")
#		pragma   comment(lib,"glaux.lib")
#		pragma   comment(lib ,"glew32.lib")


////	Lua��
//#pragma message("\t\t\t\t now input Lua(LuaPlus).lib...")
//#pragma comment(lib, "LuaPlus.lib" )


//	cximage ��
#	pragma message("\t\t\t\t now input cximage(vgImageLib).lib...")
#	if _DEBUG
#		pragma comment( lib , "vgImageLibD.lib")
#	else
#		pragma comment( lib , "vgImageLib.lib")
#	endif


////	DirectInput��
//#define DIRECTINPUT_VERSION 0x0800
//#include <dinput.h>
//#	pragma message("\t\t\t\t now input DirectInput libs...")
//#		pragma comment(lib,"dinput8.lib")
//#		pragma  comment(lib, "dxguid.lib")


//	CG ��
#include <Cg/cg.h>
#include <Cg/cgGL.h>

#	pragma message("\t\t\t\t now input CG libs...")
#		pragma  comment(lib, "cg.lib")
#		pragma  comment(lib, "cgGL.lib")


////	nvidia texture ��
//#	pragma message("\t\t\t\t now input nvidia texture tools(vgImageTools).lib...")
//#	if _DEBUG
//#		pragma comment( lib, "vgImageToolsD.lib" )
//#	else
//#		pragma comment( lib, "vgImageTools.lib" )
//#	endif




#endif//_VGCOMMON_IMPORT_LIBS_H_INCLUDED_
