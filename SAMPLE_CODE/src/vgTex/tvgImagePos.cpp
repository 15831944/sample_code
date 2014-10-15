
#include <vgStableHeaders.h>

#include <vgTex/tvgImagePos.h>


namespace vgCore {
	
// ��mfc�п��ܻ���Ĭ�ϵ�max�����.
#undef MAX
#undef MIN
#undef  max
#undef  min

	const ImagePos ImagePos::ZERO (0);

	const ImagePos ImagePos::MAX (
		std::numeric_limits<Integer>::max() ,
		std::numeric_limits<Integer>::max() );

	const ImagePos ImagePos::MIN (
		std::numeric_limits<Integer>::min() ,
		std::numeric_limits<Integer>::min() );
	
	
	//----------------------------------------------------------------
	String ImagePos::getDetails() const
	{
		std::ostringstream o;
		o << "ImagePos( " << x <<  " , " << y << " )";

		return o.str();
	}
	//----------------------------------------------------------------


}// end of namespace vgCore
