


#ifndef __VGKRGBAQUAD_H__
#define __VGKRGBAQUAD_H__

#include <vgKernel/vgkForward.h>

	
namespace vgKernel {

	/**
		@date 	2009/05/05  10:02	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	class  /*VGK_EXPORT*/ RgbaQuad
	{
	public:
		RgbaQuad()
			: blue(0),
			green(0),
			red(0),
			alpha(0)
		{
			
		}

		RgbaQuad( BYTE r, BYTE g, BYTE b, BYTE a )
			: red(r),
			green(g),
			blue(b),
			alpha(a)
		{

		}

		RgbaQuad(const RgbaQuad& src)
			: blue( src.blue ),
			green( src.green ),
			red( src.red ),
			alpha( src.alpha )
		{

		}

		~RgbaQuad()
		{
			
		}

		RgbaQuad& operator= ( RGBQUAD quad )
		{
			blue = quad.rgbBlue;
			red = quad.rgbRed;
			alpha = quad.rgbReserved;
			green = quad.rgbGreen;
			return *this;
		}

		RgbaQuad operator+ ( const RgbaQuad& quad )
		{
			return RgbaQuad(
				red + quad.red,
				green + quad.green,
				blue + quad.blue,
				alpha + quad.alpha );
		}

		RgbaQuad operator- ( const RgbaQuad& quad )
		{
			return RgbaQuad(
				red - quad.red,
				green - quad.green,
				blue - quad.blue,
				alpha - quad.alpha );
		}

		RGBQUAD* getRGBQUAD()
		{
			return (RGBQUAD*)this;
		}

	public:

		BYTE    blue;
		BYTE    green;
		BYTE    red;
		BYTE    alpha;
	};
	
	
}// end of namespace vgKernel
	


#endif // end of __VGKRGBAQUAD_H__