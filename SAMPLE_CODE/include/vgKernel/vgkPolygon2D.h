



#ifndef __VGKPOLYGON2D_H__
#define __VGKPOLYGON2D_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkVec2.h>
	
namespace vgKernel {

	/**
		@date 	2009/01/07  11:04	
		@author  xy
	
		@brief 	
	
		@see    
	*/
	typedef std::vector<Vec2> Vec2Vector;


	class  VGK_EXPORT Polygon2D
	{
	public:
		Polygon2D()
		{
			
		}
		Polygon2D( Vec2Vector* vertices )
		{
			setPoints( vertices );
		}
		~Polygon2D()
		{
			
		}
	

	public:

		void setPoints( Vec2Vector* vertices )
		{
			_polyPoints = *vertices;
		}

		Vec2Vector* getPoints()
		{
			return &_polyPoints;
		}

		bool contains (const Vec2& rkP) const
		{
			bool bInside = false;
			int	iQuantity = _polyPoints.size();
			for (int i = 0, j = iQuantity-1; i < iQuantity; j = i++)
			{
				const Vec2& rkU0 = _polyPoints[i];
				const Vec2& rkU1 = _polyPoints[j];
				float fRHS, fLHS;

				if (rkP[1] < rkU1[1])  // U1 above ray
				{
					if (rkU0[1] <= rkP[1])  // U0 on or below ray
					{
						fLHS = (rkP[1]-rkU0[1])*(rkU1[0]-rkU0[0]);
						fRHS = (rkP[0]-rkU0[0])*(rkU1[1]-rkU0[1]);
						if (fLHS > fRHS)
						{
							bInside = !bInside;
						}
					}
				}
				else if (rkP[1] < rkU0[1])  // U1 on or below ray, U0 above ray
				{
					fLHS = (rkP[1]-rkU0[1])*(rkU1[0]-rkU0[0]);
					fRHS = (rkP[0]-rkU0[0])*(rkU1[1]-rkU0[1]);
					if (fLHS < fRHS)
					{
						bInside = !bInside;
					}
				}
			}
			return bInside;
		}
	
	private:
	
		Vec2Vector _polyPoints;
	
	};//class Polygon2D
	
	

}// end of namespace vgKernel
	


#endif // end of __VGKPOLYGON2D_H__