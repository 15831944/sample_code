


#ifndef __VGTHUGEPROCESSFILEBLOCK_H__
#define __VGTHUGEPROCESSFILEBLOCK_H__

#include <vgKernel/vgkForward.h>
#include <vgKernel/vgkImageRect.h>
#include <vgKernel/vgkSystemUtility.h>
#include <vgTerritory/vgtHugeProcessCutBlock.h>
#include <vgTerritory/vgtHugeProcessGlobal.h>
#include <vgThreads/CriticalSection.h>

	
namespace vgTerritory {


	namespace HugeProcess 
	{
		using namespace vgThreads;
		using namespace vgKernel;
		
		class SmallBlock;


		class FileBlock
		{
		public:
			FileBlock( String filename , ImageRect abs_rect ,  Global* global)
			{
				_global = global;
				//assert( SystemUtility::checkFileExist( filename ) == true );
				_filePath = filename;
				_absoluteRect = abs_rect;
			}
			~FileBlock()
			{
				
			}
		
			String getFilePath() const
			{
				return _filePath;
			}
 
			ImageRect getAbsoluteRect() const
			{
				return _absoluteRect;
			}

			// ��fileblock�п�(����smallblock), �����output_cutblocks��
			void beginToCut( std::vector<CutBlock*>& output_cutblocks , 
				CriticalSection* cutblock_critialsect );

			void beginToCutImpl( std::vector<CutBlock*>& output_cutblocks , 
				CriticalSection* cutblock_critialsect );

			/**
				�����ǳ���,����ֵ��Ϊfalse,��ʾû���ཻ��.
			*/
			bool testIntersects( std::vector<SmallBlock*>& input_small_blocks );

		private:

			Global* _global;

			String _filePath;
			ImageRect _absoluteRect;

			std::vector<SmallBlock*> _intersectSmallBlocks;

		};
	}
	
	
	
}// end of namespace vgTerritory
	


#endif // end of __VGTHUGEPROCESSFILEBLOCK_H__